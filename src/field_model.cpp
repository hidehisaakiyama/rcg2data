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

#include "field_model.h"

using namespace rcsc;
using namespace rcsc::rcg;

/*-------------------------------------------------------------------*/
/*!

 */
FieldModel::FieldModel()
{
    M_field_states.reserve( 12000 );
}

/*-------------------------------------------------------------------*/
/*!

 */
FieldModel::~FieldModel()
{

}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldModel::updateTeamNames( const std::string & team_l,
                             const std::string & team_r )
{
    M_left_team_name = team_l;
    M_right_team_name = team_r;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldModel::updateTime( const int received_time,
                        const bool by_show )
{
    GameTime new_time( received_time, 0 );

    if ( M_field_states.empty() )
    {
        M_time = new_time;
        return;
    }

    FieldState::Ptr last_state = M_field_states.back();
    if ( ! last_state )
    {
        M_time = new_time;
        return;
    }

    if ( received_time > last_state->time().cycle() )
    {
        M_time = new_time;
        return;
    }

    if ( by_show
         && last_state->time().cycle() == received_time )
    {
        M_time.assign( received_time,
                       last_state->time().stopped() + 1 );
    }
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldModel::updateGameMode( const PlayMode pmode )
{
    static const char * playmode_strings[] = PLAYMODE_STRINGS;

    M_game_mode.update( playmode_strings[pmode], M_time );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldModel::updateScore( const int score_l,
                         const int score_r )
{
    M_game_mode.setScore( score_l, score_r );
}

/*-------------------------------------------------------------------*/
/*!

 */
void
FieldModel::setNewState( const ShowInfoT & show )
{
    updateTime( show.time_, true );

    FieldState::ConstPtr prev_state;
    if ( ! M_field_states.empty() )
    {
        prev_state = M_field_states.back();
    }

    FieldState::Ptr ptr( new FieldState( M_time, M_game_mode, show, prev_state ) );
    M_field_states.emplace_back( ptr );
}