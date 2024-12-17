// -*-c++-*-

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

#include "analysis_state.h"

#include "analysis_context.h"
#include "field_model.h"

using namespace rcsc;

/*-------------------------------------------------------------------*/
AnalysisState::AnalysisState( const size_t index,
                              const rcsc::GameTime & begin_time,
                              const rcsc::GameMode & begin_mode )
    : M_frame_index( index ),
      M_begin_time( begin_time ),
      M_begin_mode( begin_mode )
{

}

/*-------------------------------------------------------------------*/
StateNeutral::StateNeutral( const size_t index,
                            const rcsc::GameTime & begin_time,
                            const rcsc::GameMode & begin_mode,
                            const rcsc::Vector2D & first_ball_pos )
    : AnalysisState( index, begin_time, begin_mode ),
      M_first_ball_pos( first_ball_pos )
{

}

/*-------------------------------------------------------------------*/
void
StateNeutral::analyze( AnalysisContext & context,
                       const FieldModel & model,
                       const size_t target_index )
{
    if ( target_index < 1
         || model.fieldStates().size() <= target_index )
    {
        return;
    }

    FieldState::ConstPtr target_state = model.fieldStates()[ target_index ];

    if ( ! target_state )
    {
        return;
    }

    if ( target_state->gameMode().type() != GameMode::PlayOn )
    {
        return;
    }

    // std::cerr << "(StateNeutral::update) " << target_state->time() << std::endl;

    FieldState::ConstPtr prev_state = model.fieldStates()[ target_index - 1 ];

    if ( ! prev_state )
    {
        return;
    }

    // single kicker
    if ( target_state->kickers().size() == 1
         && target_state->tacklers().empty()
         && target_state->catchers().empty() )
    {
        const CoachPlayerObject * kicker = target_state->kickers().front();
        std::shared_ptr< AnalysisState > new_state( new StateSingleKick( target_index - 1,
                                                                         prev_state->time(),
                                                                         prev_state->gameMode(),
                                                                         kicker->side(),
                                                                         kicker->unum(),
                                                                         kicker->dashCount(),
                                                                         prev_state->ball().pos() ) );
        context.setAnalysisState( new_state );
        return;
    }

    // SideID kicker_side = NEUTRAL;
    // if ( ! target_state->kickers().empty() )
    // {
    //     for ( const CoachPlayerObject * p : target_state->kickers() )
    //     {
    //         kicker_side = p->side();
    //     }
    // }

};

/*-------------------------------------------------------------------*/
std::string
StateNeutral::name()
{
    return "Neutral";
}

/*-------------------------------------------------------------------*/
StateSingleKick::StateSingleKick( const size_t index,
                                  const rcsc::GameTime & begin_time,
                                  const rcsc::GameMode & begin_mode,
                                  const rcsc::SideID kicker_side,
                                  const int kicker_unum,
                                  const int kicker_dash_count,
                                  const rcsc::Vector2D & first_ball_pos )
    : AnalysisState( index, begin_time, begin_mode ),
      M_kicker_side( kicker_side ),
      M_kicker_unum( kicker_unum ),
      M_kicker_dash_count( kicker_dash_count ),
      M_first_ball_pos( first_ball_pos )
{

}

/*-------------------------------------------------------------------*/
void
StateSingleKick::analyze( AnalysisContext & context,
                          const FieldModel & model,
                          const size_t target_index )
{
    FieldState::ConstPtr target_state = model.fieldStates()[ target_index ];

    if ( ! target_state )
    {
        return;
    }

    FieldState::ConstPtr prev_state = model.fieldStates()[ target_index - 1 ];

    if ( ! prev_state )
    {
        return;
    }

    // single kick event
    if ( detectSingleKick( context, *target_state, *prev_state ) )
    {
        return;
    }

    // detect single tackle event
    if ( detectSingleTackle( context, *target_state, *prev_state ) )
    {
        return;
    }

    // detect multiple kick/tackle
    if ( detectMultiKick( context, *target_state, *prev_state ) )
    {
        return;
    }

    //
    if ( detectOutOfBounds( context, *target_state, *prev_state ) )
    {
        return;
    }

    //
    if ( detectGoal( context, *target_state, *prev_state ) )
    {
        return;
    }
}

