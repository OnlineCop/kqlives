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

#ifndef __ITEMMENU_H
#define __ITEMMENU_H



void camp_item_menu (void);	/*  menu.c  */
int check_inventory (int, int);	/*  many functions ;)  */
void remove_item (int, int);	/*  (eqp|shop)menu.c, heroc.c  */
int item_effects (int, int, int);	/*  heroc.c  */



#if 0
void draw_itemmenu (int, int, int);	/*  staticed  */
void sort_items (void);		/*  staticed  */
void join_items (void);		/*  staticed  */
int confirm_drop (void);	/*  staticed  */
int item_quantity (int, int, int);	/*  did not exist  */
void camp_item_targetting (int);	/*  staticed  */
void sort_inventory (void);	/*  staticed  */
#endif /*  0  */



#endif /*  __ITEMMENU_H  */
