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

#ifndef __ENTITY_H
#define __ENTITY_H

/*! \file */

#define MAX_ENT     50
#define ID_ENEMY    254

typedef struct
{
   unsigned char chrx;  // Entity's identity (what s/he looks like)
   unsigned short x; // x-coord on map
   unsigned short y; // y-coord on map
   unsigned short tilex;   // x-coord tile that entity is standing on
   unsigned short tiley;   // y-coord tile that entity is standing on
   unsigned char eid;   // Entity type (fighter, enemy, normal)
   unsigned char active;   // "Alive" or not
   unsigned char facing;   // Direction
   unsigned char moving;   // In the middle of a move
   unsigned char movcnt;   // How far along the move entity is
   unsigned char framectr; // Counter for determining animation frame
   unsigned char movemode; // Stand, wander, script or chasing
   unsigned char obsmode;  // Determine if affected by obstacles or not
   unsigned char delay; // Movement delay (between steps)
   unsigned char delayctr; // Counter for movement delay
   unsigned char speed; // How hyperactive the entity is
   unsigned char scount;
   unsigned char cmd;   // Move/Wait/Facing command
   unsigned char sidx;  // Script ID number
   unsigned char extra;
   unsigned char chasing;  // Entity is following another
   int cmdnum; // Number of times we need to repeat 'cmd'
   unsigned char atype;
   unsigned char snapback; // Snaps back to direction previously facing
   unsigned char facehero; // Look at player when talked to
   unsigned char transl;   // Entity is see-through or not
   char script[60];  // Movement/action script (pacing, etc.)
}
s_entity;



void process_entities (void);   /*  kq.c  */
int entityat (int, int, int);   /*  kq.c  */
void set_script (int, char *);  /*  intrface.c  */
void place_ent (int, int, int); /*  intrface.c, kq.c  */
void count_entities (void);     /*  kq.c  */



#if 0
int entity_near (int, int, int);        /*  staticed  */
void speed_adjust (int);        /*  staticed  */
void process_entity (int);      /*  staticed  */
void lastm_check (int);         /*  staticed  */
void follow (void);             /*  staticed  */
void wander (int);              /*  staticed  */
void player_move (void);        /*  staticed  */
void moveup (int);              /*  staticed  */
void movedown (int);            /*  staticed  */
void moveright (int);           /*  staticed  */
void moveleft (int);            /*  staticed  */
int obstruction (int, int, int, int);   /*  staticed  */
void parsems (int);             /*  staticed  */
void getcommand (int);          /*  staticed  */
void entscript (int);           /*  staticed  */
#endif /*  0  */



#endif /*  __ENTITY_H  */
