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

#ifndef __ENEMYC_H
#define __ENEMYC_H



int select_encounter (int, int);        /*  combat.c  */
void enemy_init (void);         /*  combat.c  */
void enemy_chooseaction (int);  /*  combat.c  */
void enemy_charmaction (int);   /*  combat.c, heroc.c  */



#if 0
void load_enemyframes (int, int, int);  /*  staticed  */
int enemy_cancast (int, int);   /*  staticed  */
void enemy_curecheck (int);     /*  staticed  */
void enemy_spellcheck (int, int);       /*  staticed  */
int enemy_stscheck (int, int);  /*  staticed  */
void enemy_skillcheck (int, int);       /*  staticed  */
int spell_setup (int, int);     /*  staticed  */
int skill_setup (int, int);     /*  staticed  */
void enemy_attack (int);        /*  staticed  */
void enemy_run (int);           /*  unused  */
void enemy_spell (int);         /*  did not exist  */
#endif /*  0  */



#endif /*  __ENEMYC_H  */
