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
#include "config.h"
#endif

#include "options.h"

#include <rcsc/param/param_map.h>
#include <rcsc/param/cmd_line_parser.h>

#include <iostream>

using namespace rcsc;

namespace {

void
print_usage( const std::vector< std::shared_ptr< ParamMap > > & all_options )
{
    std::cerr << "Usage: " << PACKAGE_NAME
              << " [options ... ] <GameLogFile>\n";
    for ( const auto & i : all_options )
    {
        i->printHelp( std::cerr );
    }
}

}


/*-------------------------------------------------------------------*/
/*!

*/
Options &
Options::instance()
{
    static Options s_instance;
    return s_instance;
}

/*-------------------------------------------------------------------*/
/*!

*/
Options::Options()
    : M_verbose( 0 ),
      M_game_log_file_path()
{

}

/*-------------------------------------------------------------------*/
/*!

*/
bool
Options::parse( int argc,
                char ** argv )
{
    bool help = false;

    // create parameter map
    std::vector< std::shared_ptr< ParamMap > > all_options;

    std::shared_ptr< ParamMap > system_options( new ParamMap( "System Options" ) );
    all_options.push_back( system_options );
    system_options->add()
        ( "help", "h", rcsc::BoolSwitch( &help ) )
        ( "verbose", "", &M_verbose, "verbose level(0=NONE,1=INFO,2=WARN).")
        ;

    std::shared_ptr< ParamMap > file_options( new ParamMap( "File Options" ) );
    all_options.push_back( file_options );
    file_options->add()
        ( "input", "i",
          &M_game_log_file_path,
          "set the path to an input Game Log file(.rcg)." )
        ;



    // parse options

    rcsc::CmdLineParser parser( argc, argv );
    for ( auto & i : all_options )
    {
        parser.parse( *i );
    }

    // help message
    if ( help
         || parser.failed() )
    {
        if ( parser.failed() )
        {
            std::cerr << "Detect unsupported options:\n";
            parser.printOptionNameArgs( std::cerr, '\n' );
            std::cerr << std::endl;
        }

        print_usage( all_options );
        return false;
    }

    //
    if ( ( ! M_game_log_file_path.empty()
           && ! parser.positionalOptions().empty() )
         || parser.positionalOptions().size() > 1 )
    {
        std::cerr << "Found several input files." << std::endl;
        print_usage( all_options );
        return false;
    }

    // set file path from positional options
    if ( M_game_log_file_path.empty()
         && ! parser.positionalOptions().empty() )
    {
        M_game_log_file_path = parser.positionalOptions().front();
    }

    if ( M_game_log_file_path.empty() )
    {
        print_usage( all_options );
        return false;
    }

    return true;
}
