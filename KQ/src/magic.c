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

#include <stdio.h>
#include <string.h>

#include "setup.h"
#include "effects.h"
#include "kq.h"
#include "itemmenu.h"
#include "menu.h"
#include "draw.h"
#include "combat.h"
#include "enemyc.h"
#include "magic.h"
#include "ssprites.h"
#include "res.h"
#include "itemdefs.h"
#include "progress.h"



/*  internal functions  */
static void set_timed_sts_effect (int, int);
static void special_spells (int, int);
static void cure_oneall_allies (int, int, int);
static void heal_one_ally (int, int, int);
static void spell_damage (int, int, int, int);
static void damage_oneall_enemies (int, int, int);
static void damage_all_enemies (int, int);
static void beffect_one_enemy (int, int, int);
static void beffect_all_enemies (int, int);
static void geffect_one_ally (int, int, int);
static void geffect_all_allies (int, int);



/*
   This function just calls the right magic routine based on the spell's
   type and target.  This function also displays the caster and spell
   effects.
*/
int combat_spell (int whom, int is_item)
{
   int a, b, st, tgt, sn, tall = 0, nt = 1, ss = 0;

   sn = fighter[whom].csmem;
   if (magic[sn].tgt == TGT_NONE)
      return 0;
   tgt = fighter[whom].ctmem;
   nt = 1;
   st = tgt;
   if (magic[sn].tgt >= TGT_ALLY_ONE && magic[sn].tgt <= TGT_ALLY_ALL)
     {
	if (tgt == SEL_ALL_ALLIES)
	  {
	     tall = 1;
	     if (whom < PSIZE)
	       {
		  nt = numchrs;
		  st = 0;
	       }
	     else
	       {
		  nt = numens;
		  st = PSIZE;
	       }
	  }
     }
   else
     {
	if (tgt == SEL_ALL_ENEMIES)
	  {
	     tall = 1;
	     if (whom < PSIZE)
	       {
		  nt = numens;
		  st = PSIZE;
	       }
	     else
	       {
		  nt = numchrs;
		  st = 0;
	       }
	  }
     }
   strcpy (ctext, magic[sn].name);
   if (is_item == 0)
      draw_castersprite (whom, eff[magic[sn].eff].kolor);
   if (magic[sn].dmg > 0)
      ss = 1;
   if (sn == M_LIFE || sn == M_FULLLIFE)
      deadeffect = 1;
   if (sn == M_TREMOR || sn == M_EARTHQUAKE)
     {
	if (st == 0)
	   draw_hugesprite (st, 80, 126, magic[sn].eff, 1);
	else
	   draw_hugesprite (st, 80, 66, magic[sn].eff, 1);
     }
   else
     {
	if (sn == M_FLOOD || sn == M_TSUNAMI)
	  {
	     if (st == 0)
		draw_hugesprite (st, 80, 108, magic[sn].eff, 1);
	     else
		draw_hugesprite (st, 80, 56, magic[sn].eff, 1);
	  }
	else
	  {
	     if (sn != M_VISION && sn != M_WARP)
		draw_spellsprite (st, tall, magic[sn].eff, ss);
	  }
     }

   if (sn == M_LIFE || sn == M_FULLLIFE)
      deadeffect = 0;

   if (cast_spell (whom, is_item) == 0)
     {
	/* do failure sound and/or graphic */
	return 0;
     }

   if (sn == M_ABSORB || sn == M_DRAIN)
     {
	if (sn == M_ABSORB)
	  {
	     if (ta[tgt] <= 0)
	       {
		  display_amount (tgt, FRED, 0);
		  adjust_mp (tgt, ta[tgt]);
		  display_amount (whom, FGREEN, 0);
		  adjust_mp (whom, ta[whom]);
	       }
	     else
	       {
		  display_amount (whom, FRED, 0);
		  adjust_mp (whom, ta[whom]);
		  display_amount (tgt, FGREEN, 0);
		  adjust_mp (tgt, ta[tgt]);
	       }
	  }
	else
	  {
	     if (ta[tgt] <= 0)
	       {
		  display_amount (tgt, FNORMAL, 0);
		  adjust_hp (tgt, ta[tgt]);
		  display_amount (whom, FYELLOW, 0);
		  adjust_hp (whom, ta[whom]);
	       }
	     else
	       {
		  display_amount (whom, FNORMAL, 0);
		  adjust_hp (whom, ta[whom]);
		  display_amount (tgt, FYELLOW, 0);
		  adjust_hp (tgt, ta[tgt]);
	       }
	  }
     }
   else
     {
	if (ss == 0)
	  {
	     b = 0;
	     for (a = st; a < st + nt; a++)
		if (ta[a] == MISS && ta[a] != NODISPLAY)
		   b++;
	     if (b > 0)
		display_amount (st, FNORMAL, tall);
	  }
	else
	  {
	     display_amount (st, FDECIDE, tall);
	     for (a = st; a < st + nt; a++)
		adjust_hp (a, ta[a]);
	  }
     }
   b = 0;
   for (a = st; a < st + nt; a++)
     {
	if (fighter[a].sts[S_DEAD] == 0 && fighter[a].hp <= 0)
	  {
	     fkill (a);
	     ta[a] = 1;
	     b++;
	  }
	else
	   ta[a] = 0;
     }
   if (b > 0)
      death_animation (st, tall);

   return 1;
}

