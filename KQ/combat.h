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

#ifndef __COMBAT_H
#define __COMBAT_H

#define BATTLE_INC   20
#define ROUND_MAX    400
#define DMG_RND_MIN  2

int combat_check(int,int);
int combat(int);
void init_fighters(void);
void snap_togrid(void);
void roll_initiative(void);
void battle_render(int,int,int);
void do_round(void);
void do_action(int);
int do_move(int,int);
void draw_fighter(int,int);
int check_adjacent(int,int);
int check_end(void);
int fight(int,int,int);
void multi_fight(int);
int attack_result(int,int);
void enemies_win(void);
void heroes_win(void);
int give_ap(int,int);
void fkill(int);

extern int combatend,cspd[NUM_FIGHTERS],cact[NUM_FIGHTERS];
extern int curx,cury,curw,numens;
extern int tmpd[NUM_FIGHTERS];
extern int ta[NUM_FIGHTERS],deffect[NUM_FIGHTERS];
extern int nspeed[NUM_FIGHTERS],bspeed[NUM_FIGHTERS],rcount;
extern unsigned char vspell,hs,ms;
extern DATAFILE *backart;

#endif
