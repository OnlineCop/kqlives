/*! \page License
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
 * \brief Bounding areas
 * \author TT
 * \date 20060720
 */

#include <ctype.h>
#include <stdio.h>

#include "../include/bounds.h"


s_bound bound_box[MAX_BOUNDS];


/*! \brief See if this bounding area overlaps (or is contained inside of) any
 * other bounding area, or vice versa.
 *
 * \param   which - Bounding area to check
 * \param   num_bound_boxes Number of elements in the \sa bound_box array
 * \return  1 if 'which' coords found anywhere withing any other boxes
 */
int bound_in_bound (s_bound *which, int num_bound_boxes)
{
   /* Check if any part of box1 is inside box2 (or box2 in box1) */

   int i, j, k;
   int true1, true2;            // See if an entire query is true
   int was_found;

   /* We can use an array to speed this up so we don't have to duplicate
    * identical code
    */
   int x1[2] = { which->left, which->right };
   int y1[2] = { which->top, which->bottom };
   int x2[2];                   // Defined inside for..loop for all bounding areas
   int y2[2];                   // Defined inside for..loop for all bounding areas

   /* This will contain a small hack. Assign the results of the queries to
    * variables. We can break out with a "nothing found" in these cases:
    *
    * 1) NEITHER x-coords is true
    * 2) NEITHER y-coords is true
    *
    * Else, something was overlapping, and we'll return '1'
    */
   was_found = 0;
   for (i = 0; i < num_bound_boxes; i++) {
      x2[0] = bound_box[i].left;
      x2[1] = bound_box[i].right;
      y2[0] = bound_box[i].top;
      y2[1] = bound_box[i].bottom;

      for (j = 0; j < 2; j++) {
         true1 = (x1[j] >= x2[j] && x1[j] <= x2[1 - j]);
         true2 = (x2[j] >= x1[j] && x2[j] <= x1[1 - j]);
         if (!true1 && !true2) {
            continue;
         }

         for (k = 0; k < 2; k++) {
            true1 = (y1[k] >= y2[k] && y1[k] <= y2[1 - k]);
            true2 = (y2[k] >= y1[k] && y2[k] <= y1[1 - k]);
            if (!true1 && !true2) {
               continue;
            } else {
               was_found = 1;
            }
         }
      }
      if (was_found) {
         break;
      }
   }

   return was_found;
}



/*! \brief See if this bounding area overlaps (or is contained inside of) any
 * bounding area in the given array, or vice versa. Note that this function is
 * identical to the one above, except that we pass a pointer to the bound_box
 * array, rather than just assuming the one we want to check against.
 *
 * \param   which - Bounding area to check
 * \param   bound_box - pointer to the bounding boxes we are checking against
 * \param   num_bound_boxes Number of elements in the \sa bound_box array
 * \return  1 if 'which' coords found anywhere withing any other boxes
 */
int bound_in_bound2 (s_bound *which, s_bound *bound_box, int num_bound_boxes)
{
   /* Check if any part of box1 is inside box2 (or box2 in box1) */

   int i, j, k;
   int true1, true2;            // See if an entire query is true
   int was_found;

   /* We can use an array to speed this up so we don't have to duplicate
    * identical code
    */
   int x1[2] = { which->left, which->right };
   int y1[2] = { which->top, which->bottom };
   int x2[2];                   // Defined inside for..loop for all bounding areas
   int y2[2];                   // Defined inside for..loop for all bounding areas

   /* This will contain a small hack. Assign the results of the queries to
    * variables. We can break out with a "nothing found" in these cases:
    *
    * 1) NEITHER x-coords is true
    * 2) NEITHER y-coords is true
    *
    * Else, something was overlapping, and we'll return '1'
    */
   was_found = 0;
   for (i = 0; i < num_bound_boxes; i++) {
      x2[0] = bound_box[i].left;
      x2[1] = bound_box[i].right;
      y2[0] = bound_box[i].top;
      y2[1] = bound_box[i].bottom;

      for (j = 0; j < 2; j++) {
         true1 = (x1[j] >= x2[j] && x1[j] <= x2[1 - j]);
         true2 = (x2[j] >= x1[j] && x2[j] <= x1[1 - j]);
         if (!true1 && !true2) {
            continue;
         }

         for (k = 0; k < 2; k++) {
            true1 = (y1[k] >= y2[k] && y1[k] <= y2[1 - k]);
            true2 = (y2[k] >= y1[k] && y2[k] <= y1[1 - k]);
            if (!true1 && !true2) {
               continue;
            } else {
               was_found = 1;
            }
         }
      }
      if (was_found) {
         break;
      }
   }

   return was_found;
}



/*! \brief Determine whether the coordinates are within any bounding boxes
 *
 * \param   boxes_array - Address of array
 * \param   num_boxes - Number of array elements
 * \param   left - Left edge of current bounding area
 * \param   top - Top edge of current bounding area
 * \param   right - Right edge of current bounding area
 * \param   bottom - Bottom edge of current bounding area
 *
 * \returns NULL if not found, else address within boxes_array array
 */
s_bound *is_contained_bound (s_bound *boxes_array, unsigned int num_boxes,
                             int left, int top, int right, int bottom)
{
   unsigned int i;

   for (i = 0; i < num_boxes; i++) {
      if (left >= boxes_array[i].left && right <= boxes_array[i].right
          && top >= boxes_array[i].top && bottom <= boxes_array[i].bottom) {
         return (&boxes_array[i]);
      }
   }

   return NULL;
}



/* Assign the given bounding area with the x and y coords.
 * Check whether left < right or top < bottom, and swap accordingly.
 *
 * \param   which_bound - Where we will store the correct x/y coords
 * \param   left, top, right, bottom - All the coords
 */
void set_bounds (s_bound *which_bound, int left, int top, int right,
                 int bottom)
{
   // This check ensures that the given `left' is always <= `right'
   if (left <= right) {
      which_bound->left = left;
      which_bound->right = right;
   } else {
      which_bound->left = right;
      which_bound->right = left;
   }

   // This check ensures that the given `top' is always <= `bottom'
   if (top <= bottom) {
      which_bound->top = top;
      which_bound->bottom = bottom;
   } else {
      which_bound->top = bottom;
      which_bound->bottom = top;
   }
}
