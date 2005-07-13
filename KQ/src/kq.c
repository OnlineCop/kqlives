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
 * \brief Main file for KQ
 *
 * \author JB
 * \date ????????
 *
 * This file includes the main() function,
 * most of the global variables, and some other stuff,
 * for example, controls, some initialisation and
 * timing.
 *
 * \note 23: I don't know if we're going to do anything to lessen the number of
 * globals, but I tried to lay them out as attractivly as possible until we
 * figure out what all of them are for. Plus I tried to keep everything below
 * 80 characters a line, and labels what few variables struck me as obvious
 *
 * Thanks due to Edge <hardedged@excite.com> and Caz Jones for BeOS joystick fixes
 */

#include <stdio.h>
#include <time.h>
#ifdef ALLEGRO_BEOS
#include <sys/time.h>
#endif
#include <string.h>

#if 0
#if defined(HAVE_GETPWUID)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(HAVE_LOADLIBRARY)
#include <direct.h>
#endif
#endif

#include <allegro.h>

#include "kq.h"
#include "draw.h"
#include "entity.h"
#include "fade.h"
#include "intrface.h"
#include "itemdefs.h"
#include "itemmenu.h"
#include "masmenu.h"
#include "menu.h"
#include "mpcx.h"
#include "music.h"
#include "progress.h"
#include "res.h"
#include "setup.h"
#include "scrnshot.h"
#include "sgame.h"
#include "shopmenu.h"
#include "disk.h"
#include "credits.h"

/*! Name of the current map */
char curmap[16];

/*! \brief Which keys are pressed.
 *
 * \note 23: apparently flags for determining keypresses and player movement.
 * Seems to use some kind of homebrew Hungarian notation; I assume 'b' means
 * bool.  Most if not all of these are updated in readcontrols() below ....
 */
int right, left, up, down, besc, balt, bctrl, benter, bhelp;
/*!  Scan codes for the keys (help is always F1)*/
int kright, kleft, kup, kdown, kesc, kenter, kalt, kctrl;
/*! Joystick buttons */
int jbalt, jbctrl, jbenter, jbesc;

/*! View and character positions */
int vx, vy, mx, my;
/*! What was the last direction each player moved in */
int steps = 0, lastm[PSIZE];

/*! 23: various global bitmaps */
BITMAP *double_buffer, *fx_buffer, *map_icons[MAX_TILES], *back, *tc, *tc2,
   *bub[8], *b_shield, *b_shell, *b_repulse, *b_mp,
   *cframes[NUM_FIGHTERS][MAXCFRAMES], *tcframes[NUM_FIGHTERS][MAXCFRAMES],
   *frames[MAXCHRS][MAXFRAMES], *eframes[MAXE][MAXEFRAMES], *pgb[9],
   *sfonts[5], *bord[8], *menuptr, *mptr, *sptr, *stspics, *sicons, *bptr,
   *missbmp, *noway, *upptr, *dnptr, *shadow[MAX_SHADOWS], *kfonts;

#ifdef DEBUGMODE
BITMAP *obj_mesh;
#endif

/*! Layers in the map */
unsigned short *map_seg = NULL, *b_seg = NULL, *f_seg = NULL;
/*! Zone, shadow and obstacle layers */
unsigned char *z_seg = NULL, *s_seg = NULL, *o_seg = NULL;
/*! keeps track of tasks completed and treasure chests opened */
unsigned char *progress = NULL, *treasure = NULL;
/*! Current map */
s_map g_map;
/*! Current entities (players+NPCs) */
s_entity g_ent[MAX_ENT + PSIZE];

#define MAX_TILESETS 10
/*! The different tilesets allowed in the game (loaded in from tileset.kq) */
s_tileset tilesets[MAX_TILESETS];
int num_tilesets = 0;

/*! Tile animation specifiers for the current tileset */
s_anim adata[MAX_ANIM];

/*! Number of enemies */
int noe = 0;
/*! Identifies characters in the party */
int pidx[MAXCHRS];
/*! Number of characters in the party */
int numchrs = 0;
/*! Current gold */
int gp = 0;
/*! pixel offset in the current map view */
int xofs, yofs;
/*! Sound and music volume */
int gsvol = 250, gmvol = 250;
/*! Is the party under 'automatic' (i.e. scripted) control */
unsigned char autoparty = 0;
/*! Are all heroes dead? */
unsigned char alldead = 0;
/*! Is sound activated/available? */
unsigned char is_sound = 1, sound_avail;
/*! Makes is_active() return TRUE even if the character is dead */
unsigned char deadeffect = 0;
/*! Does the viewport follow the characters?*/
unsigned char vfollow = 1;
/*! Whether the sun stone can be used in this map*/
unsigned char use_sstone = 0;
/*! Version number (used for version control in sgame.c) */
// TT: Shouldn't this be a const?
unsigned char kq_version = 91;
/*! If non-zero, don't do fade effects. The only place this is
 * set is in scripts. */
unsigned char hold_fade = 0;
/*! True if player can save at this point */
unsigned char cansave = 0;
/*! True if the intro is to be skipped (the bit where the heroes learn of the quest) */
unsigned char skip_intro = 0;
/*! Graphics mode settings */
unsigned char wait_retrace = 0, windowed = 0, stretch_view = 0;
/*! Current sequence position of animated tiles */
unsigned short tilex[MAX_TILES];
/*! Current 'time' for animated tiles. When this increments to adata[].delay,
 * the next tile is shown */
unsigned short adelay[MAX_ANIM];
/*! Temporary buffer for string operations (used everywhere!) */
char *strbuf = NULL;
/* char *savedir = NULL; */


/*! Characters in play. The pidx[] array references this for the heroes actually
 * on screen, e.g. party[pidx[0]] is the 'lead' character,
 * party[pidx[1]] is the follower, if there are 2 in the party.
 * We need to store all of them, because heroes join and leave during the game.
 */
s_player party[MAXCHRS];


/*! Initial character data
 *
 * \note 23: Self explanatory. This would all correspond to the s_player
 * structure. I had to invent my own little (somewhat ugly) layout since it
 * all shot past the 80-character mark by quite a ways :)
 */
s_heroinfo players[MAXCHRS];



/*! Table to manage stats for the level up process (see level_up()) */
unsigned short lup[MAXCHRS][20] = {
   {10, 70, 9, 2, 190, 90, 150, 60, 70, 15, 20, 20, 50, 50, 0, 10, 0},
   {10, 70, 8, 4, 120, 120, 120, 90, 80, 10, 50, 45, 25, 45, 0, 25, 0},
   {10, 70, 6, 6, 90, 80, 100, 190, 80, 15, 100, 45, 10, 10, 0, 15, 0},
   {10, 70, 8, 5, 110, 90, 80, 80, 190, 15, 20, 100, 20, 15, 0, 10, 0},
   {10, 70, 8, 6, 50, 130, 50, 180, 120, 20, 100, 60, 0, 5, 0, 20, 0},
   {10, 70, 8, 3, 140, 90, 160, 50, 100, 15, 20, 70, 35, 35, 0, 15, 0},
   {10, 70, 7, 5, 110, 170, 90, 120, 70, 25, 50, 20, 25, 25, 0, 30, 0},
   {10, 70, 8, 6, 50, 100, 50, 160, 160, 10, 90, 90, 5, 0, 0, 20, 0}
};



