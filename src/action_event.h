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
#include <rcsc/game_mode.h>
#include <rcsc/types.h>

#include <memory>

class ActionEvent {
public:

    using Ptr = std::shared_ptr< ActionEvent >;
    using ConstPtr = std::shared_ptr< const ActionEvent >;

    // Action types defined in SPADL
    // https://socceraction.readthedocs.io/en/latest/documentation/spadl/SPADL_definitions.htlm
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

    rcsc::SideID M_begin_player_side;
    int M_begin_player_unum;

    rcsc::GameTime M_begin_time;
    rcsc::GameMode M_begin_mode;
    rcsc::Vector2D M_begin_pos;

    rcsc::SideID M_end_player_side;
    int M_end_player_unum;

    rcsc::GameTime M_end_time;
    rcsc::Vector2D M_end_pos;

    bool M_success;

    // BodyPart M_body_part;

protected:

    ActionEvent( const Type action_type,
                 const rcsc::SideID begin_player_side,
                 const int begin_player_unum,
                 const rcsc::GameTime & begin_time,
                 const rcsc::GameMode & begin_mode,
                 const rcsc::Vector2D & begin_pos,
                 const rcsc::SideID end_player_side,
                 const int end_player_unum,
                 const rcsc::GameTime & end_time,
                 const rcsc::Vector2D & end_pos,
                 const bool success );

public:

    Type actionType() const { return M_action_type; }
    rcsc::SideID beginPlayerSide() const { return M_begin_player_side; }
    int beginPlayerUnum() const { return M_begin_player_unum; }
    const rcsc::GameTime & beginTime() const { return M_begin_time; }
    const rcsc::GameMode & beginMode() const { return M_begin_mode; }
    const rcsc::Vector2D & beginPos() const { return M_begin_pos; }
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
           const rcsc::GameTime & begin_time,
           const rcsc::GameMode & begin_mode,
           const rcsc::Vector2D & begin_pos,
           const rcsc::GameTime & end_time,
           const rcsc::Vector2D & end_pos,
           const bool success );

    const char * actionName() const override;
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
          const rcsc::GameTime & begin_time,
          const rcsc::GameMode & begin_mode,
          const rcsc::Vector2D & begin_pos,
          const rcsc::SideID receiver_side,
          const int receiver_unum,
          const rcsc::GameTime & end_time,
          const rcsc::Vector2D & end_pos,
          const bool success );

    const char * actionName() const override;

};

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class Tackle
    : public ActionEvent {
private:

public:

    Tackle( const rcsc::SideID begin_ball_holder_side,
            const int begin_ball_holder_unum,
            const rcsc::GameTime & begin_time,
            const rcsc::GameMode & begin_mode,
            const rcsc::Vector2D & begin_pos,
            const rcsc::SideID tackler_side,
            const int tackler_unum,
            const rcsc::GameTime & end_time,
            const rcsc::Vector2D & end_pos );

    const char * actionName() const override;

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
                  const rcsc::GameTime & begin_time,
                  const rcsc::GameMode & begin_mode,
                  const rcsc::Vector2D & begin_pos,
                  const rcsc::SideID receiver_side,
                  const int receiver_unum,
                  const rcsc::GameTime & end_time,
                  const rcsc::Vector2D & end_pos );

    const char * actionName() const override;

};


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class KeeperSave
    : public ActionEvent {
private:

public:

    KeeperSave( const rcsc::SideID begin_ball_holder_side,
                const int begin_ball_holder_unum,
                const rcsc::GameTime & begin_time,
                const rcsc::GameMode & begin_mode,
                const rcsc::Vector2D & begin_pos,
                const rcsc::SideID keeper_side,
                const int keeper_unum,
                const rcsc::GameTime & end_time,
                const rcsc::Vector2D & end_pos );

    const char * actionName() const override;

};


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class BallTouch
    : public ActionEvent {
private:

public:

    BallTouch( const rcsc::SideID player_side,
               const int player_unum,
               const rcsc::GameTime & begin_time,
               const rcsc::GameMode & begin_mode,
               const rcsc::Vector2D & begin_pos,
               const rcsc::GameTime & end_time,
               const rcsc::Vector2D & end_pos );

    BallTouch( const rcsc::SideID player_side,
               const int player_unum,
               const rcsc::GameTime & begin_time,
               const rcsc::GameMode & begin_mode,
               const rcsc::Vector2D & begin_pos,
               const rcsc::SideID end_player_side,
               const int end_player_unum,
               const rcsc::GameTime & end_time,
               const rcsc::Vector2D & end_pos );


    const char * actionName() const override;

};

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class Dribble
    : public ActionEvent {
private:

public:

    Dribble( const rcsc::SideID kicker_side,
             const int kicker_unum,
             const rcsc::GameTime & begin_time,
             const rcsc::GameMode & begin_mode,
             const rcsc::Vector2D & begin_pos,
             const rcsc::GameTime & end_time,
             const rcsc::Vector2D & end_pos,
             const bool success );

    const char * actionName() const override;

};

#endif
