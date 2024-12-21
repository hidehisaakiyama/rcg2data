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

    struct Player {
        rcsc::SideID side_;
        int unum_;
        int dash_count_;

        Player()
            : side_( rcsc::NEUTRAL ),
              unum_( rcsc::Unum_Unknown ),
              dash_count_( 0 )
        { }

        explicit Player( const rcsc::CoachPlayerObject * p )
            : side_( p->side() ),
              unum_( p->unum() ),
              dash_count_( p->dashCount() )
        { }
    };

    std::vector< ActionEvent::ConstPtr > M_action_events;

    rcsc::GameTime M_touch_time;
    rcsc::GameMode M_touch_mode;
    rcsc::Vector2D M_touched_ball_pos;
    std::vector< Player > M_touchers; // kick or tackle players
    std::vector< Player > M_kickers;
    std::vector< Player > M_kickers_left;
    std::vector< Player > M_kickers_right;
    std::vector< Player > M_tacklers;
    std::vector< Player > M_tacklers_left;
    std::vector< Player > M_tacklers_right;
    std::vector< Player > M_catchers;

public:
    GameAnalyzer();

    bool analyze( const FieldModel & model );

    std::ostream & print( std::ostream & os ) const;

private:
    void clearBallTouchers();

    void updateBallToucher( const FieldModel & model,
                            const size_t frame_index );

    // before updateBallToucher
    void analyzeSingleKick( const FieldState & prev,
                            const FieldState & current );
    void analyzeSingleTackle( const FieldState & prev,
                              const FieldState & current );
    // void analyzeMultiTouch( const FieldState & prev,
    //                        const FieldState & current );

    // after updateBallToucher
    void analyzeGoal( const FieldState & prev,
                      const FieldState & current );
    void analyzeOutOfBounds( const FieldState & prev,
                             const FieldState & current );
};

#endif