/*
   generic function called from camp or combat to cast a spell
*/
int cast_spell (int whom, int is_item)
{
   int sn = fighter[whom].csmem;
   int tgt = fighter[whom].ctmem;
   int c;

   if (is_item == 0)
     {
	c = mp_needed (whom, sn);
	if (c < 1)
	   c = 1;
	fighter[whom].mp -= c;
	/*
	   check for spell failure - only applies to spells that
	   don't have a hit% or do damage
	 */
	if (magic[sn].dmg == 0 && magic[sn].bon == 0 && magic[sn].hit == 0)
	  {
	     if (rand () % 100 + 1 >
		 fighter[whom].stats[A_AUR + magic[sn].stat])
		return 0;
	  }
     }

   /* call the appropriate spell effect function */
   switch (magic[sn].icon)
     {
     case 40:
     case 41:
	special_spells (whom, sn);
	break;
     case 45:
	cure_oneall_allies (whom, tgt, sn);
	break;
     case 46:
	heal_one_ally (whom, tgt, sn);
	break;
     case 47:
     case 42:
	if (magic[sn].tgt == TGT_ALLY_ONE)
	   geffect_one_ally (whom, tgt, sn);
	else
	   geffect_all_allies (whom, sn);
	break;
     case 48:
     case 43:
	if (magic[sn].tgt == TGT_ENEMY_ONE)
	   beffect_one_enemy (whom, tgt, sn);
	else
	   beffect_all_enemies (whom, sn);
	break;
     case 49:
     case 44:
	if (magic[sn].tgt == TGT_ENEMY_ALL)
	   damage_all_enemies (whom, sn);
	else
	   damage_oneall_enemies (whom, tgt, sn);
	break;
     }
   return 1;
}

/*
   This is used to invoke items inbued with a spell
*/
void cast_imbued_spell (int w, int i, int d, int t)
{
   int a, ts[4];

   for (a = 0; a < 4; a++)
      ts[a] = fighter[w].stats[A_INT + a];
   fighter[w].stats[A_INT] = d;
   fighter[w].stats[A_SAG] = d;
   fighter[w].stats[A_AUR] = 100;
   fighter[w].stats[A_SPI] = 100;
   fighter[w].csmem = i;
   fighter[w].ctmem = t;
   if (t == TGT_CASTER)
     {
	fighter[w].ctmem = w;
	cast_spell (w, 1);
     }
   else
      combat_spell (w, 1);
   for (a = 0; a < 4; a++)
      fighter[w].stats[A_INT + a] = ts[a];
}

/*
   Special spells like warp and vision.
*/
static void special_spells (int cs, int sn)
{
   if (cs >= PSIZE)
     {
	sprintf (strbuf, "Enemy %d tried to cast %s?!", cs, magic[sn].name);
	klog (strbuf);
     }
   switch (sn)
     {
     case M_VISION:
	fade_out (2);
	vspell = 1;
	battle_render (0, 0, 0);
	blit2screen (0, 0);
	fade_in (pal, 2);
	break;
     case M_WARP:
	if (in_combat == 1)
	  {
	     fade_out (2);
	     revert_equipstats ();
	     drawmap ();
	     blit2screen (xofs, yofs);
	     fade_in (pal, 2);
	     combatend = 2;
	  }
	else
	  {
	     if (g_map.map_no == MAP_MAIN)
		change_map ("town4", g_map.warpx, g_map.warpy, g_map.warpx,
			    g_map.warpy);
	     else
		change_map ("main", g_map.warpx, g_map.warpy, g_map.warpx,
			    g_map.warpy);
	  }
	break;
     case M_REPULSE:
	progress[P_REPULSE] = 150;
	break;
     }
}

