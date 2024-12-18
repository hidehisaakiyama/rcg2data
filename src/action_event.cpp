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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "action_event.h"

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
ActionEvent::ActionEvent( const Type action_type,
                          const SideID begin_player_side,
                          const int begin_player_unum,
                          const GameTime & begin_time,
                          const GameMode & begin_mode,
                          const Vector2D & begin_pos,
                          const SideID end_player_side,
                          const int end_player_unum,
                          const GameTime & end_time,
                          const Vector2D & end_pos,
                          const bool success )
    : M_action_type( action_type ),
      M_begin_player_side( begin_player_side ),
      M_begin_player_unum( begin_player_unum ),
      M_begin_time( begin_time ),
      M_begin_mode( begin_mode ),
      M_begin_pos( begin_pos ),
      M_end_player_side( end_player_side ),
      M_end_player_unum( end_player_unum ),
      M_end_time( end_time ),
      M_end_pos( end_pos ),
      M_success( success )
{

}

/*-------------------------------------------------------------------*/
std::ostream &
ActionEvent::printCSV( std::ostream & os ) const
{
    os << actionName() << ','
       << side_str( beginPlayerSide() ) << ','
       << beginPlayerUnum() << ','
       << beginTime().cycle() << ','
       << beginMode().toCString() << ','
       << beginPos().x << ',' << beginPos().y << ','
       << side_str( endPlayerSide() ) << ','
       << endPlayerUnum() << ','
       << endTime().cycle() << ','
       << endPos().x << ',' << endPos().y << ','
       << std::boolalpha << isSuccess() << '\n';

    return os;
}

