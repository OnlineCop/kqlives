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
/** \file 
 * \brief Hero combat
 *
 * Stuff relating to hero's special combat skills
 *\author Josh Bolduc
 *\date   ????????
 *\todo PH make sure we understand the two methods of referring to 
 * a hero - either as an index in the pidx array or an index in the 
 * party array
 */

#include <stdio.h>
#include <string.h>

#include "setup.h"
#include "eqpmenu.h"
#include "draw.h"
#include "kq.h"
#include "heroc.h"
#include "combat.h"
#include "selector.h"
#include "menu.h"
#include "effects.h"
#include "skills.h"
#include "magic.h"
#include "res.h"
#include "progress.h"
#include "itemdefs.h"
#include "itemmenu.h"
#include "timing.h"

/** Text names of hero skills */
char sk_names[MAXCHRS][7] =
   { "Rage", "Sweep", "Infuse", "Prayer", "Boost", "Cover", "Steal",
   "Sense"
};



/*  internal functions  */
static int hero_attack (int);
static void combat_draw_items (int);
static int combat_item_menu (int);
static int combat_item_usable (int);
static int combat_item (int, int, int);
static void draw_invokable (int);
static int can_invoke_item (int);
static int hero_invoke (int);
static int hero_invokeitem (int, int);
static void hero_run (int);
static void combat_draw_spell_menu (int, int, int);
static int combat_spell_targeting (int);
static int combat_castable (int, int);


/** \brief Set up heroes
 *
 * This sets up the heroes fighter vars and frames.
 */
void hero_init (void)
{
   int z, i, p, n;
   DATAFILE *pb;

   update_equipstats ();
   pb = load_datafile_object (PCX_DATAFILE, "USBAT_PCX");
   for (i = 0; i < numchrs; i++)
     {
        for (p = 0; p < MAXCFRAMES; p++)
           clear_bitmap (cframes[i][p]);
        z = pidx[i];
        blit ((BITMAP *) pb->dat, cframes[i][0], 0, z * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][1], 32, z * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][2], 64, z * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][3], 96, z * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][4], 128, z * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][5], 160, z * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][6], z * 64 + 192,
              fighter[i].cwt * 32, 0, 0, 32, 32);
        blit ((BITMAP *) pb->dat, cframes[i][7], z * 64 + 224,
              fighter[i].cwt * 32, 0, 0, 32, 32);
        if (fighter[i].cwt > 0 && items[party[z].eqp[0]].kol > 0)
          {
             for (n = 0; n < cframes[i][0]->h; n++)
               {
                  for (p = 0; p < cframes[i][0]->w; p++)
                    {
                       if (cframes[i][6]->line[n][p] == 168)
                          cframes[i][6]->line[n][p] =
                             items[party[z].eqp[0]].kol;
                       else
                         {
                            if (cframes[i][6]->line[n][p] == 175)
                               cframes[i][6]->line[n][p] =
                                  items[party[z].eqp[0]].kol + 4;
                         }
                       if (cframes[i][7]->line[n][p] == 168)
                          cframes[i][7]->line[n][p] =
                             items[party[z].eqp[0]].kol;
                       else
                         {
                            if (cframes[i][7]->line[n][p] == 175)
                               cframes[i][7]->line[n][p] =
                                  items[party[z].eqp[0]].kol + 4;
                         }
                    }
               }
          }
        for (p = 0; p < MAXCFRAMES; p++)
           blit (cframes[i][p], tcframes[i][p], 0, 0, 0, 0, 32, 32);
        fighter[i].cw = 32;
        fighter[i].cl = 32;
        fighter[i].aframe = 0;
     }
   unload_datafile_object (pb);
}

/**\brief Show menu for action selection
 *
 * Give the player a menu for a specific character and         
 * allow him/her to choose an action.      
 *\param    who Index of player (see constants in progress.h)
 */
