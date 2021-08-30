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

#include "composite_handler.h"

using namespace rcsc;
using namespace rcsc::rcg;

/*-------------------------------------------------------------------*/
/*!

 */
void
CompositeHandler::add( Ptr handler )
{
    M_handlers.push_back( handler );
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleLogVersion( const int ver )
{
    if ( ! rcsc::rcg::Handler::handleLogVersion( ver ) )
    {
        return false;
    }

    for ( auto & i : M_handlers )
    {
        if ( ! i->handleLogVersion( ver ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleEOF()
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handleEOF() )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleShow( const ShowInfoT & show )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handleShow( show ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleMsg( const int time,
                             const int board,
                             const std::string & msg )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handleMsg( time, board, msg ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleDraw( const int time,
                              const drawinfo_t & draw )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handleDraw( time, draw ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handlePlayMode( const int time,
                                  const PlayMode pm )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handlePlayMode( time, pm ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleTeam( const int time,
                              const TeamT & team_l,
                              const TeamT & team_r )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handleTeam( time, team_l, team_r ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handleServerParam( const std::string & msg )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handleServerParam( msg ) )
        {
            return false;
        }
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handlePlayerParam( const std::string & msg )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handlePlayerParam( msg ) )
        {
            return false;
        }
    }

    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
bool
CompositeHandler::handlePlayerType( const std::string & msg )
{
    for ( auto & i : M_handlers )
    {
        if ( ! i->handlePlayerType( msg ) )
        {
            return false;
        }
    }

    return true;
}
