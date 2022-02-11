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

#ifndef GAME_ANALYZER_H
#define GAME_ANALYZER_H

#include "action_event.h"
#include "field_model.h"

class GameAnalyzer {
private:

    struct Kick {
        size_t index_; // index of the states
        rcsc::SideID side_;
        int unum_;
        rcsc::GameTime time_;
        rcsc::Vector2D ball_pos_;
        rcsc::Vector2D ball_vel_;
    };

    std::vector< Kick > M_kicks;

    std::vector< ActionEvent::Ptr > M_shoot_events;

public:
    GameAnalyzer();

    bool analyze( const FieldModel & model );

    bool print( const FieldModel & model ) const;

private:
    void extractKickEvent( const FieldModel & model,
                           const size_t idx );

    void analyzeShoot( const FieldModel & model );

    bool printKickEvent( const FieldModel & model ) const;
    bool printShoot( const FieldModel & model ) const;
};

#endif