void hero_choose_action (int who)
{
   int stop = 0, sptr = 1, ptr = 0, rd = 1, a, amy;
   int my = 0, chi[9], tt;
   char ca[9][8];

   if (cact[who] == 0)
      return;
   unpress ();
   fighter[who].defend = 0;
   fighter[who].facing = 0;
   if (pidx[who] != CORIN && pidx[who] != CASANDRA)
      fighter[who].aux = 0;
   while (stop == 0)
     {
        if (rd == 1)
          {
             battle_render (who + 1, who + 1, 0);
             my = 0;
             strcpy (ca[my], "Attack");
             chi[my] = C_ATTACK;
             my++;
/*
      strcpy(ca[my],"Combo");
      chi[my] = C_COMBO;
      my++;
*/
             if (hero_skillcheck (who))
               {
                  strcpy (ca[my], sk_names[pidx[who]]);
                  chi[my] = C_SKILL;
                  my++;
               }
             if (fighter[who].sts[S_MUTE] == 0 && available_spells (who) > 0)
               {
                  strcpy (ca[my], "Spell");
                  chi[my] = C_SPELL;
                  my++;
               }
             if (progress[P_USEITEMINCOMBAT] == 0)
               {
                  strcpy (ca[my], "Item");
                  chi[my] = C_ITEM;
                  my++;
               }
             tt = 0;
             for (a = 0; a < 6; a++)
                if (can_invoke_item (party[pidx[who]].eqp[a]) == 0)
                   tt++;
             if (tt > 0)
               {
                  strcpy (ca[my], "Invoke");
                  chi[my] = C_INVOKE;
                  my++;
               }
             if (my > 5)
                amy = 224 - (my * 8);
             else
                amy = 184;
             menubox (double_buffer, 120, amy, 8, my, BLUE);
             for (a = 0; a < my; a++)
                print_font (double_buffer, 136, a * 8 + amy + 8, ca[a],
                            FNORMAL);
             if (sptr == 1)
                draw_sprite (double_buffer, menuptr, 120, ptr * 8 + amy + 8);
             if (sptr == 0)
               {
                  menubox (double_buffer, 64, amy, 6, 1, BLUE);
                  print_font (double_buffer, 72, amy + 8, "Defend", FNORMAL);
               }
             if (sptr == 2)
               {
                  menubox (double_buffer, 192, amy, 3, 1, BLUE);
                  print_font (double_buffer, 200, amy + 8, "Run", FNORMAL);
               }
             blit2screen (0, 0);
          }
        rd = 0;
        readcontrols ();
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = my - 1;
             rd = 1;
             play_effect (SND_CLICK, 128);
          }
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr >= my)
                ptr = 0;
             rd = 1;
             play_effect (SND_CLICK, 128);
          }
        if (left)
          {
             unpress ();
             sptr--;
             if (sptr < 0)
                sptr = 0;
             rd = 1;
          }
        if (right)
          {
             unpress ();
             sptr++;
             if (sptr > 1 + can_run)
                sptr = 1 + can_run;
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             if (sptr == 0)
               {
                  fighter[who].defend = 1;
                  cact[who] = 0;
                  stop = 1;
               }
             if (sptr == 2)
               {
                  hero_run (who);
                  cact[who] = 0;
                  stop = 1;
               }
             if (sptr == 1)
               {
                  switch (chi[ptr])
                    {
                    case C_ATTACK:
                       if (hero_attack (who) == 1)
                         {
                            cact[who] = 0;
                            stop = 1;
                         }
                       break;
                    case C_ITEM:
                       if (combat_item_menu (who) == 1)
                         {
                            cact[who] = 0;
                            stop = 1;
                         }
                       break;
                    case C_INVOKE:
                       if (hero_invoke (who) == 1)
                         {
                            cact[who] = 0;
                            stop = 1;
                         }
                       break;
                    case C_SPELL:
                       if (combat_spell_menu (who) == 1)
                         {
                            cact[who] = 0;
                            stop = 1;
                         }
                       break;
                    case C_SKILL:
                       if (skill_use (who) == 1)
                         {
                            cact[who] = 0;
                            stop = 1;
                         }
                       break;
                    }
               }
             rd = 1;
          }
     }
}

