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

void draw_itemmenu(int,int,int);
void camp_item_menu(void);
void sort_items(void);
void join_items(void);
int confirm_drop(void);
int item_quantity(int,int,int);
void camp_item_targetting(int);
int check_inventory(int,int);
void remove_item(int,int);
void sort_inventory(void);
int item_effects(int,int,int);

#endif
