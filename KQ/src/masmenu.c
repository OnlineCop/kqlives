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

#include "kq.h"
#include "masmenu.h"
#include "draw.h"
#include "setup.h"
#include "menu.h"
#include "skills.h"
#include "magic.h"
#include "res.h"
#include "progress.h"
#include "combat.h"
#include "selector.h"



/*  global variables  */
int close_menu;



/*  internal functions  */
static int need_spell (int, int, int);
static void camp_draw_spell_menu (int, int, int);
static void camp_spell_targeting (int, int);
static int camp_castable (int, int);

/*
   Does the target even need the spell that's been selected?
*/
static int need_spell (int ca, int ta, int sn)
{
   int a, b, vc = 0, cs;

   cs = pidx[ca];
   if (ta < numchrs)
      vc = pidx[ta];
   switch (sn)
     {
     case M_RESTORE:
        if (party[vc].sts[S_POISON] == 0 && party[vc].sts[S_BLIND] == 0)
           return 0;
        break;
     case M_RECOVERY:
        a = 0;
        for (b = 0; b < 7; b++)
           if (party[vc].sts[b] != 0)
              a++;
        if (a == 0 || party[vc].sts[S_DEAD] != 0)
           return 0;
        break;
     case M_LIFE:
     case M_FULLLIFE:
        if (party[vc].sts[S_DEAD] == 0)
           return 0;
        break;
     case M_CURE1:
     case M_CURE2:
     case M_CURE3:
     case M_CURE4:
        if (ta == SEL_ALL_ALLIES)
          {
             b = 0;
             for (a = 0; a < numchrs; a++)
                if (party[pidx[a]].hp == party[pidx[a]].mhp
                    || party[pidx[a]].sts[S_STONE] != 0
                    || party[pidx[a]].sts[S_DEAD] != 0)
                   b++;
             if (b == numchrs)
                return 0;
          }
        else
          {
             if (party[vc].hp == party[vc].mhp)
                return 0;
          }
        break;
     case M_WARP:
/* RB FIXME What was this supposed to do?
      if (g_map.can_warp == 0);
*/
        return 0;
        break;
     case M_REPULSE:
        return 1;
     default:
        return 0;
     }
   return 1;
}

/*
   This function is called by level_up() and checks to see
   if the character has learned any new spells (that s/he did
   not already know).
*/
int learn_new_spells (int who)
{
   int a, p, i, nog, g = 0;

   blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
   for (a = 1; a < NUM_SPELLS; a++)
     {
        nog = 1;
        for (i = 0; i < 60; i++)
           if (party[who].spells[i] == a)
              nog = 0;
        if (magic[a].clvl[who] == 0 || magic[a].clvl[who] > party[who].lvl)
           nog = 0;
        if (nog == 1)
          {
             p = 60;
             for (i = 59; i >= 0; i--)
                if (party[who].spells[i] == 0)
                   p = i;
             if (p < 60)
               {
                  if (in_combat == 1)
                    {
                       sprintf (strbuf, "%s learned!", magic[a].name);
                       blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
                       menubox (double_buffer, 148 - (strlen (strbuf) * 4),
                                152, strlen (strbuf) + 1, 1, BLUE);
                       draw_icon (double_buffer, magic[a].icon,
                                  156 - (strlen (strbuf) * 4), 160);
                       print_font (double_buffer, 164 - (strlen (strbuf) * 4),
                                   160, strbuf, FNORMAL);
                       blit2screen (0, 0);
                       wait_enter ();
                       g++;
                    }
                  party[who].spells[p] = a;
               }
          }
     }
   blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
   return g;
}

/*
   Draw the current character's grimoire contents.
*/
static void camp_draw_spell_menu (int c, int pg, int ptr)
{
   int l, z, j, b, a;

   l = pidx[c];
   b = party[l].spells[pg * 12 + ptr];
   menubox (double_buffer, 80 + xofs, 12 + yofs, 18, 1, BLUE);
   print_font (double_buffer, 140 + xofs, 20 + yofs, "Magic", FGOLD);
   menubox (double_buffer, 80 + xofs, 36 + yofs, 18, 5, BLUE);
   draw_playerstat (double_buffer, l, 88 + xofs, 44 + yofs);
   menubox (double_buffer, 80 + xofs, 92 + yofs, 18, 12, BLUE);
   for (j = 0; j < 12; j++)
     {
        z = party[l].spells[pg * 12 + j];
        a = FDARK;
        if (camp_castable (c, z) == 1)
           a = FNORMAL;
        if (z > 0)
          {
             draw_icon (double_buffer, magic[z].icon, 96 + xofs,
                        j * 8 + 100 + yofs);
             print_font (double_buffer, 104 + xofs, j * 8 + 100 + yofs,
                         magic[z].name, a);
             sprintf (strbuf, "%d", mp_needed (c, z));
             print_font (double_buffer, 232 - (strlen (strbuf) * 8) + xofs,
                         j * 8 + 100 + yofs, strbuf, a);
          }
     }
   menubox (double_buffer, 40 + xofs, 204 + yofs, 28, 1, BLUE);
   print_font (double_buffer, (160 - (strlen (magic[b].desc) * 4)) + xofs,
               212 + yofs, magic[b].desc, FNORMAL);
   draw_sprite (double_buffer, pgb[pg], 230 + xofs, 194 + yofs);
}

