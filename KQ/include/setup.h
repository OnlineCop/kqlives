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

#ifndef __SETUP_H
#define __SETUP_H

/*! \file*/

#include <allegro.h>

/*  RB IDEA: We can use the COUNT definition of the new datafile  */
/*           dump rather than hardcoding the value here.          */
#define MAX_SAMPLES  43

#define SND_MENU     0
#define SND_CLICK    1
#define SND_BAD      2
#define SND_ITEM     3
#define SND_EQUIP    4
#define SND_UNEQUIP  5
#define SND_MONEY    6
#define SND_TWINKLE  7
#define SND_EXPLODE 42


void parse_setup (void);        /*  kq.c  */
void config_menu (void);        /*  sgame.c  */
void set_graphics_mode (void);  /*  kq.c  */
void sound_init (void);         /*  kq.c  */
void free_samples (void);       /*  kq.c  */
                                 /*  sgame.c                               */
void play_effect (int, int);    /*  draw.c, effects.c, entity.c, heroc.c, */
                                 /*  (eqp|item|mas|shop)menu.c, hskill.c,  */
                                 /*  intrface.c, menu.c, selector.c,       */
                                 /*  sgame.c                               */

extern char debugging;          /*  combat.c, setup.c, sgame.c  */
extern char slow_computer;      /*  effects.c, heroc.c, setup.c  */





#endif /*  __SETUP_H  */
