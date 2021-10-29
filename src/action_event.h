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

#ifndef ACTION_EVENT_H
#define ACTION_EVENT_H

#include <rcsc/geom/vector_2d.h>
#include <rcsc/game_time.h>
#include <rcsc/types.h>

#include <memory>

class ActionEvent {
public:

    typedef std::shared_ptr< ActionEvent > Ptr;

    enum Type {
        Shoot,
        Pass,
        Dribble,
        Hold,
        None,
    };

private:

    Type M_action_type;

    rcsc::SideID M_player_side;
    int M_player_unum;

    rcsc::GameTime M_start_time;
    rcsc::Vector2D M_start_pos;

    rcsc::GameTime M_end_time;
    rcsc::Vector2D M_end_pos;

    bool M_success;

    // BodyPart M_body_part;

protected:

    ActionEvent( const Type action_type,
                 const rcsc::SideID player_side,
                 const int player_unum,
                 const rcsc::GameTime & start_time,
                 const rcsc::Vector2D & start_pos,
                 const rcsc::GameTime & end_time,
                 const rcsc::Vector2D & end_pos,
                 const bool success );

public:

    Type actionType() const { return M_action_type; }
    rcsc::SideID playerSide() const { return M_player_side; }
    int playerUnum() const { return M_player_unum; }
    const rcsc::GameTime & startTime() const { return M_start_time; }
    const rcsc::Vector2D & startPos() const { return M_start_pos; }
    const rcsc::GameTime & endTime() const { return M_end_time; }
    const rcsc::Vector2D & endPos() const { return M_end_pos; }
    bool success() const { return M_success; }


    virtual
    const char * actionName() const = 0;
};

#endif
