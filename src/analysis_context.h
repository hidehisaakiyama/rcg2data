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

#ifndef ANALYSIS_CONTEXT_H
#define ANALYSIS_CONTEXT_H

#include "action_event.h"

#include <memory>

class AnalysisState;
class FieldModel;

class AnalysisContext {
private:
    std::shared_ptr< AnalysisState > M_analysis_state;
    std::vector< ActionEvent::ConstPtr > M_action_events;

public:
    AnalysisContext();

    void setAnalysisState( std::shared_ptr< AnalysisState > new_state );
    void addActionEvent( ActionEvent::ConstPtr event );

    void analyze( const FieldModel & model,
                  const size_t target_index );

    std::ostream & printActionEvents( std::ostream & os ) const;

};

#endif