/*
   This function only handles healing spells (one or all allied targets).
*/
static void cure_oneall_allies (int cs, int tgt, int sn)
{
   int a = 0, b = 0, z = 0, spwr;
   int nt, st, nn;

   if (tgt == SEL_ALL_ALLIES)
     {
	if (cs < PSIZE)
	  {
	     nt = numchrs;
	     st = 0;
	  }
	else
	  {
	     nt = numens;
	     st = PSIZE;
	  }
	nn = 1;
     }
   else
     {
	st = tgt;
	nt = 1;
	nn = 0;
     }
   spwr =
      magic[sn].dmg +
      (fighter[cs].stats[A_INT + magic[sn].stat] * magic[sn].bon / 100);
   if (spwr < DMG_RND_MIN * 5)
      b = rand () % DMG_RND_MIN + spwr;
   else
      b = rand () % (spwr / 5) + spwr;
   a = fighter[cs].stats[A_AUR + magic[sn].stat];
   b = b * a / 100;
   if (b < 1)
      b = 1;
   for (a = st; a < st + nt; a++)
      if (fighter[a].sts[S_STONE] == 0 && fighter[a].sts[S_DEAD] == 0)
	 z++;
   if (z == 0)
     {
	klog ("... the hell, how can there be nobody to cure?");
	return;
     }
   if (tgt == SEL_ALL_ALLIES)
      b = b / z;
/*
   If you wonder why I do this separate like this, it's just for looks.
   This way, it displays the amounts on screen, then adds the hp after
   the visual effect has taken place... it just looks nicer that way.
*/
   for (a = st; a < st + nt; a++)
     {
	if (fighter[a].sts[S_STONE] == 0 && fighter[a].sts[S_DEAD] == 0)
	  {
	     ta[a] = b;
	     ta[a] = do_shell_check (a, ta[a]);
	  }
     }
}

/*
   This is for a special category of spells which are beneficial, but
   not really effect spells or curative spells.
*/
static void heal_one_ally (int cs, int tgt, int sn)
{
   int a, b = 0;

   if (rand () % 100 + 1 > fighter[cs].stats[A_AUR + magic[sn].stat])
     {
	ta[tgt] = MISS;
	return;
     }
   switch (sn)
     {
     case M_RESTORE:
	if (fighter[tgt].sts[S_DEAD] == 0)
	  {
	     fighter[tgt].sts[S_POISON] = 0;
	     fighter[tgt].sts[S_BLIND] = 0;
	  }
	break;
     case M_RECOVERY:
	if (fighter[tgt].sts[S_DEAD] == 0)
	  {
	     for (b = 0; b < 7; b++)
		fighter[tgt].sts[b] = 0;
	  }
	break;
     case M_LIFE:
	if (fighter[tgt].sts[S_DEAD] == 1)
	  {
	     for (a = 0; a < 24; a++)
		fighter[tgt].sts[a] = 0;
	     fighter[tgt].hp = 1;
	     fighter[tgt].aframe = 0;
	  }
	else
	   ta[tgt] = MISS;
	break;
     case M_FULLLIFE:
	if (fighter[tgt].sts[S_DEAD] == 1)
	  {
	     for (a = 0; a < 24; a++)
		fighter[tgt].sts[a] = 0;
	     fighter[tgt].hp = fighter[tgt].mhp;
	     fighter[tgt].aframe = 0;
	  }
	else
	   ta[tgt] = MISS;
	break;
     }
}

/*
   These are 'good' effect spells that affect a single allied target.
*/
static void geffect_one_ally (int cs, int tgt, int sn)
{
   if (rand () % 100 + 1 > fighter[cs].stats[A_AUR + magic[sn].stat]
       || fighter[tgt].sts[S_STONE] > 0)
     {
	ta[tgt] = MISS;
	return;
     }
   switch (sn)
     {
     case M_TRUEAIM:
	if (fighter[tgt].sts[S_TRUESHOT] == 0)
	   fighter[tgt].sts[S_TRUESHOT] = 1;
	else
	   ta[tgt] = MISS;
	break;
     case M_THROUGH:
	if (fighter[tgt].sts[S_ETHER] == 0)
	   fighter[tgt].sts[S_ETHER] = 3;
	else
	   ta[tgt] = MISS;
	break;
     case M_REGENERATE:
	if (fighter[tgt].sts[S_REGEN] == 0)
	   set_timed_sts_effect (tgt, S_REGEN);
	else
	   ta[tgt] = MISS;
	break;
     case M_HOLYMIGHT:
	if (fighter[tgt].sts[S_STRENGTH] < 2)
	   fighter[tgt].sts[S_STRENGTH]++;
	else
	   ta[tgt] = MISS;
	break;
     case M_SHELL:
	if (fighter[tgt].sts[S_RESIST] == 0)
	   fighter[tgt].sts[S_RESIST] = 1;
	else
	   ta[tgt] = MISS;
	break;
     case M_WALL:
	if (fighter[tgt].sts[S_RESIST] != 2)
	   fighter[tgt].sts[S_RESIST] = 2;
	else
	   ta[tgt] = MISS;
	break;
     case M_SHIELD:
	if (fighter[tgt].sts[S_SHIELD] == 0)
	   fighter[tgt].sts[S_SHIELD] = 1;
	else
	   ta[tgt] = MISS;
	break;
     case M_HASTEN:
	if (fighter[tgt].sts[S_TIME] != 2)
	  {
	     if (fighter[tgt].sts[S_TIME] == 1)
		fighter[tgt].sts[S_TIME] = 0;
	     else
		fighter[tgt].sts[S_TIME] = 2;
	  }
	else
	   ta[tgt] = MISS;
	break;
     }
}

