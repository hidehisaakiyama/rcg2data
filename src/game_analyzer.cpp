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

#include "rcg_reader.h"

#include <rcsc/gz.h>
#include <rcsc/rcg/parser.h>

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
GameAnalyzer::read( const std::string & filepath )
{
    rcsc::gzifstream fin( filepath.c_str() );

    if ( ! fin.is_open() )
    {
        std::cerr << "ERROR: Could not open the file [" << filepath << ']' << std::endl;
        return false;
    }

    std::cerr << "Start analyzing : [" << filepath << "]" << std::endl;

    rcsc::rcg::Parser::Ptr parser = rcsc::rcg::Parser::create( fin );

    if ( ! parser )
    {
        std::cerr << "ERROR: Could not create an rcg parser." << std::endl;
        return false;
    }

    std::unique_ptr< rcg::Handler > handler( new RCGReader( M_field_model ) );

    parser->parse( fin, *handler );

    std::cerr << "Finished : [" << filepath << "]" << std::endl;

    return true;
}

/*-------------------------------------------------------------------*/
/*!

*/
bool
GameAnalyzer::analyze( const std::string & filepath )
{
    if ( ! read( filepath ) )
    {
        return false;
    }

    return true;
}
