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



#define BATTLE_INC    20
#define ROUND_MAX    400
#define DMG_RND_MIN    2



/*  combat function prototypes  */
int combat_check (int, int);    /*  entity.c  */
int combat (int);               /*  intrface.c  */
void battle_render (int, int, int);     /*  effects.c, [eh]skill.c, heroc.c,  */
                                    /*  magic.c, selector.c  */
void draw_fighter (int, int);   /*  effects.c, hskill.c  */
int fight (int, int, int);      /*  enemyc.c, [eh]skill.c, heroc.c  */
void multi_fight (int);         /*  hskill.c  */
void fkill (int);               /*  heroc.c, hskill.c, magic.c  */



/*  global combat related variables  */
extern int combatend;           /*  heroc.c, magic.c  */
extern int cact[NUM_FIGHTERS];  /*  enemyc.c, heroc.c, hskill.c  */
extern int curx;                /*  effects.c, heroc.c, hskill.c  */
extern int cury;                /*  effects.c, heroc.c, hskill.c  */
extern int numens;              /*  draw.c, effects.c, enemyc.c  */
extern int ta[NUM_FIGHTERS];    /*  effects.c, [he]skill.c, hero.c,  */
                                    /*  (item|mas)menu.c, kq.c, magic.c  */
extern int deffect[NUM_FIGHTERS];       /*  effects.c, hskill.c  */
extern int rcount;              /*  magic.c  */
extern unsigned char vspell;    /*  magic.c  */
extern unsigned char ms;        /*  heroc.c  */
extern struct DATAFILE *backart;        /*  hskill.c  */



#if 0
void init_fighters (void);      /*  staticed  */
void snap_togrid (void);        /*  staticed  */
void roll_initiative (void);    /*  staticed  */
void do_round (void);           /*  staticed  */
void do_action (int);           /*  staticed  */
int check_end (void);           /*  staticed  */
int attack_result (int, int);   /*  staticed  */
void enemies_win (void);        /*  staticed  */
void heroes_win (void)          /*  staticed  */
     int give_ap (int, int);    /*  does not exist  */
     int check_adjacent (int, int);     /*  does not exist  */
     int do_move (int, int);    /*  does not exist  */

     extern int cspd[NUM_FIGHTERS];     /*  unused  */
     extern int curw;           /*  staticed  */
     extern int nspeed[NUM_FIGHTERS];   /*  staticed  */
     extern int bspeed[NUM_FIGHTERS];   /*  staticed  */
     extern unsigned char hs;   /*  staticed  */
     extern int tmpd[NUM_FIGHTERS];     /*  staticed into selector.c, as it is  */
                                /*  only used there.  */
#endif



#endif /*  __COMBAT_H  */
