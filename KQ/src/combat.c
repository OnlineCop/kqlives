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
 * \brief Combat mode
 *
 * This is the main file where combat is initiated.
 * \author JB
 * \date ????????
 */

#include <stdio.h>
#include <string.h>

#include "kq.h"
#include "combat.h"

#include "draw.h"
#include "effects.h"
#include "enemyc.h"
#include "fade.h"
#include "heroc.h"
#include "itemmenu.h"
#include "magic.h"
#include "masmenu.h"
#include "menu.h"
#include "music.h"
#include "progress.h"
#include "res.h"
#include "setup.h"
#include "timing.h"
/*! \name global variables  */
/*\{*/
int combatend;
int cact[NUM_FIGHTERS];
int curx;
int cury;
int numens;
int ta[NUM_FIGHTERS];
int deffect[NUM_FIGHTERS];
int rcount;
unsigned char vspell;
unsigned char ms;
DATAFILE *backart;
/*\}*/


/*  internal variables  */
static int curw;
static int nspeed[NUM_FIGHTERS];
static int bspeed[NUM_FIGHTERS];
static unsigned char hs;



/*  internal prototypes  */
static void heroes_win (void);
static void enemies_win (void);
static int attack_result (int, int);
static int check_end (void);
static void do_action (int);
static void do_round (void);
static void roll_initiative (void);
static void snap_togrid (void);
static void init_fighters (void);



/*! \brief Does current location call for combat?                           
*
* This function checks the zone at the specified co-ordinates
* and calls combat based on the map and zone.
* \param comx x-coord of player
* \param comy y-coord of player
* \returns Outcome of combat() or 0 if no combat
*                                                                         
*/
int combat_check (int comx, int comy)
{
   int zn;
   int i;
   int w = -1;

   zn = z_seg[comy * g_map.xsize + comx];

   /*  RB TODO: adding a break will make this a bit faster, plus  */
   /*           calling combat with the FIRST zone, not the LAST  */
   /*           one.                                              */
   for (i = 0; i < NUM_BATTLES; i++)
      if (battles[i].mapnum == g_map.map_no && battles[i].zonenum == zn)
         w = i;

   return ((w > -1) ? combat (w) : 0);
}



/*! \brief Main combat function                                              
*
*   The big one... I say that because the game is mostly combat    
*            :p  First, check to see if a random encounter has occured.     
*            The check is skipped if it's a scripted battle. Then call all  
*            the helper and setup functions and start the combat by calling 
*            do_round.                                                      
* \param bno combat identifier (index into battles[])
* \returns 0 if no combat, 1 otherwise
*/
int combat (int bno)
{
   int zoom_step;

   /* ML 2002-09-22: not needed right now int saved_song; */
   int encounter;
   int lc;

   /*  RB: check if we had had a random encounter  */
   if (battles[bno].enc > 1)
      if ((steps < STEPS_NEEDED) || ((rand () % battles[bno].enc) > 0))
         return 0;

   /*  RB: had one! choose what we had just found  */
   steps = 0;
   encounter = select_encounter (battles[bno].etnum, battles[bno].eidx);
   if (fighter[0].lvl >= erows[encounter].lvl + 5 && battles[bno].eidx == 99)
     {
        lc = (fighter[0].lvl - erows[encounter].lvl) * 5;
        if ((rand () % 100) < lc)
           return 0;
     }

   if (progress[P_REPULSE] > 0)
     {
        lc = (fighter[0].lvl - erows[encounter].lvl) * 20;
        if (lc < 5)
           lc = 5;

        if ((rand () % 100) < lc)
           return 0;
     }

   in_combat = 1;
   init_fighters ();
   backart = load_datafile_object (PCX_DATAFILE, battles[bno].backimg);
   if (battles[bno].eidx == 99)
     {
        if ((numchrs == 1) && (pidx[0] == AYLA))
          {
             hs = rand () % 100 + 1;
             ms = rand () % 3 + 1;
          }
        else
          {
             if (numchrs > 1 && (in_party (AYLA) > 0))
               {
                  hs = rand () % 20 + 1;
                  ms = rand () % 5 + 1;
               }
             else
               {
                  hs = rand () % 10 + 1;
                  ms = rand () % 10 + 1;
               }
          }
     }
   else
     {
        hs = 10;
        ms = 10;
     }

   /*  RB: do the zoom at the beginning of the combat.  */
   pause_music ();
   set_music_volume ((gmvol / 250.0) * 0.75);
   play_music (battles[bno].bmusic, 0);
   if (stretch_view == 2)

     {
        do_transition (TRANS_FADE_OUT, 2);
        clear_bitmap (double_buffer);
        do_transition (TRANS_FADE_IN, 64);
     }

   else
      for (zoom_step = 0; zoom_step < 9; zoom_step++)

        {
           poll_music ();

           /*  RB FIXME: stretching when 640x480, stretching when 320x240?  */
           /*            shouldn't one of those be the "common" size, and   */
           /*            therefore not needing to stretch it?               */
           /*            320x240 is the double_buffer size...               */
           if (stretch_view == 1)
              stretch_blit (double_buffer, screen, zoom_step * 16 + xofs,
                            zoom_step * 12 + yofs, 320 - (zoom_step * 32),
                            240 - (zoom_step * 24), 0, 0, 640, 480);

           else
              stretch_blit (double_buffer, screen, zoom_step * 16 + xofs,
                            zoom_step * 12 + yofs, 320 - (zoom_step * 32),
                            240 - (zoom_step * 24), 0, 0, 320, 240);

           /*  RB FIXME: should we vsync here rather than rest?  */
           wait (100);
        }
   snap_togrid ();
   roll_initiative ();
   curx = 0;
   cury = 0;
   vspell = 0;
   combatend = 0;

   /*  RB: execute combat  */
   do_round ();
   unload_datafile_object (backart);
   set_music_volume (gmvol / 250.0);
   resume_music ();
   if (alldead)
      stop_music ();
   steps = 0;
   in_combat = 0;
   timer = 0;
   timer_count = 0;
   return (1);
}



