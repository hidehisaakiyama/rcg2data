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
        using Ptr = std::shared_ptr< Kick >;
        using ConstPtr = std::shared_ptr< const Kick >;

        size_t index_; // index of the states
        rcsc::SideID side_;
        int unum_;
        rcsc::GameTime time_;
        rcsc::GameMode mode_;
        rcsc::Vector2D pos_;
        rcsc::Vector2D vel_;

        Kick( size_t index,
              const rcsc::SideID side,
              const int unum,
              const rcsc::GameTime & time,
              const rcsc::GameMode & mode,
              const rcsc::Vector2D & pos,
              const rcsc::Vector2D & vel )
            : index_( index ),
              side_( side ),
              unum_( unum ),
              time_( time ),
              mode_( mode ),
              pos_( pos ),
              vel_( vel )
        { }
    };

    struct KickSequence {
        using Ptr = std::shared_ptr< KickSequence >;

        rcsc::SideID side_;
        std::vector< Kick::ConstPtr > kicks_;
    };

    std::vector< Kick::ConstPtr > M_single_kicks;

    std::vector< KickSequence::Ptr > M_kick_sequences;

    std::vector< ActionEvent::ConstPtr > M_shoot_events;
    std::vector< ActionEvent::ConstPtr > M_pass_events;
    std::vector< ActionEvent::ConstPtr > M_intercept_events;
public:
    GameAnalyzer();

    bool analyze( const FieldModel & model );

    bool print( const FieldModel & model ) const;

private:
    void extractKickEvent( const FieldModel & model,
                           const size_t idx );

    void extractShootEvent( const FieldModel & model );
    void extractPassEvent( const FieldModel & model );
    void extractPassEventSimple( const FieldModel & model );
    void extractPassEventByKick( const FieldModel & model );

    bool printKickEvents( const FieldModel & model ) const;
    bool printShootEvents( const FieldModel & model ) const;
    bool printPassEvents( const FieldModel & model ) const;
};

#endif
