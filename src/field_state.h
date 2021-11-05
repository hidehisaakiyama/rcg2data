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

#ifndef FIELD_STATE_H
#define FIELD_STATE_H

#include <rcsc/coach/coach_ball_object.h>
#include <rcsc/coach/coach_player_object.h>
#include <rcsc/game_mode.h>
#include <rcsc/game_time.h>

#include <memory>

class FieldState {
public:
    typedef std::shared_ptr< FieldState > Ptr;
    typedef std::shared_ptr< const FieldState > ConstPtr;

private:
    rcsc::GameTime M_time;
    rcsc::GameMode M_game_mode;
    int M_score_left;
    int M_score_right;

    // TODO: penalty kick state

    rcsc::CoachBallObject M_ball;
    rcsc::CoachPlayerObject::Cont M_all_players; // instance
    rcsc::CoachPlayerObject::Cont M_left_players; // reference
    rcsc::CoachPlayerObject::Cont M_right_players; // reference

    //! left player array (reference)
    rcsc::CoachPlayerObject * M_left_players_array[11];
    //! right player array (reference)
    rcsc::CoachPlayerObject * M_right_players_array[11];

    // players that performed a kick/tackle command successfully (reference)
    rcsc::CoachPlayerObject::Cont M_kickers;

    //rcsc::SideID M_ball_owner_side; //!< estimated ball owner team side.
    //const rcsc::CoachPlayerObject * M_ball_owner; // estimated current ball owner
    //const rcsc::CoachPlayerObject * M_fastest_intercept_player; // estimated next ball owner
    //const rcsc::CoachPlayerObject * M_fastest_intercept_teammate;
    //const rcsc::CoachPlayerObject * M_fastest_intercept_opponent;

    // not used
    FieldState() = delete;
    FieldState( const FieldState & ) = delete;
    const FieldState & operator=( const FieldState & ) = delete;

public:

    explicit FieldState( const rcsc::GameTime & time );

    FieldState( const rcsc::GameTime & time,
                const rcsc::GameMode & mode,
                const rcsc::rcg::ShowInfoT & show,
                const ConstPtr & prev_state );
    ~FieldState();

    const rcsc::GameTime & time() const
    {
        return M_time;
    }

    const rcsc::GameMode & gameMode() const
    {
        return M_game_mode;
    }

    int scoreLeft() const { return M_score_left; }
    int scoreRight() const { return M_score_right; }

    const rcsc::CoachBallObject & ball() const
    {
        return M_ball;
    }

    const rcsc::CoachPlayerObject::Cont & allPlayers() const
    {
        return M_all_players;
    }

    const rcsc::CoachPlayerObject * getPlayer( const rcsc::SideID side,
                                               const int unum ) const;

    const rcsc::CoachPlayerObject::Cont & kickers() const
    {
        return M_kickers;
    }

private:
    void setScore( const int score_left,
                   const int score_right );
    void setBall( const rcsc::rcg::BallT & ball,
                  const ConstPtr & prev_state );
    void setPlayer( const rcsc::rcg::PlayerT & player,
                    const ConstPtr & prev_state );
    void setKickers();
};

#endif
