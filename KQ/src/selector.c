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
/**\file
 * \brief Various hero and enemy selectors
 *
 *\author Josh Bolduc
 *\date ????????
 */
#include <stdio.h>
#include <string.h>

#include "kq.h"
#include "selector.h"
#include "combat.h"
#include "draw.h"
#include "menu.h"
#include "setup.h"



/*  internal functions  */
static int can_attack (int);



/*  internal variables  */
static int tmpd[NUM_FIGHTERS];

/**\brief Select player from main menu
 *
 * This is used to select a player from the main menu.
 * Used in menu.c
 *
 * \returns  Index of player (0..numchrs-1) or -1 if cancelled
*/
int select_player (void)
{
   int stop = 0, ptr, rd = 1;

   if (numchrs == 1)
      return 0;
   ptr = 0;
   while (!stop)
     {
        if (rd == 1)
          {
             draw_mainmenu (ptr);
             blit2screen (xofs, yofs);
          }
        rd = 0;
        readcontrols ();
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = numchrs - 1;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr > numchrs - 1)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             stop = 1;
          }
        if (bctrl)
          {
             unpress ();
             return -1;
          }
     }
   return ptr;
}

/**\brief Select player or players
 *
 *        This is used to select a recipient or recipients for
 *            items/spells.
 *            Used in itemmenu.c and masmenu.c
 * \param   csa mode (target one, one/all or all)
 * \param   icn icon to draw (see also draw_icon() in draw.c)
 * \param   msg prompt message
 * \returns  Index of player (0..numchrs-1) or -1 if cancelled
 *           or SEL_ALL_ALLIES if 'all' was selected (by pressing L or R)
 * \todo PH Should use TGT_* constants (in kq.h) to compare to csa
 * for an example, see camp_item_targetting() in itemmenu.c
 * \todo PH  This seems awfully complicated for what it does. Is there
 * any visual clue as to whether you can select all or not?
 * Should there be?
 * \sa draw_icon()
 * \sa camp_item_targetting()
*/
int select_any_player (int csa, int icn, char *msg)
{
   int stop = 0, ptr, rd = 1, shy, k, sa;

   shy = 120 - (numchrs * 28);
   if (csa == 2)
      sa = 1;
   else
      sa = 0;
   ptr = 0;
   while (!stop)
     {
        if (rd == 1)
          {
             drawmap ();
             if (csa < 3)
               {
                  menubox (double_buffer,
                           152 - ((strlen (msg) + 1) * 4) + xofs, 8 + yofs,
                           strlen (msg) + 1, 1, BLUE);
                  draw_icon (double_buffer, icn,
                             160 - ((strlen (msg) + 1) * 4) + xofs, 16 + yofs);
                  print_font (double_buffer,
                              168 - ((strlen (msg) + 1) * 4) + xofs,
                              16 + yofs, msg, FNORMAL);
               }
             for (k = 0; k < numchrs; k++)
               {
                  menubox (double_buffer, 80 + xofs, k * 56 + shy + yofs, 18,
                           5, BLUE);
                  draw_playerstat (double_buffer, pidx[k], 88 + xofs,
                                   k * 56 + shy + 8 + yofs);
                  if (csa < 3)
                    {
                       if (sa == 0)
                         {
                            if (k == ptr)
                               draw_sprite (double_buffer, menuptr, 72 + xofs,
                                            k * 56 + shy + 24 + yofs);
                         }
                       else
                          draw_sprite (double_buffer, menuptr, 72 + xofs,
                                       k * 56 + shy + 24 + yofs);
                    }
               }
             blit2screen (xofs, yofs);
          }
        rd = 0;
        readcontrols ();
        if (csa < 3)
          {
             if (left)
               {
                  unpress ();
                  if (csa == 1)
                    {
                       if (sa == 0)
                          sa = 1;
                       else
                          sa = 0;
                    }
                  rd = 1;
               }
             if (right)
               {
                  unpress ();
                  if (csa == 1)
                    {
                       if (sa == 0)
                          sa = 1;
                       else
                          sa = 0;
                    }
                  rd = 1;
               }
             if (up)
               {
                  unpress ();
                  if (ptr > 0)
                     ptr--;
                  play_effect (SND_CLICK, 128);
                  rd = 1;
               }
             if (down)
               {
                  unpress ();
                  if (ptr < numchrs - 1)
                     ptr++;
                  play_effect (SND_CLICK, 128);
                  rd = 1;
               }
          }
        if (balt)
          {
             unpress ();
             stop = 1;
          }
        if (bctrl)
          {
             unpress ();
             stop = 2;
          }
     }
   if (csa == 3 || stop == 2)
      return -1;
   if (sa == 1)
      return SEL_ALL_ALLIES;
   else
      return ptr;
}

