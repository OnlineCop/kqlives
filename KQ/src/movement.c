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
 * \brief NPC movement routines
 *
 * Set of functions to allow NPCs move to a certain spot without having
 * to tell the directions.
 *
 * \author RB
 * \date March 2005
 */

#include <stdio.h>
#include <string.h>
#include "kq.h"



static void copy_map (int *);
static int search_paths (int, int *, int, int, int, int, int, int, int, int, int);
static int compose_path (AL_CONST int *, int, int, char *, int);
static int minimize_path (AL_CONST char *, char *, int);



/*! \brief Path search implementation for KQ
 *
 * Call this function to calculate the shortest path between a given
 * NPC and a target point.
 *
 * \param[in]  id        The ID of the entity moving around.
 * \param[in]  source_x  The x coordenate of the source point.
 * \param[in]  source_y  The y coordenate of the source point.
 * \param[in]  target_x  The x coordenate of the target point.
 * \param[in]  target_y  The y coordenate of the target point.
 * \param[out] buffer    A buffer where the result will be stored.
 * \param[in]  size      The size of the result buffer.
 *
 * \return Whether it could or not find the path.
 * \retval 0 Success
 * \retval 1 No path found
 * \retval 2 Path found but result buffer too small to hold the answer.
 * \retval 3 Misc error
 *
 * \sa copy_map compose_path search_paths minimize_path
 */
int find_path (int id, int source_x, int source_y, int target_x, int target_y,
               char *buffer, int size)
{
   int *map = NULL;
   int result = 0;

   if (buffer == NULL || size == 0)
      return (3);

   memset (buffer, '\0', size);

   /*  TODO: Allocate memory once instead of every call  */
   result = g_map.xsize * g_map.ysize * sizeof(int);
   map = (int *)malloc(result);
   if (map == NULL)
      return (3);

   memset (map, 0, result);
   copy_map (map);
   result = search_paths (id, map, 1, source_x, source_y, target_x, target_y,
                          0, 0, g_map.xsize, g_map.ysize);

   if (! result)
      result = compose_path (map, target_x, target_y, buffer, size);
   else
      result = 1;

   free (map);
   return (result);
}



/*! \brief Internal path search routine.
 *
 * This function uses recursivity to find the shortest path to the target
 * point. Once it returns 0, 
 *
 * \param[in]     id       The ID of the entity moving around.
 * \param[in,out] map      The map in where to write the paths.
 * \param[in]     step     The current step in recursivity.
 * \param[in]     source_x The x coordenate of the source point.
 * \param[in]     source_y The y coordenate of the source point.
 * \param[in]     target_x The x coordenate of the target point.
 * \param[in]     target_y The y coordenate of the target point.
 * \param[in]     start_x  The minimun value of the x axis.
 * \param[in]     start_y  The minimun value of the y axis.
 * \param[in]     limit_x  The maximun value of the x axis.
 * \param[in]     limit_y  The maximun value of the y axis.
 *
 * \return If a path was found.
 * \retval 0 Success.
 * \retval 1 No path found.
 *
 * \sa copy_map compose_path find_path minimize_path
 */
static int search_paths (int id, int *map, int step, int source_x, int source_y,
                         int target_x, int target_y, int start_x, int start_y,
                         int limit_x, int limit_y)
{
   int index;
   int value;
   int result = 1;

   index = source_y * limit_x + source_x;
   value = map[index];
   if ((value != -1) && (value == 0 || value > step) && (step == 1 || ! entityat(source_x, source_y, id))) {
      map[index] = step;

      if ((source_x == target_x) && (source_y == target_y))
          return 0;

      if (source_x > start_x)
         result &= search_paths (id, map, step + 1, source_x - 1, source_y,
                                 target_x, target_y, start_x, start_y,
                                 limit_x, limit_y);

      if (source_x < limit_x - 1)
         result &= search_paths (id, map, step + 1, source_x + 1, source_y,
                                 target_x, target_y, start_x, start_y,
                                 limit_x, limit_y);

      if (source_y > start_y)
         result &= search_paths (id, map, step + 1, source_x, source_y - 1,
                                 target_x, target_y, start_x, start_y,
                                 limit_x, limit_y);

      if (source_y < limit_y - 1)
         result &= search_paths (id, map, step + 1, source_x, source_y + 1,
                                 target_x, target_y, start_x, start_y,
                                 limit_x, limit_y);
   }

   return (result);
}