/** \brief Auto-choose options for confused player
 *
 *         Chooses actions for the character when s/he is                 
 *         charmed/confused.  This is pretty much the same as the         
 *         enemy_charmaction function and I really should incorporate     
 *         them into one.                                                 
 *\param   who Index of player (see constants in progress.h)     
 *\todo    Incorporate enemy_charmaction
 */
void auto_herochooseact (int who)
{
   int eact;

   if (cact[who] == 0)
      return;
   if (fighter[who].sts[S_DEAD] != 0 || fighter[who].hp <= 0)
     {
        cact[who] = 0;
        return;
     }
   fighter[who].facing = 0;
   eact = rand () % 4;
   if (eact == 0)
     {
        cact[who] = 0;
        return;
     }
   if (eact == 1)
     {
        fighter[who].ctmem = 0;
        hero_attack (who);
        cact[who] = 0;
        return;
     }
   fighter[who].ctmem = auto_select_hero (who, 0);
   hero_attack (who);
   cact[who] = 0;
}

/**\brief Select target for hero
 *
 *  Select a target for the hero to attack.                        
 *
 *\param  whom Index of player (see constants in progress.h)              
 *\return Index of target                                            
*/
static int hero_attack (int whom)
{
   int tgt;

   if (fighter[whom].sts[S_CHARM] == 0)
      tgt = select_enemy (whom, 0);
   else
     {
        if (fighter[whom].ctmem == 0)
           /* PH fixme: replaced 99 with NO_STS_CHECK */
           /* was 99 a bug? see auto_select_hero()  */
           tgt = auto_select_enemy (whom, NO_STS_CHECK);
        else
           tgt = auto_select_hero (whom, NO_STS_CHECK);
     }
   if (tgt == -1)
      return 0;
   fighter[whom].aframe = 6;
   curx = -1;
   cury = -1;
   battle_render (0, 0, 0);
   blit2screen (0, 0);
   wait (150);
   fight (whom, tgt, 0);
   return 1;
}

/**\brief Display item list
 *
 * This displays a list of the heroes items for use in
 * combat.
 *\param  pg Page of the item list  to display
*/
static void combat_draw_items (int pg)
{
   int a, b, c, k;

   menubox (double_buffer, 72, 8, 20, 16, BLUE);
   for (a = 0; a < 16; a++)
     {
        b = g_inv[pg * 16 + a][0];
        c = g_inv[pg * 16 + a][1];
        draw_icon (double_buffer, items[b].icon, 88, a * 8 + 16);
        if (combat_item_usable (b) == 1)
           k = FNORMAL;
        else
           k = FDARK;
        print_font (double_buffer, 96, a * 8 + 16, items[b].name, k);
        if (c > 1)
          {
             sprintf (strbuf, "^%d", c);
             print_font (double_buffer, 224, a * 8 + 16, strbuf, k);
          }
     }
   draw_sprite (double_buffer, pgb[pg], 238, 142);
}

