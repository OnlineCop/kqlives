/*
   KQ is Copyright (C) 2002 by Josh Bolduc

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
 * \brief Timing handler functions
 * \author ML
 * \date 20102002
 *
 * Looks after keeping the music playing whilst the game is 'paused'
 */
#include <allegro.h>
#include "kq.h"
#include "timing.h"

#include "music.h"



/*! \brief Poll the music system
 *
 * call poll_music() to ensure that music plays
 * \remark PH does this need locking with LOCK_FUNCTION
 *            like a timer function does?
 */
static void _kq_rest_callback (void)
{
   poll_music ();
}



/*! \brief Pause the game for a period of time
 *
 * Calls _kq_rest_callback() continuously to ensure all actions are running
 *
 * \param   ms Time to pause in milliseconds
 */
void wait (long ms)
{
   rest_callback (ms, _kq_rest_callback);
}
