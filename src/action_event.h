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

    using Ptr = std::shared_ptr< ActionEvent >;
    using ConstPtr = std::shared_ptr< const ActionEvent >;

    // Action types defined in SPADL
    // https://socceraction.readthedocs.io/en/latest/documentation/SPADL_definitions.html
    enum Type {
        Pass,
        Cross,
        KickIn, // ThrowIn
        CrossedFreeKick,
        // ShortFreeKick,
        // CrossedCorner,
        ShortCorner,
        TakeOn, // Attempt to dribble past opponent
        Foul,
        Tackle,
        Interception,
        Shot,
        PenaltyShot,
        FreeKickShot, // diret freekick on goal
        KeeperSave, // Keeper saves a shot on goal
        KeeperClaim, // Keeper catches a cross
        KeeperPunch, // Keeper Punches the ball clear
        // KeeperPickUp, // Keeper picks up the ball
        Clearance,
        BallTouch, // Player makes a bad touch and loses the ball
        Dribble, // Player dribbles at least 3 meters with the ball
        GoalKick,
        None,
    };

private:

    Type M_action_type;

    rcsc::SideID M_start_player_side;
    int M_start_player_unum;

    rcsc::GameTime M_start_time;
    rcsc::Vector2D M_start_pos;

    rcsc::SideID M_end_player_side;
    int M_end_player_unum;

    rcsc::GameTime M_end_time;
    rcsc::Vector2D M_end_pos;

    bool M_success;

    // BodyPart M_body_part;

protected:

    ActionEvent( const Type action_type,
                 const rcsc::SideID start_player_side,
                 const int start_player_unum,
                 const rcsc::GameTime & start_time,
                 const rcsc::Vector2D & start_pos,
                 const rcsc::SideID end_player_side,
                 const int end_player_unum,
                 const rcsc::GameTime & end_time,
                 const rcsc::Vector2D & end_pos,
                 const bool success );

public:

    Type actionType() const { return M_action_type; }
    rcsc::SideID startPlayerSide() const { return M_start_player_side; }
    int startPlayerUnum() const { return M_start_player_unum; }
    const rcsc::GameTime & startTime() const { return M_start_time; }
    const rcsc::Vector2D & startPos() const { return M_start_pos; }
    rcsc::SideID endPlayerSide() const { return M_end_player_side; }
    int endPlayerUnum() const { return M_end_player_unum; }
    const rcsc::GameTime & endTime() const { return M_end_time; }
    const rcsc::Vector2D & endPos() const { return M_end_pos; }
    bool isSuccess() const { return M_success; }

    virtual
    const char * actionName() const = 0;

    virtual
    std::ostream & printCSV( std::ostream & os ) const;

    static
    std::ostream & print_header_csv( std::ostream & os );
};

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class Shoot
    : public ActionEvent {
private:

public:

    Shoot( const rcsc::SideID kicker_side,
           const int kicker_unum,
           const rcsc::GameTime & start_time,
           const rcsc::Vector2D & start_pos,
           const rcsc::GameTime & end_time,
           const rcsc::Vector2D & end_pos,
           const bool success );

    const char * actionName() const;
};


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class Pass
    : public ActionEvent {
private:

public:

    Pass( const rcsc::SideID kicker_side,
          const int kicker_unum,
          const rcsc::GameTime & start_time,
          const rcsc::Vector2D & start_pos,
          const rcsc::SideID receiver_side,
          const int receiver_unum,
          const rcsc::GameTime & end_time,
          const rcsc::Vector2D & end_pos,
          const bool success );

    const char * actionName() const;

};


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class Interception
    : public ActionEvent {
private:

public:

    Interception( const rcsc::SideID kicker_side,
                  const int kicker_unum,
                  const rcsc::GameTime & start_time,
                  const rcsc::Vector2D & start_pos,
                  const rcsc::SideID receiver_side,
                  const int receiver_unum,
                  const rcsc::GameTime & end_time,
                  const rcsc::Vector2D & end_pos );

    const char * actionName() const;

};


#endif