/*-------------------------------------------------------------------*/
bool
StateSingleKick::detectSingleKick( AnalysisContext & context,
                                   const FieldState & current,
                                   const FieldState & prev )
{
    if ( current.kickers().size() != 1
         || ! current.tacklers().empty()
         || ! current.catchers().empty() )
    {
        return false;
    }

    const CoachPlayerObject * kicker = current.kickers().front();

    // pass
    if ( kicker->side() == M_kicker_side
         && kicker->unum() != M_kicker_unum )
    {
        ActionEvent::ConstPtr event( new Pass( M_kicker_side, M_kicker_unum,
                                               beginTime(), beginMode(), M_first_ball_pos,
                                               kicker->side(), kicker->unum(),
                                               prev.time(), prev.ball().pos(),
                                               true ) );
        context.addActionEvent( event );
    }
    // interception
    else if ( kicker->side() != M_kicker_side
              || kicker->unum() != M_kicker_unum )
    {
        ActionEvent::ConstPtr event( new Interception( M_kicker_side, M_kicker_unum,
                                                       beginTime(), beginMode(), M_first_ball_pos,
                                                       kicker->side(), kicker->unum(),
                                                       prev.time(), prev.ball().pos() ) );
        context.addActionEvent( event );
    }
    // dribble
    else if ( kicker->side() == M_kicker_side
              && kicker->unum() == M_kicker_unum
              && kicker->dashCount() > M_kicker_dash_count )
    {
        ActionEvent::ConstPtr event( new Dribble( kicker->side(), kicker->unum(),
                                                  beginTime(), beginMode(), M_first_ball_pos,
                                                  prev.time(), prev.ball().pos(),
                                                  true ) );
        context.addActionEvent( event );
    }

    //
    // transition to the new single kick state
    //

    std::shared_ptr< AnalysisState > new_state( new StateSingleKick( current.frameIndex() - 1,
                                                                     prev.time(),
                                                                     prev.gameMode(),
                                                                     kicker->side(),
                                                                     kicker->unum(),
                                                                     kicker->dashCount(),
                                                                     prev.ball().pos() ) );
    context.setAnalysisState( new_state );
    return true;
}

/*-------------------------------------------------------------------*/
bool
StateSingleKick::detectSingleTackle( AnalysisContext & context,
                                     const FieldState & current,
                                     const FieldState & prev )
{
    if ( current.tacklers().size() != 1
         || ! current.kickers().empty()
         || ! current.catchers().empty() )
    {
        return false;
    }

    const CoachPlayerObject * tackler = current.tacklers().front();

    // the same kicker lost controll of the ball?
    if ( tackler->side() == M_kicker_side
         && tackler->unum() == M_kicker_unum )
    {
        ActionEvent::ConstPtr event( new BallTouch( M_kicker_side, M_kicker_unum,
                                                    beginTime(), beginMode(), M_first_ball_pos,
                                                    prev.time(), prev.ball().pos() ) );
        context.addActionEvent( event );
    }
    // the receiver lost controll of the ball?
    else if ( tackler->side() == M_kicker_side
              && tackler->unum() != M_kicker_unum )
    {
        ActionEvent::ConstPtr event( new Pass( M_kicker_side, M_kicker_unum,
                                               beginTime(), beginMode(), M_first_ball_pos,
                                               tackler->side(), tackler->unum(),
                                               prev.time(), prev.ball().pos(),
                                               false ) );
        context.addActionEvent( event );
    }
    else if ( tackler->side() != M_kicker_side )
    {
        ActionEvent::ConstPtr event( new Interception( M_kicker_side, M_kicker_unum,
                                                       beginTime(), beginMode(), M_first_ball_pos,
                                                       tackler->side(), tackler->unum(),
                                                       prev.time(), prev.ball().pos() ) );
        context.addActionEvent( event );
    }
    else
    {
        std::cerr << "(StateSingleKick::detectSingleTackle) unexpected reach." << std::endl;
    }

    //
    // transition to the new neutral state
    //

    std::shared_ptr< AnalysisState > new_state( new StateNeutral( current.frameIndex() - 1,
                                                                  prev.time(),
                                                                  prev.gameMode(),
                                                                  prev.ball().pos() ) );
    context.setAnalysisState( new_state );
    return true;
}

