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
 * \brief Specifics for Mac platforms
 *
 * \author PH
 * \date 20030723
 *
 * This includes any bits which are specific for Mac OSX  platforms
 */

#import <Foundation/Foundation.h>
/*#import <Foundation/NSPathUtilities.h>*/
#include <string.h>

/* I can't include kq.h here as it clashes with some stupid OS9 names like shadow and strbuf */
#define DATA_DIR       0
#define MAP_DIR        1
#define SAVE_DIR       2
#define MUSIC_DIR      3
#define SCRIPT_DIR     4
#define SETTINGS_DIR   5

static NSString *user_dir;
static NSString *game_dir;
static BOOL init_path = NO;



/*! \brief Return the name of 'significant' directories.
 *
 * \param dir Enumerated constant for directory type  \sa DATA_DIR et al.
 * \param file File name below that directory.
 * \returns The combined path
 */
const char *kqres (int dir, const char *file)
{
   static char ans[PATH_MAX];
   NSString *base, *sub, *ff;
   NSArray *arr;
   if (init_path == NO) {
      /* Get home directory */

      arr = NSSearchPathForDirectoriesInDomains (NSLibraryDirectory,
                                                 NSUserDomainMask, YES);
      user_dir =[[arr objectAtIndex: 0] stringByAppendingPathComponent:@"KQ"];
        [user_dir retain];
      [[NSFileManager defaultManager] createDirectoryAtPath: user_dir attributes:nil];

      /* Now the data directory */
      game_dir =[[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent: @"Contents/Resources"];
      [game_dir retain];
	/*    NSLog(@"Putting user data in %@, game data in %@\n", user_dir, game_dir);*/
      init_path = YES;
   }

   switch (dir) {
   case DATA_DIR:
      base = game_dir;
      sub = @"data";
      break;
   case MUSIC_DIR:
      base = game_dir;
      sub = @"music";
      break;
   case MAP_DIR:
      base = game_dir;
      sub = @"maps";
      break;
   case SAVE_DIR:
   case SETTINGS_DIR:
      base = user_dir;
      sub = @".";
      break;
   case SCRIPT_DIR:
      base = game_dir;
      sub = @"scripts";
      break;
   default:
      return NULL;
   }

   ff = [NSString stringWithCString: file];
   arr = [NSArray arrayWithObjects: base, sub,ff, nil];
   return strncpy (ans,[[NSString pathWithComponents:arr] cString], sizeof
                   (ans));
}
