/*
   KQ is Copyright (C) 2002 by Josh Bolduc

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
 * \brief Save and Load game
 * \author JB
 * \date ????????
 *
 * Support for saving and loading games.
 * Also includes the main menu and the system menu
 * (usually accessible by pressing ESC).
 *
 * \todo PH Do we _really_ want things like controls and screen
 *          mode to be saved/loaded ?
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <allegro.h>
/*#include <allegro/internal/aintern.h>*/

#include "setup.h"
#include "fade.h"
#include "kq.h"
#include "draw.h"
#include "sgame.h"
/*#include "kqsnd.h"*/
#include "combat.h"
#include "res.h"
#include "masmenu.h"
#include "magic.h"
#include "shopmenu.h"
#include "timing.h"
#include "progress.h"
#include "menu.h"
#include "music.h"
#include "disk.h"
/*! \name Globals */

int snc[NUMSG], sgp[NUMSG], shr[NUMSG], smin[NUMSG], sid[NUMSG][PSIZE],
   slv[NUMSG][PSIZE];
unsigned char shp[NUMSG][PSIZE], smp[NUMSG][PSIZE];
int save_ptr = 0;


/*! \name Internal functions  */
static void show_sgstats (int);
static int save_game (void);
static int load_game (void);
static int saveload (int);
static int confirm_save (void);



/*! \brief Load mini stats
 *
 * This loads the mini stats for each saved game.
 * These mini stats are just for displaying info about the save game on the
 * save/load game screen.
 */
void load_sgstats (void)
{
   PACKFILE *ldat;
   int a, b, c;
   unsigned char vc;
   s_player tpm;

   for (a = 0; a < NUMSG; a++) {
      sprintf (strbuf, "sg%d.sav", a);
      ldat = pack_fopen (kqres (SAVE_DIR, strbuf), F_READ_PACKED);
      if (!ldat) {
         snc[a] = 0;
         sgp[a] = 0;
         shr[a] = 0;
         smin[a] = 0;
         for (b = 0; b < PSIZE; b++) {
            sid[a][b] = 0;
            shp[a][b] = 0;
            smp[a][b] = 0;
         }
      } else {
         /*             pack_fread (&vc, sizeof (vc), ldat); */
         vc = pack_getc (ldat);
         if (vc != kq_version)
            snc[a] = -1;
         else {
            /*
               pack_fread (&snc[a], sizeof (snc[a]), ldat);
               pack_fread (&sgp[a], sizeof (sgp[a]), ldat);
               pack_fread (&shr[a], sizeof (shr[a]), ldat);
               pack_fread (&smin[a], sizeof (smin[a]), ldat);
             */
            snc[a] = pack_igetl (ldat);
            sgp[a] = pack_igetl (ldat);
            shr[a] = pack_igetl (ldat);
            smin[a] = pack_igetl (ldat);
            for (b = 0; b < PSIZE; b++) {
               /*                     pack_fread (&sid[a][b], sizeof (sid[a][b]), ldat); */
               sid[a][b] = pack_igetl (ldat);
            }
            for (b = 0; b < MAXCHRS; b++) {
               /*                       pack_fread (&tpm, sizeof (tpm), ldat); */
               load_s_player (&tpm, ldat);
               for (c = 0; c < PSIZE; c++) {
                  if (b == sid[a][c]) {
                     slv[a][c] = tpm.lvl;
                     shp[a][c] = tpm.hp * 100 / tpm.mhp;
                     if (tpm.mmp > 0)
                        smp[a][c] = tpm.mp * 100 / tpm.mmp;
                     else
                        smp[a][c] = 0;
                  }
               }
            }
         }
         pack_fclose (ldat);
      }
   }
}



/*! \brief Display saved game statistics
 *
 * This is the routine that displays the information about
 * each saved game for the save/load screen.
 *
 * \param saving 0 if loading, 1 if saving.
 */
