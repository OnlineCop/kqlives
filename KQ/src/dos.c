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
 * \brief Specifics for DOS platforms
 *
 * \author PH
 * \date 20030723
 *
 * This includes any bits which are specific for DOS platforms
 */
#include "kq.h"
#include <stdio.h>

/*! \brief Return the name of 'significant' directories.
 *
 * \param dir Enumerated constant for directory type  \sa DATA_DIR et al.
 * \param file File name below that directory.
 * \returns The combined path
 */
const char *kqres (int dir, const char *file)
{
   static char ans[PATH_MAX];
   switch (dir)
     {
     case DATA_DIR:
        sprintf (ans, "data/%s", file);
        break;
     case MUSIC_DIR:
        sprintf (ans, "music/%s", file);
        break;
     case MAP_DIR:
        sprintf (ans, "maps/%s", file);
        break;
     case SAVE_DIR:
        sprintf (ans, "saves/%s", file);
	break;
     case SETTINGS_DIR:
        sprintf (ans, "./%s", file);
        break;
     case SCRIPT_DIR:
        sprintf (ans, "scripts/%s", file);
        break;
     default:
        return NULL;
     }
   return ans;
}
