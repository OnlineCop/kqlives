/*
   KQ is Copyright (C) 2002 - Josh Bolduc

   This file is part of KQ... a freeware RPG.

   KQ is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   KQ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KQ; see the file COPYING.  If not, write to
   the Free Software Foundation,
       675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*! \file
 * \brief Constants that are included in the C and Lua scripts
 */
#ifndef __PROGRESS_H
#define __PROGRESS_H

/* I'm deleting this file. It is still available in CVS archives. I'm trying
 * to come up with a solution that would enable quests to be created and
 * added through scripts to:
   1) alleviate the need to recompile when adding a quest.
   2) alleviate the need to prepare (prep.pl) when modifying any script
   3) separate content (quests) from the engine (c code) which will:
      a) be easier for us to manage
      b) make the KQ engine more viable for other projects to use

   I am also trying to make any given data only available once. That makes it
   easier to change the data, and the behavior that affects it.

   Note that the progress variables that were here are now in global.lua.
   The rest were deleted or moved. If they were moved, they were moved to
   a more appropriate place. If they were deleted, they were either unused
   or duplicates of values in other header files.

   -- Winter Knight
*/

/* The spec_items function in menu.c needs these constants. I intend to move
 * the spec_items function to global.lua. */

#define P_UCOIN 36
#define P_CANCELROD 37
#define P_GOBLINITEM 17
#define P_UNDEADJEWEL 35
#define P_WSTONES 24
#define P_BSTONES 25
#define P_EMBERSKEY 46
#define P_BRONZEKEY 70
#define P_DENORIAN 55
#define P_OPALHELMET 43
#define P_OPALSHIELD 50
#define P_IRONKEY 66
#define P_OPALBAND 69
#define P_OPALARMOUR 90
#define P_CAVEKEY 71
#define P_TALK_TSORIN 108
#define P_TALKOLDMAN 110





#endif /*  __PROGRESS_H  */
