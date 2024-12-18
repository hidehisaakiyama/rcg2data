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
GameAnalyzer::GameAnalyzer()
    : M_touch_time( -1, 0 )
{

}

/*-------------------------------------------------------------------*/
bool
GameAnalyzer::analyze( const FieldModel & model )
{
    if ( model.fieldStates().empty() )
    {
        return false;
    }

    for ( size_t i = 1; i < model.fieldStates().size(); ++i )
    {
        FieldState::ConstPtr prev = model.fieldStates()[ i - 1 ];
        if ( ! prev ) continue;

        FieldState::ConstPtr current = model.fieldStates()[ i ];
        if ( ! current ) continue;

        analyzeSingleKick( *prev, *current );

        updateBallToucher( model, i );

        analyzeGoal( *prev, *current ); // need to be called after updating ball toucher
        analyzeOutOfBounds( *prev, *current ); // need to be called after updating ball toucher
    }

    std::sort( M_action_events.begin(), M_action_events.end(),
               []( const ActionEvent::ConstPtr & lhs,
                   const ActionEvent::ConstPtr & rhs )
               {
                   return lhs->beginTime() < rhs->beginTime();
               } );

    return true;
}

/*-------------------------------------------------------------------*/
std::ostream &
GameAnalyzer::print( std::ostream & os ) const
{
    ActionEvent::print_header_csv( os );

    for ( const ActionEvent::ConstPtr & ev : M_action_events )
    {
        ev->printCSV( os );
    }

    return os;
}

/*-------------------------------------------------------------------*/
void
GameAnalyzer::clearBallTouchers()
{
    M_touchers.clear();
    M_kickers.clear();
    M_kickers_left.clear();
    M_kickers_right.clear();
    M_tacklers.clear();
    M_tacklers_left.clear();
    M_tacklers_right.clear();
    M_catchers.clear();
}

/*-------------------------------------------------------------------*/
void
GameAnalyzer::analyzeSingleKick( const FieldState & prev,
                                 const FieldState & current )
{
    // check the number of the last ball kicker/tackler
    if ( M_kickers.size() + M_tacklers.size() != 1 )
    {
        return;
    }

    if ( current.kickers().size() != 1 )
    {
        return;
    }

    const Player begin_kicker = ( M_kickers.size() == 1 ? M_kickers.front()
                                  : M_tacklers.size() == 1 ? M_tacklers.front()
                                  : Player() );

    const CoachPlayerObject * end_kicker = current.kickers().front();
    if ( ! end_kicker ) return;

    if ( end_kicker->side() == begin_kicker.side_ )
    {
        if ( end_kicker->unum() != begin_kicker.unum_ )
        {
            ActionEvent::ConstPtr event( new Pass( begin_kicker.side_, begin_kicker.unum_,
                                                   M_touch_time, M_touch_mode, M_touched_ball_pos,
                                                   end_kicker->side(), end_kicker->unum(),
                                                   prev.time(), prev.ball().pos(),
                                                   true ) );
            M_action_events.push_back( event );
        }
        else if ( end_kicker->unum() == begin_kicker.unum_
                  && end_kicker->dashCount() > begin_kicker.dash_count_ )
        {
            ActionEvent::ConstPtr event( new Dribble( begin_kicker.side_, begin_kicker.unum_,
                                                      M_touch_time, M_touch_mode, M_touched_ball_pos,
                                                      prev.time(), prev.ball().pos(),
                                                      true ) );
            M_action_events.push_back( event );
        }
    }
    else
    {
        ActionEvent::ConstPtr event( new Interception( begin_kicker.side_, begin_kicker.unum_,
                                                       M_touch_time, M_touch_mode, M_touched_ball_pos,
                                                       end_kicker->side(), end_kicker->unum(),
                                                       prev.time(), prev.ball().pos() ) );
        M_action_events.push_back( event );
    }
}

/*-------------------------------------------------------------------*/
void
GameAnalyzer::analyzeGoal( const FieldState & prev,
                           const FieldState & current )
{
    if ( prev.gameMode().type() == GameMode::AfterGoal_
         || current.gameMode().type() != GameMode::AfterGoal_ )
    {
        return;
    }

    Player toucher;
    for ( const Player & p : M_touchers )
    {
        // std::cerr << current.time() << " "
        //           << side_char( p.side_ ) << ' ' << p.unum_ << std::endl;
        if ( toucher.side_ != NEUTRAL
             && toucher.side_ != p.side_ )
        {
            toucher.side_ = NEUTRAL;
            toucher.unum_ = Unum_Unknown;
            break;
        }

        if ( toucher.side_ == p.side_
             && toucher.unum_ != Unum_Unknown )
        {
            toucher.unum_ = Unum_Unknown;
            continue;
        }

        if ( toucher.side_ == NEUTRAL
             && toucher.unum_ == Unum_Unknown )
        {
            toucher = p;
        }
    }

    if ( toucher.side_ != current.gameMode().side() )
    {
        // TODO: estimate the exact scoring kicker
        ActionEvent::ConstPtr event( new OwnGoal( toucher.side_, toucher.unum_,
                                                  M_touch_time, M_touch_mode, M_touched_ball_pos,
                                                  current.time(), current.ball().pos() ) );
        M_action_events.push_back( event );
    }
    else
    {
        ActionEvent::ConstPtr event( new Shoot( toucher.side_, toucher.unum_,
                                                M_touch_time, M_touch_mode, M_touched_ball_pos,
                                                current.time(), current.ball().pos() ) );
        M_action_events.push_back( event );
    }
}