/**\brief Choose combat item
 *
 * This is the menu used to display the hero's items in
 * combat and to allow him/her to select one.
 * The player then selects the target and the action is
 * performed.
 *\param  whom Index of character who is doing the choosing
 *\returns 0 if cancelled, 1 if item was chosen
*/
static int combat_item_menu (int whom)
{
   int z, rd = 1, stp = 0, ptr = 0, pptr = 0;

   blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
   while (!stp)
     {
        if (rd == 1)
          {
             blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
             combat_draw_items (pptr);
             draw_sprite (double_buffer, menuptr, 72, ptr * 8 + 16);
             blit2screen (0, 0);
          }
        rd = 0;
        readcontrols ();
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = 15;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr > 15)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (left)
          {
             unpress ();
             pptr--;
             if (pptr < 0)
                pptr = 3;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (right)
          {
             unpress ();
             pptr++;
             if (pptr > 3)
                pptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             if (items[g_inv[pptr * 16 + ptr][0]].tgt >= TGT_ENEMY_ONE)
                z = select_enemy (whom,
                                  items[g_inv[pptr * 16 + ptr][0]].tgt - 4);
             else
               {
                  if (g_inv[pptr * 16 + ptr][0] == I_LTONIC)
                     z = select_hero (whom,
                                      items[g_inv[pptr * 16 + ptr][0]].tgt -
                                      1, 1);
                  else
                     z = select_hero (whom,
                                      items[g_inv[pptr * 16 + ptr][0]].tgt -
                                      1, 0);
               }
             if (z > -1)
               {
                  if (combat_item (0, g_inv[pptr * 16 + ptr][0], z) == 1)
                    {
                       if (items[fighter[whom].csmem].use != USE_ANY_INF
                           && items[fighter[whom].csmem].use != USE_COMBAT_INF)
                          remove_item (pptr * 16 + ptr, 1);
                       stp = 2;
                    }
               }
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             stp = 1;
             rd = 1;
          }
     }
   return stp - 1;
}

/**\brief Can item be used in combat
 *
 *   This tells us whether or not the specified item
 *  is usable in combat.
 *\param  itno Index of item
 *\return non-zero if item itno can be used
*/
static int combat_item_usable (int itno)
{
   if (items[itno].type != 6 || items[itno].tgt == TGT_NONE)
      return 0;
   if (items[itno].use == USE_NOT || items[itno].use == USE_CAMP_ONCE
       || items[itno].use == USE_CAMP_INF)
      return 0;
   return 1;
}

/**\brief Use item
 *
 * Use the selected item and show the effects.
 *\param  ss Index of character attacking or 
 * PSIZE if an enemy is attacking
 *\param  t1 Item to use
 *\param  tg Index of target
 *\return Non-zero if anything happened
*/
static int combat_item (int ss, int t1, int tg)
{
   int a, b, r, st = tg, tt = 0, tl = 0;

   if (combat_item_usable (t1) == 0)
      return 0;
   strcpy (ctext, items[t1].name);
   dct = 1;
   r = item_effects (ss, tg, t1);
   dct = 0;
   if (r < 2)
      return r;
   if (items[t1].tgt == TGT_ENEMY_ALL)
     {
        tl = 1;
        if (ss == PSIZE)
          {
             st = 0;
             tt = numchrs;
          }
        else
          {
             st = PSIZE;
             tt = numens;
          }
     }
   display_amount (st, FDECIDE, tl);
   for (a = st; a < st + tt; a++)
      adjust_hp (a, ta[a]);
   b = 0;
   for (a = st; a < st + tt; a++)
     {
        if (fighter[a].sts[S_DEAD] == 0 && fighter[a].hp <= 0)
          {
             fkill (a);
             b++;
          }
        else
           ta[a] = 0;
     }
   if (b > 0)
      death_animation (st, tl);
   return 1;
}

/**\brief Draw equipment list
 * 
 * Draw the character's list of equipment.
 *\param dud id of party member to draw
*/
static void draw_invokable (int dud)
{
   int a, tt, grd;

   menubox (double_buffer, 72, 80, 20, 6, BLUE);
   for (a = 0; a < 6; a++)
     {
        grd = FNORMAL;
        tt = party[dud].eqp[a];
        if (can_invoke_item (tt) == 0)
           grd = FDARK;
        draw_icon (double_buffer, items[tt].icon, 88, a * 8 + 88);
        print_font (double_buffer, 96, a * 8 + 88, items[tt].name, grd);
     }
}

