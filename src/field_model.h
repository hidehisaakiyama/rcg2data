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

#ifndef FIELD_MODEL_H
#define FIELD_MODEL_H

#include "field_state.h"

#include <rcsc/game_mode.h>
#include <rcsc/game_time.h>
#include <rcsc/rcg/types.h>

class FieldModel {
private:
    std::string M_left_team_name;
    std::string M_right_team_name;

    rcsc::GameTime M_time; // last updated time
    rcsc::GameMode M_game_mode; // last game mode

    std::vector< FieldState::Ptr > M_field_states;

    // not used
    FieldModel( const FieldModel & ) = delete;
    const FieldState & operator=( const FieldState & ) = delete;

public:
    FieldModel();
    ~FieldModel();

    const std::string leftTeamName() const { return M_left_team_name; }
    const std::string rightTeamName() const { return M_right_team_name; }
    const std::vector< FieldState::Ptr > & fieldStates() const { return M_field_states; }

    void updateTeamNames( const std::string & team_l,
                          const std::string & team_r );

    void updateTime( const int time,
                     const bool by_show );
    void updateGameMode( const rcsc::PlayMode pmode );
    void updateScore( const int score_l,
                      const int score_r );
    void setNewState( const rcsc::rcg::ShowInfoT & show );

    ssize_t findState( const rcsc::GameTime & target_time ) const;

    ssize_t findKickersStateBefore( const rcsc::GameTime & target_time ) const;

    FieldState::ConstPtr getState( const size_t idx ) const;
};

#endif