/*
   Display the magic spell menu and allow the player to
   arrange or cast spells.
*/
void camp_spell_menu (int c)
{
   int stop = 0, pg[2] = { 0, 0 }, ptr[2] =
   {
   0, 0}
   , a, b = 0;
   int rd = 1, smove = 0, tsn;

   if (party[pidx[c]].sts[S_MUTE] > 0)
     {
        play_effect (SND_BAD, 128);
        return;
     }
   update_equipstats ();
   play_effect (SND_MENU, 128);
   while (stop == 0)
     {
        if (rd == 1)
          {
             drawmap ();
             camp_draw_spell_menu (c, pg[smove], ptr[smove]);
             if (smove == 0)
                draw_sprite (double_buffer, mptr, 88 + xofs,
                             ptr[0] * 8 + 100 + yofs);
             else
               {
                  if (pg[0] == pg[1])
                     draw_sprite (double_buffer, mptr, 88 + xofs,
                                  ptr[0] * 8 + 100 + yofs);
                  draw_sprite (double_buffer, sptr, 88 + xofs,
                               ptr[1] * 8 + 100 + yofs);
               }
             blit2screen (xofs, yofs);
          }
        readcontrols ();
        rd = 0;
        if (down)
          {
             unpress ();
             ptr[smove]++;
             if (ptr[smove] > 11)
                ptr[smove] = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (up)
          {
             unpress ();
             ptr[smove]--;
             if (ptr[smove] < 0)
                ptr[smove] = 11;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (right)
          {
             unpress ();
             pg[smove]++;
             if (pg[smove] > 4)
                pg[smove] = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (left)
          {
             unpress ();
             pg[smove]--;
             if (pg[smove] < 0)
                pg[smove] = 4;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             if (smove == 1)
               {
                  smove = 0;
                  if (pg[0] != pg[1] || ptr[0] != ptr[1])
                    {
                       a = pg[0] * 12 + ptr[0];
                       b = pg[1] * 12 + ptr[1];
                       tsn = party[pidx[c]].spells[a];
                       party[pidx[c]].spells[a] = party[pidx[c]].spells[b];
                       party[pidx[c]].spells[b] = tsn;
                    }
                  if (pg[0] == pg[1] && ptr[0] == ptr[1])
                    {
                       a = pg[0] * 12 + ptr[0];
                       tsn = party[pidx[c]].spells[a];
                       if (tsn > 0)
                         {
                            if (camp_castable (c, tsn) == 1)
                               camp_spell_targeting (c, tsn);
                         }
                       smove = 0;
                    }
                  pg[0] = pg[1];
                  ptr[0] = ptr[1];
               }
             else
               {
                  if (party[pidx[c]].spells[pg[0] * 12 + ptr[0]] > 0)
                    {
                       smove = 1;
                       pg[1] = pg[0];
                       ptr[1] = ptr[0];
                    }
               }
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             if (smove == 1)
                smove = 0;
             else
                stop = 1;
             rd = 1;
          }
        if (close_menu == 1)
           stop = 1;
     }
}

/*
   Select any necessary targets and prepare the spell.
*/
static void camp_spell_targeting (int mc, int sn)
{
   int tg = 0, a;

   if (magic[sn].tgt == TGT_NONE || magic[sn].tgt > TGT_ALLY_ALL)
      return;
   while (tg > -1)
     {
        if (party[pidx[mc]].mp < mp_needed (mc, sn))
           return;
        if (magic[sn].use != USE_ANY_INF && magic[sn].use != USE_CAMP_INF)
           return;
        if (sn != M_WARP && sn != M_REPULSE)
          {
             tg =
                select_any_player (magic[sn].tgt - 1, magic[sn].icon,
                                   magic[sn].name);
             if (tg < 0)
                return;
             if (need_spell (mc, tg, sn) == 0)
               {
                  play_effect (SND_BAD, 128);
                  return;
               }
             fighter[mc].ctmem = tg;
          }
        else
           tg = SEL_ALL_ALLIES;
        fighter[mc].csmem = sn;
        for (a = 0; a < numchrs; a++)
           ta[a] = 0;
        if (cast_spell (mc, 0) == 1)
          {
             if (sn == M_WARP || sn == M_REPULSE)
               {
                  close_menu = 1;
                  return;
               }
             else
                play_effect (SND_TWINKLE, 128);
          }
        else
           play_effect (SND_TWINKLE, 128);      /* this should be a failure sound */
        for (a = 0; a < numchrs; a++)
           adjust_hp (a, ta[a]);
        revert_equipstats ();
     }
}

/*
   Perform the necessary checking to determine if a spell can be
   cast while camping and if the mp exists to do so.
*/
static int camp_castable (int who, int sno)
{
   if (sno == M_VISION || (sno == M_WARP && g_map.can_warp == 0))
      return 0;
   if (magic[sno].use == USE_ANY_INF || magic[sno].use == USE_CAMP_INF)
     {
        if (party[pidx[who]].mp >= mp_needed (who, sno))
           return 1;
     }
   return 0;
}