/*
   These are 'good' effect spells that affect all allied targets.
*/
static void geffect_all_allies (int cs, int sn)
{
   int nt, st, a, b = 0;

   if (cs < PSIZE)
     {
	nt = numchrs;
	st = 0;
     }
   else
     {
	nt = numens;
	st = PSIZE;
     }
   if (rand () % 100 + 1 > fighter[cs].stats[A_AUR + magic[sn].stat])
     {
	for (b = st; b < st + nt; b++)
	   ta[b] = MISS;
	return;
     }
   switch (sn)
     {
     case M_BLESS:
	for (b = st; b < st + nt; b++)
	  {
	     if (fighter[b].sts[S_BLESS] < 3)
	       {
		  a = fighter[b].mhp / 10;
		  if (a < 10)
		     a = 10;
		  fighter[b].hp += a;
		  fighter[b].mhp += a;
		  fighter[b].sts[S_BLESS]++;
		  ta[b] = NODISPLAY;
	       }
	     else
		ta[b] = MISS;
	  }
	break;
     case M_SHIELDALL:
	for (b = st; b < st + nt; b++)
	  {
	     if (fighter[b].sts[S_SHIELD] < 2)
	       {
		  fighter[b].sts[S_SHIELD] = 2;
		  ta[b] = NODISPLAY;
	       }
	     else
		ta[b] = MISS;
	  }
	break;
     case M_DIVINEGUARD:
	for (b = st; b < st + nt; b++)
	  {
	     if (fighter[b].sts[S_SHIELD] < 2 || fighter[b].sts[S_RESIST] < 2)
	       {
		  if (fighter[b].sts[S_SHIELD] < 2)
		     fighter[b].sts[S_SHIELD] = 2;
		  if (fighter[b].sts[S_RESIST] < 2)
		     fighter[b].sts[S_RESIST] = 2;
	       }
	     else
		ta[b] = MISS;
	  }
	break;
     case M_QUICKEN:
	for (b = st; b < st + nt; b++)
	  {
	     if (fighter[b].sts[S_TIME] != 2 && fighter[b].sts[S_STONE] == 0)
	       {
		  if (fighter[b].sts[S_TIME] == 1)
		     fighter[b].sts[S_TIME] = 0;
		  else
		    {
		       fighter[b].sts[S_TIME] = 2;
		       ta[b] = NODISPLAY;
		    }
	       }
	     else
		ta[b] = MISS;
	  }
	break;
     }
}

