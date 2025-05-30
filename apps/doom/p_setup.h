//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//   Setup a game, startup stuff.
//

#ifndef __P_SETUP__
#define __P_SETUP__

#include "d_mode.h"

// NOT called by W_Ticker. Fixme.
void P_SetupLevel(int episode,
                  int map,
                  int playermask,
                  skill_t skill);

// Called by startup code.
void P_Init(void);

#endif
