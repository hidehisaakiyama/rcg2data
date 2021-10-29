// -*-c++-*-

/*!
  \file game_analyer.cpp
  \brief game analyzer Source File
*/

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

#include "game_analyzer.h"

#include "shoot.h"

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

*/
GameAnalyzer::GameAnalyzer()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GameAnalyzer::analyze( const FieldModel & model )
{
    analyzeShoot( model );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
void
GameAnalyzer::analyzeShoot( const FieldModel & model )
{
    const std::vector< FieldState::Ptr > & states = model.fieldStates();
    const size_t max_states = states.size();

    GameMode mode;

    for ( size_t i = 1; i < max_states; ++i )
    {
        FieldState::ConstPtr s = states[i];
        if ( mode.type() != GameMode::AfterGoal_
             && s->gameMode().type() == GameMode::AfterGoal_ )
        {
            std::cerr << "Detect shoot " << s->time() << std::endl;
            SideID kicker_side = NEUTRAL;
            int kicker_unum = Unum_Unknown;
            Vector2D start_pos = Vector2D::INVALIDATED;
            GameTime start_time;
            Vector2D end_pos = s->ball().pos();
            GameTime end_time = s->time();

            // find the kick
            for ( size_t j = i - 1; j > 0; --j )
            {
                if ( ! states[j]->kickers().empty() )
                {
                    std::cerr << "Find kicker " << states[j]->time() << std::endl;
                    const rcsc::CoachPlayerObject * kicker = nullptr;
                    for ( CoachPlayerObject::Cont::const_iterator p = states[j]->kickers().begin();
                          p != states[j]->kickers().end();
                          ++p )
                    {
                        if ( (*p)->side() == s->gameMode().side() )
                        {
                            kicker = *p;
                            break;
                        }
                    }

                    if ( kicker )
                    {
                        kicker_side = kicker->side();
                        kicker_unum = kicker->unum();
                        start_pos = states[j-1]->ball().pos();
                        start_time = states[j-1]->time();
                        break;
                    }
                }
            }

            if ( kicker_side != NEUTRAL
                 && start_pos.isValid() )
            {
                ActionEvent::Ptr ptr( new Shoot( kicker_side, kicker_unum, start_time, start_pos, end_time, end_pos, true ) );
                M_shoot_events.emplace_back( ptr );
            }
        }

        mode = s->gameMode();
    }
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GameAnalyzer::print( const FieldModel & model ) const
{
    printShoot( model );
    return true;
}


/*-------------------------------------------------------------------*/
/*!

*/
bool
GameAnalyzer::printShoot( const FieldModel & model ) const
{
    const std::string team_l = model.leftTeamName();
    const std::string team_r = model.rightTeamName();

    std::cout << "Shoot,"
              << "TeamName,"
              << "Side,"
              << "Kicker,"
              << "StartTime,"
              << "StartX,"
              << "StartY,"
              << "EndTime,"
              << "EndX,"
              << "EndY\n";
    for ( const auto & i : M_shoot_events )
    {
        std::cout << "Shoot,"
                  << ( i->playerSide() == LEFT ? team_l : team_r ) << ','
                  << side_char( i->playerSide() ) << ','
                  << i->playerUnum() << ','
                  << i->startTime().cycle() << ','
                  << i->startPos().x << ','
                  << i->startPos().y << ','
                  << i->endTime().cycle() << ','
                  << i->endPos().x << ','
                  << i->endPos().y << '\n';
    }

    std::cout << std::flush;

    return true;
}
