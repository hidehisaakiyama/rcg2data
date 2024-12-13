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

#include "rcg_reader.h"

#include "field_model.h"

#include <rcsc/common/server_param.h>
#include <rcsc/common/player_param.h>
#include <rcsc/common/player_type.h>

using namespace rcsc;
using namespace rcsc::rcg;

/*-------------------------------------------------------------------*/
/*!

 */
RCGReader::RCGReader( FieldModel & field_model )
    : M_field_model( field_model ),
      M_playmode( PM_Null ),
      M_left_score( 0 ),
      M_right_score( 0 )
{

}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleLogVersion( const int ver )
{
    Handler::handleLogVersion( ver );

    if ( logVersion() != REC_VERSION_JSON
         && logVersion() <= 3 )
    {
        std::cerr << "Unsupported RCG version [" << ver << ']' << std::endl;
        return false;
    }

    return true;
}


/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleEOF()
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleShow( const ShowInfoT & show )
{
    M_field_model.appendState( show );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleMsg( const int ,
                        const int ,
                        const std::string & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleDraw( const int ,
                          const drawinfo_t & )
{
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handlePlayMode( const int time,
                             const PlayMode pmode )
{
    M_playmode = pmode;

    M_field_model.updateTime( time, false );
    M_field_model.updateGameMode( pmode );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleTeam( const int ,
                         const TeamT & team_l,
                         const TeamT & team_r )
{
    M_field_model.updateTeamNames( team_l.name_, team_r.name_ );
    M_field_model.updateScore( team_l.score_, team_r.score_ );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handleServerParam( const ServerParamT & param )
{
    ServerParam::instance().convertFrom( param );

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
RCGReader::handlePlayerParam( const PlayerParamT & param )
{
    PlayerParam::instance().convertFrom( param );

    return true;
}

/*-------------------------------------------------------------------*/
bool
RCGReader::handlePlayerType( const PlayerTypeT & param )
{
    const PlayerType ptype( param );

    if ( ptype.id() < 0
         || ptype.id() >= PlayerParam::i().playerTypes() )
    {
        return false;
    }

    PlayerTypeSet::instance().insert( ptype );

    return true;
}

/*-------------------------------------------------------------------*/
bool
RCGReader::handleTeamGraphic( const char ,
                              const int ,
                              const int ,
                              const std::vector< std::string > & )
{
    return true;
}