/*! Characters when they are in combat */
s_fighter fighter[NUM_FIGHTERS];
/*! Temp store for adjusted stats */
s_fighter tempa, tempd;

/*! Name of current shop */
char sname[39];
/*! Number of items in a shop */
int noi;
/*! Items in a shop */
int shin[SHOPITEMS];
/*! Should we display a box with ctext in it (used in combat) */
int dct = 0;
/*! Name of current spell or special ability */
char ctext[39];

/* PH: needed these fwd declarations */
#ifdef DEBUGMODE
static void data_dump (void);
#endif

static void allocate_stuff (void);
static void load_heroes (void);
void reset_timer_events (void);

/*! \note 23: for keeping time. timer_counter is the game timer the main game
 * loop uses for logic (see int main()) and the rest track your playtime in
 * hours, minutes and seconds. They're all used in the my_counter() timer
 * function just below
 */
volatile int timer = 0, ksec = 0, kmin = 0, khr = 0, timer_count = 0;

/*! Current colour map */
COLOR_MAP cmap;

/*! Party can run away from combat? */
unsigned char can_run = 1;
/*! Is the map description is displayed on screen? */
unsigned char display_desc = 0;
/*! Which map layers should be drawn. These are set when the map is loaded;
    see change_map()
 */
unsigned char draw_background = 1, draw_middle = 1,
   draw_foreground = 1, draw_shadow = 1;
/*! Items in inventory. g_inv[][0] is the item id, g_inv[][1] is the quantity */
unsigned short g_inv[MAX_INV][2];
/*! View coordinates; the view is a way of selecting a subset of the map to show. */
int view_x1, view_y1, view_x2, view_y2, view_on = 0;
/*! Are we in combat mode? */
int in_combat = 0;
/*! Frame rate stuff */
int skips = 0, frate, mfrate = 0, show_frate = 0;
/*! Should we use the joystick */
int use_joy = 1;

#ifdef KQ_CHEATS
/*! Is cheat mode activated? */
int cheat = 0;
#endif

/*! The number of frames per second */
#define KQ_FPS 100



/*! \brief Allegro timer callback
 *
 * New interrupt handler set to keep game time.
 */
void my_counter (void)
{
   timer++;

   if (timer >= KQ_FPS) {
      timer = 0;
      ksec++;
      mfrate = frate;
      frate = 0;
   }

   timer_count++;
}

END_OF_FUNCTION (my_counter);



static void time_counter (void)
{
   kmin++;
   if (kmin >= 60) {
      kmin = 0;
      khr++;
   }

}

END_OF_FUNCTION (time_counter);



#ifdef ALLEGRO_BEOS
static inline long long gettime ()
{
   struct timeval tv;
   gettimeofday (&tv, 0);
   return (tv.tv_sec * 1000000) + (tv.tv_usec);
}



int maybe_poll_joystick ()
{
   long long lasttime = 0;
   long long nowtime = gettime ();
   if ((unsigned long long) nowtime > (unsigned long long) lasttime) {
      lasttime = nowtime + 150000;
      return poll_joystick ();
   } else
      return -1;
}

#else
#define maybe_poll_joystick poll_joystick
#endif



/*! \brief Handle user input.
 *
 * Updates all of the game controls according to user input.
 * 2003-05-27 PH: updated to re-enable the joystick
 * 2003-09-07 Edge <hardedged@excite.com>: removed duplicate input, joystick code
 * 2003-09-07 Caz Jones: last time code workaround pci-gameport bug
 * (should not affect non-buggy drivers - please report to edge)
 */
void readcontrols (void)
{
   JOYSTICK_INFO *stk;
   poll_music ();

   /* PH 2002.09.21 in case this is needed (not sure on which platforms it is) */
   if (keyboard_needs_poll ()) {
      poll_keyboard ();
   }

   balt = key[kalt];
   besc = key[kesc];
   bctrl = key[kctrl];
   benter = key[kenter];
   bhelp = key[KEY_F1];

   up = key[kup];
   down = key[kdown];
   left = key[kleft];
   right = key[kright];

// TT: Is there a reason this has to be called twice in one function?
//   poll_music ();

   /* Emergency kill-game set. */
   /* PH modified - need to hold down for 0.50 sec */
   if (key[KEY_ALT] && key[KEY_X]) {
      int kill_time = timer_count + KQ_FPS / 2;
      while (key[KEY_ALT] && key[KEY_X]) {
         if (timer_count >= kill_time) {
            /* Pressed, now wait for release */
            clear_bitmap (screen);
            while (key[KEY_ALT] && key[KEY_X]) {
            }
            program_death ("X-ALT pressed... exiting.");
         }
      }
   }
#ifdef DEBUGMODE
   if (debugging > 0) {
      if (key[KEY_F11])
         data_dump ();

      /* Back to menu - by pretending all the heroes died.. hehe */
      if (key[KEY_ALT] && key[KEY_M])
         alldead = 1;
   }
#endif

   /* ML,2002.09.21: Saves sequential screen captures to disk. See scrnshot.c/h for more info. */
   if (key[KEY_F12]) {
      save_screenshot (screen, "kq");
      play_effect (SND_TWINKLE, 128);
      /* Wait for key to be released before continuing */
      /* PH 2002.09.21 n.b. keyboard not necessarily in polling mode */
      while (key[KEY_F12]) {
         if (keyboard_needs_poll ())
            poll_keyboard ();
      }
   }

   if (use_joy > 0 && maybe_poll_joystick () == 0) {
      stk = &joy[use_joy - 1];
      left |= stk->stick[0].axis[0].d1;
      right |= stk->stick[0].axis[0].d2;
      up |= stk->stick[0].axis[1].d1;
      down |= stk->stick[0].axis[1].d2;

      balt |= stk->button[0].b;
      bctrl |= stk->button[1].b;
      benter |= stk->button[2].b;
      besc |= stk->button[3].b;

   }
}



#ifdef DEBUGMODE
/*! \brief Write debug data to disk
 *
 * Writes the treasure and progress arrays in text format to "treasure.log"
 * and "progress.log" respectively. This happens in response to user hitting
 * the F11 key.
 */