/**\brief Is item invokable
 *
 * Tells us whether or not a specified item is invokable.
 *\param t1 index of item in items array
 *\returns 1 if item can be invoked
*/
static int can_invoke_item (int t1)
{
   if (items[t1].type > 5)
      return 0;
   if (items[t1].use != USE_COMBAT_ONCE && items[t1].use != USE_COMBAT_INF)
      return 0;
   return 1;
}

/**\brief Display and choose item
 *
 * Displays the characters list of equipment and which
 * ones are invokable.  The player may then choose one
 * (if any) to invoke.
 *\param whom id of character
 *\returns 0 if cancelled, 1 if item was selected
*/
static int hero_invoke (int whom)
{
   int rd = 1, stp = 0, ptr = 0;
   int dud;

   blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
   dud = pidx[whom];
   while (!stp)
     {
        if (rd == 1)
          {
             blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
             draw_invokable (dud);
             draw_sprite (double_buffer, menuptr, 72, ptr * 8 + 88);
             blit2screen (0, 0);
          }
        rd = 0;
        readcontrols ();
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = 5;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr > 5)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             if (can_invoke_item (party[dud].eqp[ptr]) == 1)
               {
                  if (hero_invokeitem (whom, party[dud].eqp[ptr]) == 1)
                     stp = 2;
               }
             else
                play_effect (SND_BAD, 128);
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             stp = 1;
             rd = 1;
          }
     }
   return stp - 1;
}

/**\brief Invoke hero item.
 *
 * Invoke the specified item according to target.
 * Calls select_hero or select_enemy as required.
 *\param whom Hero identifier
 *\param eno item that is being invoked
*/
static int hero_invokeitem (int whom, int eno)
{
   int tg = 0, a, b;

   if (items[eno].tgt <= TGT_ALLY_ALL && items[eno].tgt != 0)
     {
        tg = select_hero (whom, items[eno].tgt - 1, 0);
        if (tg == -1)
           return 0;
     }
   if (items[eno].tgt >= TGT_ENEMY_ONE)
     {
        tg = select_enemy (whom, items[eno].tgt - 4);
        if (tg == -1)
           return 0;
     }
   if (items[eno].imb > 0)
     {
        cast_imbued_spell (whom, items[eno].imb, items[eno].stats[A_ATT], tg);
        return 1;
     }

/*
   This will likely become a separate function, but here is
   where we are invoking items.
*/
   if (eno == I_STAFF1)
     {
        strcpy (ctext, "Neutralize Poison");
        draw_spellsprite (0, 1, 27, 0);
        for (a = 0; a < numchrs; a++)
           if (fighter[a].sts[S_DEAD] == 0)
              fighter[a].sts[S_POISON] = 0;
     }
   if (eno == I_ROD1)
     {
        b = rand () % 3 + 1;
        strcpy (ctext, "Magic Missiles");
        dct = 1;
        for (a = 0; a < b; a++)
          {
             if (fighter[tg].sts[S_DEAD] == 0)
               {
                  draw_attacksprite (tg, 0, 4, 1);
                  special_damage_oneall_enemies (whom, 16, -1, tg, 0);
               }
          }
        dct = 0;
     }
   return 1;
}

