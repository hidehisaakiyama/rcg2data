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

#ifndef RCG2DATA_OPTIONS_H
#define RCG2DATA_OPTIONS_H

#include <string>

/*!
  \class Options
  \brief command line option holder
*/
class Options {
private:

    int M_verbose;

    std::string M_game_log_file_path;

    Options(); // private for singleton

    Options( const Options & ); // not used
    const Options & operator=( const Options & ); // not used

public:

    static
    Options & instance();

    bool parse( int argc,
                char ** argv );

    int verbose() const { return M_verbose; }

    const std::string & gameLogFilePath() const { return M_game_log_file_path; }
};

#endif