static void show_sgstats (int saving)
{
   int a, sg, hx, hy, b;

   if (saving) {
      menubox (double_buffer, 8, save_ptr * 48 + 12, 6, 1, BLUE);
      print_font (double_buffer, 16, save_ptr * 48 + 20, "Saving", FGOLD);
   } else {
      menubox (double_buffer, 0, save_ptr * 48 + 12, 7, 1, BLUE);
      print_font (double_buffer, 8, save_ptr * 48 + 20, "Loading", FGOLD);
   }
   for (sg = 0; sg < NUMSG; sg++) {
      if (sg == save_ptr)
         menubox (double_buffer, 72, sg * 48, 29, 4, DARKBLUE);
      else
         menubox (double_buffer, 72, sg * 48, 29, 4, BLUE);
      if (snc[sg] == -1)
         print_font (double_buffer, 136, sg * 48 + 20, "Wrong version",
                     FNORMAL);
      else {
         if (snc[sg] == 0)
            print_font (double_buffer, 168, sg * 48 + 20, "Empty", FNORMAL);
         else {
            for (a = 0; a < snc[sg]; a++) {
               hx = a * 72 + 84;
               hy = sg * 48 + 12;
               draw_sprite (double_buffer, frames[sid[sg][a]][1], hx, hy + 4);
               sprintf (strbuf, "L: %02d", slv[sg][a]);
               print_font (double_buffer, hx + 16, hy, strbuf, FNORMAL);
               print_font (double_buffer, hx + 16, hy + 8, "H:", FNORMAL);
               print_font (double_buffer, hx + 16, hy + 16, "M:", FNORMAL);
               rectfill (double_buffer, hx + 33, hy + 9, hx + 65, hy + 15, 2);
               rectfill (double_buffer, hx + 32, hy + 8, hx + 64, hy + 14, 35);
               rectfill (double_buffer, hx + 33, hy + 17, hx + 65, hy + 23, 2);
               rectfill (double_buffer, hx + 32, hy + 16, hx + 64, hy + 22,
                         19);
               b = shp[sg][a] * 32 / 100;
               rectfill (double_buffer, hx + 32, hy + 9, hx + 32 + b, hy + 13,
                         41);
               b = smp[sg][a] * 32 / 100;
               rectfill (double_buffer, hx + 32, hy + 17, hx + 32 + b, hy + 21,
                         25);
            }
            sprintf (strbuf, "T %d:%02d", shr[sg], smin[sg]);
            print_font (double_buffer, 236, sg * 48 + 12, strbuf, FNORMAL);
            sprintf (strbuf, "G %d", sgp[sg]);
            print_font (double_buffer, 236, sg * 48 + 28, strbuf, FNORMAL);
         }
      }
   }
}



/*! \brief Save game
 *
 * You guessed it... save the game.
 *
 * \returns 0 if save failed, 1 if success
 */
