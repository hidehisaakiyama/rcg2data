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
#include <fstream>
#include <filesystem>

namespace {

/*-------------------------------------------------------------------*/
std::string
get_base_name( const std::string & path )
{
    // remove all extension (".rcg" or ".rcg.gz")from file name
    std::filesystem::path p( path );

    // if file name has .gz extension, remove it
    if ( p.extension() == ".gz" )
    {
        p.replace_extension();
    }

    // if file name has .rcg extension, remove it
    if ( p.extension() == ".rcg" )
    {
        p.replace_extension();
    }

    return p.string();
}

}

/*-------------------------------------------------------------------*/
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

    const std::string infile = Options::instance().gameLogFilePath();

    rcsc::gzifstream fin( infile.c_str() );
    if ( ! fin.is_open() )
    {
        std::cerr << "ERROR: Could not open the file [" << infile << ']' << std::endl;
        return 1;
    }


    const std::string basename = get_base_name( infile );
    const std::string event_csv = basename + ".event.csv";

    std::ofstream fout( event_csv );
    if ( ! fout.is_open() )
    {
        std::cerr << "Failed to open the output file : " << event_csv << std::endl;
        return 1;
    }


    rcsc::rcg::Parser::Ptr parser = rcsc::rcg::Parser::create( fin );
    if ( ! parser )
    {
        std::cerr << "ERROR: Could not create an rcg parser." << std::endl;
        return 1;
    }

    std::cerr << "in ...  [" << infile << "]" << std::endl;
    std::cerr << "out ... [" << event_csv << "]" << std::endl;

    FieldModel field_model;
    RCGReader reader( field_model );

    if ( ! parser->parse( fin, reader ) )
    {
        return 1;
    }

    GameAnalyzer analyzer;

    if ( ! analyzer.analyze( field_model ) )
    {
        return 1;
    }

    analyzer.print( fout );

    return 0;
}
