// -*-c++-*-

/*
 *Copyright:

 Copyright (C) Hiroki SHIMORA, Hidehisa AKIYAMA

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

#ifndef COMPOSITE_HANDLER_H
#define COMPOSITE_HANDLER_H

#include <rcsc/rcg/handler.h>

#include <vector>
#include <memory>

class CompositeHandler
    : public rcsc::rcg::Handler {
public:
    typedef std::shared_ptr< rcsc::rcg::Handler > Ptr;

private:

    std::vector< Ptr > M_handlers;

public:

    CompositeHandler() = default;
    ~CompositeHandler() = default;


    void add( Ptr handler );

    virtual
    bool handleLogVersion( const int ver );

    virtual
    bool handleEOF();

    virtual
    bool handleShow( const rcsc::rcg::ShowInfoT & show );

    virtual
    bool handleMsg( const int time,
                    const int board,
                    const std::string & msg );

    virtual
    bool handleDraw( const int time,
                     const rcsc::rcg::drawinfo_t & draw );

    virtual
    bool handlePlayMode( const int time,
                         const rcsc::PlayMode pm );

    virtual
    bool handleTeam( const int time,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r );

    virtual
    bool handleServerParam( const std::string & msg );

    virtual
    bool handlePlayerParam( const std::string & msg );

    virtual
    bool handlePlayerType( const std::string & msg );

};

#endif
