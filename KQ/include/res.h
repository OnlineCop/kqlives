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
 * \brief Definitions of resource types
 * \author JB
 * \date ????????
 */
#ifndef __RES_H
#define __RES_H



#include <allegro.h>

/*!\name Size of resource tables */
/*\{*/
#define NUM_ITEMS     190
#define NUM_SPELLS     61
#define NUM_EFFECTS    52
#define NUM_ETROWS    106
#define NUM_BATTLES    56
/*\}*/


/*! \brief An item */
typedef struct
{
   char name[17];
   unsigned char icon;
   unsigned char kol;
   char desc[40];
   unsigned char tgt;
   unsigned char type;
   unsigned char use;
   unsigned char ilvl;
   unsigned char hnds;
   unsigned char bst;
   unsigned char elem;
   unsigned char imb;
   unsigned char eff;
   int bon;
   int price;
   unsigned char eq[8];
   int stats[13];
   char res[16];
}
s_item;


/*! \brief A spell */
typedef struct
{
   char name[13];
   unsigned char icon;
   char desc[26];
   unsigned char stat;
   unsigned char mpc;
   unsigned char use;
   unsigned char tgt;
   int dmg;
   int bon;
   int hit;
   unsigned char elem;
   unsigned char dlvl;
   unsigned char eff;
   int clvl[8];
}
s_spell;


/*! \brief A special effect */
typedef struct
{
   unsigned char numf;
   unsigned short xsize;
   unsigned short ysize;
   unsigned char orient;
   unsigned short delay;
   unsigned char kolor;
   unsigned char snd;
   char ename[16];
}
s_effect;


/*! \brief An encounter */
typedef struct
{
   unsigned char tnum;
   unsigned char lvl;
   unsigned char per;
   unsigned char idx[5];        /*!< Index of enemies */
}
s_erow;


/*! \brief An actual battle */
typedef struct
{
   unsigned char mapnum;
   unsigned char zonenum;
   unsigned char enc;
   unsigned char etnum;
   unsigned char eidx;
   char bmusic[16];
   char backimg[20];
}
s_encounter;



extern PALETTE pal;             /*  draw.c, hskill.c, intrface.c,    */
                                    /*  kq.c, magic.c, menu.c, sgame.c,  */
                                    /*  shopmenu.c                       */
extern s_item items[NUM_ITEMS]; /*  combat.c, effects.c, heroc.c,   */
                                    /*  (eqp|item|shop)menu.c, menu.c,  */
                                    /*  hskill.c, intrface.c,           */
extern s_spell magic[NUM_SPELLS];       /*  enemyc.c, heroc.c, hskill.c,  */
                                    /*  (item|mas)menu.c, magic.c     */
extern s_effect eff[NUM_EFFECTS];       /*  effects.c, hskill.c, magic.c  */
extern s_erow erows[NUM_ETROWS];        /*  combat.c, enemyc.c  */
extern s_encounter battles[NUM_BATTLES];        /*  only in combat.c  */



#endif /*  __RES_H  */
