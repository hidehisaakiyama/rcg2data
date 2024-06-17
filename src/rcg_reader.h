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

#ifndef RCG_READER_H
#define RCG_READER_H

#include <rcsc/rcg/handler.h>
#include <rcsc/rcg/types.h>

class FieldModel;

class RCGReader
    : public rcsc::rcg::Handler {
private:

    FieldModel & M_field_model;

    rcsc::PlayMode M_playmode; // the last handled playmode ID
    int M_left_score;
    int M_right_score;

    RCGReader() = delete;
public:

    RCGReader( FieldModel & field_model );
    ~RCGReader() = default;

    bool handleLogVersion( const int ver ) override;

    bool handleEOF() override;

    bool handleShow( const rcsc::rcg::ShowInfoT & show ) override;

    bool handleMsg( const int time,
                    const int board,
                    const std::string & msg ) override;

    bool handleDraw( const int time,
                     const rcsc::rcg::drawinfo_t & draw ) override;

    bool handlePlayMode( const int time,
                         const rcsc::PlayMode pm ) override;

    bool handleTeam( const int time,
                     const rcsc::rcg::TeamT & team_l,
                     const rcsc::rcg::TeamT & team_r ) override;


    bool handleServerParam( const rcsc::rcg::ServerParamT & param ) override;

    bool handlePlayerParam( const rcsc::rcg::PlayerParamT & param ) override;

    bool handlePlayerType( const rcsc::rcg::PlayerTypeT & param ) override;

    bool handleTeamGraphic( const char side,
                            const int x,
                            const int y,
                            const std::vector< std::string > & xpm_data ) override;

};

#endif