/*-------------------------------------------------------------------*/
bool
StateSingleKick::detectMultiKick( AnalysisContext & context,
                                  const FieldState & current,
                                  const FieldState & prev )
{
    if ( ( current.kickers().size() + current.tacklers().size() <= 1 )
         || ! current.catchers().empty() )
    {
        return false;
    }

    // count the number of players
    bool self = false;
    int teammate_count = 0;
    int opponent_count = 0;
    const CoachPlayerObject * teammate = nullptr;
    const CoachPlayerObject * opponent = nullptr;

    for ( const CoachPlayerObject * p : current.kickers() )
    {
        if ( p->side() == M_kicker_side )
        {
            if ( p->unum() == M_kicker_unum )
            {
                self = true;
            }
            else
            {
                ++teammate_count;
                teammate = p;
            }
        }
        else
        {
            ++opponent_count;
            opponent = p;
        }
    }

    if ( opponent )
    {
        ActionEvent::ConstPtr event( new Interception( M_kicker_side, M_kicker_unum,
                                                       beginTime(), beginMode(), M_first_ball_pos,
                                                       opponent->side(), opponent->unum(),
                                                       prev.time(), prev.ball().pos() ) );
        context.addActionEvent( event );
    }
    else if ( self && teammate )
    {
        ActionEvent::ConstPtr event( new BallTouch( M_kicker_side, M_kicker_unum,
                                                    beginTime(), beginMode(), M_first_ball_pos,
                                                    teammate->side(), teammate->unum(),
                                                    prev.time(), prev.ball().pos() ) );
        context.addActionEvent( event );
    }
    else if ( ! self
              && opponent_count == 0
              && teammate_count >= 2 )
    {
        ActionEvent::ConstPtr event( new Pass( M_kicker_side, M_kicker_unum,
                                               beginTime(), beginMode(), M_first_ball_pos,
                                               teammate->side(), Unum_Unknown,
                                               prev.time(), prev.ball().pos(),
                                               true ) );
        context.addActionEvent( event );
    }

    std::shared_ptr< AnalysisState > new_state( new StateNeutral( current.frameIndex() - 1,
                                                                  prev.time(),
                                                                  prev.gameMode(),
                                                                  prev.ball().pos() ) );
    context.setAnalysisState( new_state );
    return true;
}

/*-------------------------------------------------------------------*/
bool
StateSingleKick::detectOutOfBounds( AnalysisContext & context,
                                    const FieldState & current,
                                    const FieldState & prev )
{
    if ( prev.gameMode().type() == GameMode::PlayOn
         && ( current.gameMode().type() == GameMode::KickIn_
              || current.gameMode().type() == GameMode::CornerKick_
              || current.gameMode().type() == GameMode::GoalKick_ ) )
    {
        ActionEvent::ConstPtr event( new BallTouch( M_kicker_side, M_kicker_unum,
                                                    beginTime(), beginMode(), M_first_ball_pos,
                                                    current.time(), current.ball().pos() ) );
        context.addActionEvent( event );

        std::shared_ptr< AnalysisState > new_state( new StateNeutral( current.frameIndex() - 1,
                                                                      prev.time(),
                                                                      prev.gameMode(),
                                                                      prev.ball().pos() ) );
        context.setAnalysisState( new_state );
        return true;
    }

    return false;
}

/*-------------------------------------------------------------------*/
bool
StateSingleKick::detectGoal( AnalysisContext & context,
                             const FieldState & current,
                             const FieldState & prev )
{
    if ( current.gameMode().type() != GameMode::AfterGoal_
         || prev.gameMode().type() == GameMode::AfterGoal_ )
    {
        return false;
    }

    if ( M_kicker_side == current.gameMode().side() )
    {
        ActionEvent::ConstPtr event( new Shoot( M_kicker_side, M_kicker_unum,
                                                beginTime(), beginMode(), M_first_ball_pos,
                                                current.time(), current.ball().pos(),
                                                true ) );
        context.addActionEvent( event );
    }
    else // own goal?
    {
        ActionEvent::ConstPtr event( new Shoot( M_kicker_side, M_kicker_unum,
                                                beginTime(), beginMode(), M_first_ball_pos,
                                                current.time(), current.ball().pos(),
                                                true ) );
        context.addActionEvent( event );
    }

    //
    // transition to the new neutral state
    //

    std::shared_ptr< AnalysisState > new_state( new StateNeutral( current.frameIndex() - 1,
                                                                  current.time(),
                                                                  current.gameMode(),
                                                                  current.ball().pos() ) );
    context.setAnalysisState( new_state );
    return true;
}

/*-------------------------------------------------------------------*/
std::string
StateSingleKick::name()
{
    return "SingleKick";
}