static int save_game (void)
{
   PACKFILE *sdat;
   int a, b;

   for (b = 0; b < PSIZE; b++) {
      sid[save_ptr][b] = 0;
      shp[save_ptr][b] = 0;
      smp[save_ptr][b] = 0;
      slv[save_ptr][b] = 0;
   }
   for (b = 0; b < numchrs; b++) {
      sid[save_ptr][b] = pidx[b];
      shp[save_ptr][b] = party[pidx[b]].hp * 100 / party[pidx[b]].mhp;
      if (party[pidx[b]].mmp > 0)
         smp[save_ptr][b] = party[pidx[b]].mp * 100 / party[pidx[b]].mmp;
      slv[save_ptr][b] = party[pidx[b]].lvl;
   }
   snc[save_ptr] = numchrs;
   sgp[save_ptr] = gp;
   smin[save_ptr] = kmin;
   shr[save_ptr] = khr;
   sprintf (strbuf, "sg%d.sav", save_ptr);
   sdat = pack_fopen (kqres (SAVE_DIR, strbuf), F_WRITE_PACKED);
   if (!sdat) {
      message ("Could not save game data.", 255, 0, 0, 0);
      return 0;
   }
   /*   pack_fwrite (&kq_version, sizeof (kq_version), sdat);
      pack_fwrite (&numchrs, sizeof (numchrs), sdat);
      pack_fwrite (&gp, sizeof (gp), sdat);
      pack_fwrite (&shr[save_ptr], sizeof (shr[save_ptr]), sdat);
      pack_fwrite (&smin[save_ptr], sizeof (smin[save_ptr]), sdat);
    */
   pack_putc (kq_version, sdat);
   pack_iputl (numchrs, sdat);
   pack_iputl (gp, sdat);
   pack_iputl (shr[save_ptr], sdat);
   pack_iputl (smin[save_ptr], sdat);
   for (a = 0; a < PSIZE; a++) {
      /*      pack_fwrite (&pidx[a], sizeof (pidx[a]), sdat); */
      pack_iputl (pidx[a], sdat);
   }
   for (a = 0; a < MAXCHRS; a++) {
      /*
         pack_fwrite (&party[a], sizeof (s_player), sdat);
       */
      save_s_player (&party[a], sdat);
   }
   pack_fwrite (curmap, 16, sdat);
/*    pack_fwrite (progress, 2000, sdat); */
   for (a = 0; a < 2000; ++a) {
      pack_putc (progress[a], sdat);
   }
/*    pack_fwrite (treasure, 1000, sdat); */
   for (a = 0; a < 1000; ++a) {
      pack_putc (treasure[a], sdat);
   }
/*    pack_fwrite (shopq, NUMSHOPS * SHOPITEMS * sizeof (shopq[0][0]), sdat); */
   for (a = 0; a < NUMSHOPS; ++a) {
      for (b = 0; b < SHOPITEMS; ++b) {
         pack_iputw (shopq[a][b], sdat);
      }
   }
/* pack_fwrite (g_inv, MAX_INV * 2 * sizeof (g_inv[0][0]), sdat); */
   for (a = 0; a < MAX_INV; ++a) {
      pack_iputw (g_inv[a][0], sdat);
      pack_iputw (g_inv[a][1], sdat);
   }
   /* PH FIXME: do we _really_ want things like controls and screen */
   /* mode to be saved/loaded ? */
/*    pack_fwrite (&gsvol, sizeof (gsvol), sdat); */
/*    pack_fwrite (&gmvol, sizeof (gmvol), sdat); */
/*    pack_fwrite (&windowed, sizeof (windowed), sdat); */
/*    pack_fwrite (&stretch_view, sizeof (stretch_view), sdat); */
/*    pack_fwrite (&wait_retrace, sizeof (wait_retrace), sdat); */
   pack_iputl (gsvol, sdat);
   pack_iputl (gmvol, sdat);
   pack_putc (windowed, sdat);
   pack_putc (stretch_view, sdat);
   pack_putc (wait_retrace, sdat);
/*    pack_fwrite (&kup, sizeof (kup), sdat); */
/*    pack_fwrite (&kdown, sizeof (kdown), sdat); */
/*    pack_fwrite (&kleft, sizeof (kleft), sdat); */
/*    pack_fwrite (&kright, sizeof (kright), sdat); */
/*    pack_fwrite (&kalt, sizeof (kalt), sdat); */
/*    pack_fwrite (&kctrl, sizeof (kctrl), sdat); */
/*    pack_fwrite (&kenter, sizeof (kenter), sdat); */
/*    pack_fwrite (&kesc, sizeof (kesc), sdat); */
/*    pack_fwrite (&jbalt, sizeof (jbalt), sdat); */
/*    pack_fwrite (&jbctrl, sizeof (jbctrl), sdat); */
/*    pack_fwrite (&jbenter, sizeof (jbenter), sdat); */
/*    pack_fwrite (&jbesc, sizeof (jbesc), sdat); */
   pack_iputl (kup, sdat);
   pack_iputl (kdown, sdat);
   pack_iputl (kleft, sdat);
   pack_iputl (kright, sdat);
   pack_iputl (kalt, sdat);
   pack_iputl (kctrl, sdat);
   pack_iputl (kenter, sdat);
   pack_iputl (kesc, sdat);
   pack_iputl (jbalt, sdat);
   pack_iputl (jbctrl, sdat);
   pack_iputl (jbenter, sdat);
   pack_iputl (jbesc, sdat);
/*    pack_fwrite (&g_ent[0].tilex, sizeof (g_ent[0].tilex), sdat); */
/*    pack_fwrite (&g_ent[0].tiley, sizeof (g_ent[0].tiley), sdat); */
   pack_iputw (g_ent[0].tilex, sdat);
   pack_iputw (g_ent[0].tiley, sdat);
   pack_fclose (sdat);
   return 1;
}