void data_dump (void)
{
   FILE *ff;
   int a;

   if (debugging > 0) {
      ff = fopen ("treasure.log", "w");
      if (!ff)
         program_death ("Could not open treasure.log!");
      for (a = 0; a < 200; a++)
         fprintf (ff, "%d = %d\n", a, treasure[a]);
      fclose (ff);

      ff = fopen ("progress.log", "w");
      if (!ff)
         program_death ("Could not open progress.log!");
      for (a = 0; a < 200; a++)
         fprintf (ff, "%d = %d\n", a, progress[a]);
      for (a = P_SHOPSTART; a < P_SHOPSTART + NUMSHOPS; a++)
         fprintf (ff, "%d = %d\n", a, progress[a]);
      fclose (ff);
   }
}
#endif



/*! \brief Move the viewport if necessary to include the players
 *
 * This is used to determine what part of the map is
 * visible on the screen.  Usually, the party can walk around
 * in the center of the screen a bit without causing it to
 * scroll.  The center parameter is mostly used for warps and
 * such, so that the players start in the center of the screen.
 *
 * \param   center Unused variable
 */
void calc_viewport (int center)
{
   int sx, sy, bl, br, bu, bd, zx, zy;

   (void) center;               // ML,2002.09.21: unused variable right now, casting to void to prevent warnings

   if (vfollow && numchrs > 0) {
      zx = g_ent[0].x;
      zy = g_ent[0].y;
   } else {
      zx = vx;
      zy = vy;
   }

#if 0
   if (center) {
#endif

      bl = 152;
      br = 152;
      bu = 112;
      bd = 112;
#if 0
   } else {
      bl = 64;
      br = 240;
      bu = 64;
      bd = 164;
   }
#endif
   sx = zx - vx;
   sy = zy - vy;
   if (sx < bl) {
      vx = zx - bl;

      if (vx < 0)
         vx = 0;
   }

   if (sy < bu) {
      vy = zy - bu;

      if (vy < 0)
         vy = 0;
   }

   if (sx > br) {
      vx = zx - br;

      if (vx > mx)
         vx = mx;
   }

   if (sy > bd) {
      vy = zy - bd;

      if (vy > my)
         vy = my;
   }

   if (vx > mx)
      vx = mx;
   if (vy > my)
      vy = my;
}



/*! \brief allocate memory for map
 *
 * Allocate memory arrays for the map, shadows, obstacles etc.
 * according to the size specified in g_map
 * \author  PH 20031010
 */
static void map_alloc (void)
{
   int tiles = g_map.xsize * g_map.ysize;
   free (map_seg);
   map_seg = (unsigned short *) malloc (tiles * sizeof (short));

   free (b_seg);
   b_seg = (unsigned short *) malloc (tiles * sizeof (short));

   free (f_seg);
   f_seg = (unsigned short *) malloc (tiles * sizeof (short));

   free (z_seg);
   z_seg = (unsigned char *) malloc (tiles);

   free (s_seg);
   s_seg = (unsigned char *) malloc (tiles);

   free (o_seg);
   o_seg = (unsigned char *) malloc (tiles);
}



/*! \brief Free old map data and load a new one
 *
 * This loads a new map and performs all of the functions
 * that accompany the loading of a new map.
 *
 * \param   map_name Base name of map (xxx -> maps/xxx.map)
 * \param   msx New x-coord for player. Pass 0 for msx and msy
 *              to use the 'default' position stored in the
 *              map file (s_map::stx and s_map::sty)
 * \param   msy New y-coord for player
 * \param   mvx New x-coord for camera. Pass 0 for mvx and mvy
 *              to use the default (also s_map::stx and
 *              s_map::sty)
 * \param   mvy New y-coord for camera
 */
void change_map (char *map_name, int msx, int msy, int mvx, int mvy)
{
   int i, o;
   PACKFILE *pf;
   DATAFILE *pb;
   BITMAP *pcxb;
   unsigned char cc[4];
   reset_timer_events ();
   if (hold_fade == 0)
      do_transition (TRANS_FADE_OUT, 4);

   sprintf (strbuf, "%s.map", map_name);

   pf = pack_fopen (kqres (MAP_DIR, strbuf), F_READ_PACKED);

   if (!pf) {
      clear_bitmap (screen);
      clear_bitmap (double_buffer);

      if (hold_fade == 0)
         do_transition (TRANS_FADE_IN, 16);

      g_map.xsize = -1;
      sprintf (strbuf, "Could not load map %s!", map_name);
      program_death (strbuf);
   }

   load_s_map (&g_map, pf);
   for (i = 0; i < 50; ++i)
      load_s_entity (&g_ent[PSIZE + i], pf);
   map_alloc ();
   for (i = 0; i < g_map.xsize * g_map.ysize; ++i)
      map_seg[i] = pack_igetw (pf);
   for (i = 0; i < g_map.xsize * g_map.ysize; ++i)
      b_seg[i] = pack_igetw (pf);
   for (i = 0; i < g_map.xsize * g_map.ysize; ++i)
      f_seg[i] = pack_igetw (pf);

   pack_fread (z_seg, (g_map.xsize * g_map.ysize), pf);
   pack_fread (s_seg, (g_map.xsize * g_map.ysize), pf);
   pack_fread (o_seg, (g_map.xsize * g_map.ysize), pf);
   pack_fclose (pf);

   /* PH fixme: cc[] were not initialised to zero */
   cc[0] = cc[1] = cc[2] = cc[3] = 0;

   for (i = 0; i < g_map.xsize * g_map.ysize; i++) {
      if (map_seg[i] > 0)
         cc[0] = 1;
      if (b_seg[i] > 0)
         cc[1] = 1;
      if (f_seg[i] > 0)
         cc[2] = 1;
      if (s_seg[i] > 0)
         cc[3] = 1;
   }

   draw_background = cc[0];
   draw_middle = cc[1];
   draw_foreground = cc[2];
   draw_shadow = cc[3];

   for (i = 0; i < numchrs; i++) {
      /* This allows us to either go to the map's default starting coords
       * or specify exactly where on the map to go to (like when there
       * are stairs or a doorway that they should start at).
       */
      if (msx == 0 && msy == 0)
         // Place players at default map starting coords
         place_ent (i, g_map.stx, g_map.sty);
      else
         // Place players at specific coordinates in the map
         place_ent (i, msx, msy);

      lastm[i] = 0;
      g_ent[i].speed = 4;
      g_ent[i].obsmode = 1;
      g_ent[i].moving = 0;
   }

   for (i = 0; i < MAX_ENT; i++) {
      if (g_ent[i].chrx == 38 && g_ent[i].active == 1) {
         g_ent[i].eid = ID_ENEMY;
         g_ent[i].speed = rand () % 4 + 1;
         g_ent[i].obsmode = 1;
         g_ent[i].moving = 0;
         g_ent[i].movemode = MM_CHASE;
         g_ent[i].chasing = 0;
         g_ent[i].extra = 50 + rand () % 50;
         g_ent[i].delay = rand () % 25 + 25;
      }
   }

   pb = load_datafile_object (PCX_DATAFILE, tilesets[g_map.tileset].icon_set);
   pcxb = (BITMAP *) pb->dat;

   for (o = 0; o < pcxb->h / 16; o++)
      for (i = 0; i < pcxb->w / 16; i++)
         blit ((BITMAP *) pb->dat, map_icons[o * (pcxb->w / 16) + i], i * 16,
               o * 16, 0, 0, 16, 16);

   unload_datafile_object (pb);

   for (o = 0; o < MAX_ANIM; o++)
      adelay[o] = 0;

   play_music (g_map.song_file, 0);
   mx = g_map.xsize * 16 - 304;
   /*PH fixme: was 224, drawmap() draws 16 rows, so should be 16*16=256 */
   my = g_map.ysize * 16 - 256;

   if (mvx == 0 && mvy == 0) {
      vx = g_map.stx * 16;
      vy = g_map.sty * 16;
   } else {
      vx = mvx * 16;
      vy = mvy * 16;
   }

   calc_viewport (1);

   for (i = 0; i < MAX_TILES; i++)
      tilex[i] = i;
   for (i = 0; i < MAX_ANIM; i++)
      adata[i] = tilesets[g_map.tileset].tanim[i];

   noe = 0;
   for (i = 0; i < numchrs; i++)
      g_ent[i].active = 1;

   count_entities ();

   for (i = 0; i < MAX_ENT; i++)
      g_ent[i].delayctr = 0;

   strcpy (curmap, map_name);
   set_view (0, 0, 0, 0, 0);

   if (strlen (g_map.map_desc) > 1)
      display_desc = 1;
   else
      display_desc = 0;

   do_luakill ();
   do_luainit (map_name);
   do_autoexec ();

   if (hold_fade == 0 && numchrs > 0) {
      drawmap ();
      blit2screen (xofs, yofs);
      do_transition (TRANS_FADE_IN, 4);
   }

   use_sstone = g_map.use_sstone;
   cansave = g_map.can_save;
   timer_count = 0;
   timer = 0;
   do_postexec ();
   timer_count = 0;
   timer = 0;
}