/*! \brief Initiate fighter structs and initial vars
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : Pre-combat setup of fighter structures and initial vars.
 *  Returns : Nothing
*/
static void init_fighters (void)
{
   int index;

   for (index = 0; index < NUM_FIGHTERS; index++)
     {
        deffect[index] = 0;
        fighter[index].mhp = 0;
        fighter[index].aux = 0;
     }

   // TT: These two are only called once in the game; should we move them here?
   hero_init ();
   enemy_init ();
   for (index = 0; index < (PSIZE + numens); index++)
      nspeed[index] = (fighter[index].stats[A_SPD] + 50) / 5;
}



/*! \brief fighter on-screen locations in battle
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : Calculate where the fighters should be drawn.
 *  Returns : Nothing
*/
static void snap_togrid (void)
{
   int index;
   int hf = 0;
   int mf = 1;
   int a;

   if (hs == 1)
      hf = 1;
   if (ms == 1)
      mf = 0;

   for (index = 0; index < numchrs; index++)
      fighter[index].facing = hf;

   for (index = PSIZE; index < (PSIZE + numens); index++)
      fighter[index].facing = mf;

   hf = 170 - (numchrs * 24);
   for (index = 0; index < numchrs; index++)
     {
        fighter[index].cx = index * 48 + hf;
        fighter[index].cy = 128;
     }

   a = fighter[PSIZE].cw + 16;
   mf = 170 - (numens * a / 2);
   for (index = PSIZE; index < PSIZE + numens; index++)
     {
        fighter[index].cx = (index - PSIZE) * a + mf;

        if (fighter[index].cl < 104)
           fighter[index].cy = 104 - fighter[index].cl;
        else
           fighter[index].cy = 8;
     }
}



/*! \brief Choose who attacks first, speeds, etc.
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : Set up surprise vars, speeds, act vars, etc.
 *  Returns : Nothing
*/
static void roll_initiative (void)
{
   int i, j;

   if (hs == 1 && ms == 1)
     {
        hs = 10;
        ms = 10;
     }

   for (i = 0; i < NUM_FIGHTERS; i++)
     {
        fighter[i].csmem = 0;
        fighter[i].ctmem = 0;
        cact[i] = 1;
        j = ROUND_MAX * 66 / 100;
        if (j < 1)
           j = 1;

        bspeed[i] = rand () % j;
     }

   for (i = 0; i < numchrs; i++)
     {
        if (ms == 1)
           bspeed[i] = ROUND_MAX;
        else if (hs == 1)
           bspeed[i] = 0;
     }

   for (i = PSIZE; i < PSIZE + numens; i++)
     {
        if (hs == 1)
           bspeed[i] = ROUND_MAX;
        else if (ms == 1)
           bspeed[i] = 0;
     }

   rcount = 0;
   for (i = 0; i < NUM_FIGHTERS; i++)
     {
        /*  TODO: Unroll this loop  */
        for (j = 0; j < 2; j++)
           if (fighter[i].imb[j] > 0)
              cast_imbued_spell (i, fighter[i].imb[j], 1, TGT_CASTER);
     }

   battle_render (-1, -1, 0);
   blit2screen (0, 0);
   if ((hs == 1) && (ms > 1))
      message ("You have been ambushed!", 255, 1500, 0, 0);

   if ((hs > 1) && (ms == 1))
      message ("You've surprised the enemy!", 255, 1500, 0, 0);
}



