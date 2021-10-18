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
FieldState::FieldState( const rcsc::GameTime & time,
                        const rcsc::GameMode & mode,
                        const rcsc::rcg::ShowInfoT & show )
    : M_time( time ),
      M_game_mode( mode ),
      M_ball_owner_side( NEUTRAL )
{
    setShowInfo( show );

    M_all_players.reserve( 22 );
    M_left_players.reserve( 11 );
    M_right_players.reserve( 11 );
    std::fill( M_left_players_array, M_left_players_array + 11, nullptr );
    std::fill( M_right_players_array, M_right_players_array + 11, nullptr );
}

/*-------------------------------------------------------------------*/
/*!

 */
FieldState::~FieldState()
{
    for ( CoachPlayerObject::Cont::iterator p = M_all_players.begin(), end = M_all_players.end();
          p != end;
          ++p )
    {
        delete *p;
        *p = nullptr;
    }

    M_all_players.clear();
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldState::setShowInfo( const ShowInfoT & show )
{
    std::cerr << "FieldState::setShowInfo " << show.time_ << std::endl;

}