/*! \brief Zone event handler
 *
 * This routine is called after every final step onto
 * a new tile (not after warps or such things).  It
 * just checks if the zone value for this co-ordinate is
 * not zero and then it calls the event handler.  However,
 * there is a member of the map structure called zero_zone
 * that let's you call the event handler on 0 zones if you
 * wish.
 * This function also handles the Repulse functionality
 */
void zone_check (void)
{
   unsigned short stc, zx, zy;
   zx = g_ent[0].x / 16;
   zy = g_ent[0].y / 16;

   if (progress[P_REPULSE] > 0) {
      if (g_map.map_no == MAP_MAIN)
         progress[P_REPULSE]--;
      else {
         if (progress[P_REPULSE] > 1)
            progress[P_REPULSE] -= 2;
         else
            progress[P_REPULSE] = 0;
      }

      if (progress[P_REPULSE] < 1)
         message ("Repulse has worn off!", 255, 0, xofs, yofs);
   }

   stc = z_seg[zy * g_map.xsize + zx];

   if (g_map.zero_zone != 0)
      do_zone (stc);
   else {
      if (stc > 0)
         do_zone (stc);
   }
}



/*! \brief Move player(s) to new coordinates
 *
 * Fade out... change co-ordinates... fade in.
 * The wtx/wty co-ordinates indicate where to put the player.
 * The wvx/wvy co-ordinates indicate where to put the camera.
 *
 * \param   wtx New x-coord
 * \param   wty New y-coord
 * \param   fspeed Speed of fading (See do_transition())
 */
void warp (int wtx, int wty, int fspeed)
{
   int i, f;

   if (hold_fade == 0)
      do_transition (TRANS_FADE_OUT, fspeed);

   if (numchrs == 0)
      f = 1;
   else
      f = numchrs;

   for (i = 0; i < f; i++) {
      place_ent (i, wtx, wty);
      g_ent[i].moving = 0;
      g_ent[i].movcnt = 0;
      g_ent[i].framectr = 0;
      lastm[i] = 0;
   }

   vx = wtx * 16;
   vy = wty * 16;

   calc_viewport (1);
   drawmap ();
   blit2screen (xofs, yofs);

   if (hold_fade == 0)
      do_transition (TRANS_FADE_IN, fspeed);

   timer_count = 0;
}



/*! \brief Do tile animation
 *
 * This updates tile indexes for animation threads.
 */
void check_animation (void)
{
   int i, j;

   for (i = 0; i < MAX_ANIM; i++) {
      if (adata[i].start != 0) {
         if (adata[i].delay && adata[i].delay < adelay[i]) {
            adelay[i] = 0;

            for (j = adata[i].start; j <= adata[i].end; j++)
               if (tilex[j] < adata[i].end)
                  tilex[j]++;
               else
                  tilex[j] = adata[i].start;
         }

         adelay[i]++;
      }
   }
}



/*! \brief Alt key handler
 *
 * This function is called when the player presses the 'alt' key.
 * Things that can be activated are entities and zones that are
 * obstructed.
 */
void activate (void)
{
   int zx, zy, looking_at_x = 0, looking_at_y = 0, p, q,
      target_char_facing = 0, tf, mb;

   unpress ();

   /* Determine which direction the player's character is facing.  For
    * 'looking_at_y', a negative value means "toward north" or "facing up",
    * and a positive means that you are "facing down".  For 'looking_at_x',
    * negative means to face left and positive means to face right.
    */

   switch (g_ent[0].facing) {
   case FACE_DOWN:
      looking_at_y = 1;
      target_char_facing = FACE_UP;
      break;

   case FACE_UP:
      looking_at_y = -1;
      target_char_facing = FACE_DOWN;
      break;

   case FACE_LEFT:
      looking_at_x = -1;
      target_char_facing = FACE_RIGHT;
      break;

   case FACE_RIGHT:
      looking_at_x = 1;
      target_char_facing = FACE_LEFT;
      break;
   }

   zx = g_ent[0].x / 16;
   zy = g_ent[0].y / 16;

   looking_at_x += zx;
   looking_at_y += zy;

   q = z_seg[looking_at_y * g_map.xsize + looking_at_x];

   if (o_seg[looking_at_y * g_map.xsize + looking_at_x] != 0 && q > 0)
      do_zone (q);

   p = entityat (looking_at_x, looking_at_y, 0);

   if (p >= PSIZE) {
      tf = g_ent[p - 1].facing;

      if (g_ent[p - 1].facehero == 0)
         g_ent[p - 1].facing = target_char_facing;

      drawmap ();
      blit2screen (xofs, yofs);
      mb = g_map.map_no;

      zx = abs (g_ent[p - 1].x - g_ent[0].x);
      zy = abs (g_ent[p - 1].y - g_ent[0].y);

      if ((zx <= 16 && zy <= 3) || (zx <= 3 && zy <= 16))
         do_entity (p - 1);
      if (g_ent[p - 1].movemode == MM_STAND && g_map.map_no == mb)
         g_ent[p - 1].facing = tf;
   }
}