/*! \brief draw the battle screen
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : 20020914 - 16:16 (RB)
 *  Purpose : Draw the battle screen.
 *  Returns : Nothing
 * \param plyr player
*/
void battle_render (int plyr, int hl, int sall)
{
   int a = 0;
   int z;
   int t;
   int b = 0;
   int sz;

   if (plyr > 0)
     {
        curx = fighter[plyr - 1].cx;
        cury = fighter[plyr - 1].cy;
        curw = fighter[plyr - 1].cw;
     }
   else
     {
        curx = -1;
        cury = -1;
     }

   clear_bitmap (double_buffer);
   blit ((BITMAP *) backart->dat, double_buffer, 0, 0, 0, 0, 320, 240);
   if (debugging > 2)
     {
        rectfill (double_buffer, 0, 0, rcount / 2, 9, 15);
        sprintf (strbuf, "%d", rcount);
        print_font (double_buffer, 0, 20, strbuf, FNORMAL);
        sprintf (strbuf, "0: %d - %d", fighter[0].sts[S_POISON],
                 fighter[0].sts[S_REGEN]);

        print_font (double_buffer, 0, 28, strbuf, FNORMAL);
        sprintf (strbuf, "1: %d - %d", fighter[1].sts[S_POISON],
                 fighter[1].sts[S_REGEN]);
        print_font (double_buffer, 0, 36, strbuf, FNORMAL);
     }

   if ((sall == 0) && (curx > -1) && (cury > -1))
     {
        draw_sprite (double_buffer, bptr, curx + (curw / 2) - 8, cury - 8);
        if (plyr - 1 >= PSIZE)
          {
             t = curx + (curw / 2);
             t -= (strlen (fighter[plyr - 1].name) * 4);
             z = fighter[plyr - 1].cy - 32;

             if (z < 0)
                z = fighter[plyr - 1].cy + fighter[plyr - 1].cl;

             menubox (double_buffer, t - 8, z,
                      strlen (fighter[plyr - 1].name), 1, BLUE);
             print_font (double_buffer, t, z + 8, fighter[plyr - 1].name,
                         FNORMAL);
          }

        if (debugging > 2)
          {
/*
         RB TODO: Check this out.

         sprintf(strbuf, "HP:%d (%d)", fighter[plyr-1].hp, fighter[plyr-1].mhp);
         print_font(double_buffer, 0,   8, strbuf, FNORMAL);
         sprintf(strbuf, "MP:%d (%d)", fighter[plyr-1].mp, fighter[plyr-1].mmp);
         print_font(double_buffer, 0,  16, strbuf, FNORMAL);
         print_font(double_buffer, 0,  24, "Str" , FNORMAL);
         print_font(double_buffer, 0,  32, "Agi" , FNORMAL);
         print_font(double_buffer, 0,  40, "Vit" , FNORMAL);
         print_font(double_buffer, 0,  48, "Int" , FNORMAL);
         print_font(double_buffer, 0,  56, "Sag" , FNORMAL);
         print_font(double_buffer, 0,  64, "Spd" , FNORMAL);
         print_font(double_buffer, 0,  72, "Aura", FNORMAL);
         print_font(double_buffer, 0,  80, "Spir", FNORMAL);
         print_font(double_buffer, 0,  88, "Att" , FNORMAL);
         print_font(double_buffer, 0,  96, "Hit%", FNORMAL);
         print_font(double_buffer, 0, 104, "Def" , FNORMAL);
         print_font(double_buffer, 0, 112, "Evd%", FNORMAL);
         print_font(double_buffer, 0, 120, "Mdef", FNORMAL);
         sprintf(strbuf, "%d, %d, %d, %d", fighter[plyr - 1].cwt,
                                           fighter[plyr - 1].welem,
                                           fighter[plyr - 1].unl,
                                           fighter[plyr - 1].crit);
         print_font(double_buffer, 0, 128, strbuf, FNORMAL);
         sprintf(strbuf, "i -> %d,%d,%d,%d", fighter[plyr - 1].imb_s,
                                             fighter[plyr - 1].imb_a,
                                             fighter[plyr - 1].imb[0],
                                             fighter[plyr - 1].imb[1]);
         print_font(double_buffer, 0, 136, strbuf, FNORMAL);
         for (t = 0; t < 13; t++) {
            sprintf(strbuf, "%d", fighter[plyr - 1].stats[t]);
            print_font(double_buffer, 40, t * 8 + 24, strbuf, FNORMAL);
         }
*/
          }
     }

   for (z = 0; z < numchrs; z++)
     {
        b = z * 216;

        if (fighter[z].sts[S_DEAD] == 0)
          {
             draw_fighter (z, (sall == 1));
#if 0
             if (sall == 1)
                draw_fighter (z, 1);
             else
                draw_fighter (z, 0);
#endif
          }
        else
          {
             fighter[z].aframe = 3;
             draw_fighter (z, 0);
          }

        menubox (double_buffer, b, 184, 11, 5, BLUE);
        if (fighter[z].sts[S_DEAD] == 0)
          {
             sz = bspeed[z] * 88 / ROUND_MAX;
             if (sz > 88)
                sz = 88;

             a = 116;
             if (fighter[z].sts[S_TIME] == 1)
                a = 83;

             if (fighter[z].sts[S_TIME] == 2)
                a = 36;

             a += (sz / 11);
             hline (double_buffer, b + 8, 229, b + sz + 8, a + 1);
             hline (double_buffer, b + 8, 230, b + sz + 8, a);
             hline (double_buffer, b + 8, 231, b + sz + 8, a - 1);
          }

        print_font (double_buffer, b + 8, 192, fighter[z].name,
                    (hl == z + 1) ? FGOLD : FNORMAL);
#if 0
        if (hl == z + 1)
           print_font (double_buffer, b + 8, 192, fighter[z].name, FGOLD);
        else
           print_font (double_buffer, b + 8, 192, fighter[z].name, FNORMAL);
#endif

        sprintf (strbuf, "HP: %3d/%3d", fighter[z].hp, fighter[z].mhp);
        /*  RB IDEA: If the character has less than 1/5 of his/her max    */
        /*           health points, it shows the amount with red (the     */
        /*           character is in danger). I suggest setting that '5'  */
        /*           as a '#define WARNING_LEVEL 5' or something like     */
        /*           that, so we can change it easily (maybe we can let   */
        /*           the player choose when it should be turned red).     */
        /*  TT TODO: I like this idea; maybe somewhere in the Options     */
        /*           menu?  I find that when the bar flashes red/yellow   */
        /*          to warn the player, it's much more eye-pleasing than  */
        /*          just a solid color (and not too hard to implement).   */

        print_font (double_buffer, b + 8, 208, strbuf,
                    (fighter[z].hp < (fighter[z].mhp / 5)) ? FRED : FNORMAL);
#if 0
        if (fighter[z].hp < (fighter[z].mhp / 5))
           print_font (double_buffer, b + 8, 208, strbuf, FRED);
        else
           print_font (double_buffer, b + 8, 208, strbuf, FNORMAL);
#endif

        hline (double_buffer, b + 8, 216, b + 95, 21);
        sz = (fighter[z].hp > 0) ? fighter[z].hp * 88 / fighter[z].mhp : 88;
#if 0
        if (fighter[z].hp > 0)
           sz = fighter[z].hp * 88 / fighter[z].mhp;
        else
           sz = 88;
#endif

        hline (double_buffer, b + 8, 216, b + 8 + sz, 12);
        sprintf (strbuf, "MP: %3d/%3d", fighter[z].mp, fighter[z].mmp);

        /*  RB IDEA: Same suggestion as with health, just above.  */
        print_font (double_buffer, b + 8, 218, strbuf,
                    (fighter[z].mp < (fighter[z].mmp / 5)) ? FRED : FNORMAL);
#if 0
        if (fighter[z].mp < (fighter[z].mmp / 5))
           print_font (double_buffer, b + 8, 218, strbuf, FRED);
        else
           print_font (double_buffer, b + 8, 218, strbuf, FNORMAL);
#endif
        hline (double_buffer, b + 8, 226, b + 95, 21);
        sz = (fighter[z].mp > 0) ? fighter[z].mp * 88 / fighter[z].mmp : 88;
#if 0
        if (fighter[z].mp > 0)
           sz = fighter[z].mp * 88 / fighter[z].mmp;
        else
           sz = 88;
#endif
        hline (double_buffer, b + 8, 226, b + 8 + sz, 12);
        draw_stsicon (double_buffer, 1, z, 17, b + 8, 200);
     }

   for (t = PSIZE; t < PSIZE + numens; t++)
     {
        if (fighter[t].sts[S_DEAD] == 0)
          {
             draw_fighter (t, (sall == 2));
#if 0
             if (sall == 2)
                draw_fighter (t, 1);
             else
                draw_fighter (t, 0);
#endif
          }
     }

   if (dct == 1)
     {
        menubox (double_buffer, 152 - (strlen (ctext) * 4), 8, strlen (ctext),
                 1, BLUE);
        print_font (double_buffer, 160 - (strlen (ctext) * 4), 16, ctext,
                    FNORMAL);
     }
}