/*! \brief Load game
 *
 * Uh-huh.
 * PH 20030805 Made endian-safe
 * PH 20030914 Now ignores keyboard settings etc in the save file
 * \returns 1 if load succeeded, 0 otherwise
 */
static int load_game (void)
{
   PACKFILE *sdat;
   int a, b, thr, tmin;
   unsigned char tv, ww, ss;

   ww = windowed;
   ss = stretch_view;
   sprintf (strbuf, "sg%d.sav", save_ptr);
   sdat = pack_fopen (kqres (SAVE_DIR, strbuf), F_READ_PACKED);
   if (!sdat) {
      message ("Could not load saved game.", 255, 0, 0, 0);
      return 0;
   }
   /*   pack_fread (&tv, sizeof (tv), sdat); */
   tv = pack_getc (sdat);
   if (tv != kq_version) {
      pack_fclose (sdat);
      message ("Saved game format is not current.", 255, 0, 0, 0);
      return 0;
   }
   /*
      pack_fread (&numchrs, sizeof (numchrs), sdat);
      pack_fread (&gp, sizeof (gp), sdat);
      pack_fread (&thr, sizeof (thr), sdat);
      pack_fread (&tmin, sizeof (tmin), sdat);
    */
   numchrs = pack_igetl (sdat);
   gp = pack_igetl (sdat);
   thr = pack_igetl (sdat);
   tmin = pack_igetl (sdat);
   for (a = 0; a < PSIZE; a++) {
      /*
         pack_fread (&pidx[a], sizeof (pidx[a]), sdat);
       */
      pidx[a] = pack_igetl (sdat);
      g_ent[a].active = 0;
      if (a < numchrs) {
         g_ent[a].eid = pidx[a];
         g_ent[a].active = 1;
      }
   }
   for (a = 0; a < MAXCHRS; a++) {
      /*
         pack_fread (&party[a], sizeof (s_player), sdat);
       */
      load_s_player (&party[a], sdat);
   }
   pack_fread (curmap, 16, sdat);
   for (a = 0; a < 2000; ++a) {
      /*
         pack_fread (progress, 2000, sdat);
       */
      progress[a] = pack_getc (sdat);
   }
   for (a = 0; a < 1000; ++a) {
      /*
         pack_fread (treasure, 1000, sdat);
       */
      treasure[a] = pack_getc (sdat);
   }
   /*
      pack_fread (shopq, NUMSHOPS * SHOPITEMS * sizeof (shopq[0][0]), sdat);
    */
   for (a = 0; a < NUMSHOPS; ++a) {
      for (b = 0; b < SHOPITEMS; ++b) {
         shopq[a][b] = pack_igetw (sdat);
      }
   }
   for (a = 0; a < MAX_INV; ++a) {
      g_inv[a][0] = pack_igetw (sdat);
      g_inv[a][1] = pack_igetw (sdat);
   }
   /* PH: I've decided that I'm going to ignore 
    * these things in the save file. and use
    * the global values instead.
    gsvol = pack_igetl (sdat);
    gmvol = pack_igetl (sdat);
    windowed = pack_getc (sdat);
    stretch_view = pack_getc (sdat);
    wait_retrace = pack_getc (sdat);
    kup = pack_igetl (sdat);
    kdown = pack_igetl (sdat);
    kleft = pack_igetl (sdat);
    kright = pack_igetl (sdat);
    kalt = pack_igetl (sdat);
    kctrl = pack_igetl (sdat);
    kenter = pack_igetl (sdat);
    kesc = pack_igetl (sdat);
    jbalt = pack_igetl (sdat);
    jbctrl = pack_igetl (sdat);
    jbenter = pack_igetl (sdat);
    jbesc = pack_igetl (sdat);
    */
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_getc (sdat);
   pack_getc (sdat);
   pack_getc (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);
   pack_igetl (sdat);

   g_ent[0].tilex = pack_igetw (sdat);
   g_ent[0].tiley = pack_igetw (sdat);
   pack_fclose (sdat);
   load_sgstats ();
   timer_count = 0;
   kmin = tmin;
   khr = thr;
   ksec = 0;
   strcpy (strbuf, curmap);
   for (a = 0; a < (signed int) strlen (strbuf); a++)
      curmap[a] = tolower (strbuf[a]);
   curmap[strlen (strbuf) + 1] = 0;
   hold_fade = 0;
   if (ww != windowed || ss != stretch_view)
      set_graphics_mode ();
   change_map (curmap, g_ent[0].tilex, g_ent[0].tiley, g_ent[0].tilex,
               g_ent[0].tiley);
   /* Set music and sound volume */
   set_volume (gsvol, -1);
   set_music_volume (((float) gmvol) / 255.0);
   return 1;
}