/*! \brief Wait for key release
 *
 * This is used to wait and make sure that the user has
 * released a key before moving on.
 * 20030728 PH re-implemented in IMHO a neater way
 *
 * \note Waits at most 20 'ticks'
 */
void unpress (void)
{
   timer_count = 0;
   while (timer_count < 20) {
      readcontrols ();
      if (!(balt || bctrl || benter || besc || up || down || right || left))
         break;
   }
   timer_count = 0;
}



/*! \brief Wait for ALT
 *
 * Simply wait for the 'alt' key to be pressed.
 */
void wait_enter (void)
{
   int stop = 0;

   unpress ();

   while (!stop) {
      readcontrols ();
      if (balt) {
         unpress ();
         stop = 1;
      }
      kq_yield ();
   }

   timer_count = 0;
}



/*! \brief Log events
 *
 * This is for logging events within the program.  Very
 * useful for debugging and tracing.
 * \note klog is deprecated; use Allegro's TRACE instead.
 *
 * \param   msg String to add to log file
 */
void klog (char *msg)
{
   TRACE ("%s\n", msg);
#if 0
   FILE *ff;

   ff = fopen ("game.log", "a");

   if (!ff)
      program_death ("Could not open log!");

   fprintf (ff, "%s\n", msg);
   fclose (ff);
#endif
}



/*! \brief Application start-up code
 *
 * Set up allegro, set up variables, load stuff, blah...
 * This is called once per game.
 */
static void startup (void)
{
   int p, i, q;
   time_t t;
   DATAFILE *pcxb;
   DATAFILE *pb;
   PACKFILE *pf;

   allegro_init ();

   /* Buffers to allocate */
   strbuf = (char *) malloc (4096);

   map_seg = b_seg = f_seg = NULL;
   s_seg = z_seg = o_seg = NULL;
   memset (&g_map, 0, sizeof (s_map));
   progress = (unsigned char *) malloc (2000);
   treasure = (unsigned char *) malloc (1000);

   allocate_stuff ();
   install_keyboard ();
   install_timer ();
   /* KQ uses digi sound but it doesn't use MIDI */
   //   reserve_voices (8, 0);
   sound_avail = install_sound (DIGI_AUTODETECT, MIDI_NONE, NULL) < 0 ? 0 : 1;
   if (!sound_avail)
      TRACE ("Error with sound: %s\n", allegro_error);
   parse_setup ();
   sound_init ();
   set_graphics_mode ();

   if (use_joy == 1)
      install_joystick (JOY_TYPE_AUTODETECT);

   if (num_joysticks == 0)
      use_joy = 0;
   else {
#if 0
      sprintf (strbuf, "%d joysticks detected.", num_joysticks);
      klog (strbuf);
      for (i = 0; i < num_joysticks; i++) {
         sprintf (strbuf, "joystick %d: %d sticks, %d buttons", i,
                  joy[i].num_sticks, joy[i].num_buttons);
         klog (strbuf);
      }
#endif
      use_joy = 0;

      if (poll_joystick () == 0) {
         for (i = num_joysticks - 1; i >= 0; i--)
            if (joy[i].num_buttons >= 4)
               use_joy = i + 1;
      }

      if (use_joy == 0) {
         klog ("Only joysticks/gamepads with at least 4 buttons can be used.");
         remove_joystick ();
      }
   }

   srand ((unsigned) time (&t));
   pcxb = load_datafile_object (PCX_DATAFILE, "MISC_PCX");

   if (!pcxb)
      program_death ("Could not load misc.pcx!");

   blit ((BITMAP *) pcxb->dat, menuptr, 24, 0, 0, 0, 16, 8);
   blit ((BITMAP *) pcxb->dat, sptr, 0, 0, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, mptr, 8, 0, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, upptr, 0, 8, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, dnptr, 8, 8, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, bptr, 24, 8, 0, 0, 16, 8);
   blit ((BITMAP *) pcxb->dat, noway, 64, 16, 0, 0, 16, 16);
   blit ((BITMAP *) pcxb->dat, missbmp, 0, 16, 0, 0, 20, 6);
   blit ((BITMAP *) pcxb->dat, b_shield, 0, 80, 0, 0, 48, 48);
   blit ((BITMAP *) pcxb->dat, b_shell, 48, 80, 0, 0, 48, 48);
   blit ((BITMAP *) pcxb->dat, b_repulse, 0, 64, 0, 0, 16, 16);
   blit ((BITMAP *) pcxb->dat, b_mp, 0, 24, 0, 0, 10, 8);
   blit ((BITMAP *) pcxb->dat, sfonts[0], 0, 128, 0, 0, 60, 8);

   for (i = 0; i < 8; i++) {
      for (p = 0; p < 60; p++) {
         if (sfonts[0]->line[i][p] == 15) {
            sfonts[1]->line[i][p] = 22;
            sfonts[2]->line[i][p] = 105;
            sfonts[3]->line[i][p] = 39;
            sfonts[4]->line[i][p] = 8;
         } else {
            sfonts[1]->line[i][p] = sfonts[0]->line[i][p];
            sfonts[2]->line[i][p] = sfonts[0]->line[i][p];
            sfonts[3]->line[i][p] = sfonts[0]->line[i][p];
            sfonts[4]->line[i][p] = sfonts[0]->line[i][p];
         }
      }
   }

   for (p = 0; p < 27; p++)
      blit ((BITMAP *) pcxb->dat, stspics, p * 8 + 40, 0, 0, p * 8, 8, 8);

   for (p = 0; p < 40; p++)
      blit ((BITMAP *) pcxb->dat, sicons, p * 8, 32, 0, p * 8, 8, 8);

   for (p = 0; p < 40; p++)
      blit ((BITMAP *) pcxb->dat, sicons, p * 8, 40, 0, p * 8 + 320, 8, 8);

   for (p = 0; p < MAX_SHADOWS; p++)
      blit ((BITMAP *) pcxb->dat, shadow[p], p * 16, 160, 0, 0, 16, 16);

   for (p = 0; p < 8; p++)
      blit ((BITMAP *) pcxb->dat, bub[p], p * 16, 144, 0, 0, 16, 16);

   for (p = 0; p < 3; p++) {
      blit ((BITMAP *) pcxb->dat, bord[p], p * 8 + 96, 64, 0, 0, 8, 8);
      blit ((BITMAP *) pcxb->dat, bord[5 + p], p * 8 + 96, 84, 0, 0, 8, 8);
   }

   blit ((BITMAP *) pcxb->dat, bord[3], 96, 72, 0, 0, 8, 12);
   blit ((BITMAP *) pcxb->dat, bord[4], 112, 72, 0, 0, 8, 12);

   for (i = 0; i < 9; i++)
      blit ((BITMAP *) pcxb->dat, pgb[i], i * 16, 48, 0, 0, 9, 9);

   unload_datafile_object (pcxb);
   load_heroes ();

   pb = load_datafile_object (PCX_DATAFILE, "ALLFONTS_PCX");
   blit ((BITMAP *) pb->dat, kfonts, 0, 0, 0, 0, 744, 60);
   unload_datafile_object (pb);

   pb = load_datafile_object (PCX_DATAFILE, "ENTITIES_PCX");
   for (q = 0; q < MAXE; q++)
      for (p = 0; p < MAXEFRAMES; p++)
         blit ((BITMAP *) pb->dat, eframes[q][p], p * 16, q * 16, 0, 0, 16,
               16);
   unload_datafile_object (pb);

   /* Initialize tilesets */
   pf = pack_fopen (kqres (DATA_DIR, "tileset.kq"), F_READ_PACKED);
   if (!pf) {
      program_death ("Could not load tileset.kq");
   }
   while (!pack_feof (pf)) {
      load_s_tileset (&tilesets[num_tilesets], pf);
      TRACE ("%d. %s\n", num_tilesets, tilesets[num_tilesets].icon_set);
      if ((++num_tilesets) >= MAX_TILESETS)
         program_death ("Too many tilesets defined in tileset.kq");
   }
   pack_fclose (pf);

   /* Initialise players */
   init_players ();

   LOCK_VARIABLE (timer);
   LOCK_VARIABLE (timer_count);
   LOCK_VARIABLE (ksec);
   LOCK_VARIABLE (kmin);
   LOCK_VARIABLE (khr);
   LOCK_FUNCTION (my_counter);
   LOCK_FUNCTION (time_counter);

   install_int_ex (my_counter, BPS_TO_TIMER (KQ_FPS));
   /* tick every minute */
   install_int_ex (time_counter, BPM_TO_TIMER (1));
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;
   load_sgstats ();

#ifdef DEBUGMODE
   /* TT: Create the mesh object to see 4-way obstacles (others ignored) */
   obj_mesh = create_bitmap (16, 16);
   clear (obj_mesh);
   for (q = 0; q < 16; q += 2) {
      for (p = 0; p < 16; p += 2)
         putpixel (obj_mesh, p, q, 255);
      for (p = 1; p < 16; p += 2)
         putpixel (obj_mesh, p, q + 1, 255);
   }
#endif
}



