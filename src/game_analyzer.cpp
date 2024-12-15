// -*-c++-*-

/*!
  \file game_analyer.cpp
  \brief game analyzer Source File
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "game_analyzer.h"

#include <rcsc/common/server_param.h>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
GameAnalyzer::GameAnalyzer()
{

}

/*-------------------------------------------------------------------*/
/*!

 */
bool
GameAnalyzer::analyze( const FieldModel & model )
{
    for ( size_t i = 0; i < model.fieldStates().size(); ++i )
    {
        extractKickEvent( model, i );
    }

    extractShootEvent( model );
    extractPassEvent( model );
    extractDribbleEvent( model );

    std::sort( M_action_events.begin(), M_action_events.end(),
               []( const ActionEvent::ConstPtr & lhs,
                   const ActionEvent::ConstPtr & rhs )
               {
                   return lhs->startTime() < rhs->startTime();
               } );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
GameAnalyzer::extractKickEvent( const FieldModel & model,
                                const size_t idx )
{
    FieldState::ConstPtr state = model.getState( idx );
    if ( ! state )
    {
        std::cerr << "ERROR: (GameAnalyzer::extractKickEvent) no state at " << idx << std::endl;
        return;
    }

    if ( state->kickers().empty() )
    {
        // no kicker
        return;
    }

    if ( state->kickers().size() > 1
         || ! state->tacklers().empty()
         || ! state->catchers().empty() )
    {
        // not a single kick
        return;
    }

    FieldState::ConstPtr prev_state = model.getState( idx - 1 );

    if ( ! prev_state )
    {
        std::cerr << "ERROR: (GameAnalyzer::extractKickEvent) no previous state at " << idx << std::endl;
        return;
    }

    const CoachPlayerObject * kicker = state->kickers().front();

    // check the ball volocity to confirm the successful kick
    // (prev_vel + accel + error) * decay = vel
    // prev_vel + accel + error = vel / decay
    // accel + error = vel / decay - prev_vel

    const Vector2D prev_vel = prev_state->ball().vel();
    const double prev_speed = prev_vel.r();
    const double max_rand = prev_speed * ServerParam::i().ballRand();

    const Vector2D estimated_error = state->ball().vel() / ServerParam::i().ballDecay() - prev_vel;

    if ( estimated_error.r() < max_rand )
    {
        // no kick effect -> failed kick?
        std::cerr << "INFO: (GameAnalyzer::extractKickEvent) detect kicker or tackler, but detect no kick effect." << idx << std::endl;
        return;
    }

    Kick::ConstPtr kick( new Kick( idx - 1,
                                   kicker->side(),
                                   kicker->unum(),
                                   prev_state->time(),
                                   prev_state->gameMode(),
                                   prev_state->ball().pos(),
                                   ( state->ball().pos() - prev_state->ball().pos() ) ) );
    M_single_kicks.push_back( kick );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
GameAnalyzer::extractShootEvent( const FieldModel & model )
{
    const std::vector< FieldState::Ptr > & states = model.fieldStates();
    const size_t max_states = states.size();

    GameMode mode;

    for ( size_t i = 1; i < max_states; ++i )
    {
        FieldState::ConstPtr s = states[i];
        if ( mode.type() != GameMode::AfterGoal_
             && s->gameMode().type() == GameMode::AfterGoal_ )
        {
            //std::cerr << "Detect shoot " << s->time() << std::endl;
            SideID kicker_side = NEUTRAL;
            int kicker_unum = Unum_Unknown;
            Vector2D start_pos = Vector2D::INVALIDATED;
            GameTime start_time;
            GameMode start_mode;
            Vector2D end_pos = s->ball().pos();
            GameTime end_time = s->time();

            // find the kick
            for ( size_t j = i - 1; j > 0; --j )
            {
                if ( ! states[j]->kickers().empty() )
                {
                    //std::cerr << "Find shoot kicker " << states[j]->time() << std::endl;
                    const rcsc::CoachPlayerObject * kicker = nullptr;
                    for ( CoachPlayerObject::Cont::const_iterator p = states[j]->kickers().begin();
                          p != states[j]->kickers().end();
                          ++p )
                    {
                        if ( ( *p )->side() == s->gameMode().side() )
                        {
                            kicker = *p;
                            break;
                        }
                    }

                    if ( kicker )
                    {
                        kicker_side = kicker->side();
                        kicker_unum = kicker->unum();
                        start_pos = states[j - 1]->ball().pos();
                        start_time = states[j - 1]->time();
                        start_mode = states[j - 1]->gameMode();
                        break;
                    }
                }
            }

            if ( kicker_side != NEUTRAL
                 && start_pos.isValid() )
            {
                ActionEvent::ConstPtr ptr( new Shoot( kicker_side, kicker_unum, start_time, start_mode, start_pos, end_time, end_pos, true ) );
                M_action_events.push_back( ptr );
            }
        }

        mode = s->gameMode();
    }
}


/*-------------------------------------------------------------------*/
/*!

 */
void
GameAnalyzer::extractPassEvent( const FieldModel & model )
{
    extractPassEventSimple( model );
}


/*-------------------------------------------------------------------*/
namespace {

bool
exist_other_ball_collider( const CoachPlayerObject * kicker,
                           const CoachPlayerObject::Cont & colliders )
{
    for ( const CoachPlayerObject * p : colliders )
    {
        if ( kicker != p )
        {
            return true;
        }
    }

    return false;
}

}

/*-------------------------------------------------------------------*/
/*!

 */
void
GameAnalyzer::extractPassEventSimple( const FieldModel & model )
{
    GameTime last_kick_time( -1, 0 );
    GameMode last_kick_mode;
    SideID last_kicker_side = NEUTRAL;
    int last_kicker_unum = Unum_Unknown;
    Vector2D last_kick_pos = Vector2D::INVALIDATED;

    for ( size_t i = 1; i < model.fieldStates().size(); ++i )
    {
        const FieldState::ConstPtr prev_state = model.getState( i - 1 );
        const FieldState::ConstPtr state = model.getState( i );

        if ( ! prev_state ) continue;
        if ( ! state ) continue;

        if ( ( prev_state->gameMode().type() == GameMode::PlayOn
               && state->gameMode().type() != GameMode::PlayOn )
             ||  ( state->gameMode().type() != GameMode::PlayOn
                   && state->gameMode().type() != GameMode::GoalKick_ )
             )
        {
            last_kick_time.assign( -1, 0 );
            last_kick_mode = GameMode();
            last_kicker_side = NEUTRAL;
            last_kicker_unum = Unum_Unknown;
            last_kick_pos = Vector2D::INVALIDATED;
            continue;
        }

        if ( state->kickers().empty() )
        {
            continue;
        }

        const CoachPlayerObject * kicker =  state->kickers().front();
        if ( state->kickers().size() == 1
             && ! exist_other_ball_collider( kicker, state->ballColliders() )
             && state->tacklers().empty()
             && state->catchers().empty() )
        {
            // only 1 kicker

            if ( last_kicker_side == NEUTRAL )
            {
                // new kick sequence
                //std::cerr << "NewKickSequence? " << prev_state->time() << std::endl;
            }
            else if ( kicker->side() != last_kicker_side )
            {
                // interception
                //std::cerr << "Intercept? " << prev_state->time() << std::endl;
                ActionEvent::ConstPtr intercept( new Interception( last_kicker_side, last_kicker_unum,
                                                                   last_kick_time, last_kick_mode, last_kick_pos,
                                                                   kicker->side(), kicker->unum(),
                                                                   prev_state->time(), prev_state->ball().pos() ) );
                M_action_events.push_back( intercept );
            }
            else if ( kicker->unum() == last_kicker_unum )
            {
                // hold or dribble dribble
                //std::cerr << "HoldOrDribble? " << prev_state->time() << std::endl;
            }
            else
            {
                // pass
                // std::cerr << "Pass? "
                //           << last_kick_time << "," << prev_state->time()
                //           << "," << side_str( last_kicker_side )
                //           << "," << ( last_kicker_side == LEFT ? model.leftTeamName()
                //                       : last_kicker_side == RIGHT ? model.rightTeamName()
                //                       : "Unknown" )
                //           << "," << last_kicker_unum
                //           << "," << kicker->unum()
                //           << std::endl;
                ActionEvent::ConstPtr pass( new Pass( last_kicker_side, last_kicker_unum,
                                                      last_kick_time, last_kick_mode, last_kick_pos,
                                                      kicker->side(), kicker->unum(),
                                                      prev_state->time(), prev_state->ball().pos(),
                                                      true ) );
                M_action_events.push_back( pass );
            }

            last_kick_time = prev_state->time();
            last_kick_mode = prev_state->gameMode();
            last_kicker_side = kicker->side();
            last_kicker_unum = kicker->unum();
            last_kick_pos = prev_state->ball().pos();
        }
        else
        {
            // Several players touch the ball
            if ( last_kicker_side != NEUTRAL )
            {
                ActionEvent::ConstPtr intercept;

                for ( const CoachPlayerObject * p : state->ballColliders() )
                {
                    if ( p->side() != last_kicker_side )
                    {
                        //std::cerr << "Collide? and Intercept?" << std::endl;
                        intercept = ActionEvent::ConstPtr( new Interception(  last_kicker_side, last_kicker_unum,
                                                                              last_kick_time, last_kick_mode, last_kick_pos,
                                                                              p->side(), p->unum(),
                                                                              prev_state->time(), prev_state->ball().pos() ) );
                        break;
                    }
                    else
                    {
                        //std::cerr << "Collide but same side?" << std::endl;
                    }
                }

                for ( const CoachPlayerObject * p : state->tacklers() )
                {
                    if ( p->side() != last_kicker_side )
                    {
                        //std::cerr << "Tackle?" << std::endl;
                        intercept = ActionEvent::ConstPtr( new Interception( last_kicker_side, last_kicker_unum,
                                                                             last_kick_time, last_kick_mode, last_kick_pos,
                                                                             p->side(), p->unum(),
                                                                             prev_state->time(), prev_state->ball().pos() ) );
                        break;
                    }
                }

                for ( const CoachPlayerObject * p : state->catchers() )
                {
                    if ( p->side() != last_kicker_side )
                    {
                        //std::cerr << "Catch?" << std::endl;
                        intercept = ActionEvent::ConstPtr( new Interception( last_kicker_side, last_kicker_unum,
                                                                             last_kick_time, last_kick_mode, last_kick_pos,
                                                                             p->side(), p->unum(),
                                                                             prev_state->time(), prev_state->ball().pos() ) );
                        break;
                    }
                }

                for ( const CoachPlayerObject * p : state->kickers() )
                {
                    if ( p->side() != last_kicker_side )
                    {
                        //std::cerr << "Intercept?" << std::endl;
                        intercept = ActionEvent::ConstPtr( new Interception( last_kicker_side, last_kicker_unum,
                                                                             last_kick_time, last_kick_mode, last_kick_pos,
                                                                             p->side(), p->unum(),
                                                                             prev_state->time(), prev_state->ball().pos() ) );
                        break;
                    }
                }

                if ( intercept )
                {
                    M_action_events.push_back( intercept );
                }
            }

            // reset the kick sequence
            last_kick_time.assign( -1, 0 );
            last_kick_mode = GameMode();
            last_kicker_side = NEUTRAL;
            last_kicker_unum = Unum_Unknown;
            last_kick_pos = Vector2D::INVALIDATED;
        }
    }
}

#if 0
/*-------------------------------------------------------------------*/
/*!

 */
void
GameAnalyzer::extractPassEventByKicks( const FieldModel & /*model*/ )
{
    Kick last_kick;
    last_kick.side_ = NEUTRAL;

    for ( const Kick::ConstPtr & kick : M_single_kicks )
    {
        if ( kick.mode_.type() != GameMode::PlayOn )
        {
            last_kick = kick;
            continue;
        }

        if ( kick.unum_ == last_kick.unum_ )
        {
            last_kick = kick;
            continue;
        }

        if ( kick.side_ == NEUTRAL
             || kick.side_ != last_kick.side_ )
        {
            std::cout << "Intercept?? "
                      << last_kick.time_ << " " << side_str( last_kick.side_ ) << " " << last_kick.unum_ << " " << last_kick.pos_ << " -> "
                      << kick.time_ << " " << side_str( kick.side_ ) << " " << kick.unum_ << " " << last_kick.pos_ << std::endl;
        }
        else
        {
            std::cout << "Pass?? "
                      << last_kick.time_ << " " << side_str( last_kick.side_ ) << " " << last_kick.unum_ << " " << last_kick.pos_ << " -> "
                      << kick.time_ << " " << side_str( kick.side_ ) << " " << kick.unum_ << " " << last_kick.pos_ << std::endl;
        }

        last_kick = kick;
    }
}
#endif

/*-------------------------------------------------------------------*/
void
GameAnalyzer::extractDribbleEvent( const FieldModel & model )
{
    Kick last_kick;
    Vector2D kicker_pos = Vector2D::INVALIDATED;
    int kicker_dash_count = 0;

    for ( size_t i = 1; i < model.fieldStates().size(); ++i )
    {
        const FieldState::ConstPtr prev_state = model.getState( i - 1 );
        const FieldState::ConstPtr state = model.getState( i );

        if ( ! prev_state ) continue;
        if ( ! state ) continue;

        // playmode changed -> reset last kicker
        if ( ( prev_state->gameMode().type() == GameMode::PlayOn
             && state->gameMode().type() != GameMode::PlayOn )
             ||  ( state->gameMode().type() != GameMode::PlayOn
                   && state->gameMode().type() != GameMode::GoalKick_ )
             )

        {
            last_kick.reset();
            kicker_dash_count = 0;
            continue;
        }

        // exist tackling player -> reset
        if ( ! state->tacklers().empty() )
        {
            last_kick.reset();
            kicker_pos = Vector2D::INVALIDATED;
            kicker_dash_count = 0;
            continue;
        }

        // no kicking player
        if ( state->kickers().empty() )
        {
            continue;
        }

        // multiple kickers -> reset last kicker
        if ( state->kickers().size() >= 2 )
        {
            last_kick.reset();
            kicker_pos = Vector2D::INVALIDATED;
            kicker_dash_count = 0;
            continue;
        }

        const CoachPlayerObject * kicker = state->kickers().front();

        // same player kicks the ball
        if ( last_kick.side_ == kicker->side()
             && last_kick.unum_ == kicker->unum()
             && kicker->dashCount() > kicker_dash_count )
        {
            // kicker performs dashes after the first kick.
            ActionEvent::ConstPtr dribble( new Dribble( kicker->side(), kicker->unum(),
                                                        last_kick.time_, last_kick.mode_, last_kick.pos_,
                                                        prev_state->time(), prev_state->ball().pos() ) );
            M_action_events.push_back( dribble );
        }

        // reset the last kick information
        last_kick.reset();
        kicker_dash_count = 0;

        // set kicker
        if ( ! last_kick.isValid() )
        {
            last_kick.assign( i - 1,
                              kicker->side(),
                              kicker->unum(),
                              prev_state->time(),
                              prev_state->gameMode(),
                              prev_state->ball().pos(),
                              ( state->ball().pos() - prev_state->ball().pos() ) );
            const CoachPlayerObject * prev_kicker = prev_state->getPlayer( kicker->side(), kicker->unum() );
            kicker_pos = ( prev_kicker
                           ? prev_kicker->pos()
                           : kicker->pos()  );
            kicker_dash_count = kicker->dashCount();
            continue;
        }
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
GameAnalyzer::print( const FieldModel & /*model*/ ) const
{
    printActionEvents();
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
GameAnalyzer::printKickEvents( const FieldModel & model ) const
{
    const std::string team_l = model.leftTeamName();
    const std::string team_r = model.rightTeamName();
    const std::string unknown = "Unknown";

    std::cout << "Type,"
              << "TeamName,"
              << "Side,"
              << "Kicker,"
              << "Time,"
              << "X,"
              << "Y,"
              << "VelX,"
              << "VelY"
              << std::endl;
    for ( const Kick::ConstPtr & k : M_single_kicks )
    {
        const std::string team = ( k->side_ == LEFT ? team_l
                                   : k->side_ == RIGHT ? team_r
                                   : unknown );
        std::cout << "Kick,"
                  << team << ','
                  << side_char( k->side_ ) << ','
                  << k->unum_ << ','
                  << k->time_.cycle() << ','
                  << k->pos_.x << ','
                  << k->pos_.y << ','
                  << k->vel_.x << ','
                  << k->vel_.y << ','
                  << '\n';
    }

    return true;
}

/*-------------------------------------------------------------------*/
bool
GameAnalyzer::printActionEvents() const
{
    ActionEvent::print_header_csv( std::cout );

    for ( const ActionEvent::ConstPtr & action : M_action_events )
    {
        action->printCSV( std::cout );
    }

    return true;
}

// bool
// GameAnalyzer::printShootEvents( const FieldModel & model ) const
// {
//     const std::string team_l = model.leftTeamName();
//     const std::string team_r = model.rightTeamName();

//     std::cout << "Type,"
//               << "TeamName,"
//               << "Side,"
//               << "Kicker,"
//               << "StartTime,"
//               << "StartX,"
//               << "StartY,"
//               << "EndTime,"
//               << "EndX,"
//               << "EndY\n";
//     for ( const ActionEvent::ConstPtr & ev : M_shoot_events )
//     {
//         std::cout << "Shoot,"
//                   << ( ev->startPlayerSide() == LEFT ? team_l : team_r ) << ','
//                   << side_char( ev->startPlayerSide() ) << ','
//                   << ev->startPlayerUnum() << ','
//                   << ev->startTime().cycle() << ','
//                   << ev->startPos().x << ','
//                   << ev->startPos().y << ','
//                   << ev->endTime().cycle() << ','
//                   << ev->endPos().x << ','
//                   << ev->endPos().y << '\n';
//     }

//     std::cout << std::flush;

//     return true;
// }

// bool
// GameAnalyzer::printPassEvents( const FieldModel & model ) const
// {
//     const std::string team_l = model.leftTeamName();
//     const std::string team_r = model.rightTeamName();

//     for ( const ActionEvent::ConstPtr & ev : M_pass_events )
//     {
//         std::cout << "Pass,"
//                   << ( ev->startPlayerSide() == LEFT ? team_l : team_r ) << ','
//                   << side_char( ev->startPlayerSide() ) << ','
//                   << ev->startPlayerUnum() << ','
//                   << ev->startTime().cycle() << ','
//                   << ev->startPos().x << ','
//                   << ev->startPos().y << ','
//                   << ev->endTime().cycle() << ','
//                   << ev->endPos().x << ','
//                   << ev->endPos().y << '\n';
//     }

//     return true;
// }
