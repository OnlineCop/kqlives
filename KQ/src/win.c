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
 * \brief Specifics for Windows
 *
 * \author PH
 * \date 20030723
 *
 * This includes any bits which are specific for Windows platforms
 */
#include <allegro.h>
#include <winalleg.h>
#include "kq.h"
static int init_path = 0;
static char user_dir[PATH_MAX];
static char game_dir[PATH_MAX];
typedef HRESULT (WINAPI * SHGETFOLDERPATH) (HWND, int, HANDLE, DWORD, LPTSTR);

#  define CSIDL_FLAG_CREATE 0x8000
#  define CSIDL_APPDATA 0x1A
#  define SHGFP_TYPE_CURRENT 0

/*! \brief Return the name of 'significant' directories.
 *
 * \param dir Enumerated constant for directory type  \sa DATA_DIR et al.
 * \param file File name below that directory.
 * \returns The combined path
 */
const char *kqres (int dir, const char *file)
{
   HINSTANCE SHFolder;
   SHGETFOLDERPATH SHGetFolderPath;
   char *home;
   static char ans[PATH_MAX];
   if (!init_path)
     {
     	home=ans;
        /* Get home directory; this bit originally written by SH */
        SHFolder = LoadLibrary ("shfolder.dll");
        if (SHFolder != NULL)
          {
             SHGetFolderPath =
                (void *) GetProcAddress (SHFolder, "SHGetFolderPathA");
             if (SHGetFolderPath != NULL)
               {
                  /* Get the "Application Data" folder for the current user */
                  if (SHGetFolderPath (NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE,
                                       NULL, SHGFP_TYPE_CURRENT, home) != S_OK)
                     *home = '\0';
               }
             FreeLibrary (SHFolder);
          }

        /* Do not get fooled by a corrupted $HOME */
        if (home != NULL && strlen (home) < PATH_MAX)
          {
             sprintf (user_dir, "%s/KQ", home);
             /* Always try to make the directory, just to be sure. */
             mkdir (savedir);
          }
        else
          {
             strcpy (user_dir, ".");
          }
        /* Now the data directory */
        strcpy (game_dir, ".");
        init_path = 1;
     }
   switch (dir)
     {
     case DATA_DIR:
        sprintf (ans, "%s/data/%s", game_dir, file);
        break;
     case MUSIC_DIR:
        sprintf (ans, "%s/music/%s", game_dir, file);
        break;
     case MAP_DIR:
        sprintf (ans, "%s/maps/%s", game_dir, file);
        break;
     case SAVE_DIR:
     case SETTINGS_DIR:
        sprintf (ans, "%s/%s", user_dir, file);
        break;
     case SCRIPT_DIR:
        sprintf (ans, "%s/scripts/%s", game_dir, file);
        break;
     default:
        return NULL;
     }
   return ans;
}