/**\brief select a hero or heroes
 *
 *   This is used for selecting an allied target.
 *          The multi parameter works the same here as it does
 *           for select_target above.
 *           Used in heroc.c
 * \param   whom =person that is doing the action ??
 * \param    multi =mode (target one, one/all or all)
 * \param    csd =allows you to select a dead character
 * \returns  Index of player (0..numchrs-1) or -1 if cancelled
 *           or SEL_ALL_ALLIES if 'all' was selected (by pressing U or D)

 * \todo PH  Should use TGT_* constants (in kq.h) to compare to multi
 *          csd should use NO_STS_CHECK like auto_select_hero ??
 * \todo PH  tmpd should be a local var?
*/
int select_hero (int whom, int multi, int csd)
{
   int cntr = 0, ptr = 0, stp = 0, rd = 1, sa, a;

   if (multi == 2)
      sa = 1;
   else
      sa = 0;
   for (a = 0; a < numchrs; a++)
     {
        if (fighter[a].sts[S_DEAD] == 0)
          {
             tmpd[a] = a;
             cntr++;
          }
        else
          {
             if (csd == 1)
               {
                  tmpd[a] = a;
                  cntr++;
                  ptr = a;
               }
          }
     }
   while (!stp)
     {
        if (rd == 1)
          {
             if (multi > 0 && sa == 1)
                battle_render (tmpd[ptr] + 1, whom + 1, 1);
             else
                battle_render (tmpd[ptr] + 1, whom + 1, 0);
             blit2screen (0, 0);
          }
        readcontrols ();
        rd = 0;
        if (balt)
          {
             unpress ();
             stp = 1;
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             return -1;
          }
        if (left)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = cntr - 1;
             rd = 1;
          }
        if (right)
          {
             unpress ();
             ptr++;
             if (ptr >= cntr)
                ptr = 0;
             rd = 1;
          }
        if (multi == 1 && cntr > 1)
          {
             if (up)
               {
                  unpress ();
                  if (sa == 0)
                     sa = 1;
                  else
                     sa = 0;
                  rd = 1;
               }
             if (down)
               {
                  unpress ();
                  if (sa == 0)
                     sa = 1;
                  else
                     sa = 0;
                  rd = 1;
               }
          }
     }
   if (sa == 0)
      return tmpd[ptr];
   else
      return SEL_ALL_ALLIES;
}

/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated :                                                                */
/*  Purpose : Choose a target.  This is used for all combat enemy            */
/*          : target selection, whether selected one or all enemies          */
/*          : this is the function to use.                                   */
/*          : The multi parameter specifies what we can select:              */
/*          : 0 indicates that we can select one target only.                */
/*          : 1 indicates that we can select one target or all.              */
/*          : 2 indicates that we can only select all enemies                */
/*          : Used in  heroc.c hskill.c                                      */
/*  Params  : whom=person that is doing the action ??                        */
/*          : multi=mode (target one, one/all or all)                        */
/*  Returns : Index of enemy (PSIZE..PSIZE+numens-1) or -1 if cancelled      */
/*          : or SEL_ALL_ENEMIES if 'all' was selected (by pressing U or D)  */

/* PH FIXME Should use TGT_* constants (in kq.h) to compare to multi */
/* PH TODO tmpd should be a local var? */
/*
*/
int select_enemy (int whom, int multi)
{
   int a, cntr = 0, ptr, stp, rd, sa;

   for (a = PSIZE; a < PSIZE + numens; a++)
      if (can_attack (a) == 1)
         tmpd[cntr++] = a;
   if (multi == 2)
      sa = 1;
   else
      sa = 0;
   ptr = 0;
   stp = 0;
   rd = 1;
   while (!stp)
     {
        if (rd == 1)
          {
             if (multi > 0 && sa == 1)
                battle_render (tmpd[ptr] + 1, whom + 1, 2);
             else
                battle_render (tmpd[ptr] + 1, whom + 1, 0);
          }
        blit2screen (0, 0);
        readcontrols ();
        rd = 0;
        if (balt)
          {
             unpress ();
             rd = 1;
             stp = 1;
          }
        if (bctrl)
          {
             unpress ();
             return -1;
          }
        if (left)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = cntr - 1;
             rd = 1;
          }
        if (right)
          {
             unpress ();
             ptr++;
             if (ptr > cntr - 1)
                ptr = 0;
             rd = 1;
          }
        if (up)
          {
             unpress ();
             if (multi == 1 && cntr > 1)
               {
                  if (sa == 0)
                     sa = 1;
                  else
                     sa = 0;
                  rd = 1;
               }
          }
        if (down)
          {
             unpress ();
             if (multi == 1 && cntr > 1)
               {
                  if (sa == 0)
                     sa = 1;
                  else
                     sa = 0;
                  rd = 1;
               }
          }
     }
   if (sa == 0)
      return tmpd[ptr];
   else
      return SEL_ALL_ENEMIES;
}