/*! \brief Load initial hero stuff from file
 *
 * \author PH
 * \date 20030320
 * Loads the hero stats from a file.
 *
 */
void load_heroes (void)
{
   PACKFILE *f;
   DATAFILE *pcxb;
   int i;
   /* Hero stats */
   if ((f = pack_fopen (kqres (DATA_DIR, "hero.kq"), F_READ_PACKED)) == NULL) {
      program_death ("Cannot open hero data file");
   }
   for (i = 0; i < MAXCHRS; ++i) {
      /*        pack_fread (&players[i].plr, sizeof (s_player), f); */
      load_s_player (&players[i].plr, f);
   }
   pack_fclose (f);
   /* portraits */
   pcxb = load_datafile_object (PCX_DATAFILE, "KQFACES_PCX");

   if (!pcxb)
      program_death ("Could not load kqfaces.pcx!");

   for (i = 0; i < 4; ++i) {
      blit ((BITMAP *) pcxb->dat, players[i].portrait, 0, i * 40, 0, 0, 40,
            40);
      blit ((BITMAP *) pcxb->dat, players[i + 4].portrait, 40, i * 40, 0, 0,
            40, 40);
   }

   unload_datafile_object (pcxb);
}



/*! \brief Initialise all players
 *
 * Set up the player characters and load data specific
 * to them. This happens at the start of every game.
 */
void init_players (void)
{
   DATAFILE *pb;
   int i, j;

   for (j = 0; j < MAXCHRS; j++) {
      for (i = 0; i < 24; i++)
         party[j].sts[i] = 0;

      for (i = 0; i < 6; i++)
         party[j].eqp[i] = 0;

      for (i = 0; i < 60; i++)
         party[j].spells[i] = 0;

      learn_new_spells (j);
   }

   gp = 0;

   pb = load_datafile_object (PCX_DATAFILE, "USCHRS_PCX");

   if (!pb)
      program_death ("Could not load character graphics!");

   set_palette (pal);

   for (i = 0; i < MAXCHRS; i++)
      for (j = 0; j < MAXFRAMES; j++)
         blit ((BITMAP *) pb->dat, frames[i][j], j * 16, i * 16, 0, 0, 16, 16);

   unload_datafile_object (pb);
}



#ifdef DEBUGMODE
/*! \brief Create bitmap
 *
 * This function allocates a bitmap and kills the
 * program if it fails. The name you supply is
 * shown if this happens.
 * \note PH is this really necessary?
 *
 * \param   bx Width
 * \param   by Height
 * \param   bname Name of bitmap
 * \returns the pointer to the created bitmap
 */
BITMAP *alloc_bmp (int bx, int by, char *bname)
{
   BITMAP *tmp;

   tmp = create_bitmap (bx, by);

   if (!tmp) {
      sprintf (strbuf, "Could not allocate %s!.", bname);
      program_death (strbuf);
   }

   return tmp;
}
#else
#define alloc_bmp(w, h, n) create_bitmap((w), (h))
#endif



/*! \brief Create bitmaps
 *
 * A separate function to create all global bitmaps needed in the game.
 */
