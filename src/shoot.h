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

#ifndef SHOOT_H
#define SHOOT_H

#include <rcsc/geom/vector_2d.h>
#include <rcsc/game_time.h>
#include <rcsc/types.h>

#include <memory>

class Shoot {
public:

    typedef std::shared_ptr< Shoot > Ptr;

private:

    rcsc::SideID M_kicker_side;
    int M_kicker_unum;

    rcsc::GameTime M_start_time;
    rcsc::Vector2D M_start_pos;

    rcsc::GameTime M_end_time;
    rcsc::Vector2D M_end_pos;

public:

    Shoot( const rcsc::SideID kicker_side,
           const int kicker_unum,
           const rcsc::GameTime & start_time,
           const rcsc::Vector2D & start_pos,
           const rcsc::GameTime & end_time,
           const rcsc::Vector2D & end_pos );

    rcsc::SideID kickerSide() const { return M_kicker_side; }
    int kickerUnum() const { return M_kicker_unum; }
    const rcsc::GameTime & startTime() const { return M_start_time; }
    const rcsc::Vector2D & startPos() const { return M_start_pos; }
    const rcsc::GameTime & endTime() const { return M_end_time; }
    const rcsc::Vector2D & endPos() const { return M_end_pos; }

};

#endif