/*! \brief Generates an internal map.
 *
 * The function generates a map setting to -1 any square that is blocked
 * by either an object or an entity.
 *
 * \param[out] map The map where the result will be copied
 *
 * \return Nothing.
 *
 * \sa search_paths compose_path find_path minimize_path
 */
static void copy_map (int *map)
{
   register int x, y;
   int index;

   for (y = 0; y < g_map.ysize; y++)
      for (x = 0; x < g_map.xsize; x++) {
         index = y * g_map.xsize + x;

         if (o_seg[index])
            map[index] = -1;
      }

   /*  RB: faster to do this than to check if there is an entity at every square  */
   for (index = 0; index < MAX_ENT; index++)
      if (g_ent[index].active)
         map[g_ent[index].tilex * g_map.ysize + g_ent[index].tiley] = -1;
}



/*! \brief Generates the solution path.
 *
 * The only way of doing this is walking the path backwards, minimizing and
 * turning it the way around.
 *
 * \param[in]  map      The map with the paths.
 * \param[in]  target_x Target x coordenate.
 * \param[in]  target_y Target y coordenate.
 * \param[out] buffer   Buffer to store the solution.
 * \param[in]  size     Size of the solution buffer.
 *
 * \return If the solution was copied.
 * \retval 0 The solution was copied.
 * \retval 1 The buffer was too small for the solution to be copied.
 * \retval 2 There was no solution, or internal error.
 *
 * \sa search_paths copy_map find_path minimize_path
 */
static int compose_path (AL_CONST int *map, int target_x, int target_y,
                         char *buffer, int size)
{
   char temp[1024];
   int index = 0;
   int x;
   int y;
   int value;

   x = target_x;
   y = target_y;
   value = map[y * g_map.xsize + x];
   index = value - 2;
   memset(temp, '\0', sizeof(temp));

   while (value > 1) {

      /*  move as many squares up as possible  */
      while ((y > 0) && (map[(y - 1) * g_map.xsize + x] == (value - 1)) && (value > 1)) {
         value--;
         y--;
         temp[index--] = 'D';
      }

      /*  move as many squares left as possible  */
      while ((x > 0) && (map[y * g_map.xsize + (x - 1)] == (value - 1)) && (value > 1)) {
         value--;
         x--;
         temp[index--] = 'R';
      }

      /*  move as many squares down as possible  */
      while ((y < g_map.ysize - 1) &&
             (map[(y + 1) * g_map.xsize + x] == (value - 1)) && (value > 1)) {
         value--;
         y++;
         temp[index--] = 'U';
      }

      /*  move as many squares right as possible  */
      while ((x < g_map.xsize - 1) &&
             (map[y * g_map.xsize + (x + 1)] == (value - 1)) && (value > 1)) {
         value--;
         x++;
         temp[index--] = 'L';
      }
   }

   return (minimize_path(temp, buffer, size));
}



/*! \brief Minimizes a path.
 *
 * Given a path like "RRRRDRRDLU", this functions generates "R4D1R2D1L1U1".
 *
 * \param[in]  source The original string.
 * \param[out] target The buffer where the result will be stored.
 * \param[in]  size   The result buffer size.
 *
 * \return If the solution was copied.
 * \retval 0 The solution was copied.
 * \retval 1 The buffer was too small for the solution to be copied.
 *
 * \sa search_paths copy_map find_path compose_path
 */
static int minimize_path (AL_CONST char *source, char *target, int size)
{
   int  source_index = 0;
   int  repetition = 0;
   char value;
   char temp[16];
   char buffer[512];

   memset(buffer, '\0', sizeof(buffer));
   while (source[source_index] != '\0') {
      value = source[source_index];

      source_index++;
      repetition = 1;
      while ((source[source_index] == value) &&
             (source[source_index] != '\0')) {
         source_index++;
         repetition++;
      }

      /*  FIXME: check to see if the buffer is long enough?  */
      snprintf(temp, sizeof(temp), "%c%d", value, repetition);
      strncat(buffer, temp, sizeof(buffer));
   }

   if (strlen(buffer) < (unsigned int)size) {
      strcpy(target, buffer);
      return (0);
   }
   else
      return (1);
}