/*-------------------------------------------------------------------*/
void
GameAnalyzer::analyzeOutOfBounds( const FieldState & prev,
                                  const FieldState & current )
{
    if ( prev.gameMode().type() == current.gameMode().type() )
    {
        return;
    }

    if ( current.gameMode().type() == GameMode::KickIn_
         || current.gameMode().type() == GameMode::CornerKick_
         || current.gameMode().type() == GameMode::GoalKick_ )
    {
        Player toucher;

        toucher.side_ = ( current.gameMode().side() == LEFT
                          ? RIGHT
                          : LEFT );
        toucher.unum_ = Unum_Unknown;

        std::cerr << current.time()
                  << " OutOfBounds"
                  << " l=" << M_kickers_left.size() << ',' << M_tacklers_left.size()
                  << " r=" << M_kickers_right.size() << ',' <<  M_tacklers_right.size()
                  << std::endl;

        if ( toucher.side_ == LEFT
             && M_kickers_left.size() + M_tacklers_left.size() == 1 )
        {
            toucher.unum_ = ( M_kickers_left.size() == 1 ? M_kickers_left.front().unum_
                              : M_tacklers_left.size() == 1 ? M_tacklers_left.front().unum_
                              : Unum_Unknown );
        }
        else if ( toucher.side_ == RIGHT
                  && M_kickers_right.size() + M_tacklers_right.size() == 1 )
        {
            toucher.unum_ = ( M_kickers_right.size() == 1 ? M_kickers_right.front().unum_
                              : M_tacklers_right.size() == 1 ? M_tacklers_right.front().unum_
                              : Unum_Unknown );
        }

        ActionEvent::ConstPtr event( new OutOfBounds( toucher.side_, toucher.unum_,
                                                      M_touch_time, M_touch_mode, M_touched_ball_pos,
                                                      current.time(), current.ball().pos() ) );
        M_action_events.push_back( event );

        // clear ball touch flags
        clearBallTouchers();
    }
}

/*-------------------------------------------------------------------*/
void
GameAnalyzer::updateBallToucher( const FieldModel & model,
                                 const size_t frame_index )
{
    if ( frame_index < 1
         || model.fieldStates().size() <= frame_index )
    {
        return;
    }

    FieldState::ConstPtr current = model.fieldStates()[ frame_index ];
    if ( ! current )
    {
        return;
    }

    if ( current->kickers().empty()
         && current->tacklers().empty()
         && current->catchers().empty() )
    {
        return;
    }

    FieldState::ConstPtr prev = model.fieldStates()[ frame_index - 1 ];
    if ( ! prev )
    {
        return;
    }

    if ( current->gameMode().type() != GameMode::PlayOn
         && current->gameMode().type() != GameMode::GoalKick_
         && current->gameMode().type() == prev->gameMode().type()
         && current->gameMode().side() == prev->gameMode().side() )
    {
        clearBallTouchers();
        return;
    }

    clearBallTouchers();

    M_touch_time = prev->time();
    M_touch_mode = prev->gameMode();
    M_touched_ball_pos = prev->ball().pos();

    for ( const CoachPlayerObject * p : current->kickers() )
    {
        M_touchers.emplace_back( p );
        M_kickers.emplace_back( p );
        if ( p->side() == LEFT ) M_kickers_left.emplace_back( p );
        else if ( p->side() == RIGHT ) M_kickers_right.emplace_back( p );
    }

    for ( const CoachPlayerObject * p : current->tacklers() )
    {
        M_touchers.emplace_back( p );
        M_tacklers.emplace_back( p );
        if ( p->side() == LEFT ) M_tacklers_left.emplace_back( p );
        else if ( p->side() == RIGHT ) M_tacklers_right.emplace_back( p );
    }

    for ( const CoachPlayerObject * p : current->catchers() )
    {
        M_catchers.emplace_back( p );
    }

    if ( 39 <= current->time().cycle() &&
         current->time().cycle() <= 46 )
        std::cerr << current->time() << " updateTouchers " << M_touchers.size()
                  << " l=" << M_kickers_left.size() << "," << M_tacklers_left.size()
                  << " r=" << M_kickers_right.size() << "," << M_tacklers_right.size()
                  << std::endl;

}