/*
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : 20020914 - 16:16 (RB)
 *  Purpose : This function controls the battle guages and calls for action
 *            when necessary. This is also where things like poison, sleep,
 *            and what-not are checked.
 *  Returns : Nothing
*/
static void do_round (void)
{
   int a;
   int index;
   int st;

   st = 0;
   timer_count = 0;
   while (combatend == 0)
     {
        if (timer_count >= 10)
          {
             rcount += BATTLE_INC;

             if (rcount >= ROUND_MAX)
                rcount = 0;

             for (index = 0; index < PSIZE + numens; index++)
               {
                  if ((index < numchrs) || (index >= PSIZE))
                    {
                       if (((fighter[index].sts[S_POISON] - 1) == rcount) &&
                           (fighter[index].hp > 1))
                         {
                            a = rand () % ((fighter[index].mhp / 20) + 1);

                            if (a < 2)
                               a = 2;

                            if ((fighter[index].hp - a) < 1)
                               a = fighter[index].hp - 1;

                            ta[index] = a;
                            display_amount (index, FNORMAL, 0);
                            fighter[index].hp -= a;
                         }

                       /*  RB: the character is regenerating? when needed, get a  */
                       /*      random value (never lower than 5), and increase    */
                       /*      the character's health by that amount.             */
                       if ((fighter[index].sts[S_REGEN] - 1) == rcount)
                         {
                            a = rand () % 5 + (fighter[index].mhp / 10);

                            if (a < 5)
                               a = 5;

                            ta[index] = a;
                            display_amount (index, FYELLOW, 0);
                            adjust_hp (index, a);
                         }

                       /*  RB: the character has ether actived?  */
                       cact[index] = 1;
                       if ((fighter[index].sts[S_ETHER] > 0) && (rcount == 0))
                          fighter[index].sts[S_ETHER]--;
#if 0
                       if (fighter[index].sts[S_ETHER] > 0)
                         {
                            if (rcount == 0)
                               fighter[index].sts[S_ETHER]--;
                         }
#endif

                       /*  RB: the character is stopped?  */
                       if (fighter[index].sts[S_STOP] > 0)
                         {
                            if (pidx[index] == TEMMIN)
                               fighter[index].aux = 0;

                            if (rcount == 0)
                               fighter[index].sts[S_STOP]--;

                            cact[index] = 0;
                         }

                       /*  RB: the character is sleeping?  */
                       if (fighter[index].sts[S_SLEEP] > 0)
                         {
                            if (pidx[index] == TEMMIN)
                               fighter[index].aux = 0;

                            if (rcount == 0)
                               fighter[index].sts[S_SLEEP]--;

                            cact[index] = 0;
                         }

                       /*  RB: the character is petrified?  */
                       if (fighter[index].sts[S_STONE] > 0)
                         {
                            if (pidx[index] == TEMMIN)
                               fighter[index].aux = 0;

                            if (rcount == 0)
                               fighter[index].sts[S_STONE]--;

                            cact[index] = 0;
                         }

                       if ((fighter[index].sts[S_DEAD] != 0) ||
                           (fighter[index].mhp <= 0))
                         {
                            if (pidx[index] == TEMMIN)
                               fighter[index].aux = 0;

                            bspeed[index] = 0;
                            cact[index] = 0;
                         }

                       if (cact[index] > 0)
                         {
                            if (fighter[index].sts[S_TIME] == 0)
                               bspeed[index] += nspeed[index];
                            else
                              {
                                 if (fighter[index].sts[S_TIME] == 1)
                                    bspeed[index] += (nspeed[index] / 2 + 1);
                                 else
                                    bspeed[index] += (nspeed[index] * 2);
                              }
                         }
                    }
                  else
                     cact[index] = 0;
               }

             readcontrols ();
             battle_render (0, 0, 0);
             blit2screen (0, 0);

             for (index = 0; index < (PSIZE + numens); index++)
               {
                  if ((bspeed[index] >= ROUND_MAX) && (cact[index] > 0))
                    {
                       do_action (index);
                       fighter[index].ctmem = 0;
                       fighter[index].csmem = 0;
                       cact[index] = 1;
                       bspeed[index] = 0;
                    }

                  if (combatend)
                     return;
               }

             timer_count = 0;
          }
     }
}



