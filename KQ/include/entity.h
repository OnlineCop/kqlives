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


#ifndef __ENTITY_H
#define __ENTITY_H 1


/*! \file
 * \brief Stuff related to entities on the map
 *
 * An entity is a hero, or an NPC.
 * \author JB
 * \date ??????
 */


void process_entities (void);   /*  kq.c  */
int entityat (int, int, int);   /*  kq.c  */
void set_script (int, const char *);  /*  intrface.c  */
void place_ent (int, int, int); /*  intrface.c, kq.c  */
void count_entities (void);     /*  kq.c  */


/* move modes */
#define MM_STAND  0
#define MM_WANDER 1
#define MM_SCRIPT 2
#define MM_CHASE  3
#define MM_TARGET 4


#endif  /* __ENTITY_H */
