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

#include "shoot.h"

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
Shoot::Shoot( const SideID kicker_side,
              const int kicker_unum,
              const GameTime & start_time,
              const Vector2D & start_pos,
              const GameTime & end_time,
              const Vector2D & end_pos )
    : M_kicker_side( kicker_side ),
      M_kicker_unum( kicker_unum ),
      M_start_time( start_time ),
      M_start_pos( start_pos ),
      M_end_time( end_time ),
      M_end_pos( end_pos )
{

}