/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated :                                                                */
/*  Purpose : Automatically select a hero.                                   */
/*          : Used in enemyc.c and heroc.c                                   */
/*  Params  : whom=person that is doing the action ?? (unused)               */
/*          : csts=Only select characters whose .sts[csts]==0                */
/*          : or select any if csts==NO_STS_CHECK                            */
/*  Returns : Index of hero  (0..numchrs-1)                                  */
int auto_select_hero (int whom, int csts)
{
   int a, cntr = 0;
/*  RB TODO  */
   whom = whom;
   if (numchrs == 1)
     {
        if (can_attack (0))
           return 0;
        else
           return -1;
     }
   for (a = 0; a < numchrs; a++)
     {
        if (can_attack (a))
          {
             if (csts == NO_STS_CHECK)
               {
                  tmpd[cntr] = a;
                  cntr++;
               }
             else
               {
                  if (fighter[a].sts[csts] == 0)
                    {
                       tmpd[cntr] = a;
                       cntr++;
                    }
               }
          }
     }
   if (cntr <= 1)
      return tmpd[0];
   return tmpd[rand () % cntr];
}

/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated :                                                                */
/*  Purpose : Automatically select an enemy                                  */
/*          : Used in enemyc.c and heroc.c                                   */
/*  Params  : whom=particular enemy, if csts!=NO_STS_CHECK then it is 75%    */
/*          : likely to return 'whom'                                        */
/*          : csts=Only select characters whose .sts[csts]==0                */
/*          : or special case if csts==S_BLESS then  .sts[S_BLESS]<3         */
/*          : or special case if csts==S_STRENGTH then  .sts[S_STRENGTH]<2   */
/*          : or select only where HP<75% of MHP     if csts==CURE_CHECK     */
/*          : or select any if csts==NO_STS_CHECK                            */
/*          : Never selects a dead enemy                                     */
/*  Returns : Index of enemy (PSIZE..PSIZE+numens-1) or -1 if no enemy found */
int auto_select_enemy (int whom, int csts)
{
   int a, ne = 0;

   for (a = PSIZE; a < PSIZE + numens; a++)
     {
        if (fighter[a].sts[S_DEAD] == 0)
          {
             if (csts == NO_STS_CHECK)
               {
                  tmpd[ne] = a;
                  ne++;
               }
             else
               {
                  if (csts == CURE_CHECK)
                    {
                       if (fighter[a].hp < fighter[a].mhp * 75 / 100)
                         {
                            tmpd[ne] = a;
                            ne++;
                         }
                    }
                  else
                    {
                       if ((csts == S_BLESS && fighter[a].sts[csts] < 3)
                           || (csts == S_STRENGTH && fighter[a].sts[csts] < 2)
                           || (csts != S_BLESS && csts != S_STRENGTH
                               && fighter[a].sts[csts] == 0))
                         {
                            tmpd[ne] = a;
                            ne++;
                         }
                    }
               }
          }
     }
   if (ne == 0)
      return -1;
   if (csts != NO_STS_CHECK)
     {
        for (a = 0; a < ne; a++)
           if (tmpd[a] == whom && rand () % 4 != 3)
              return whom;
     }
   if (ne <= 1)
      return tmpd[0];
   return tmpd[rand () % ne];
}

/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated :                                                                */
/*  Purpose : Just checks the status of a fighter for purposes of targetting.*/
/*  Params  : tgt=index into fighter array                                   */
/*  Returns : 1 unless the fighter is dead or has HP<1 or MHP<1              */

static int can_attack (int tgt)
{
   if (fighter[tgt].mhp < 1 || fighter[tgt].hp < 1
       || fighter[tgt].sts[S_DEAD] != 0)
      return 0;
   return 1;
}