static void allocate_stuff (void)
{
   int i, p;

   kfonts = alloc_bmp (744, 60, "kfonts");

   for (i = 0; i < 5; i++)
      sfonts[i] = alloc_bmp (60, 8, "sfonts[i]");

   menuptr = alloc_bmp (16, 8, "menuptr");
   sptr = alloc_bmp (8, 8, "sptr");
   mptr = alloc_bmp (8, 8, "mptr");
   stspics = alloc_bmp (8, 216, "stspics");
   sicons = alloc_bmp (8, 640, "sicons");
   bptr = alloc_bmp (16, 8, "bptr");
   upptr = alloc_bmp (8, 8, "upptr");
   dnptr = alloc_bmp (8, 8, "dnptr");
   noway = alloc_bmp (16, 16, "noway");
   missbmp = alloc_bmp (20, 6, "missbmp");

   for (i = 0; i < 9; i++)
      pgb[i] = alloc_bmp (9, 9, "pgb[x]");

   tc = alloc_bmp (16, 16, "tc");
   tc2 = alloc_bmp (16, 16, "tc2");
   b_shield = alloc_bmp (48, 48, "b_shield");
   b_shell = alloc_bmp (48, 48, "b_shell");
   b_repulse = alloc_bmp (16, 166, "b_repulse");
   b_mp = alloc_bmp (10, 8, "b_mp");

   for (p = 0; p < MAXE; p++) {
      for (i = 0; i < MAXEFRAMES; i++)
         eframes[p][i] = alloc_bmp (16, 16, "eframes[x][x]");
   }

   for (i = 0; i < MAXCHRS; i++) {
      for (p = 0; p < MAXFRAMES; p++)
         frames[i][p] = alloc_bmp (16, 16, "frames[x][x]");
   }

   for (p = 0; p < MAXCFRAMES; p++) {
      for (i = 0; i < NUM_FIGHTERS; i++) {
         cframes[i][p] = alloc_bmp (32, 32, "cframes[x][x]");
         tcframes[i][p] = alloc_bmp (32, 32, "tcframes[x][x]");
      }
   }

   double_buffer = alloc_bmp (352, 280, "double_buffer");
   back = alloc_bmp (352, 280, "back");
   fx_buffer = alloc_bmp (352, 280, "fx_buffer");

   for (p = 0; p < MAX_SHADOWS; p++)
      shadow[p] = alloc_bmp (16, 16, "shadow[x]");

   for (p = 0; p < 8; p++)
      bub[p] = alloc_bmp (16, 16, "bub[x]");

   for (p = 0; p < 3; p++) {
      bord[p] = alloc_bmp (8, 8, "bord[x]");
      bord[p + 5] = alloc_bmp (8, 8, "bord[x]");
   }

   for (p = 3; p < 5; p++)
      bord[p] = alloc_bmp (8, 12, "bord[x]");

   for (p = 0; p < 8; p++)
      players[p].portrait = alloc_bmp (40, 40, "portrait[x]");

   for (p = 0; p < MAX_TILES; p++)
      map_icons[p] = alloc_bmp (16, 16, "map_icons[x]");
   allocate_credits ();
}



/*! \brief Free allocated memory
 *
 * This frees memory and such things.
 */
static void deallocate_stuff (void)
{
   int i, p;

   destroy_bitmap (kfonts);

   for (i = 0; i < 5; i++)
      destroy_bitmap (sfonts[i]);

   destroy_bitmap (menuptr);
   destroy_bitmap (sptr);
   destroy_bitmap (mptr);
   destroy_bitmap (upptr);
   destroy_bitmap (dnptr);
   destroy_bitmap (stspics);
   destroy_bitmap (sicons);
   destroy_bitmap (bptr);
   destroy_bitmap (noway);
   destroy_bitmap (missbmp);

   for (i = 0; i < 9; i++)
      destroy_bitmap (pgb[i]);

   destroy_bitmap (tc);
   destroy_bitmap (tc2);
   destroy_bitmap (b_shield);
   destroy_bitmap (b_shell);
   destroy_bitmap (b_repulse);
   destroy_bitmap (b_mp);

   for (p = 0; p < MAXE; p++)
      for (i = 0; i < MAXEFRAMES; i++)
         destroy_bitmap (eframes[p][i]);

   for (i = 0; i < MAXFRAMES; i++)
      for (p = 0; p < MAXCHRS; p++)
         destroy_bitmap (frames[p][i]);

   for (i = 0; i < MAXCFRAMES; i++) {
      for (p = 0; p < NUM_FIGHTERS; p++) {
         destroy_bitmap (cframes[p][i]);
         destroy_bitmap (tcframes[p][i]);
      }
   }

   destroy_bitmap (double_buffer);
   destroy_bitmap (back);
   destroy_bitmap (fx_buffer);

   for (p = 0; p < MAX_SHADOWS; p++)
      destroy_bitmap (shadow[p]);

   for (p = 0; p < 8; p++)
      destroy_bitmap (bub[p]);

   for (p = 0; p < 8; p++)
      destroy_bitmap (bord[p]);

   for (p = 0; p < MAXCHRS; p++)
      destroy_bitmap (players[p].portrait);

   for (p = 0; p < MAX_TILES; p++)
      destroy_bitmap (map_icons[p]);

   if (map_seg)
      free (map_seg);
   if (b_seg)
      free (b_seg);
   if (f_seg)
      free (f_seg);
   if (z_seg)
      free (z_seg);
   if (s_seg)
      free (s_seg);
   if (o_seg)
      free (o_seg);
   if (progress)
      free (progress);
   if (treasure)
      free (treasure);
   if (strbuf)
      free (strbuf);
/*    if (savedir) */
/*       free (savedir); */

   if (is_sound) {
      shutdown_music ();
      free_samples ();
   }
   deallocate_credits ();

#ifdef DEBUGMODE
   destroy_bitmap (obj_mesh);
#endif
}



/*! \brief Pause for a time
 *
 * Why not just use rest() you ask?  Well, this function
 * kills time, but it also processes entities.  This function
 * is basically used to run entity scripts and for automatic
 * party movement.
 *
 * \param   dtime Time in frames
 */
void kwait (int dtime)
{
   int cnt = 0;

   autoparty = 1;
   timer_count = 0;

   while (cnt < dtime) {
      poll_music ();
      while (timer_count > 0) {
         poll_music ();
         timer_count--;
         cnt++;
         process_entities ();
         check_animation ();
      }

      drawmap ();
      blit2screen (xofs, yofs);
#ifdef DEBUGMODE
      if (debugging > 0) {
         if (key[KEY_W] && key[KEY_ALT]) {
            klog ("Alt+W Pressed:");
            sprintf (strbuf, "\tkwait(); cnt = %d, dtime = %d, timer_count = %d",
                     cnt, dtime, timer_count);
            klog (strbuf);
            break;
         }
      }
#endif
      if (key[KEY_X] && key[KEY_ALT]) {
         if (debugging > 0) {
            sprintf (strbuf, "kwait(); cnt = %d, dtime = %d, timer_count = %d",
                     cnt, dtime, timer_count);
         } else {
            sprintf (strbuf, "Program terminated: user pressed Alt+X");
         }
         program_death (strbuf);
      }
   }

   timer_count = 0;
   autoparty = 0;
}



/*! \brief Wait for scripted movement to finish
 *
 * This does like wait() and processes entities...
 * however, this function waits for particular entities
 * to finish scripted movement rather than waiting for
 * a specific amount of time to pass.
 * Specify a range of entities to wait for.
 * \note 20030810 PH implemented this in a neater way, need to check if it always works though.
 *
 * \param   est First entity
 * \param   efi Last entity
 */
