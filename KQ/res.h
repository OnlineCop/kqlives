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

#ifndef __RES_H
#define __RES_H

#include "allegro.h"

#define NUM_ITEMS     190
#define NUM_SPELLS     61
#define NUM_EFFECTS    52
#define NUM_ETROWS    106
#define NUM_BATTLES    56

typedef struct
{
   char name[17];
   unsigned char icon, kol;
   char desc[40];
   unsigned char tgt, type, use, ilvl, hnds, bst, elem, imb, eff;
   int bon, price;
   unsigned char eq[8];
   int stats[13];
   char res[16];
}
s_item;

typedef struct
{
   char name[13];
   unsigned char icon;
   char desc[26];
   unsigned char stat, mpc, use, tgt;
   int dmg, bon, hit;
   unsigned char elem, dlvl, eff;
   int clvl[8];
}
s_spell;

typedef struct
{
   unsigned char numf;
   unsigned short xsize, ysize;
   unsigned char orient;
   unsigned short delay;
   unsigned char kolor;
   unsigned char snd;
   char ename[16];
}
s_effect;

typedef struct
{
   unsigned char tnum, lvl, per;
   unsigned char idx[5];
}
s_erow;

typedef struct
{
   unsigned char mapnum, zonenum;
   unsigned char enc, etnum, eidx;
   char bmusic[16];
   char backimg[20];
}
s_encounter;

extern PALETTE pal;
extern s_item items[NUM_ITEMS];
extern s_spell magic[NUM_SPELLS];
extern s_effect eff[NUM_EFFECTS];
extern s_erow erows[NUM_ETROWS];
extern s_encounter battles[NUM_BATTLES];

#endif
