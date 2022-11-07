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

#include "field_state.h"

using namespace rcsc;
using namespace rcsc::rcg;


/*-------------------------------------------------------------------*/
/*!

 */
FieldState::FieldState( const GameTime & time )

    : M_time( time ),
      M_game_mode(),
      M_score_left( 0 ),
      M_score_right( 0 )
{
    M_all_players.reserve( 22 );
    M_left_players.reserve( 11 );
    M_right_players.reserve( 11 );
    std::fill( M_left_players_array, M_left_players_array + 11, nullptr );
    std::fill( M_right_players_array, M_right_players_array + 11, nullptr );
}

/*-------------------------------------------------------------------*/
/*!

 */
FieldState::FieldState( const GameTime & time,
                        const GameMode & mode,
                        const ShowInfoT & show,
                        const ConstPtr & prev_state )

    : M_time( time ),
      M_game_mode( mode ),
      M_score_left( 0 ),
      M_score_right( 0 )
{
    setScore( mode.scoreLeft(), mode.scoreRight() );

    M_all_players.reserve( 22 );
    M_left_players.reserve( 11 );
    M_right_players.reserve( 11 );
    std::fill( M_left_players_array, M_left_players_array + 11, nullptr );
    std::fill( M_right_players_array, M_right_players_array + 11, nullptr );

    setBall( show.ball_, prev_state );
    for ( int i = 0; i < MAX_PLAYER*2; ++i )
    {
        setPlayer( show.player_[i], prev_state );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
FieldState::~FieldState()
{
    for ( auto p : M_all_players )
    {
        delete p;
        p = nullptr;
    }

    M_all_players.clear();
}

/*-------------------------------------------------------------------*/
/*!

 */
const CoachPlayerObject *
FieldState::getPlayer( const SideID side,
                       const int unum ) const
{
    if ( unum < 1 || 11 < unum )
    {
        return nullptr;
    }

    if ( side == LEFT )
    {
        return M_left_players_array[unum-1];
    }

    if ( side == RIGHT )
    {
        return M_right_players_array[unum-1];
    }

    return nullptr;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldState::setScore( const int score_left,
                      const int score_right )
{
    M_score_left = score_left;
    M_score_right = score_right;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldState::setBall( const BallT & ball,
                     const ConstPtr & prev_state )
{
    if ( ball.hasVelocity() )
    {
        M_ball.setValue( ball.x(), ball.y(), ball.deltaX(), ball.deltaY() );
    }
    else
    {
        double vx = 0.0;
        double vy = 0.0;
        if ( prev_state )
        {
            vx = prev_state->ball().pos().x - ball.x();
            vy = prev_state->ball().pos().y - ball.y();
        }

        M_ball.setValue( ball.x(), ball.y(), vx, vy );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldState::setPlayer( const PlayerT & player,
                       const ConstPtr & prev_state )
{
    if ( ! player.isAlive() )
    {
        return;
    }

    CoachPlayerObject * p = nullptr;

    if ( prev_state )
    {
        const CoachPlayerObject * pp = prev_state->getPlayer( player.side(), player.unum() );
        if ( pp )
        {
            p = pp->clone();
            p->update( player );
        }
    }

    if ( ! p )
    {
        p = new CoachPlayerObject();
        p->update( player );
    }

    M_all_players.push_back( p );

    if ( p->side() == LEFT )
    {
        M_left_players.push_back( p );
        if ( 1 <= p->unum() && p->unum() <= 11 )
        {
            M_left_players_array[p->unum() - 1] = p;
        }
    }
    else if ( p->side() == RIGHT )
    {
        M_right_players.push_back( p );
        if ( 1 <= p->unum() && p->unum() <= 11 )
        {
            M_right_players_array[p->unum() - 1] = p;
        }
    }

    if ( player.isCollidedBall() )
    {
        M_ball_colliders.push_back( p );
    }

    if ( player.isKicking() )
    {
        M_kickers.push_back( p );
    }

    if ( player.isTackling() )
    {
        M_tacklers.push_back( p );
    }

    if ( player.isCatching() )
    {
        M_catchers.push_back( p );
    }
}
