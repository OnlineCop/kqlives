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
   unsigned char chrx;
   unsigned short x;
   unsigned short y;
   unsigned short tilex;
   unsigned short tiley;
   unsigned char eid;
   unsigned char active;
   unsigned char facing;
   unsigned char moving;
   unsigned char movcnt;
   unsigned char framectr;
   unsigned char movemode;
   unsigned char obsmode;
   unsigned char delay;
   unsigned char delayctr;
   unsigned char speed;
   unsigned char scount;
   unsigned char cmd;
   unsigned char sidx;
   unsigned char extra;
   unsigned char chasing;
   int cmdnum;
   unsigned char atype;
   unsigned char snapback;
   unsigned char facehero;
   unsigned char transl;
   char script[60];
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