/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated : Never.                                                         */
/*  Purpose : Choose a fighter action.                                       */
/*  Returns : Nothing                                                        */
/*                                                                           */
static void do_action (int dude)
{
   int index;

   for (index = 0; index < 2; index++)
      if (fighter[dude].imb[index] > 0)
         cast_imbued_spell (dude, fighter[dude].imb[index], 1, TGT_CASTER);

   if (fighter[dude].sts[S_MALISON] > 0)
     {
        if ((rand () % 100) < fighter[dude].sts[S_MALISON] * 5)
           cact[dude] = 0;
     }

   if (fighter[dude].sts[S_CHARM] > 0)
     {
        fighter[dude].sts[S_CHARM]--;

        if (dude < PSIZE)
           auto_herochooseact (dude);
        else
           enemy_charmaction (dude);
     }

   if (cact[dude] != 0)
     {
        revert_cframes (dude, 0);
        if (dude < PSIZE)
          {
             if (fighter[dude].sts[S_CHARM] == 0)
                hero_choose_action (dude);
          }
        else
           enemy_chooseaction (dude);
     }

   cact[dude] = 0;
   if (check_end () == 1)
      combatend = 1;
}



/*
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : 20020914 - 16:37 (RB)
 *  Purpose : Display a single fighter on the screen. Checks for dead and
 *            stone, and if the fighter is selected. Also displays 'Vision'
 *            spell information.
 *  Returns : Nothing
*/
void draw_fighter (int dude, int dcur)
{
   int xx;
   int yy;
   int ff;

   xx = fighter[dude].cx;
   yy = fighter[dude].cy;

   ff = (!fighter[dude].aframe) ? fighter[dude].facing : fighter[dude].aframe;
#if 0
   if (fighter[dude].aframe == 0)
      ff = fighter[dude].facing;
   else
      ff = fighter[dude].aframe;
#endif

   if (fighter[dude].sts[S_STONE] > 0)
      convert_cframes (dude, 2, 12, 0);

   if (fighter[dude].sts[S_ETHER] > 0)
      draw_trans_sprite (double_buffer, cframes[dude][ff], xx, yy);
   else
      draw_sprite (double_buffer, cframes[dude][ff], xx, yy);

   if (dcur == 1)
      draw_sprite (double_buffer, bptr, xx + (fighter[dude].cw / 2) - 8,
                   yy - 8);

   if ((vspell == 1) && (dude >= PSIZE))
     {
        ff = fighter[dude].hp * 30 / fighter[dude].mhp;
        if ((fighter[dude].hp > 0) && (ff < 1))
           ff = 1;

        xx += fighter[dude].cw / 2;
        rect (double_buffer, xx - 16, yy + fighter[dude].cl + 2,
              xx + 15, yy + fighter[dude].cl + 5, 0);
        if (ff > 20)
           rectfill (double_buffer, xx - 15, yy + fighter[dude].cl + 3,
                     xx - 15 + ff - 1, yy + fighter[dude].cl + 4, 40);

        if ((ff <= 20) && (ff > 10))
           rectfill (double_buffer, xx - 15, yy + fighter[dude].cl + 3,
                     xx - 15 + ff - 1, yy + fighter[dude].cl + 4, 104);

        if ((ff <= 10) && (ff > 0))
           rectfill (double_buffer, xx - 15, yy + fighter[dude].cl + 3,
                     xx - 15 + ff - 1, yy + fighter[dude].cl + 4, 24);
     }
}



/*
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : Just check to see if all the enemies or heroes are dead.
 *  Returns : 1 if the battle ended (either the heroes or the enemies won);
 *            0 otherwise.
*/
static int check_end (void)
{
   int index;
   int alive = 0;

   /*  RB: count the number of heroes alive. If there is none, the   */
   /*      enemies won the battle.                                   */
   for (index = 0; index < numchrs; index++)
      if (fighter[index].sts[S_DEAD] == 0)
         alive++;

   if (alive == 0)
     {
        enemies_win ();
        return 1;
     }

   /*  RB: count the number of enemies alive. If there is none, the  */
   /*      heroes won the battle.                                    */
   alive = 0;
   for (index = 0; index < numens; index++)
      if (fighter[index + PSIZE].sts[S_DEAD] == 0)
         alive++;

   if (alive == 0)
     {
        heroes_win ();
        return 1;
     }

   return 0;
}



