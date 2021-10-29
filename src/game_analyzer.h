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

#include "field_model.h"
#include "action_event.h"

class GameAnalyzer {
private:

    FieldModel M_field_model;

    std::vector< ActionEvent::Ptr > M_shoot_events;

public:

    GameAnalyzer();

    bool read( const std::string & filepath );
    bool analyze( const std::string & filepath );

    bool print() const;

private:

    void analyzeShoot();
    bool printShoot() const;
};

#endif
