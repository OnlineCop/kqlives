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

#ifndef __HEROC_H
#define __HEROC_H

void hero_init(void);
void hero_choose_action(int);
void auto_herochooseact(int);
int hero_attack(int);
void combat_draw_items(int);
int combat_item_menu(int);
int combat_item_usable(int);
int combat_item(int,int,int);
void draw_invokable(int);
int can_invoke_item(int);
int hero_invoke(int);
int hero_invokeitem(int,int);
void hero_run(int);
int available_spells(int);
void combat_draw_spell_menu(int,int,int);
int combat_spell_menu(int);
int combat_spell_targeting(int);
int combat_castable(int,int);

#endif