/*
   This function handles 'bad' effect spells that have a single target.
*/
static void beffect_one_enemy (int cs, int tgt, int sn)
{
   int r, a = 0, sp_hit;

   if (fighter[tgt].sts[S_STONE] > 0)
     {
	ta[tgt] = MISS;
	return;
     }
   if (res_throw (tgt, magic[sn].elem) == 1)
     {
	ta[tgt] = MISS;
	return;
     }
   sp_hit = magic[sn].hit;
   switch (sn)
     {
     case M_BLIND:
	if (non_dmg_save (tgt, sp_hit) == 0 && fighter[tgt].sts[S_BLIND] == 0)
	   fighter[tgt].sts[S_BLIND] = 1;
	else
	   ta[tgt] = MISS;
	break;
     case M_CONFUSE:
	if (non_dmg_save (tgt, sp_hit) == 0 && fighter[tgt].sts[S_CHARM] == 0)
	   fighter[tgt].sts[S_CHARM] = rand () % 3 + 3;
	else
	   ta[tgt] = MISS;
	break;
     case M_STONE:
	if (non_dmg_save (tgt, sp_hit) == 0)
	  {
	     for (a = 0; a < 24; a++)
		if (a != S_DEAD)
		   fighter[tgt].sts[a] = 0;
	     fighter[tgt].sts[S_STONE] = rand () % 3 + 3;
	  }
	else
	   ta[tgt] = MISS;
	break;
     case M_DIFFUSE:
	if (non_dmg_save (tgt, sp_hit) == 0)
	  {
	     r = 0;
	     if (fighter[tgt].sts[S_RESIST] > 0)
	       {
		  fighter[tgt].sts[S_RESIST] = 0;
		  r++;
	       }
	     if (fighter[tgt].sts[S_TIME] > 1)
	       {
		  fighter[tgt].sts[S_TIME] = 0;
		  r++;
	       }
	     if (fighter[tgt].sts[S_SHIELD] > 0)
	       {
		  fighter[tgt].sts[S_SHIELD] = 0;
		  r++;
	       }
	     if (fighter[tgt].sts[S_BLESS] > 0)
	       {
		  fighter[tgt].sts[S_BLESS] = 0;
		  r++;
	       }
	     if (fighter[tgt].sts[S_STRENGTH] > 0)
	       {
		  fighter[tgt].sts[S_STRENGTH] = 0;
		  r++;
	       }
	     if (r == 0)
		ta[tgt] = MISS;
	  }
	else
	   ta[tgt] = MISS;
	break;
     case M_HOLD:
	if (non_dmg_save (tgt, sp_hit) == 0 && fighter[tgt].sts[S_STOP] == 0)
	   fighter[tgt].sts[S_STOP] = rand () % 3 + 2;
	else
	   ta[tgt] = MISS;
	break;
     case M_SILENCE:
	if (non_dmg_save (tgt, sp_hit) == 0 && fighter[tgt].sts[S_MUTE] == 0)
	   fighter[tgt].sts[S_MUTE] = 1;
	else
	   ta[tgt] = MISS;
	break;
     case M_SLEEP:
	if (non_dmg_save (tgt, sp_hit) == 0 && fighter[tgt].sts[S_SLEEP] == 0)
	   fighter[tgt].sts[S_SLEEP] = rand () % 2 + 4;
	else
	   ta[tgt] = MISS;
	break;
     case M_ABSORB:
	spell_damage (cs, sn, tgt, 1);
	r = ta[tgt];
	if (non_dmg_save (tgt, sp_hit) == 1)
	   r = r / 2;
	if (fighter[tgt].mp < abs (r))
	   r = 0 - fighter[tgt].mp;
	ta[tgt] = r;
	ta[cs] = 0 - r;
	break;
     case M_DRAIN:
	spell_damage (cs, sn, tgt, 1);
	r = ta[tgt];
	if (non_dmg_save (tgt, sp_hit) == 1)
	   r = r / 2;
	if (fighter[tgt].unl > 0)
	  {
	     if (fighter[cs].hp < abs (r))
		r = 0 - fighter[cs].hp;
	     ta[tgt] = 0 - r;
	     ta[cs] = r;
	  }
	else
	  {
	     if (fighter[tgt].hp < abs (r))
		r = 0 - fighter[tgt].hp;
	     ta[tgt] = r;
	     ta[cs] = 0 - r;
	  }
	break;
     case M_DOOM:
	if (non_dmg_save (tgt, sp_hit) == 0)
	  {
	     a = fighter[tgt].hp * 3 / 4;
	     if (a < 1)
		a = 1;
	     if (fighter[tgt].hp - a < 1)
		a = fighter[tgt].hp - 1;
	     ta[tgt] = 0 - a;
	  }
	else
	   ta[tgt] = MISS;
	break;
     case M_DEATH:
	if (non_dmg_save (tgt, sp_hit) == 0)
	  {
	     a = fighter[tgt].hp;
	     ta[tgt] = 0 - a;
	  }
	else
	   ta[tgt] = MISS;
	break;
     case M_NAUSEA:
	if (non_dmg_save (tgt, sp_hit) == 0
	    && fighter[tgt].sts[S_MALISON] == 0)
	   fighter[tgt].sts[S_MALISON] = 1;
	else
	   ta[tgt] = MISS;
	break;
     }
}