/*-------------------------------------------------------------------*/
std::ostream &
ActionEvent::print_header_csv( std::ostream & os )
{
    os << "Type,"
       << "Side1,"
       << "Unum1,"
       << "Time1,"
       << "Mode1,"
       << "X1,Y1,"
       << "Side2,"
       << "Unum2,"
       << "Time2,"
       << "X2,Y2,"
       << "Success"
       << '\n';
    return os;
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

 */
Shoot::Shoot( const SideID kicker_side,
              const int kicker_unum,
              const GameTime & begin_time,
              const GameMode & begin_mode,
              const Vector2D & begin_pos,
              const GameTime & end_time,
              const Vector2D & end_pos,
              const bool success )
    : ActionEvent( ActionEvent::Shot,
                   kicker_side, kicker_unum,
                   begin_time, begin_mode, begin_pos,
                   NEUTRAL, Unum_Unknown,
                   end_time, end_pos,
                   success )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
const char *
Shoot::actionName() const
{
    return "Shoot";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

 */
Pass::Pass( const SideID kicker_side,
            const int kicker_unum,
            const GameTime & begin_time,
            const GameMode & begin_mode,
            const Vector2D & begin_pos,
            const SideID receiver_side,
            const int receiver_unum,
            const GameTime & end_time,
            const Vector2D & end_pos,
            const bool success )
    : ActionEvent( ActionEvent::Pass,
                   kicker_side, kicker_unum,
                   begin_time, begin_mode, begin_pos,
                   receiver_side, receiver_unum,
                   end_time, end_pos,
                   success )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
const char *
Pass::actionName() const
{
    return "Pass";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

 */
Tackle::Tackle( const SideID begin_ball_holder_side,
                const int begin_ball_holder_unum,
                const GameTime & begin_time,
                const GameMode & begin_mode,
                const Vector2D & begin_pos,
                const SideID tackler_side,
                const int tackler_unum,
                const GameTime & end_time,
                const Vector2D & end_pos )
    : ActionEvent( ActionEvent::Tackle,
                   begin_ball_holder_side, begin_ball_holder_unum,
                   begin_time, begin_mode, begin_pos,
                   tackler_side, tackler_unum,
                   end_time, end_pos,
                   true )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
const char *
Tackle::actionName() const
{
    return "Tackle";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

 */
Foul::Foul( const SideID begin_ball_holder_side,
            const int begin_ball_holder_unum,
            const GameTime & begin_time,
            const GameMode & begin_mode,
            const Vector2D & begin_pos,
            const SideID tackler_side,
            const int tackler_unum,
            const GameTime & end_time,
            const Vector2D & end_pos )
    : ActionEvent( ActionEvent::Tackle,
                   begin_ball_holder_side, begin_ball_holder_unum,
                   begin_time, begin_mode, begin_pos,
                   tackler_side, tackler_unum,
                   end_time, end_pos,
                   true )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
const char *
Foul::actionName() const
{
    return "Foul";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

 */
Interception::Interception( const SideID kicker_side,
                            const int kicker_unum,
                            const GameTime & begin_time,
                            const GameMode & begin_mode,
                            const Vector2D & begin_pos,
                            const SideID receiver_side,
                            const int receiver_unum,
                            const GameTime & end_time,
                            const Vector2D & end_pos )
    : ActionEvent( ActionEvent::Interception,
                   kicker_side, kicker_unum,
                   begin_time, begin_mode, begin_pos,
                   receiver_side, receiver_unum,
                   end_time, end_pos,
                   true )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
const char *
Interception::actionName() const
{
    return "Interception";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*!

 */
KeeperSave::KeeperSave( const SideID begin_ball_holder_side,
                        const int begin_ball_holder_unum,
                        const GameTime & begin_time,
                        const GameMode & begin_mode,
                        const Vector2D & begin_pos,
                        const SideID keeper_side,
                        const int keeper_unum,
                        const GameTime & end_time,
                        const Vector2D & end_pos )
    : ActionEvent( ActionEvent::KeeperSave,
                   begin_ball_holder_side, begin_ball_holder_unum,
                   begin_time, begin_mode, begin_pos,
                   keeper_side, keeper_unum,
                   end_time, end_pos,
                   true )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
const char *
KeeperSave::actionName() const
{
    return "KeeperSave";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
BallTouch::BallTouch( const rcsc::SideID player_side,
                      const int player_unum,
                      const rcsc::GameTime & begin_time,
                      const rcsc::GameMode & begin_mode,
                      const rcsc::Vector2D & begin_pos,
                      const rcsc::GameTime & end_time,
                      const rcsc::Vector2D & end_pos )
    : ActionEvent( ActionEvent::BallTouch,
                   player_side, player_unum,
                   begin_time, begin_mode, begin_pos,
                   player_side, player_unum,
                   end_time, end_pos,
                   false )
{

}

/*-------------------------------------------------------------------*/
BallTouch::BallTouch( const rcsc::SideID begin_player_side,
                      const int begin_player_unum,
                      const rcsc::GameTime & begin_time,
                      const rcsc::GameMode & begin_mode,
                      const rcsc::Vector2D & begin_pos,
                      const rcsc::SideID end_player_side,
                      const int end_player_unum,
                      const rcsc::GameTime & end_time,
                      const rcsc::Vector2D & end_pos )
    : ActionEvent( ActionEvent::BallTouch,
                   begin_player_side, begin_player_unum,
                   begin_time, begin_mode, begin_pos,
                   end_player_side, end_player_unum,
                   end_time, end_pos,
                   false )
{

}

/*-------------------------------------------------------------------*/
const char *
BallTouch::actionName() const
{
    return "BallTouch";
}


/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
Dribble::Dribble( const SideID kicker_side,
                  const int kicker_unum,
                  const GameTime & begin_time,
                  const GameMode & begin_mode,
                  const Vector2D & begin_pos,
                  const GameTime & end_time,
                  const Vector2D & end_pos,
                  const bool success )
    : ActionEvent( ActionEvent::Dribble,
                   kicker_side, kicker_unum,
                   begin_time, begin_mode, begin_pos,
                   kicker_side, kicker_unum,
                   end_time, end_pos,
                   success )
{

}

/*-------------------------------------------------------------------*/
const char *
Dribble::actionName() const
{
    return "Dribble";
}

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
OutOfBounds::OutOfBounds( const rcsc::SideID kicker_side,
                          const int kicker_unum,
                          const rcsc::GameTime & begin_time,
                          const rcsc::GameMode & begin_mode,
                          const rcsc::Vector2D & begin_pos,
                          const rcsc::GameTime & end_time,
                          const rcsc::Vector2D & end_pos )
    : ActionEvent( ActionEvent::Dribble,
                   kicker_side, kicker_unum,
                   begin_time, begin_mode, begin_pos,
                   NEUTRAL, Unum_Unknown,
                   end_time, end_pos,
                   false )
{

}

/*-------------------------------------------------------------------*/
const char *
OutOfBounds::actionName() const
{
    return "OutOfBounds";
}
