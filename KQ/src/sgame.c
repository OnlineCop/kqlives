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
#include <ctype.h>
#include <string.h>

#include "setup.h"
#include "kq.h"
#include "draw.h"
#include "sgame.h"
#include "kqsnd.h"
#include "combat.h"
#include "res.h"
#include "masmenu.h"
#include "magic.h"
#include "shopmenu.h"
#include "progress.h"
#include "menu.h"

/*
   globals
*/
int snc[NUMSG], sgp[NUMSG], shr[NUMSG], smin[NUMSG], sid[NUMSG][PSIZE],
   slv[NUMSG][PSIZE];
unsigned char shp[NUMSG][PSIZE], smp[NUMSG][PSIZE];
int save_ptr = 0;



/*  internal functions  */
static void show_sgstats (int);
static int save_game (void);
static int load_game (void);
static int saveload (int);
static int confirm_save (void);



/*
   This loads the mini stats for each saved game.  These
   mini stats are just for displaying info about the save
   game on the save/load game screen.
*/
void load_sgstats (void)
{
   PACKFILE *ldat;
   int a, b, c;
   unsigned char vc;
   s_player tpm;

   for (a = 0; a < NUMSG; a++)
     {
	sprintf (strbuf, "%ssg%d.sav", SAVE_DIR, a);
	ldat = pack_fopen (strbuf, F_READ_PACKED);
	if (!ldat)
	  {
	     snc[a] = 0;
	     sgp[a] = 0;
	     shr[a] = 0;
	     smin[a] = 0;
	     for (b = 0; b < PSIZE; b++)
	       {
		  sid[a][b] = 0;
		  shp[a][b] = 0;
		  smp[a][b] = 0;
	       }
	  }
	else
	  {
	     pack_fread (&vc, sizeof (vc), ldat);
	     if (vc != kq_version)
		snc[a] = -1;
	     else
	       {
		  pack_fread (&snc[a], sizeof (snc[a]), ldat);
		  pack_fread (&sgp[a], sizeof (sgp[a]), ldat);
		  pack_fread (&shr[a], sizeof (shr[a]), ldat);
		  pack_fread (&smin[a], sizeof (smin[a]), ldat);
		  for (b = 0; b < PSIZE; b++)
		     pack_fread (&sid[a][b], sizeof (sid[a][b]), ldat);
		  for (b = 0; b < MAXCHRS; b++)
		    {
		       pack_fread (&tpm, sizeof (tpm), ldat);
		       for (c = 0; c < PSIZE; c++)
			 {
			    if (b == sid[a][c])
			      {
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

/*
   This is the routine that displays the information about
   each saved game for the save/load screen.
*/
static void show_sgstats (int saving)
{
   int a, sg, hx, hy, b;

   if (saving)
     {
	menubox (double_buffer, 8, save_ptr * 48 + 12, 6, 1, BLUE);
	print_font (double_buffer, 16, save_ptr * 48 + 20, "Saving", FGOLD);
     }
   else
     {
	menubox (double_buffer, 0, save_ptr * 48 + 12, 7, 1, BLUE);
	print_font (double_buffer, 8, save_ptr * 48 + 20, "Loading", FGOLD);
     }
   for (sg = 0; sg < NUMSG; sg++)
     {
	if (sg == save_ptr)
	   menubox (double_buffer, 72, sg * 48, 29, 4, DARKBLUE);
	else
	   menubox (double_buffer, 72, sg * 48, 29, 4, BLUE);
	if (snc[sg] == -1)
	   print_font (double_buffer, 136, sg * 48 + 20, "Wrong version",
		       FNORMAL);
	else
	  {
	     if (snc[sg] == 0)
		print_font (double_buffer, 168, sg * 48 + 20, "Empty",
			    FNORMAL);
	     else
	       {
		  for (a = 0; a < snc[sg]; a++)
		    {
		       hx = a * 72 + 84;
		       hy = sg * 48 + 12;
		       draw_sprite (double_buffer, frames[sid[sg][a]][1], hx,
				    hy + 4);
		       sprintf (strbuf, "L: %02d", slv[sg][a]);
		       print_font (double_buffer, hx + 16, hy, strbuf,
				   FNORMAL);
		       print_font (double_buffer, hx + 16, hy + 8, "H:",
				   FNORMAL);
		       print_font (double_buffer, hx + 16, hy + 16, "M:",
				   FNORMAL);
		       rectfill (double_buffer, hx + 33, hy + 9, hx + 65,
				 hy + 15, 2);
		       rectfill (double_buffer, hx + 32, hy + 8, hx + 64,
				 hy + 14, 35);
		       rectfill (double_buffer, hx + 33, hy + 17, hx + 65,
				 hy + 23, 2);
		       rectfill (double_buffer, hx + 32, hy + 16, hx + 64,
				 hy + 22, 19);
		       b = shp[sg][a] * 32 / 100;
		       rectfill (double_buffer, hx + 32, hy + 9, hx + 32 + b,
				 hy + 13, 41);
		       b = smp[sg][a] * 32 / 100;
		       rectfill (double_buffer, hx + 32, hy + 17, hx + 32 + b,
				 hy + 21, 25);
		    }
		  sprintf (strbuf, "T %d:%02d", shr[sg], smin[sg]);
		  print_font (double_buffer, 236, sg * 48 + 12, strbuf,
			      FNORMAL);
		  sprintf (strbuf, "G %d", sgp[sg]);
		  print_font (double_buffer, 236, sg * 48 + 28, strbuf,
			      FNORMAL);
	       }
	  }
     }
}

/*
   You guessed it... save the game.
*/
static int save_game (void)
{
   PACKFILE *sdat;
   int a, b;

   for (b = 0; b < PSIZE; b++)
     {
	sid[save_ptr][b] = 0;
	shp[save_ptr][b] = 0;
	smp[save_ptr][b] = 0;
	slv[save_ptr][b] = 0;
     }
   for (b = 0; b < numchrs; b++)
     {
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
   sprintf (strbuf, "%ssg%d.sav", SAVE_DIR, save_ptr);
   sdat = pack_fopen (strbuf, F_WRITE_PACKED);
   if (!sdat)
     {
	message ("Could not save game data.", 255, 0, 0, 0);
	return 0;
     }
   pack_fwrite (&kq_version, sizeof (kq_version), sdat);
   pack_fwrite (&numchrs, sizeof (numchrs), sdat);
   pack_fwrite (&gp, sizeof (gp), sdat);
   pack_fwrite (&shr[save_ptr], sizeof (shr[save_ptr]), sdat);
   pack_fwrite (&smin[save_ptr], sizeof (smin[save_ptr]), sdat);
   for (a = 0; a < PSIZE; a++)
      pack_fwrite (&pidx[a], sizeof (pidx[a]), sdat);
   for (a = 0; a < MAXCHRS; a++)
      pack_fwrite (&party[a], sizeof (s_player), sdat);
   pack_fwrite (curmap, 16, sdat);
   pack_fwrite (progress, 2000, sdat);
   pack_fwrite (treasure, 1000, sdat);
   pack_fwrite (shopq, NUMSHOPS * 12 * sizeof (shopq[0][0]), sdat);
   pack_fwrite (g_inv, MAX_INV * 2 * sizeof (g_inv[0][0]), sdat);
   /* PH FIXME: do we _really_ want things like controls and screen */
   /* mode to be saved/loaded ? */
   pack_fwrite (&gsvol, sizeof (gsvol), sdat);
   pack_fwrite (&gmvol, sizeof (gmvol), sdat);
   pack_fwrite (&windowed, sizeof (windowed), sdat);
   pack_fwrite (&stretch_view, sizeof (stretch_view), sdat);
   pack_fwrite (&wait_retrace, sizeof (wait_retrace), sdat);
   pack_fwrite (&kup, sizeof (kup), sdat);
   pack_fwrite (&kdown, sizeof (kdown), sdat);
   pack_fwrite (&kleft, sizeof (kleft), sdat);
   pack_fwrite (&kright, sizeof (kright), sdat);
   pack_fwrite (&kalt, sizeof (kalt), sdat);
   pack_fwrite (&kctrl, sizeof (kctrl), sdat);
   pack_fwrite (&kenter, sizeof (kenter), sdat);
   pack_fwrite (&kesc, sizeof (kesc), sdat);
   pack_fwrite (&jbalt, sizeof (jbalt), sdat);
   pack_fwrite (&jbctrl, sizeof (jbctrl), sdat);
   pack_fwrite (&jbenter, sizeof (jbenter), sdat);
   pack_fwrite (&jbesc, sizeof (jbesc), sdat);
   pack_fwrite (&g_ent[0].tilex, sizeof (g_ent[0].tilex), sdat);
   pack_fwrite (&g_ent[0].tiley, sizeof (g_ent[0].tiley), sdat);
   pack_fclose (sdat);
   return 1;
}

/*
   Uh-huh.
*/
static int load_game (void)
{
   PACKFILE *sdat;
   int a, thr, tmin;
   unsigned char tv, ww, ss;

   ww = windowed;
   ss = stretch_view;
   sprintf (strbuf, "%ssg%d.sav", SAVE_DIR, save_ptr);
   sdat = pack_fopen (strbuf, F_READ_PACKED);
   if (!sdat)
     {
	message ("Could not load saved game.", 255, 0, 0, 0);
	return 0;
     }
   pack_fread (&tv, sizeof (tv), sdat);
   if (tv != kq_version)
     {
	pack_fclose (sdat);
	message ("Saved game format is not current.", 255, 0, 0, 0);
	return 0;
     }
   pack_fread (&numchrs, sizeof (numchrs), sdat);
   pack_fread (&gp, sizeof (gp), sdat);
   pack_fread (&thr, sizeof (thr), sdat);
   pack_fread (&tmin, sizeof (tmin), sdat);
   for (a = 0; a < PSIZE; a++)
     {
	pack_fread (&pidx[a], sizeof (pidx[a]), sdat);
	g_ent[a].active = 0;
	if (a < numchrs)
	  {
	     g_ent[a].eid = pidx[a];
	     g_ent[a].active = 1;
	  }
     }
   for (a = 0; a < MAXCHRS; a++)
      pack_fread (&party[a], sizeof (s_player), sdat);
   pack_fread (curmap, 16, sdat);
   pack_fread (progress, 2000, sdat);
   pack_fread (treasure, 1000, sdat);
   pack_fread (shopq, NUMSHOPS * 12 * sizeof (shopq[0][0]), sdat);
   pack_fread (g_inv, MAX_INV * 2 * sizeof (g_inv[0][0]), sdat);
   pack_fread (&gsvol, sizeof (gsvol), sdat);
   pack_fread (&gmvol, sizeof (gmvol), sdat);
   pack_fread (&windowed, sizeof (windowed), sdat);
   pack_fread (&stretch_view, sizeof (stretch_view), sdat);
   pack_fread (&wait_retrace, sizeof (wait_retrace), sdat);
   pack_fread (&kup, sizeof (kup), sdat);
   pack_fread (&kdown, sizeof (kdown), sdat);
   pack_fread (&kleft, sizeof (kleft), sdat);
   pack_fread (&kright, sizeof (kright), sdat);
   pack_fread (&kalt, sizeof (kalt), sdat);
   pack_fread (&kctrl, sizeof (kctrl), sdat);
   pack_fread (&kenter, sizeof (kenter), sdat);
   pack_fread (&kesc, sizeof (kesc), sdat);
   pack_fread (&jbalt, sizeof (jbalt), sdat);
   pack_fread (&jbctrl, sizeof (jbctrl), sdat);
   pack_fread (&jbenter, sizeof (jbenter), sdat);
   pack_fread (&jbesc, sizeof (jbesc), sdat);
   pack_fread (&g_ent[0].tilex, sizeof (g_ent[0].tilex), sdat);
   pack_fread (&g_ent[0].tiley, sizeof (g_ent[0].tiley), sdat);
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
   return 1;
}

/*
   This is the actual save/load menu.  The only parameter to
   the function indicates whether we are saving or loading.
*/
static int saveload (int am_saving)
{
   int stop = 0, rd = 1;

   play_effect (SND_MENU, 128);
   while (!stop)
     {
	if (rd == 1)
	  {
	     clear_bitmap (double_buffer);
	     show_sgstats (am_saving);
	     blit2screen (0, 0);
	  }
	rd = 0;
	readcontrols ();
	if (up)
	  {
	     unpress ();
	     save_ptr--;
	     if (save_ptr < 0)
		save_ptr = NUMSG - 1;
	     rd = 1;
	     play_effect (SND_CLICK, 128);
	  }
	if (down)
	  {
	     unpress ();
	     save_ptr++;
	     if (save_ptr > NUMSG - 1)
		save_ptr = 0;
	     rd = 1;
	     play_effect (SND_CLICK, 128);
	  }
	if (balt)
	  {
	     unpress ();
	     if (am_saving == 1)
	       {
		  if (confirm_save () == 1)
		    {
		       if (save_game () == 1)
			  stop = 2;
		       else
			  stop = 1;
		    }
	       }
	     else
	       {
		  if (snc[save_ptr] != 0)
		    {
		       if (load_game () == 1)
			  stop = 2;
		       else
			  stop = 1;
		    }
	       }
	     rd = 1;
	  }
	if (bctrl)
	  {
	     unpress ();
	     stop = 1;
	  }
     }
   return stop - 1;
}

/*
   If the save slot selected already has a saved game in
   it, confirm that we want to overwrite.
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
   while (!stop)
     {
	readcontrols ();
	if (balt)
	  {
	     unpress ();
	     return 1;
	  }
	if (bctrl)
	  {
	     unpress ();
	     return 0;
	  }
     }
   return 0;
}

/*
   This is the main menu... just display the opening and then
   the menu and then wait for input.
*/
int start_menu (int c)
{
   int stop = 0, ptr = 0, rd = 1, a, b;
   DATAFILE *bg;
   BITMAP *staff, *dudes, *tdudes;
   PALETTE whp;

   if (debugging == 0)
     {
	if (c == 0)
	  {
	     for (a = 0; a < 256; a++)
	       {
		  whp[a].r = 63;
		  whp[a].g = 63;
		  whp[a].b = 63;
	       }
	     play_song ("oxford.s3m", 0);
	     bg = load_datafile_object (PCX_DATAFILE, "KQT_PCX");
	     staff = create_bitmap (72, 226);
	     dudes = create_bitmap (112, 112);
	     tdudes = create_bitmap (112, 112);
	     blit ((BITMAP *) bg->dat, staff, 0, 7, 0, 0, 72, 226);
	     blit ((BITMAP *) bg->dat, dudes, 80, 0, 0, 0, 112, 112);
	     clear_bitmap (double_buffer);
	     blit (staff, double_buffer, 0, 0, 124, 22, 72, 226);
	     blit2screen (0, 0);
	     fade_in (pal, 1);
	     rest (1000);
	     for (a = 0; a < 42; a++)
	       {
		  stretch_blit (staff, double_buffer, 0, 0, 72, 226,
				124 - (a * 32), 22 - (a * 96), 72 + (a * 64),
				226 + (a * 192));
		  blit2screen (0, 0);
		  rest (100);
	       }
	     for (a = 0; a < 5; a++)
	       {
		  color_scale (dudes, tdudes, 53 - a, 53 + a);
		  draw_sprite (double_buffer, tdudes, 106, 64);
		  blit2screen (0, 0);
		  rest (100);
	       }
	     draw_sprite (double_buffer, dudes, 106, 64);
	     blit2screen (0, 0);
	     rest (1000);
	     destroy_bitmap (staff);
	     destroy_bitmap (dudes);
	     destroy_bitmap (tdudes);
	     fade_from (pal, whp, 1);
	  }
	clear_to_color (double_buffer, 15);
	blit2screen (0, 0);
	set_palette (pal);
	bg = load_datafile_object (PCX_DATAFILE, "TITLE_PCX");
	for (a = 0; a < 16; a++)
	  {
	     clear_to_color (double_buffer, 15 - a);
	     masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 0, 0,
			  60 - (a * 4), 320, 124);
	     masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 148, 0, 172,
			  320, 52);
	     blit2screen (0, 0);
	     if (a == 0)
		rest (500);
	     else
		rest (100);
	  }
	if (c == 0)
	   rest (500);
     }
   else
     {
	set_palette (pal);
	bg = load_datafile_object (PCX_DATAFILE, "TITLE_PCX");
     }
   while (!stop)
     {
	if (rd)
	  {
	     clear_bitmap (double_buffer);
	     masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 0, 0, 0, 320,
			  124);
	     masked_blit ((BITMAP *) bg->dat, double_buffer, 0, 148, 0, 172,
			  320, 52);
	     menubox (double_buffer, 120, 116, 10, 3, BLUE);
	     print_font (double_buffer, 136, 124, "Continue", FNORMAL);
	     print_font (double_buffer, 136, 132, "New Game", FNORMAL);
/* PH FIXME add option to change the config settings from Main Menu */
/*   Otherwise you have to start a game, and maybe sit throught the intro before  */
/* you can change anything. */
	     print_font (double_buffer, 152, 140, "Exit", FNORMAL);
	     draw_sprite (double_buffer, menuptr, 120, ptr * 8 + 124);
	     blit2screen (0, 0);
	  }
	readcontrols ();
	if (up)
	  {
	     unpress ();
	     ptr--;
	     if (ptr < 0)
		ptr = 2;
	     play_effect (SND_CLICK, 128);
	  }
	if (down)
	  {
	     unpress ();
	     ptr++;
	     if (ptr > 2)
		ptr = 0;
	     play_effect (SND_CLICK, 128);
	  }
	if (balt)
	  {
	     unpress ();
	     if (ptr == 0)
	       {
		  if (snc[0] == 0 && snc[1] == 0 && snc[2] == 0 && snc[3] == 0
		      && snc[4] == 0)
		     stop = 2;
		  else if (saveload (0) == 1)
		     stop = 1;
	       }
	     if (ptr == 1)
		stop = 2;
	     if (ptr == 2)
	       {
		  unload_datafile_object (bg);
		  program_death ("Then exit you shall!");
	       }
	  }
     }
   unload_datafile_object (bg);
   if (stop == 2)
     {
	for (a = 0; a < MAXCHRS; a++)
	   party[a] = players[a];
	init_players ();
	memset (progress, 0, 2000);
	memset (treasure, 0, 1000);
	numchrs = 0;
	for (a = 0; a < NUMSHOPS; a++)
	   for (b = 0; b < 12; b++)
	      shopq[a][b] = shops[a][b][1];
	for (b = 0; b < 2; b++)
	  {
	     for (a = 0; a < MAX_INV; a++)
		g_inv[a][b] = 0;
	  }
	if (skip_intro == 1)
	   progress[P_SKIPINTRO] = 1;
     }
   return stop - 1;
}

/*
   This is the system menu that is invoked from within the game.
   From here you can save, load, configure a couple of options or
   exit the game altogether.
*/
int system_menu (void)
{
   int stop = 0, ptr = 0;

   unpress ();
   while (!stop)
     {
	drawmap ();
	menubox (double_buffer, xofs, yofs, 8, 4, BLUE);
	if (cansave == 1)
	   print_font (double_buffer, 16 + xofs, 8 + yofs, "Save", FNORMAL);
	else
	   print_font (double_buffer, 16 + xofs, 8 + yofs, "Save", FDARK);
	print_font (double_buffer, 16 + xofs, 16 + yofs, "Load", FNORMAL);
	print_font (double_buffer, 16 + xofs, 24 + yofs, "Config", FNORMAL);
	print_font (double_buffer, 16 + xofs, 32 + yofs, "Exit", FNORMAL);
	draw_sprite (double_buffer, menuptr, 0 + xofs, ptr * 8 + 8 + yofs);
	blit2screen (xofs, yofs);
	readcontrols ();
	if (up)
	  {
	     unpress ();
	     ptr--;
	     if (ptr < 0)
		ptr = 3;
	     play_effect (SND_CLICK, 128);
	  }
	if (down)
	  {
	     unpress ();
	     ptr++;
	     if (ptr > 3)
		ptr = 0;
	     play_effect (SND_CLICK, 128);
	  }
	if (balt)
	  {
	     unpress ();
	     if (ptr == 0)
	       {
		  if (cansave == 1)
		    {
		       saveload (1);
		       return 0;
		    }
		  else
		     play_effect (SND_BAD, 128);
	       }
	     if (ptr == 1)
	       {
		  if (snc[0] != 0 || snc[1] != 0 || snc[2] != 0)
		     if (saveload (0) == 1)
			return 0;
	       }
	     if (ptr == 2)
		config_menu ();
	     if (ptr == 3)
		return 1;
	  }
	if (bctrl)
	  {
	     unpress ();
	     return 0;
	  }
     }
   return 0;
}
/**/