/*
   These are 'bad' effect spells that affect all enemy targets.
*/
static void beffect_all_enemies (int cs, int sn)
{
   int nt, st, a, sp_hit;

   if (cs < PSIZE)
     {
	nt = numens;
	st = PSIZE;
     }
   else
     {
	nt = numchrs;
	st = 0;
     }
   sp_hit = magic[sn].hit;
   switch (sn)
     {
     case M_SLOW:
	for (a = st; a < st + nt; a++)
	  {
	     if (res_throw (a, magic[sn].elem) == 0
		 && non_dmg_save (a, sp_hit) == 0
		 && fighter[a].sts[S_STONE] == 0)
	       {
		  if (fighter[a].sts[S_TIME] == 2)
		     fighter[a].sts[S_TIME] = 0;
		  else
		    {
		       if (fighter[a].sts[S_TIME] == 0)
			 {
			    fighter[a].sts[S_TIME] = 1;
			    ta[a] = NODISPLAY;
			 }
		       else
			  ta[a] = MISS;
		    }
	       }
	     else
		ta[a] = MISS;
	  }
	break;
     case M_VISION:
	vspell = 1;
	fade_out (2);
	battle_render (0, 0, 0);
	blit2screen (0, 0);
	fade_in (pal, 2);
	break;
     case M_MALISON:
	for (a = st; a < st + nt; a++)
	  {
	     if (non_dmg_save (a, sp_hit) == 0
		 && fighter[a].sts[S_MALISON] == 0
		 && fighter[a].sts[S_STONE] == 0)
	       {
		  fighter[a].sts[S_MALISON] = 2;
		  ta[a] = NODISPLAY;
	       }
	     else
		ta[a] = MISS;
	  }
	break;
     case M_SLEEPALL:
	for (a = st; a < st + nt; a++)
	  {
	     if (res_throw (a, magic[sn].elem) == 0
		 && non_dmg_save (a, sp_hit) == 0
		 && fighter[a].sts[S_SLEEP] == 0
		 && fighter[a].sts[S_STONE] == 0)
	       {
		  fighter[a].sts[S_SLEEP] = rand () % 2 + 4;
		  ta[a] = NODISPLAY;
	       }
	     else
		ta[a] = MISS;
	  }
	break;
     }
}

/*
   These are damage spells that affect the entire enemy party.
*/
static void damage_all_enemies (int cs, int sn)
{
   int nt, st;

   if (cs < PSIZE)
     {
	nt = numens;
	st = PSIZE;
     }
   else
     {
	nt = numchrs;
	st = 0;
     }
   spell_damage (cs, sn, st, nt);
}

static void damage_oneall_enemies (int cs, int tgt, int sn)
{
   int nt, st;

   if (tgt == SEL_ALL_ENEMIES)
     {
	if (cs < PSIZE)
	  {
	     nt = numens;
	     st = PSIZE;
	  }
	else
	  {
	     nt = numchrs;
	     st = 0;
	  }
     }
   else
     {
	st = tgt;
	nt = 1;
     }
   spell_damage (cs, sn, st, nt);
}

/*
   This is for skills and items that cause damage, but don't duplicate spells.
   Essentially, this is only used for things where the user's magic power
   doesn't affect the power of the effect.
*/
void special_damage_oneall_enemies (int cs, int sp_dmg, int rt, int tgt,
				    int split)
{
   int nt, st, a, b = 0, nn = 0, ne = 0, ad = 1;

   if (tgt == SEL_ALL_ENEMIES)
     {
	if (cs < PSIZE)
	  {
	     st = PSIZE;
	     nt = numens;
	     for (a = PSIZE; a < PSIZE + numens; a++)
		if (fighter[a].sts[S_DEAD] == 0)
		   ne++;
	  }
	else
	  {
	     st = 0;
	     nt = numchrs;
	     for (a = 0; a < numchrs; a++)
		if (fighter[a].sts[S_DEAD] == 0)
		   ne++;
	  }
	nn = 1;
     }
   else
     {
	st = tgt;
	ne = 1;
	nt = 1;
     }
   if (ne == 0)
      return;
   if (sp_dmg < DMG_RND_MIN * 5)
      ad = rand () % DMG_RND_MIN + sp_dmg;
   else
      ad = rand () % (sp_dmg / 5) + sp_dmg;
   if (ne > 1 && split == 0)
      ad = ad / ne;
   for (a = st; a < st + nt; a++)
     {
	if (fighter[a].sts[S_DEAD] == 0 && fighter[a].mhp > 0)
	  {
	     tempd = status_adjust (a);
	     b = do_shell_check (a, ad);
	     b -= tempd.stats[A_MAG];
	     if (b < 0)
		b = 0;
	     b = res_adjust (a, rt, b);
	     if (fighter[a].sts[S_STONE] > 0 && rt != R_BLACK && rt != R_WHITE
		 && rt != R_EARTH && rt != R_WATER)
		b = b / 10;
	     ta[a] = 0 - b;
	     if (b < 0 && rt == R_POISON)
	       {
		  if (!res_throw (a, rt) && !non_dmg_save (a, 75))
		     set_timed_sts_effect (a, S_POISON);
	       }
	     if (ta[a] != 0)
		fighter[a].sts[S_SLEEP] = 0;
	  }
	else
	   ta[a] = 0;
     }
   display_amount (st, FDECIDE, nn);
   for (a = st; a < st + nt; a++)
      if (ta[a] != MISS)
	 adjust_hp (a, ta[a]);
   b = 0;
   for (a = st; a < st + nt; a++)
     {
	if (fighter[a].sts[S_DEAD] == 0 && fighter[a].hp <= 0)
	  {
	     fkill (a);
	     ta[a] = 1;
	     b++;
	  }
	else
	   ta[a] = 0;
     }
   if (b > 0)
      death_animation (st, nn);
}