/*! \brief Save/Load menu
 *
 * This is the actual save/load menu.  The only parameter to
 * the function indicates whether we are saving or loading.
 *
 * \param   am_saving 0 if loading, 1 if saving
 * \returns 0 if an error occurred or save/load cancelled
 */
static int saveload (int am_saving)
{
   int stop = 0, rd = 1;

   play_effect (SND_MENU, 128);
   while (!stop) {
      if (rd == 1) {
         clear_bitmap (double_buffer);
         show_sgstats (am_saving);
         blit2screen (0, 0);
      }
      rd = 0;
      readcontrols ();
      if (up) {
         unpress ();
         save_ptr--;
         if (save_ptr < 0)
            save_ptr = NUMSG - 1;
         rd = 1;
         play_effect (SND_CLICK, 128);
      }
      if (down) {
         unpress ();
         save_ptr++;
         if (save_ptr > NUMSG - 1)
            save_ptr = 0;
         rd = 1;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         if (am_saving == 1) {
            if (confirm_save () == 1) {
               if (save_game () == 1)
                  stop = 2;
               else
                  stop = 1;
            }
         } else {
            if (snc[save_ptr] != 0) {
               if (load_game () == 1)
                  stop = 2;
               else
                  stop = 1;
            }
         }
         rd = 1;
      }
      if (bctrl) {
         unpress ();
         stop = 1;
      }
   }
   return stop - 1;
}



/*! \brief Confirm save
 *
 * If the save slot selected already has a saved game in it, confirm that we
 * want to overwrite it.
 *
 * \returns 0 if cancelled, 1 if confirmed
 */
static int confirm_save (void)
{
   int stop = 0;

   if (snc[save_ptr] == 0)
      return 1;
   blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
   menubox (double_buffer, 128, save_ptr * 48 + 12, 14, 1, DARKBLUE);
   print_font (double_buffer, 136, save_ptr * 48 + 20, "Confirm/Cancel",
               FNORMAL);
   blit2screen (0, 0);
   blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
   while (!stop) {
      readcontrols ();
      if (balt) {
         unpress ();
         return 1;
      }
      if (bctrl) {
         unpress ();
         return 0;
      }
   }
   return 0;
}


extern void display_credits (void);



/*! \brief Main menu screen
 *
 * This is the main menu... just display the opening and then the menu and
 * then wait for input.  Also handles loading a saved game.
 *
 * \todo PH make the config menu accessible from here, otherwise you have to
 *          sit through the intro before you can change any settings.
 *
 * \param   c Non-zero if the splash (the bit with the staff and the eight heroes)
 *            should be displayed.
 * \returns 0 if new game, 1 if continuing, 2 if exit
 */