/**\brief can heroes run?
 *
 * Check whether or not the heroes can run, and then display
 * the little running-away sequence.
 * \param whom unused (all members of party run anyway)
 * \todo RB unused variable whom
*/
static void hero_run (int whom)
{
   int a, b = 0, c = 0, bt = 0, ct = 0, p, fr, fx, fy, g = 0;
   // TT: slow_computer additions for speed-ups
   int count;

   whom = whom;
   if (ms == 1)
      a = 125;
   else
      a = 74;
   for (p = 0; p < numchrs; p++)
     {
        if (fighter[p].sts[S_DEAD] == 0)
          {
             b++;
             bt += fighter[p].stats[A_SPD];
          }
     }
   if (b == 0)
      program_death ("Fatal error: How can a dead party run?");
   else
      bt = bt / b;
   for (p = PSIZE; p < PSIZE + numens; p++)
     {
        if (fighter[p].sts[S_DEAD] == 0)
          {
             c++;
             ct += fighter[p].stats[A_SPD];
          }
     }
   if (c == 0)
      program_death
         ("Fatal error: Why are the heroes running from dead enemies?");
   else
      ct = ct / c;
   if (bt > ct)
      a += 25;
   if (bt < ct)
      a -= 25;
   if (rand () % 100 < a)
     {
        if (rand () % 100 < (100 - a))
          {
             g = b * fighter[PSIZE].lvl * c;
             if (gp < g)
                g = gp;
             gp -= g;
          }
     }
   else
     {
        menubox (double_buffer, 84, 32, 17, 1, BLUE);
        print_font (double_buffer, 92, 40, "Could not escape!", FNORMAL);
        blit2screen (0, 0);
        wait_enter ();
        return;
     }
   if (g > 0)
      sprintf (strbuf, "Escaped! Dropped %d gold!", g);
   else
      sprintf (strbuf, "Escaped!");
   g = strlen (strbuf) * 4;

   // TT: slow_computer addition for speed-ups
   if (slow_computer)
      count = 3;
   else
      count = 20;

   for (c = 0; c < 5; c++)
     {
        for (a = 0; a < count; a++)
          {
             clear_bitmap (double_buffer);
             menubox (double_buffer, 152 - g, 32, strlen (strbuf), 1, BLUE);
             print_font (double_buffer, 160 - g, 40, strbuf, FNORMAL);
             for (b = 0; b < numchrs; b++)
               {
                  fx = fighter[b].cx;
                  fy = fighter[b].cy;
                  fr = 0;
                  if (a > 10)
                     fr++;

                  if (fighter[b].sts[S_DEAD] == 0)
                     draw_sprite (double_buffer, frames[pidx[b]][fr], fx, fy);
               }
             blit2screen (0, 0);
             wait (10);
          }
     }
   revert_equipstats ();
   combatend = 2;
}

/**\brief Count available spells
 *
 * This checks a fighter's list of spells to see if there
 * are any for her/him to cast.
 *\param who Hero id
 *\returns the number of available spells
*/
int available_spells (int who)
{
   int a, b, e, l, numsp = 0;

   l = pidx[who];
   for (a = 0; a < 60; a++)
     {
        b = party[l].spells[a];
        if (b > 0)
          {
             if (magic[b].use == USE_ANY_INF || magic[b].use == USE_COMBAT_INF)
               {
                  if (l == CORIN && fighter[who].aux == 2)
                    {
                       e = mp_needed (who, b);
                       if (fighter[who].mp >= e && magic[b].elem < 9)
                          numsp++;
                    }
                  else
                    {
                       e = mp_needed (who, b);
                       if (fighter[who].mp >= e)
                          numsp++;
                    }
               }
          }
     }
   return numsp;
}

/**\brief draw spell list
 *
 * Draw the list of spells that the character can use
 * in combat.
 *\param c Character id
 *\param ptr The current line of the menu pointer
 *\param pg The current page in the spell list
*/
static void combat_draw_spell_menu (int c, int ptr, int pg)
{
   int z, j, b;

   menubox (double_buffer, 80, 24, 18, 12, BLUE);
   for (j = 0; j < 12; j++)
     {
        z = party[pidx[c]].spells[pg * 12 + j];
        if (z > 0)
          {
             draw_icon (double_buffer, magic[z].icon, 96, j * 8 + 32);
             if (combat_castable (c, pg * 12 + j) == 1)
                print_font (double_buffer, 104, j * 8 + 32, magic[z].name,
                            FNORMAL);
             else
                print_font (double_buffer, 104, j * 8 + 32, magic[z].name,
                            FDARK);
             b = mp_needed (c, z);
             sprintf (strbuf, "%d", b);
             print_font (double_buffer, 222 - (strlen (strbuf) * 8),
                         j * 8 + 32, strbuf, FNORMAL);
             draw_sprite (double_buffer, b_mp, 222, j * 8 + 32);
          }
     }
   draw_sprite (double_buffer, menuptr, 80, ptr * 8 + 32);
   draw_sprite (double_buffer, pgb[pg], 230, 126);
}