/*
   This function does all of the damage calculating for damage
   spells, and fills the ta[] array with the damage amounts.
*/
static void spell_damage (int cs, int sn, int st, int nt)
{
   int a = 0, b = 0, ad = 0, rt = 0, ne = 0;

   if (nt > 1)
     {
	if (cs < PSIZE)
	  {
	     for (a = PSIZE; a < PSIZE + numens; a++)
		if (fighter[a].sts[S_DEAD] == 0)
		   ne++;
	  }
	else
	  {
	     for (a = 0; a < numchrs; a++)
		if (fighter[a].sts[S_DEAD] == 0)
		   ne++;
	  }
     }
   else
      ne = 1;
   if (ne == 0)
      return;
   rt = magic[sn].elem;
   ad =
      magic[sn].dmg +
      (fighter[cs].stats[A_INT + magic[sn].stat] * magic[sn].bon / 100);
   if (ad < DMG_RND_MIN * 5)
      ad += rand () % DMG_RND_MIN;
   else
      ad += rand () % (ad / 5);
   if (ad < 1)
      ad = 1;
   a = fighter[cs].stats[A_AUR + magic[sn].stat];
   ad = ad * a / 100;
   if (ad < 0)
      ad = 0;
   if (ne > 1 && magic[sn].tgt != TGT_ENEMY_ALL)
      ad = ad / ne;
   for (a = st; a < st + nt; a++)
     {
	if (fighter[a].sts[S_DEAD] == 0 && fighter[a].mhp > 0)
	  {
	     tempd = status_adjust (a);
	     b = do_shell_check (a, ad);
	     b -= tempd.stats[A_MAG];
	     if (b < 0)
		b = 0;
	     b = res_adjust (a, rt, b);
	     if (fighter[a].sts[S_STONE] > 0 && rt != R_BLACK && rt != R_WHITE
		 && rt != R_EARTH && rt != R_WATER)
		b = b / 10;
	     ta[a] = 0 - b;
	     if (b < 0 && rt == R_POISON)
	       {
		  if (!res_throw (a, rt) && !non_dmg_save (a, magic[sn].hit))
		     set_timed_sts_effect (a, S_POISON);
	       }
	     if (ta[a] != 0)
		fighter[a].sts[S_SLEEP] = 0;
	  }
	else
	   ta[a] = 0;
     }
}

/*
   This adjusts the passed damage amount based on the target's
   resistance to the passed element.  The adjusted value is
   then returned.
*/
int res_adjust (int tgt, int rs, int amt)
{
   int ad, b;
   s_fighter tf;

   if (rs > 15 || rs < 0)
      return amt;
   tf = status_adjust (tgt);
   ad = amt;
   if (tf.res[rs] < 0)
     {
	b = 10 + abs (tf.res[rs]);
	ad = ad * b / 10;
     }
   if (tf.res[rs] > 10)
      ad = ((ad * (tf.res[rs] - 10)) / 10) * -1;
   if (tf.res[rs] >= 1 && tf.res[rs] <= 10)
      ad -= ad * tf.res[rs] / 10;
   return ad;
}

/*
   This is a simple yes or no answer to an elemental/special
   resistance check.
*/
int res_throw (int tgt, int rs)
{
   s_fighter tf;

   if (rs > R_TIME || rs < R_EARTH)
      return 0;
   tf = status_adjust (tgt);
   if (tf.res[rs] < 1)
      return 0;
   if (tf.res[rs] >= 10)
      return 1;
   if (rand () % 10 < tf.res[rs])
      return 1;
   return 0;
}

/*
   This used to be fancier... but now this is basically used
   to test for status changes or other junk.
*/
int non_dmg_save (int tgt, int per)
{
/*  RB TODO:  */
   tgt = tgt;
   if (per == 0)

      return 1;
   if (rand () % 100 < per)
      return 0;
   else
      return 1;
}

