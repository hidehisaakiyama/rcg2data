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

#include "options.h"
#include "game_analyzer.h"
#include "rcg_reader.h"

#include <rcsc/gz.h>
#include <rcsc/rcg/parser.h>

#include <iostream>

int
main( int argc, char **argv )
{
    std::cerr << "******************************************************************\n"
              << " " PACKAGE_NAME " " VERSION "\n"
              << " Copyright: (C) Hidehisa Akiyama\n"
              << " All rights reserved.\n"
              << "******************************************************************\n"
              << std::endl;

    if ( ! Options::instance().parse( argc, argv ) )
    {
        return 1;
    }

    const std::string filepath = Options::instance().gameLogFilePath();

    rcsc::gzifstream fin( filepath.c_str() );
    if ( ! fin.is_open() )
    {
        std::cerr << "ERROR: Could not open the file [" << filepath << ']' << std::endl;
        return 1;
    }

    rcsc::rcg::Parser::Ptr parser = rcsc::rcg::Parser::create( fin );
    if ( ! parser )
    {
        std::cerr << "ERROR: Could not create an rcg parser." << std::endl;
        return 1;
    }

    FieldModel field_model;
    RCGReader reader( field_model );

    // std::cerr << "parsing ... [" << filepath << "]" << std::endl;
    if ( ! parser->parse( fin, reader ) )
    {
        return 1;
    }

    GameAnalyzer analyzer;

    std::cerr << "analyzing ... [" << filepath << "]" << std::endl;
    if ( ! analyzer.analyze( field_model ) )
    {
        return 1;
    }

    analyzer.print( field_model );

    return 0;
}