void wait_for_entity (int est, int efi)
{
   int e, n, m;
   autoparty = 1;
   do {
      while (timer_count > 0) {
         timer_count--;
         process_entities ();
         check_animation ();
      }
      poll_music ();
      drawmap ();
      blit2screen (xofs, yofs);

      if (key[KEY_W] && key[KEY_ALT])
         break;

      if (key[KEY_X] && key[KEY_ALT])
         program_death ("X-Alt pressed - exiting");

      n = 0;
      for (e = est; e <= efi; ++e) {
         m = g_ent[e].movemode;
         if (g_ent[e].active == 1 && (m == MM_SCRIPT || m == MM_TARGET)) {
            n = 1;
            break;
         }
      }
      kq_yield ();
   }
   while (n);
   autoparty = 0;
}



/*! \brief End program due to fatal error
 *
 * Kill the program and spit out a message.
 *
 * \param   message Text to put into log
 */
void program_death (char *message)
{
   char internal_buffer[80];
   strcat (strncpy (internal_buffer, message, sizeof (internal_buffer) - 1),
           "\n");
   TRACE ("%s\n", message);
   deallocate_stuff ();
   allegro_message (internal_buffer);
   set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
   exit (EXIT_FAILURE);
}



/*! \brief Is this character in the party?
 *
 * Determine if a given character is currently in play.
 *
 * \param   pn Character to ask about
 * \returns where it is in the party list (1 or 2), or 0 if not
 */
int in_party (int pn)
{
   int a;

   for (a = 0; a < MAXCHRS; a++)
      if (pidx[a] == pn)
         return a + 1;

   return 0;
}



/*! \brief Main function
 *
 * Well, this one is pretty obvious.
 */
int main (void)
{
   int stop, game_on, skip_splash;
   startup ();
   game_on = 1;
   /* Also this can be overridden by settings in config */
   skip_splash = 0;
   while (game_on) {
      switch (start_menu (skip_splash)) {
      case 0:
         break;
      case 1:
         change_map ("starting", 0, 0, 0, 0);
         break;
      default:
         /* Someone pressed 'EXIT'  */
         game_on = 0;
         break;
      }
      /* Only show it once at the start */
      skip_splash = 1;
      if (game_on) {
         stop = 0;
         timer_count = 0;
         timer = 0;
         alldead = 0;
         while (!stop) {
            if (timer_count < 1) {
               check_animation ();
               drawmap ();
               blit2screen (xofs, yofs);
               while (timer_count < 1) {
                  kq_yield ();
               }
            }
            timer_count--;
            poll_music ();
            process_entities ();

            frate++;

            if (key[kesc]) {
               stop = system_menu ();
            }
            if (bhelp) {
               /* TODO: In-game help system. */
            }


            if (alldead) {
               clear (screen);
               do_transition (TRANS_FADE_IN, 16);
               stop = 1;
            }

         }
      }
   }
   remove_int (my_counter);
   remove_int (time_counter);
   deallocate_stuff ();
   return EXIT_SUCCESS;
}

END_OF_MAIN ();

/*! \brief Timer Event structure
 *
 * Holds the information relating to a forthcoming event
 */
static struct timer_event
{
   char name[32];               /*!< Name of the event */
   int when;                    /*!< Time when it will trigger */
} timer_events[5];

static int next_event_time;     /*!< The time the next event will trigger */


/*! \brief Delete any pending events
*
* This removes any events from the list
*/
void reset_timer_events (void)
{
   int i;
   for (i = 0; i < 5; ++i)
      *timer_events[i].name = '\0';
   next_event_time = INT_MAX;
}


/* \brief Add a new timer event to the list
 *
 * Up to five pending events can be stored
 * at once.
 * \param n the name of the event
 * \param delta the number of seconds before the
 *        event will be called. For example 5 means
 *        five seconds in the future
 * \returns <0 if an error occurred (i.e. too many pending events)
 */
int add_timer_event (const char *n, int delta)
{
   int w = delta + ksec;
   int i;
   for (i = 0; i < 5; ++i) {
      if (*timer_events[i].name == '\0') {
         memcpy (timer_events[i].name, n, sizeof (timer_events[i].name));
         if (w < next_event_time)
            next_event_time = w;
         timer_events[i].when = w;
         return i;
      }
   }
   return -1;
}


/* \brief Get the next event if any
 *
 * Checks the pending events and returns the name
 * of the next one, or NULL if nothing is ready
 * to be triggered.
 * If more than one event is ready, only one will be returned;
 * the next one will be returned next time.
 * Each event is removed after it is triggered. If a repeating
 * event is desired, you should call add_timer_event() again
 * in the handler function.
 *
 * \returns name of the next event or NULL if none is ready
 */
char *get_timer_event (void)
{
   static char buf[32];
   int now = ksec;
   int i;
   int next = INT_MAX;
   struct timer_event *t;

   if (now < next_event_time)
      return NULL;

   *buf = '\0';
   for (i = 0; i < 5; ++i) {
      t = &timer_events[i];
      if (*t->name) {
         if (t->when <= now) {
            memcpy (buf, t->name, sizeof (buf));
            *t->name = '\0';
         } else {
            if (t->when < next)
               next = t->when;
         }
      }
   }
   next_event_time = next;
   return *buf ? buf : NULL;
}


/*! \brief Yield processor for other tasks
 *
 * This function calls rest() or yield_cpu() as appropriate for
 * the platform and allegro version
 *
 * \author PH
 * \date 20050423
 */
void kq_yield (void)
{
#if (ALLEGRO_VERSION >= 4 && ALLEGRO_SUB_VERSION >= 2)
   rest (1);
#else
   yield_timeslice ();
#endif
}


/*! \page treasure A Note on Treasure
 *
 * The treasure chests are allocated in this way:
 * - 0: town1
 * - 1..2: cave1
 * - 3..5: town2
 * - 6: town1
 * - 7: town2
 * - 8: bridge
 * - 9..11: town2
 * - 12..14: town3
 * - 15: grotto
 * - 16: cave2
 * - 17..19: cave3a
 * - 20: cave3b
 * - 21..30: temple2
 * - 31: town2
 * - 32: town5
 * - 33..44: tower
 * - 45: town1
 * - 46: town2
 * - 47: guild
 * - 48..49: grotto2
 * - 50: guild
 * - 51..53: town4
 * - 54: estate
 * - 55..61: camp
 * - 62..66: cave4
 * - 67..69: town6
 * - 70: town6
 * - 71..72: town7
 * - 73..74: pass
 * - 75..79: cult
 * - 80: grotto
 * - 81: town4
 * - 82..83: pass
 * - 84..89: free
 * - 90..96: cave5
 *
 * The names given are the base names of the maps/lua scripts
 */