/*
   This returns the amount of mp needed to cast a spell.  This
   function was created to allow for different mp consumption rates.
*/
int mp_needed (int who, int sn)
{
   int amt;

   amt = magic[sn].mpc;
   amt = amt * fighter[who].mrp / 100;
   if (amt < 1)
      amt = 1;
   if (sn > 0)
      return amt;
   else
      return 0;
}

/*
   I put this is just to make things nice and neat.
*/
void adjust_hp (int who, int amt)
{
   fighter[who].hp += amt;
   if (fighter[who].hp > fighter[who].mhp)
      fighter[who].hp = fighter[who].mhp;
   if (fighter[who].hp < 0)
      fighter[who].hp = 0;
}

/*
   I put this is just to make things nice and neat.
*/
void adjust_mp (int who, int amt)
{
   fighter[who].mp += amt;
   if (fighter[who].mp > fighter[who].mmp)
      fighter[who].mp = fighter[who].mmp;
   if (fighter[who].mp < 0)
      fighter[who].mp = 0;
}

/*
   This adjusts a fighter's stats by applying the effects
   of status-affecting spells.
*/
s_fighter status_adjust (int w)
{
   s_fighter tf;

   tf = fighter[w];
   if (tf.sts[S_STRENGTH] > 0)
      tf.stats[A_ATT] += tf.stats[A_STR] * tf.sts[S_STRENGTH] * 50 / 100;
   if (tf.sts[S_MALISON] == 1)
     {
	tf.stats[A_HIT] = tf.stats[A_HIT] * 75 / 100;
	tf.stats[A_EVD] = tf.stats[A_EVD] * 75 / 100;
     }
   if (tf.sts[S_MALISON] == 2)
     {
	tf.stats[A_HIT] = tf.stats[A_HIT] * 50 / 100;
	tf.stats[A_EVD] = tf.stats[A_EVD] * 50 / 100;
     }
   if (tf.sts[S_BLESS] > 0)
     {
	tf.stats[A_HIT] += tf.sts[S_BLESS] * 25;
	tf.stats[A_EVD] += tf.sts[S_BLESS] * 10;
     }
   if (tf.sts[S_TIME] == 1)
     {
	tf.stats[A_SPD] = tf.stats[A_SPD] * 5 / 10;
	tf.stats[A_HIT] = tf.stats[A_HIT] * 75 / 100;
	tf.stats[A_EVD] = tf.stats[A_EVD] * 75 / 100;
     }
   if (tf.sts[S_TIME] == 2)
     {
	tf.stats[A_SPD] = tf.stats[A_SPD] * 15 / 10;
	tf.stats[A_HIT] = tf.stats[A_HIT] * 15 / 10;
	tf.stats[A_EVD] = tf.stats[A_EVD] * 15 / 10;
     }
   if (tf.sts[S_TIME] == 3)
     {
	tf.stats[A_SPD] = tf.stats[A_SPD] * 2;
	tf.stats[A_HIT] = tf.stats[A_HIT] * 2;
	tf.stats[A_EVD] = tf.stats[A_EVD] * 2;
     }
   if (tf.sts[S_BLIND] > 0)
     {
	tf.stats[A_HIT] /= 4;
	if (tf.stats[A_HIT] < 1)
	   tf.stats[A_HIT] = 1;
	tf.stats[A_EVD] /= 4;
	if (tf.stats[A_EVD] < 1)
	   tf.stats[A_EVD] = 1;
     }
   if (tf.sts[S_SLEEP] > 0 || tf.sts[S_STOP] > 0)
      tf.stats[A_EVD] = 0;
   if (tf.sts[S_STONE] > 0)
     {
	tf.stats[A_DEF] *= 2;
	tf.stats[A_EVD] = 0;
     }
   return tf;
}

int do_shell_check (int tgt, int amt)
{
   int a = 0;

   if (fighter[tgt].sts[S_RESIST] == 0)
      return amt;
   if (fighter[tgt].sts[S_RESIST] == 1)
      a = amt * 75 / 100;
   if (fighter[tgt].sts[S_RESIST] == 2)
      a = amt * 5 / 10;
   return a;
}

int do_shield_check (int tgt, int amt)
{
   int a = 0;

   if (fighter[tgt].sts[S_SHIELD] == 0)
      return amt;
   if (fighter[tgt].sts[S_SHIELD] == 1)
      a = amt * 75 / 100;
   if (fighter[tgt].sts[S_SHIELD] == 2)
      a = amt * 666 / 1000;
   return a;
}

/*
   This is used to set things like poison and regen
   which activate based on the combat timer.
*/
static void set_timed_sts_effect (int who, int ss)
{
   fighter[who].sts[ss] = rcount + 1;
}