extern int optionsbox (char *, int);



int start_menu (int c)
{
   int stop = 0, ptr = 0, rd = 1, a, b;
   DATAFILE *bg;
   BITMAP *staff, *dudes, *tdudes;
#ifdef KQ_CHEATS
   if (debugging == 0) {
#endif
      play_music ("oxford.s3m", 0);
      if (c == 0) {
         bg = load_datafile_object (PCX_DATAFILE, "KQT_PCX");
         staff = create_bitmap_ex (8, 72, 226);
         dudes = create_bitmap_ex (8, 112, 112);
         tdudes = create_bitmap_ex (8, 112, 112);
         blit ((BITMAP *) bg->dat, staff, 0, 7, 0, 0, 72, 226);
         blit ((BITMAP *) bg->dat, dudes, 80, 0, 0, 0, 112, 112);
         clear_bitmap (double_buffer);
         blit (staff, double_buffer, 0, 0, 124, 22, 72, 226);
         blit2screen (0, 0);

         //do_transition (TRANS_FADE_IN, 1);
         wait (1000);
         for (a = 0; a < 42; a++) {
            stretch_blit (staff, double_buffer, 0, 0, 72, 226, 124 - (a * 32),
                          22 - (a * 96), 72 + (a * 64), 226 + (a * 192));
            blit2screen (0, 0);
            wait (100);
         }
         for (a = 0; a < 5; a++) {
            color_scale (dudes, tdudes, 53 - a, 53 + a);
            draw_sprite (double_buffer, tdudes, 106, 64);
            blit2screen (0, 0);
            wait (100);
         }
         draw_sprite (double_buffer, dudes, 106, 64);
         blit2screen (0, 0);
         wait (1000);
         destroy_bitmap (staff);
         destroy_bitmap (dudes);
         destroy_bitmap (tdudes);

         /*
            TODO: this fade should actually be to white
            if (_color_depth == 8)
            fade_from (pal, whp, 1);
            else
          */
         do_transition (TRANS_FADE_WHITE, 1);
      }
      clear_to_color (double_buffer, 15);
      blit2screen (0, 0);
      set_palette (pal);
      bg = load_datafile_object (PCX_DATAFILE, "TITLE_PCX");
      for (a = 0; a < 16; a++) {
         clear_to_color (double_buffer, 15 - a);
         masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 0, 0, 60 - (a * 4),
                      320, 124);
#if 0
         masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 148, 0, 172, 320,
                      52);
#endif
         blit2screen (0, 0);
         wait (a == 0 ? 500 : 100);
      }
      if (c == 0)
         wait (500);
#ifdef KQ_CHEATS
   } else {
      set_palette (pal);
      bg = load_datafile_object (PCX_DATAFILE, "TITLE_PCX");
   }
#endif

   while (!stop) {
      if (rd) {
         clear_bitmap (double_buffer);
         masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 0, 0, 0, 320, 124);
#if 0
         masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 148, 0, 172, 320,
                      52);
