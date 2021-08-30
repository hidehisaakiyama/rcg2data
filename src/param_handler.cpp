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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "param_handler.h"

#include <rcsc/common/server_param.h>
#include <rcsc/common/player_param.h>
#include <rcsc/common/player_type.h>

using namespace rcsc;
using namespace rcsc::rcg;

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handleEOF()
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handleShow( const ShowInfoT & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handleMsg( const int ,
                         const int ,
                         const std::string & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handleDraw( const int ,
                          const drawinfo_t & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handlePlayMode( const int ,
                              const PlayMode )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handleTeam( const int ,
                          const TeamT & ,
                          const TeamT & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handleServerParam( const std::string & msg )
{
    if ( ! ServerParam::instance().parse( msg.c_str(), 999 ) )
    {
        return false;
    }

    std::cout << "done server_param" << std::endl;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handlePlayerParam( const std::string & msg )
{
    if ( ! PlayerParam::instance().parse( msg.c_str(), 999 ) )
    {
        return false;
    }

    std::cout << "done player_param" << std::endl;

    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
bool
ParamHandler::handlePlayerType( const std::string & msg )
{
    const PlayerType ptype( msg.c_str(), 999 );

    if ( ptype.id() < 0
         || ptype.id() >= PlayerParam::i().playerTypes() )
    {
        return false;
    }

    PlayerTypeSet::instance().insert( ptype );

    std::cout << "done player_type " << ptype.id() << std::endl;

    return true;
}
