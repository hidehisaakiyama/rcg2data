// -*-c++-*-

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "shoot_handler.h"

#include <rcsc/common/server_param.h>
#include <rcsc/geom/segment_2d.h>
#include <rcsc/geom/vector_2d.h>
#include <rcsc/soccer_math.h>

#include <iostream>

using namespace rcsc;
using namespace rcsc::rcg;

namespace {

bool
check_ball_is_moving_to_goal( const Vector2D & goal_left,
                              const Vector2D & goal_right,
                              const Vector2D & ball_pos,
                              const Vector2D & ball_vel )
{
    const Vector2D ball_end = inertia_final_point( ball_pos,
                                                   ball_vel,
                                                   ServerParam::i().ballDecay() );

    const Segment2D goal( goal_left, goal_right );
    const Segment2D ball_move( ball_pos, ball_end );

    return goal.intersectsExceptEndpoint( ball_move );
}

}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleEOF()
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleShow( const ShowInfoT & show )
{
    int kicker = -1;

    for ( int i = 0; i < MAX_PLAYER*2; ++i )
    {
        const PlayerT & p = show.player_[i];

        if ( p.isKicking()
             || p.isTackling() )
        {
            if ( kicker != -1 )
            {
                // detected several kickers
                kicker = -1;
                break;
            }

            kicker = i;
        }
    }

    if ( 0 <= kicker && kicker < MAX_PLAYER )
    {
        //std::cout <<  "[" << show.time_ << "] detect left kicker " << kicker + 1 << std::endl;
        // const Vector2D goal_l( -ServerParam::i().pitchHalfLength(),
        //                        -ServerParam::i().goalHalfWidth() - 1.5 );
        // const Vector2D goal_r( -ServerParam::i().pitchHalfLength(),
        //                        +ServerParam::i().goalHalfWidth() + 1.5 );
        // const Vector2D ball_pos( show.ball_.x(), show.ball_.y() );
        // const Vector2D ball_vel( show.ball_.deltaX(), show.ball_.deltaY() );
        // if ( check_ball_is_moving_to_goal( goal_l, goal_r, ball_pos, ball_vel ) )
        // {
        //     std::cout << "[" << show.time_ << "] detect left shoot "
        //               << " [" << kicker + 1 << "]"
        //               << std::endl;
        // }
    }
    else if ( MAX_PLAYER <= kicker && kicker < MAX_PLAYER*2 )
    {
        //std::cout <<  "[" << show.time_ << "] detect right kicker " << kicker + 1 - MAX_PLAYER << std::endl;
        // const Vector2D goal_l( +ServerParam::i().pitchHalfLength(),
        //                        -ServerParam::i().goalHalfWidth() - 1.5 );
        // const Vector2D goal_r( +ServerParam::i().pitchHalfLength(),
        //                        +ServerParam::i().goalHalfWidth() + 1.5 );
        // const Vector2D ball_pos( show.ball_.x(), show.ball_.y() );
        // const Vector2D ball_vel( show.ball_.deltaX(), show.ball_.deltaY() );
        // if ( check_ball_is_moving_to_goal( goal_l, goal_r, ball_pos, ball_vel ) )
        // {
        //     std::cout << "[" << show.time_ << "] detect right shoot"
        //               << " [" << kicker + 1 - MAX_PLAYER<< "]"
        //               << std::endl;
        // }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleMsg( const int ,
                         const int ,
                         const std::string & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleDraw( const int ,
                          const drawinfo_t & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handlePlayMode( const int time,
                              const PlayMode pm )
{
    if ( pm == PM_AfterGoal_Left )
    {
        std::cout << '[' << time << "] goal left" << std::endl;
    }
    else if ( pm == PM_AfterGoal_Right )
    {
        std::cout << '[' << time << "] goal right" << std::endl;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleTeam( const int ,
                          const TeamT & ,
                          const TeamT & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleServerParam( const std::string & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handlePlayerParam( const std::string & )
{
    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handlePlayerType( const std::string & )
{
    return true;
}
