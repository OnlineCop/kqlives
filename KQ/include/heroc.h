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

/*! \file
 * \brief Hero combat header file
 *
 * Some functions declared here are not used outside heroc.c 
 *  and therefore have been #ifdef'd out.
 */

#ifndef __HEROC_H
#define __HEROC_H



void hero_init (void);          /*  combat.c  */
void hero_choose_action (int);  /*  combat.c  */
void auto_herochooseact (int);  /*  combat.c  */
int available_spells (int);     /*  hskill.c  */
int combat_spell_menu (int);    /*  hskill.c  */



#if 0
int hero_attack (int);          /*  staticed  */
void combat_draw_items (int);   /*  staticed  */
int combat_item_menu (int);     /*  staticed  */
int combat_item_usable (int);   /*  staticed  */
int combat_item (int, int, int);        /*  staticed  */
void draw_invokable (int);      /*  staticed  */
int can_invoke_item (int);      /*  staticed  */
int hero_invoke (int);          /*  staticed  */
int hero_invokeitem (int, int); /*  staticed  */
void hero_run (int);            /*  staticed  */
void combat_draw_spell_menu (int, int, int);    /*  staticed  */
int combat_spell_targeting (int);       /*  staticed  */
int combat_castable (int, int); /*  staticed  */
#endif /*  0  */



#endif /*  __HEROC_H  */