#endif
         menubox (double_buffer, 120, 116, 10, 4, BLUE);
         print_font (double_buffer, 136, 124, "Continue", FNORMAL);
         print_font (double_buffer, 136, 132, "New Game", FNORMAL);
         print_font (double_buffer, 144, 140, "Config", FNORMAL);
         print_font (double_buffer, 152, 148, "Exit", FNORMAL);
         draw_sprite (double_buffer, menuptr, 120, ptr * 8 + 124);
         rd = 0;
      }
      display_credits ();
      blit2screen (0, 0);
      readcontrols ();
      if (bhelp) {
         unpress ();
         show_help ();
         rd = 1;
      }
      if (up) {
         unpress ();
         ptr--;
         if (ptr < 0)
            ptr = 3;
         play_effect (SND_CLICK, 128);
         rd = 1;
      }
      if (down) {
         unpress ();
         ptr++;
         if (ptr > 3)
            ptr = 0;
         play_effect (SND_CLICK, 128);
         rd = 1;
      }
      if (balt) {
         unpress ();
         if (ptr == 0) {
            if (snc[0] == 0 && snc[1] == 0 && snc[2] == 0 && snc[3] == 0
                && snc[4] == 0)
               stop = 2;
            else if (saveload (0) == 1)
               stop = 1;
            rd = 1;
         } else if (ptr == 1) {
            stop = 2;
         } else if (ptr == 2) {
            clear (double_buffer);
            config_menu ();
            rd = 1;

            /* TODO: Save Global Settings Here */
         } else if (ptr == 3) {
            unload_datafile_object (bg);
            klog ("Then exit you shall!");
            return 2;
         }
      }
   }
   unload_datafile_object (bg);
   if (stop == 2) {
      /* New game init */
      for (a = 0; a < MAXCHRS; a++)
         memcpy (&party[a], &players[a].plr, sizeof (s_player));
      init_players ();
      memset (progress, 0, 2000);
      memset (treasure, 0, 1000);
      numchrs = 0;
      for (a = 0; a < NUMSHOPS; a++)
         for (b = 0; b < SHOPITEMS; b++)
            shopq[a][b] = shops[a][b][1];
      for (b = 0; b < 2; b++) {
         for (a = 0; a < MAX_INV; a++)
            g_inv[a][b] = 0;
      }
      if (skip_intro == 1) {
         progress[P_SKIPINTRO] = 1;
      }
   }
   return stop - 1;
}



/*! \brief Display system menu
 *
 * This is the system menu that is invoked from within the game.
 * From here you can save, load, configure a couple of options or
 * exit the game altogether.
 * \date 20040229 PH added 'Save anytime' facility when cheat mode is ON
 *
 * \returns 0 if cancelled or nothing happened, 1 otherwise
 */
int system_menu (void)
{
   int stop = 0, ptr = 0;

   unpress ();
   while (!stop) {
      drawmap ();
      menubox (double_buffer, xofs, yofs, 8, 4, BLUE);
      if (cansave == 1)
         print_font (double_buffer, 16 + xofs, 8 + yofs, "Save", FNORMAL);
      else {
#ifdef KQ_CHEATS
         if (cheat)
            print_font (double_buffer, 16 + xofs, 8 + yofs, "[Save]", FNORMAL);
         else
            print_font (double_buffer, 16 + xofs, 8 + yofs, "Save", FDARK);
#else
         print_font (double_buffer, 16 + xofs, 8 + yofs, "Save", FDARK);
#endif /* KQ_CHEATS */
      }
      print_font (double_buffer, 16 + xofs, 16 + yofs, "Load", FNORMAL);
      print_font (double_buffer, 16 + xofs, 24 + yofs, "Config", FNORMAL);
      print_font (double_buffer, 16 + xofs, 32 + yofs, "Exit", FNORMAL);
      draw_sprite (double_buffer, menuptr, 0 + xofs, ptr * 8 + 8 + yofs);
      blit2screen (xofs, yofs);
      readcontrols ();
      if (up) {
         unpress ();
         ptr--;
         if (ptr < 0)
            ptr = 3;
         play_effect (SND_CLICK, 128);
      }
      if (down) {
         unpress ();
         ptr++;
         if (ptr > 3)
            ptr = 0;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         if (ptr == 0) {
#ifdef KQ_CHEATS
            if (cansave == 1 || cheat) {
#else
            if (cansave == 1) {
#endif /* KQ_CHEATS */
               saveload (1);
               return 0;
            } else
               play_effect (SND_BAD, 128);
         }
         if (ptr == 1) {
            if (snc[0] != 0 || snc[1] != 0 || snc[2] != 0)
               if (saveload (0) == 1)
                  return 0;
         }
         if (ptr == 2)
            config_menu ();
         if (ptr == 3)
            return 1;
      }
      if (bctrl) {
         unpress ();
         return 0;
      }
   }
   return 0;
}
