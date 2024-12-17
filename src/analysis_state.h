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

#ifndef ANALYSIS_STATE_H
#define ANALYSIS_STATE_H

#include <rcsc/game_mode.h>
#include <rcsc/game_time.h>
#include <rcsc/geom/vector_2d.h>

class AnalysisContext;
class FieldModel;
class FieldState;

class AnalysisState {
private:
    size_t M_frame_index;
    rcsc::GameTime M_begin_time;
    rcsc::GameMode M_begin_mode;

    AnalysisState() = delete;

protected:

    AnalysisState( const size_t index,
                   const rcsc::GameTime & begin_time,
                   const rcsc::GameMode & begin_mode );
public:
    virtual
    ~AnalysisState() = default;

    virtual
    void analyze( AnalysisContext & context,
                  const FieldModel & model,
                  const size_t target_index ) = 0;

    virtual
    std::string name() = 0;

    size_t frameIndex() const
    {
        return M_frame_index;
    }

    const rcsc::GameTime & beginTime() const
    {
        return M_begin_time;
    }

    const rcsc::GameMode & beginMode() const
    {
        return M_begin_mode;
    }

};

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

/*!
  \brief No ball owner (loose ball)
 */
class StateNeutral
    : public AnalysisState {
private:
    rcsc::Vector2D M_first_ball_pos;

public:
    StateNeutral( const size_t index,
                  const rcsc::GameTime & begin_time,
                  const rcsc::GameMode & begin_mode,
                  const rcsc::Vector2D & first_ball_pos );

    void analyze( AnalysisContext & context,
                  const FieldModel & model,
                  const size_t target_index ) override;

    std::string name() override;

};

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

class StateSingleKick
    : public AnalysisState {
private:
    rcsc::SideID M_kicker_side;
    int M_kicker_unum;
    int M_kicker_dash_count;
    rcsc::Vector2D M_first_ball_pos;

    StateSingleKick() = delete;

public:

    StateSingleKick( const size_t index,
                     const rcsc::GameTime & begin_time,
                     const rcsc::GameMode & begin_mode,
                     const rcsc::SideID kicker_side,
                     const int kicker_unum,
                     const int kicker_dash_count,
                     const rcsc::Vector2D & first_ball_pos );


    void analyze( AnalysisContext & context,
                  const FieldModel & model,
                  const size_t target_index ) override;

    std::string name() override;

private:

    bool detectSingleKick( AnalysisContext & context,
                           const FieldState & current,
                           const FieldState & prev );
    bool detectSingleTackle( AnalysisContext & context,
                             const FieldState & current,
                             const FieldState & prev );
    bool detectGoal( AnalysisContext & context,
                     const FieldState & current,
                     const FieldState & prev );
};


#endif
