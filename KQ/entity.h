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

#define MAX_ENT     50
#define ID_ENEMY    254

typedef struct
{
   unsigned char chrx;
   unsigned short x, y;
   unsigned short tilex, tiley;
   unsigned char eid;
   unsigned char active;
   unsigned char facing;
   unsigned char moving, movcnt;
   unsigned char framectr;
   unsigned char movemode;
   unsigned char obsmode;
   unsigned char delay, delayctr;
   unsigned char speed, scount;
   unsigned char cmd, sidx;
   unsigned char extra, chasing;
   int cmdnum;
   unsigned char atype;
   unsigned char snapback;
   unsigned char facehero;
   unsigned char transl;
   char script[60];
}
s_entity;

void process_entities (void);
int entity_near (int, int, int);
void speed_adjust (int);
void process_entity (int);
void lastm_check (int);
void follow (void);
void wander (int);
void player_move (void);
void moveup (int);
void movedown (int);
void moveright (int);
void moveleft (int);
int obstruction (int, int, int, int);
int entityat (int, int, int);
void parsems (int);
void getcommand (int);
void entscript (int);
void set_script (int, char *);
void place_ent (int, int, int);
void count_entities (void);

#endif
