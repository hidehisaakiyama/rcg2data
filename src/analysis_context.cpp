// -*-c++-*-

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

#include "analysis_context.h"

#include "analysis_state.h"

using namespace rcsc;

/*-------------------------------------------------------------------*/
AnalysisContext::AnalysisContext()
    : M_analysis_state( new StateNeutral( 0, GameTime( 0, 0 ), GameMode(), Vector2D( 0.0, 0.0 ) ) )
{

}

/*-------------------------------------------------------------------*/
void
AnalysisContext::setAnalysisState( std::shared_ptr< AnalysisState > new_state )
{
#if 0
    std::cerr << "(Context::setState) from [";
    if ( M_analysis_state )
    {
        std::cerr << M_analysis_state->name();
    }
    std::cerr << "] to [";
    if ( new_state )
    {
        std::cerr << new_state->name();
    }
    std::cerr << "]" << std::endl;;
#endif
    M_analysis_state = new_state;
}

/*-------------------------------------------------------------------*/
void
AnalysisContext::addActionEvent( ActionEvent::ConstPtr event )
{
    M_action_events.push_back( event );
}

/*-------------------------------------------------------------------*/
void
AnalysisContext::analyze( const FieldModel & model,
                          const size_t index )
{
    if ( M_analysis_state )
    {
        M_analysis_state->analyze( *this, model, index );
    }

}

/*-------------------------------------------------------------------*/
std::ostream &
AnalysisContext::printActionEvents( std::ostream & os ) const
{
    ActionEvent::print_header_csv( os );

    for ( const ActionEvent::ConstPtr & i : M_action_events )
    {
        i->printCSV( os );
    }

    return os;
}
