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

#ifndef __MENU_H
#define __MENU_H



#include "kq.h"

void menu (void);               /*  only in entity.c  */
void draw_mainmenu (int);       /*  selector.c  */
void draw_playerstat (BITMAP *, int, int, int); /*  masmenu.c, menu.c, intrface.c  */
void spec_items (void);
void update_equipstats (void);  /*  (eqp|item|mas)menu.c, heroc.c  */
s_fighter player2fighter (int); /*  combat.c  */
void revert_equipstats (void);  /*  heroc.c, (item|mas)menu.c,  */
                                 /*  magic.c, combat.c           */
int give_xp (int, int, int);    /*  intrface.c, combat.c  */



#if 0
void status_screen (int);       /*  staticed  */
int check_xp (int, int);        /*  staticed  */
void level_up (int);            /*  staticed  */
#endif /*  0  */



#endif /*  __MENU_H  */
