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
ShootHandler::analyze( const FieldModel & model ) const
{

    for ( const auto & s : model.fieldStates() )
    {


    }



}


#endif
