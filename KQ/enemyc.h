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

int select_encounter(int,int);
void enemy_init(void);
void load_enemyframes(int,int,int);
void enemy_chooseaction(int);
int enemy_cancast(int,int);
void enemy_curecheck(int,int);
void enemy_spellcheck(int,int);
int enemy_stscheck(int,int,int);
void enemy_skillcheck(int,int);
void enemy_charmaction(int);
int spell_setup(int,int);
int skill_setup(int,int);
void enemy_attack(int);
void enemy_run(int);
void enemy_spell(int);

#endif