/**\brief Choose spell
 *
 * Draw the character's spell list and then choose a spell.
 *\param c Character id
 *\returns 0 if cancelled or 1 if something happened
*/
int combat_spell_menu (int c)
{
   int rd = 1, ptr = 0, pgno = 0, stop = 0;

   blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
   play_effect (SND_MENU, 128);
   while (!stop)
     {
        if (rd)
          {
             blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
             combat_draw_spell_menu (c, ptr, pgno);
             blit2screen (0, 0);
          }
        readcontrols ();
        rd = 0;
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr > 11)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = 11;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (right)
          {
             unpress ();
             pgno++;
             if (pgno > 4)
                pgno = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (left)
          {
             unpress ();
             pgno--;
             if (pgno < 0)
                pgno = 4;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             if (combat_castable (c, pgno * 12 + ptr) == 1)
               {
                  fighter[c].csmem = party[pidx[c]].spells[pgno * 12 + ptr];
                  stop = 2;
               }
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             stop = 1;
          }
     }
   if (stop == 2)
     {
        if ((fighter[c].csmem == M_LIFE || fighter[c].csmem == M_FULLLIFE)
            && numchrs == 1)
           return 0;
        if (pidx[c] == CORIN && fighter[c].aux == 2)
           return 1;
        if (combat_spell_targeting (c) == 1)
          {
             if (combat_spell (c, 0) > -1)
                return 1;
          }
     }
   return 0;
}

/**\brief Check spell targetting
 *
 * Perform the necessary checking to determine
 * target selection for the particular character's spell.
 *\param whom Character id
 *\returns -1 if the spell has no targetting, 
 *         0 if cancelled
 *         1 if target selected
*/
static int combat_spell_targeting (int whom)
{
   int a, tg;

   a = fighter[whom].csmem;
   if (magic[a].tgt == 0)
      return -1;
   if (magic[a].tgt <= TGT_ALLY_ALL)
     {
        if (a == M_LIFE || a == M_FULLLIFE)
           tg = select_hero (whom, magic[a].tgt - 1, NO_STS_CHECK);
        else
           tg = select_hero (whom, magic[a].tgt - 1, 0);
        if (tg == -1)
           return 0;
        else
           fighter[whom].ctmem = tg;
     }
   else
     {
        tg = select_enemy (whom, magic[a].tgt - 4);
        if (tg == -1)
           return 0;
        else
           fighter[whom].ctmem = tg;
     }
   return 1;
}

/**\brief Check spell is castable
 *
 * Perform the necessary checking to determine if a spell can be
 *  cast in combat and if the mp exists to do so.
 *\param who Character id
 *\param sno Spell id
 *\returns 0 if spell cannot be cast, 1 if it can
*/
static int combat_castable (int who, int sno)
{
   int b, c = 0;

   b = party[pidx[who]].spells[sno];
   if (b == M_WARP && can_run == 0)
      return 0;
   if (magic[b].use == USE_ANY_INF || magic[b].use == USE_COMBAT_INF)
     {
        if (pidx[who] == CORIN && fighter[c].aux == 2)
          {
             c = mp_needed (who, b);
             if (fighter[who].mp >= c && magic[b].elem < R_BLIND)
                return 1;
          }
        else
          {
             c = mp_needed (who, b);
             if (fighter[who].mp >= c)
                return 1;
          }
     }
   return 0;
}
