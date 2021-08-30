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

#include "shoot_handler.h"

using namespace rcsc;
using namespace rcsc::rcg;

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleEOF()
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleShow( const ShowInfoT & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleMsg( const int ,
                         const int ,
                         const std::string & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleDraw( const int ,
                          const drawinfo_t & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handlePlayMode( const int ,
                              const PlayMode )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleTeam( const int ,
                          const TeamT & ,
                          const TeamT & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handleServerParam( const std::string & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handlePlayerParam( const std::string & )
{
    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
bool
ShootHandler::handlePlayerType( const std::string & )
{
    return true;
}