/*
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : I don't really need to describe this :p
 *  Returns : 1 or 0 (TODO: check what each means)
*/
int fight (int ar, int dr, int sk)
{
   int a;
   int tx = -1;
   int ty = -1;
   int f;
   int ares;

   for (a = 0; a < NUM_FIGHTERS; a++)
     {
        deffect[a] = 0;
        ta[a] = 0;
     }

   /*  check the 'sk' variable to see if we are over-riding the  */
   /*  attackers stats with temporary ones... used for skills    */
   /*  and such                                                  */
   if (sk == 0)
      tempa = status_adjust (ar);

   tempd = status_adjust (dr);
   ares = attack_result (ar, dr);
   for (a = 0; a < 24; a++)
      fighter[dr].sts[a] = tempd.sts[a];

   /*  RB TODO: rest(20) or vsync() before the blit?  */
   if (ares == 2)
     {
        for (f = 0; f < 3; f++)
          {
             battle_render (dr + 1, 0, 0);
             blit2screen (0, 0);
             wait (20);
             rectfill (double_buffer, 0, 0, 320, 240, 15);
             blit2screen (0, 0);
             wait (20);
          }
     }

   if ((pidx[dr] == TEMMIN) && (fighter[dr].aux == 2))
     {
        fighter[dr].aux = 1;
        a = 1 - dr;
        tx = fighter[dr].cx;
        ty = fighter[dr].cy;
        fighter[dr].cx = fighter[a].cx;
        fighter[dr].cy = fighter[a].cy - 16;
     }

   if (ar < PSIZE)
      fighter[ar].aframe = 7;
   else
      fighter[ar].cy += 10;

   fight_animation (dr, ar, 0);
   if (ar < PSIZE)
      fighter[ar].aframe = 0;
   else
      fighter[ar].cy -= 10;

   if ((tx != -1) && (ty != -1))
     {
        fighter[dr].cx = tx;
        fighter[dr].cy = ty;
     }

   if (ta[dr] != MISS)
      ta[dr] = do_shield_check (dr, ta[dr]);

   display_amount (dr, FDECIDE, 0);
   if (ta[dr] != MISS)
     {
        fighter[dr].hp += ta[dr];
        if ((fighter[ar].imb_s > 0) && ((rand () % 5) == 0))
           cast_imbued_spell (ar, fighter[ar].imb_s, fighter[ar].imb_a, dr);

        if ((fighter[dr].hp <= 0) && (fighter[dr].sts[S_DEAD] == 0))
          {
             fkill (dr);
             death_animation (dr, 0);
          }

        if (fighter[dr].hp > fighter[dr].mhp)
           fighter[dr].hp = fighter[dr].mhp;

        if (fighter[dr].sts[S_SLEEP] > 0)
           fighter[dr].sts[S_SLEEP] = 0;

        if ((fighter[dr].sts[S_CHARM] > 0) && (ar == dr))
           fighter[dr].sts[S_CHARM] = 0;
#if 0
        if (fighter[dr].sts[S_CHARM] > 0)
           if (ar == dr)
              fighter[dr].sts[S_CHARM] = 0;
#endif
        return 1;
     }

   return 0;
}



/*! \brief Attack all enemies at once
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : This is different than fight in that all enemies are attacked
 *            simultaneously, once. As a note, the attackers stats are
 *            always over-ridden in this function. As well, critical hits
 *            are possible, but the screen doesn't flash.
 *  Returns : 1 or 0 (TODO: check what each means)
 * \param ar attacker
*/
void multi_fight (int ar)
{
   int index;
   int b;
   int st;
   int nd;
   int deadcount;
   int kw[NUM_FIGHTERS];
   int ares[NUM_FIGHTERS];

   st = nd = deadcount = 0;
   for (index = 0; index < NUM_FIGHTERS; index++)
     {
        deffect[index] = 0;
        ta[index] = 0;
        kw[index] = 0;
     }

   // if the attacker is you, target enemies
   if (ar < PSIZE)
     {
        st = PSIZE;
        nd = numens;
     }

   // if the attacker is enemy, target your party
   else
     {
        st = 0;
        nd = numchrs;
     }

   for (index = st; index < st + nd; index++)
     {
        tempd = status_adjust (index);
        if ((fighter[index].sts[S_DEAD] == 0) && (fighter[index].mhp > 0))
          {
             ares[index] = attack_result (ar, index);
             for (b = 0; b < 24; b++)
                fighter[index].sts[b] = tempd.sts[b];
          }

        if (ta[index] != MISS)
          {
             if (ta[index] != MISS)
                ta[index] = do_shield_check (index, ta[index]);

             fighter[index].hp += ta[index];
             if ((fighter[index].hp <= 0) && (fighter[index].sts[S_DEAD] == 0))
               {
                  fighter[index].hp = 0;
                  kw[index] = 1;
               }

             /*  RB: check we always have less health points than the maximun  */
             if (fighter[index].hp > fighter[index].mhp)
                fighter[index].hp = fighter[index].mhp;

             /*  RB: if sleeping, a good hit wakes him/her up  */
             if (fighter[index].sts[S_SLEEP] > 0)
                fighter[index].sts[S_SLEEP] = 0;

             /*  RB: if charmed, a good hit wakes him/her up  */
             if ((fighter[index].sts[S_CHARM] > 0) && (ta[index] > 0) &&
                 (ar == index))
                fighter[index].sts[S_CHARM] = 0;
#if 0
             if ((fighter[index].sts[S_CHARM] > 0) && (ta[index] > 0))
                if (ar == index)
                   fighter[index].sts[S_CHARM] = 0;
#endif
          }
     }

   if (ar < PSIZE)
      fighter[ar].aframe = 7;
   else
      fighter[ar].cy += 10;

   fight_animation (st, ar, 1);
   if (ar < PSIZE)
      fighter[ar].aframe = 0;
   else
      fighter[ar].cy -= 10;

   display_amount (st, FDECIDE, 1);
   for (index = st; index < st + nd; index++)
     {
        if (kw[index] == 1)
          {
             fkill (index);
             deadcount++;
          }
     }

   if (deadcount > 0)
      death_animation (st, 1);
}



