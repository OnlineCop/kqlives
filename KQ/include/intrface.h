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

#ifndef __INTRFACE_H
#define __INTRFACE_H

#include <lua.h>


/* Facing directions */
#define FACE_DOWN    0
#define FACE_UP      1
#define FACE_LEFT    2
#define FACE_RIGHT   3


#define NUM_IFUNCS   142

struct luaL_reg
{
   const char *name;
   lua_CFunction func;
};



void do_luainit (char *);       /*  kq.c  */
void do_luacheat (void);        /*  entity.c  */
void do_luakill (void);         /*  kq.c  */
void do_autoexec (void);        /*  kq.c  */
void do_postexec (void);        /*  kq.c  */
void do_zone (int);             /*  kq.c  */
void do_entity (int);           /*  kq.c  */



#if 0
void check_map_change (void);   /*  staticed  */
#endif /*  0  */



#endif /*  __INTRFACE_H  */