/*! \brief Attack all enemies at once
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : This does the actual attack calculation. The damage done to
 *            the target is kept in the ta[] array.
 *  Returns : 0 (attack was a miss), 1 (attack was successful) or 2 (attack
 *            was a critical hit).
 * \param ar attacker
 * \param dr defender
*/
static int attack_result (int ar, int dr)
{
   int c;
   int cfch;
   int ar_cs = 0;
   int crit_hit = 0;
   int base;
   int to_hit;
   int mult;
   int dmg;                     /* extra */
   int ar_att;
   int ar_hit;
   int ar_welem;
   int dr_def;
   int dr_evd;

   ar_att = tempa.stats[A_ATT];
   ar_hit = tempa.stats[A_HIT];
   ar_welem = tempa.welem;
   dr_def = tempd.stats[A_DEF];
   dr_evd = tempd.stats[A_EVD];

   /*  JB: check to see if the attacker is in critical status...  */
   /*      increases chance for a critical hit                    */
   if (tempa.mhp > 250)
     {
        if (tempa.hp <= 50)
           ar_cs = 1;
     }
   else
     {
        if (tempa.hp <= (tempa.mhp / 5))
           ar_cs = 1;
     }

   /*  JB: check to see if the defender is 'defending'  */
   if (tempd.defend == 1)
      dr_def = (dr_def * 15) / 10;

   /*  JB: if the attacker is empowered by trueshot  */
   if (tempa.sts[S_TRUESHOT] > 0)
     {
        fighter[ar].sts[S_TRUESHOT] = 0;
        dr_evd = 0;
     }

   ar_att += (tempa.stats[tempa.bstat] * tempa.bonus / 100);
   if (ar_att < DMG_RND_MIN * 5)
      base = (rand () % DMG_RND_MIN) + ar_att;
   else
      base = (rand () % (ar_att / 5)) + ar_att;

   base -= dr_def;
   if (base < 1)
      base = 1;

   mult = 0;
   to_hit = ar_hit + dr_evd;
   if (to_hit < 1)
      to_hit = 1;

   if (rand () % to_hit < ar_hit)
      mult++;

   /*  JB: If the defender is etherealized, set mult to 0  */
   if (tempd.sts[S_ETHER] > 0)
      mult = 0;

   if (mult > 0)
     {
        if (tempd.crit == 1)
          {
             cfch = 1;
             if (ar_cs == 1)
                cfch = 2;
             /* PH I _think_ this makes Sensar 2* as likely to make a critical hit */
             if (pidx[ar] == SENSAR)
                cfch = cfch * 2;

             cfch = (20 - cfch);
             if (rand () % 20 >= cfch)
               {
                  crit_hit = 1;
                  base *= 1.5;
#if 0
                  base = base * 15 / 10;
#endif
               }
          }

        /*  JB: if affected by a NAUSEA/MALISON spell, the defender  */
        /*      takes more damage than normal                        */
        if (tempd.sts[S_MALISON] > 0)
           base *= 5 / 4;

        /*  JB: check for elemental/status weapons  */
        if (base < 1)
           base = 1;

        c = ar_welem - 1;
        if ((c >= R_EARTH) && (c <= R_ICE))
           base = res_adjust (dr, c, base);

        if ((c >= R_POISON) && (c <= R_SLEEP))
          {
             if ((res_throw (dr, c) == 0) && (fighter[dr].sts[c - 8] == 0))
               {
                  if (non_dmg_save (dr, 50) == 0)
                    {
                       if ((c == R_POISON) || (c == R_PETRIFY)
                           || (c == R_SILENCE))
                          tempd.sts[c - 8] = 1;
                       else
                          tempd.sts[c - 8] = rand () % 3 + 2;
                    }
               }
          }
     }

   /*  JB: Apply the damage multiplier  */
   /*  RB FIXME: check if the changes I made here didn't break something  */
   dmg = mult * base;
   if (dmg == 0)
      dmg = MISS;
   else
     {
        ta[dr] = 0 - dmg;
        return ((crit_hit == 1) ? 2 : 1);
     }

   ta[dr] = dmg;
   return 0;
}



/*! \brief enemies defeated the player
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : Play some sad music and set the dead flag so that the game
 *            will return to the main menu.
 *  Returns : Nothing.
*/
static void enemies_win (void)
{
   play_music ("rain.s3m", 0);
   battle_render (0, 0, 0);
   /*  RB FIXME: rest()?  */
   blit2screen (0, 0);
   wait (1000);
   sprintf (strbuf, "%s was defeated!", party[pidx[0]].name);
   menubox (double_buffer, 152 - (strlen (strbuf) * 4), 48, strlen (strbuf),
            1, BLUE);
   print_font (double_buffer, 160 - (strlen (strbuf) * 4), 56, strbuf, FNORMAL);
   blit2screen (0, 0);
   wait_enter ();
   do_transition (TRANS_FADE_OUT, 4);
   alldead = 1;
}



/*! \brief player defeated the enemies
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : Never.
 *  Purpose : Distribute the booty!
 *  Returns : Nothing.
*/
static void heroes_win (void)
{
   int tgp = 0;
   int index;
   int b;
   int c;
   int z;
   int nc = 0;
   int txp = 0;
   int fitm = 0;
   int nr = 0;
   int ent = 0;
   s_fighter t1;
   s_fighter t2;

   play_music ("rend5.s3m", 0);
   wait (500);
   revert_equipstats ();
   for (index = 0; index < numchrs; index++)
      fighter[index].aframe = 4;

   battle_render (0, 0, 0);
   blit2screen (0, 0);
   wait (250);
   for (index = 0; index < numchrs; index++)
     {
        if ((fighter[index].sts[S_STONE] == 0)
            && (fighter[index].sts[S_DEAD] == 0))
           nc++;

        ta[index] = 0;
     }

   for (index = PSIZE; index < PSIZE + numens; index++)
     {
        txp += fighter[index].xp;
        tgp += fighter[index].gp;
     }

   /*  JB: nc should never be zero if we won, but whatever  */
   if (nc > 0)
      txp /= nc;

   gp += tgp;
   if (tgp > 0)
      sprintf (strbuf, "Gained %d xp and found %d gp.", txp, tgp);
   else
      sprintf (strbuf, "Gained %d xp.", txp);

   menubox (double_buffer, 152 - (strlen (strbuf) * 4), 8, strlen (strbuf), 1,
            BLUE);
   print_font (double_buffer, 160 - (strlen (strbuf) * 4), 16, strbuf, FNORMAL);
   blit2screen (0, 0);
   blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
   for (index = 0; index < numens; index++)
     {
        if ((rand () % 100) < fighter[index + PSIZE].dip)
          {
             if (fighter[index + PSIZE].ditmc > 0)
                fitm = fighter[index + PSIZE].ditmc;

             if (fighter[index + PSIZE].ditmr > 0)
               {
                  if ((rand () % 100) < 5)
                     fitm = fighter[index + PSIZE].ditmr;
#if 0
                  b = rand () % 100;
                  if (b < 5)
                     fitm = fighter[index + PSIZE].ditmr;
#endif
               }

             if (fitm > 0)
               {
                  if (check_inventory (fitm, 1) != 0)
                    {
                       sprintf (strbuf, "%s found!", items[fitm].name);
                       menubox (double_buffer, 148 - (strlen (strbuf) * 4),
                                nr * 24 + 48, strlen (strbuf) + 1, 1, BLUE);
                       draw_icon (double_buffer, items[fitm].icon,
                                  156 - (strlen (strbuf) * 4), nr * 24 + 56);
                       print_font (double_buffer, 164 - (strlen (strbuf) * 4),
                                   nr * 24 + 56, strbuf, FNORMAL);
                       nr++;
                    }
               }
          }
     }

   if (nr > 0)
     {
        blit2screen (0, 0);
        wait_enter ();
        blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
     }

   nr = 0;
   for (c = 0; c < numchrs; c++)
     {
        if ((party[pidx[c]].sts[S_STONE] == 0)
            && (party[pidx[c]].sts[S_DEAD] == 0))
          {
             b = c * 160;
             t1 = player2fighter (pidx[c]);
             if (give_xp (pidx[c], txp, 0) == 1)
               {
                  menubox (double_buffer, b, 40, 18, 9, BLUE);
                  t2 = player2fighter (pidx[c]);
                  print_font (double_buffer, b + 8, 48, "Level up!", FGOLD);
                  print_font (double_buffer, b + 8, 56, "Max HP", FNORMAL);
                  print_font (double_buffer, b + 8, 64, "Max MP", FNORMAL);
                  print_font (double_buffer, b + 8, 72, "Strength", FNORMAL);
                  print_font (double_buffer, b + 8, 80, "Agility", FNORMAL);
                  print_font (double_buffer, b + 8, 88, "Vitality", FNORMAL);
                  print_font (double_buffer, b + 8, 96, "Intellect", FNORMAL);
                  print_font (double_buffer, b + 8, 104, "Sagacity", FNORMAL);
                  sprintf (strbuf, "%3d>", t1.mhp);
                  print_font (double_buffer, b + 96, 56, strbuf, FNORMAL);
                  sprintf (strbuf, "%3d", t2.mhp);
                  print_font (double_buffer, b + 128, 56, strbuf, FGREEN);
                  sprintf (strbuf, "%3d>", t1.mmp);
                  print_font (double_buffer, b + 96, 64, strbuf, FNORMAL);
                  sprintf (strbuf, "%3d", t2.mmp);
                  print_font (double_buffer, b + 128, 64, strbuf, FGREEN);

                  for (z = 0; z < 5; z++)
                    {
                       sprintf (strbuf, "%3d>", t1.stats[z]);
                       print_font (double_buffer, b + 96, z * 8 + 72, strbuf,
                                   FNORMAL);
                       sprintf (strbuf, "%3d", t2.stats[z]);
                       if (t2.stats[z] > t1.stats[z])
                          print_font (double_buffer, b + 128, z * 8 + 72,
                                      strbuf, FGREEN);
                       else
                          print_font (double_buffer, b + 128, z * 8 + 72,
                                      strbuf, FNORMAL);
                    }

                  nr++;
               }
             else
                menubox (double_buffer, b, 104, 18, 1, BLUE);

             sprintf (strbuf, "Next level %7d", party[pidx[c]].next -
                      party[pidx[c]].xp);
             print_font (double_buffer, b + 8, 112, strbuf, FGOLD);
          }
     }

   blit2screen (0, 0);
   for (c = 0; c < numchrs; c++)
      if ((party[pidx[c]].sts[S_STONE] == 0)
          && (party[pidx[c]].sts[S_DEAD] == 0))
         ent += learn_new_spells (pidx[c]);

   if (ent == 0)
      wait_enter ();
}



/*! \brief kill a fighter
 *
 *  Author  : Josh Bolduc
 *  Created : ???????? - ??:??
 *  Updated : PH 17-sep-2002 added cheat mode
 *  Purpose : Do what it takes to put a fighter out of commission.
 *  Returns : Nothing.
 * \param victim the one who will die
*/
void fkill (int victim)
{
   int index;
   /* PH Combat cheat - when a hero dies s/he is mysteriously boosted back   */
   /* to full HP */
#ifdef KQ_CHEATS
   if (cheat && victim < 2)
     {
        fighter[victim].hp = fighter[victim].mhp;
        return;
     }
#endif
   for (index = 0; index < 24; index++)
      fighter[victim].sts[index] = 0;

   fighter[victim].sts[S_DEAD] = 1;
   fighter[victim].hp = 0;
   if (victim < PSIZE)
      fighter[victim].ditmc = 0;

   deffect[victim] = 1;
   cact[victim] = 0;
}
