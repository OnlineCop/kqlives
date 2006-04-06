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
 * \brief Character and Map drawing
 * \author JB
 * \date ????????
 *
 * Includes functions to draw characters, text and maps.
 * Also some colour manipulation.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "setup.h"
#include "kq.h"
#include "music.h"
#include "draw.h"
#include "combat.h"
#include "res.h"
#include "timing.h"
#include "entity.h"
#include "progress.h"

/* Globals */
#define MSG_ROWS 4
#define MSG_COLS 36
char msgbuf[MSG_ROWS][MSG_COLS];
int gbx, gby, gbbx, gbby, gbt, gbbw, gbbh, gbbs;
unsigned char BLUE = 2, DARKBLUE = 0, DARKRED = 4;



/*  internal prototypes  */
static void drawchar (int, int);
static void draw_backlayer (void);
static void draw_midlayer (void);
static void draw_forelayer (void);
static void draw_shadows (void);
static void border (BITMAP *, int, int, int, int);
static void set_textpos (int);
static void draw_textbox (int);
static void generic_text (int, int);
const char *parse_string (const char *);



/*! \brief Blit from double buffer to the screen
 *
 * This does the copy from the double_buffer to the screen... for the
 * longest time I called blit in every location (over 80 places) instead
 * of having a central function... am I a moron or what?
 * Handles frame-rate display, stretching and vsync waiting.
 *
 * \param   xw x-coord in double_buffer of the top-left of the screen
 * \param   yw y-coord in double_buffer of the top-left of the screen
 */
extern int skips;
void blit2screen (int xw, int yw)
{
   static int frate;
   if (show_frate == 1) {
      sprintf (strbuf, "%d %d", frate, skips);
      print_font (double_buffer, xofs, yofs, strbuf, FNORMAL);
   }
   if (stretch_view == 1)
      stretch_blit (double_buffer, screen, xw, yw, 320, 240, 0, 0, 640, 480);
   else
      blit (double_buffer, screen, xw, yw, 0, 0, 320, 240);
   frate = limit_frame_rate(25);
}



/*! \brief Make a copy of a bitmap
 *
 * Take a source bitmap and a target. If the target is NULL
 * or too small, re-allocate it. 
 * Then blit it. 
 *
 * \param   target Bitmap to copy to or NULL
 * \param   source Bitmap to copy from
 * \returns target or a new bitmap.
 */
BITMAP *copy_bitmap (BITMAP * target, BITMAP * source)
{
   if (target) {
      if (target->w < source->w || target->h < source->h) {
         /* too small */
         destroy_bitmap (target);
         target = create_bitmap (source->w, source->h);
      }
   } else {
      /* create new */
      target = create_bitmap (source->w, source->h);
   }
   /* ...and copy */
   blit (source, target, 0, 0, 0, 0, source->w, source->h);
   return target;
}



/*! \brief Scale colours
 *
 * This takes a bitmap and scales it to fit in the color range specified.
 * Output goes to a new bitmap.
 * This is used to make a monochrome version of a bitmap, for example to
 * display a green, poisoned character, or the red 'rage' effect for
 * Sensar. This relies on the palette having continuous lightness ranges
 * of one colour (as the KQ palette does!).
 * An alternative would be to use makecol(), though this would incur
 * a speed penalty.
 * Another alternative would be to precalculate some maps for each case.
 *
 * \param   src Source bitmap
 * \param   dest Destination bitmap
 * \param   st Start of output color range
 * \param   fn End of output color range
 */
void color_scale (BITMAP * src, BITMAP * dest, int st, int fn)
{
   int ix, iy, z, a;

   clear_bitmap (dest);
   for (iy = 0; iy < dest->h; iy++) {
      for (ix = 0; ix < dest->w; ix++) {
         a = src->line[iy][ix];
         if (a > 0) {
            z = pal[a].r;
            z += pal[a].g;
            z += pal[a].b;
            z = z * (fn - st) / 192;
            dest->line[iy][ix] = st + z;
         }
      }
   }
}



/*! \brief Convert multiple frames
 *
 * This is used to color_scale one or more fighter frames.
 *
 * \param   who Character to convert
 * \param   st Start of output range
 * \param   fn End of output range
 * \param   aflag If ==1 then \p who<PSIZE means convert all heroes, otherwise all enemies
 */
void convert_cframes (int who, int st, int fn, int aflag)
{
   int a, p, a1;
   if (aflag == 1) {
      if (who < PSIZE) {
         a = 0;
         a1 = numchrs;
      } else {
         a = PSIZE;
         a1 = PSIZE + numens;
      }
   } else {
      a = who;
      a1 = who + 1;
   }

   while (a < a1) {
      for (p = 0; p < MAXCFRAMES; p++) {
         color_scale (tcframes[a][p], cframes[a][p], st, fn);
      }
      ++a;
   }
}



/*! \brief Restore colours
 *
 * Restore specified fighter frames to normal color. This is done
 * by blitting the 'master copy' from tcframes.
 *
 * \param   who Character to restore
 * \param   aflag If ==1 then convert all heroes if \p who < PSIZE, otherwise convert all enemies
 */
void revert_cframes (int who, int aflag)
{
   int a, p;
   int a1;
   /* Determine the range of frames to revert */
   if (aflag == 1) {
      if (who < PSIZE) {
         a = 0;
         a1 = numchrs;
      } else {
         a = PSIZE;
         a1 = PSIZE + numens;
      }
   } else {
      a = who;
      a1 = who + 1;
   }

   while (a < a1) {
      for (p = 0; p < MAXCFRAMES; p++) {
         blit (tcframes[a][p], cframes[a][p], 0, 0, 0, 0, fighter[a].cw,
               fighter[a].cl);
      }
      ++a;
   }
}



/*! \brief Draw small icon
 *
 * Just a helper function... reduces the number of places that 'sicons'
 * has to be referenced.
 * Icons are 8x8 sub-bitmaps of sicons, representing items (sword, etc.)
 *
 * \param   where Bitmap to draw to
 * \param   ino Icon to draw
 * \param   icx x-coord
 * \param   icy y-coord
 */
void draw_icon (BITMAP * where, int ino, int icx, int icy)
{
   masked_blit (sicons, where, 0, ino * 8, icx, icy, 8, 8);
}



/*! \brief Draw status icon
 *
 * Just a helper function... reduces the number of places that 'stspics'
 * has to be referenced.
 * Status icons are 8x8 sub-bitmaps of \p stspics, representing poisoned, etc.
 *
 * \param   where Bitmap to draw to
 * \param   cc Non-zero if in combat mode (draw
 *          using info  \p fighter[] rather than \p party[] )
 * \param   who Character to draw status for
 * \param   inum The maximum number of status icons to draw.
 *          \p inum ==17 when in combat, ==8 otherwise.
 * \param   icx x-coord to draw to
 * \param   icy y-coord to draw to
 */
void draw_stsicon (BITMAP * where, int cc, int who, int inum, int icx, int icy)
{
   int j, st = 0, s;

   for (j = 0; j < inum; j++) {
      if (cc == 0)
         s = party[who].sts[j];
      else
         s = fighter[who].sts[j];
      if (s != 0) {
         masked_blit (stspics, where, 0, j * 8 + 8, st * 8 + icx, icy, 8, 8);
         st++;
      }
   }
   if (st == 0)
      masked_blit (stspics, where, 0, 0, icx, icy, 8, 8);
}



/*! \brief Draw heroes on map
 *
 * Draw the heroes on the map.  It's kind of clunky, but this is also where
 * it takes care of walking in forests and only showing a disembodied head.
 * Does not seem to do any parallaxing. (?)
 * PH modified 20030309 Simplified this a bit, removed one blit() that wasn't neeeded.
 *
 * \param   xw x-offset - always ==16
 * \param   yw y-offset - always ==16
 */
static void drawchar (int xw, int yw)
{
   int fr, dx, dy, i, f, fid;
   BITMAP **sprite_base;
   int spec = 0;
   BITMAP *spr = NULL;

   for (i = PSIZE + noe - 1; i >= 0; i--) {
      spec = 0;
      fid = g_ent[i].eid;
      dx = g_ent[i].x - vx + xw;
      dy = g_ent[i].y - vy + yw;
      if (!g_ent[i].moving)
         fr = g_ent[i].facing * 3 + 2;
      else {
         fr = g_ent[i].facing * 3 + (g_ent[i].framectr > 10 ? 1 : 0);
      }
      if (i < PSIZE && i < numchrs) {
         /* It's a hero */
         /* Masquerade: if chrx!=0 then this hero is disguised as someone else... */
         sprite_base = g_ent[i].chrx ? eframes[g_ent[i].chrx] : frames[fid];

         if (party[fid].sts[S_DEAD] != 0)
            fr = g_ent[i].facing * 3 + 2;
         if (party[fid].sts[S_POISON] != 0) {
            /* PH: we are calling this every frame? */
            color_scale (sprite_base[fr], tc2, 32, 47);
            spr = tc2;
         } else {
            spr = sprite_base[fr];
         }
         if (is_forestsquare (g_ent[i].tilex, g_ent[i].tiley)) {
            f = !g_ent[i].moving;
            if (g_ent[i].moving && is_forestsquare (g_ent[i].x / 16, g_ent[i].y / 16))
               f = 1;
            if (f) {
               clear_to_color (tc, 0);
               blit (spr, tc, 0, 0, 0, 0, 16, 6);
               spr = tc;
            }
         }
         if (party[fid].sts[S_DEAD] == 0)
            draw_sprite (double_buffer, spr, dx, dy);
         else
            draw_trans_sprite (double_buffer, spr, dx, dy);

      } else {
         /* It's an NPC */
         if (g_ent[i].active && g_ent[i].tilex >= view_x1
             && g_ent[i].tilex <= view_x2 && g_ent[i].tiley >= view_y1
             && g_ent[i].tiley <= view_y2) {
            if (dx >= -16 && dx <= 336 && dy >= -16 && dy <= 256) {
// TT: EDIT
               spr =
                  (g_ent[i].eid >=
                   ID_ENEMY) ? eframes[g_ent[i].chrx][fr] : frames[g_ent[i].
                                                                   eid][fr];

               if (g_ent[i].transl == 0)
                  draw_sprite (double_buffer, spr, dx, dy);
               else
                  draw_trans_sprite (double_buffer, spr, dx, dy);
            }
         }
      }
   }
}



/*! \brief Check for forest square
 *
 * Helper function for the drawchar routine.  Just returns whether or not
 * the tile at the specified co-ordinates is a forest tile.  This could be
 * a headache if the tileset changes!
 * Looks in the \p map_seg[] array
 * PH modified 20030309 added check for map (only main map has forest)
 *
 * \param   fx x-coord to check
 * \param   fy y-coord to check
 * \returns 1 if it is a forest square, 0 otherwise
 */
int is_forestsquare (int fx, int fy)
{
   int f;
   if (g_map.map_no != MAP_MAIN)
      return 0;
   f = map_seg[(fy * g_map.xsize) + fx];
// TT: EDIT
   switch (f) {
   case 63:
   case 65:
   case 66:
   case 67:
   case 71:
   case 72:
   case 73:
   case 74:
      return 1;
   default:
      return 0;
   }
}



/*! \brief Draw the map
 *
 * Umm... yeah.
 * Draws the background, character, middle, foreground and shadow layers.
 * The order, and the parallaxing, is specified by the mode.
 * There are 6 modes, as set in the .map file
 *  - 0 Order BMCFS,
 *  - 1 Order BCMFS,
 *  - 2 Order BMCFS, Background parallax
 *  - 3 Order BCMFS, Background & middle parallax
 *  - 4 Order BMCFS, Middle & foreground parallax
 *  - 5 Order BCMFS, Foreground parallax
 * 
 * In current KQ maps, only modes 0..2 are used, with the majority being 0.
 * Also handles the Repulse indicator and the map description display.
 * \bug PH: Shadows are never drawn with parallax (is this a bug?)
 */
void drawmap (void)
{
   if (g_map.xsize <= 0) {
      clear_to_color (double_buffer, 1);
      return;
   }
   clear_bitmap (double_buffer);
   if (draw_background)
      draw_backlayer ();
   if (g_map.map_mode == 1 || g_map.map_mode == 3 || g_map.map_mode == 5)
      drawchar (16, 16);
   if (draw_middle)
      draw_midlayer ();
   if (g_map.map_mode == 0 || g_map.map_mode == 2 || g_map.map_mode == 4)
      drawchar (16, 16);
   if (draw_foreground)
      draw_forelayer ();
   draw_shadows ();
/*
   This is an obvious hack here.  When I first started, xofs and yofs could
   have values of anywhere between 0 and 15.  Therefore, I had to use these
   offsets any time I drew to the double_buffer.  However, when I put in the
   parallaxing code, that was no longer true.  So, instead of changing all
   my code, I just put this hack in place.  It's actually kind of handy in
   case I ever have to adjust stuff again.
 */
   xofs = 16;
   yofs = 16;
   if (progress[P_REPULSE] > 0) {
      rectfill (b_repulse, 0, 16, 15, 165, 0);
      rectfill (b_repulse, 5, 16, 10, 16 + progress[P_REPULSE], 15);
      draw_trans_sprite (double_buffer, b_repulse, 2 + xofs, 2 + yofs);
   }
   if (display_desc == 1) {
      menubox (double_buffer, 152 - (strlen (g_map.map_desc) * 4) + xofs,
               8 + yofs, strlen (g_map.map_desc), 1, BLUE);
      print_font (double_buffer, 160 - (strlen (g_map.map_desc) * 4) + xofs,
                  16 + yofs, g_map.map_desc, FNORMAL);
   }
}



/*! \brief Draw background
 *
 * Draw the background layer.  Accounts for parallaxing.
 * Parallax is on for modes 2 & 3
 */
static void draw_backlayer (void)
{
   int dx, dy, pix, xtc, ytc;
   int v_x1, v_x2, v_y1, v_y2;

   if (view_on == 0) {
      view_y1 = 0;
      view_y2 = g_map.ysize - 1;
      view_x1 = 0;
      view_x2 = g_map.xsize - 1;
   }
   if (g_map.map_mode < 2 || g_map.map_mode > 3) {
      xtc = vx >> 4;
      ytc = vy >> 4;
      dx = vx;
      dy = vy;
      v_x1 = view_x1;
      v_y1 = view_y1;
      v_x2 = view_x2;
      v_y2 = view_y2;
   } else {
      dx = vx * g_map.pmult / g_map.pdiv;
      dy = vy * g_map.pmult / g_map.pdiv;
      xtc = dx >> 4;
      ytc = dy >> 4;
      v_x1 = view_x1 * g_map.pmult / g_map.pdiv;
      v_y1 = view_y1 * g_map.pmult / g_map.pdiv;
      v_x2 = view_x2 * g_map.pmult / g_map.pdiv;
      v_y2 = view_y2 * g_map.pmult / g_map.pdiv;
   }
   xofs = 16 - (dx & 15);
   yofs = 16 - (dy & 15);
   for (dy = 0; dy < 16; dy++) {
      /* TT Parallax problem here #1 */
      if (ytc + dy >= v_y1 && ytc + dy <= v_y2) {
         for (dx = 0; dx < 21; dx++) {
            /* TT Parallax problem here #2 */
            if (xtc + dx >= v_x1 && xtc + dx <= v_x2) {
               pix = map_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
               blit (map_icons[tilex[pix]], double_buffer, 0, 0,
                     dx * 16 + xofs, dy * 16 + yofs, 16, 16);

            }
            /*
               else
               blit (map_icons[0], double_buffer, 0, 0, dx * 16 + xofs,
               dy * 16 + yofs, 16, 16);
             */
         }
      }
   }
}



/*! \brief Draw middle layer
 *
 * Draw the middle layer.  Accounts for parallaxing.
 * Parallax is on for modes 3 & 4
 */
static void draw_midlayer (void)
{
   int dx, dy, pix, xtc, ytc;
   int v_x1, v_x2, v_y1, v_y2;

   if (view_on == 0) {
      view_y1 = 0;
      view_y2 = g_map.ysize - 1;
      view_x1 = 0;
      view_x2 = g_map.xsize - 1;
   }
   if (g_map.map_mode < 3 || g_map.map_mode == 5) {
      xtc = vx >> 4;
      ytc = vy >> 4;
      dx = vx;
      dy = vy;
      v_x1 = view_x1;
      v_y1 = view_y1;
      v_x2 = view_x2;
      v_y2 = view_y2;
   } else {
      dx = vx * g_map.pmult / g_map.pdiv;
      dy = vy * g_map.pmult / g_map.pdiv;
      xtc = dx >> 4;
      ytc = dy >> 4;
      v_x1 = view_x1 * g_map.pmult / g_map.pdiv;
      v_y1 = view_y1 * g_map.pmult / g_map.pdiv;
      v_x2 = view_x2 * g_map.pmult / g_map.pdiv;
      v_y2 = view_y2 * g_map.pmult / g_map.pdiv;
   }
   xofs = 16 - (dx & 15);
   yofs = 16 - (dy & 15);
   for (dy = 0; dy < 16; dy++) {
      if (ytc + dy >= v_y1 && ytc + dy <= v_y2) {
         for (dx = 0; dx < 21; dx++) {
            if (xtc + dx >= v_x1 && xtc + dx <= v_x2) {
               pix = b_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
               draw_sprite (double_buffer, map_icons[tilex[pix]],
                            dx * 16 + xofs, dy * 16 + yofs);
            }
         }
      }
   }
}



/*! \brief Draw foreground
 *
 * Draw the foreground layer.  Accounts for parallaxing.
 * Parallax is on for modes 4 & 5.
 */
static void draw_forelayer (void)
{
   int dx, dy, pix, xtc, ytc;
   int v_x1, v_x2, v_y1, v_y2;

   if (view_on == 0) {
      view_y1 = 0;
      view_y2 = g_map.ysize - 1;
      view_x1 = 0;
      view_x2 = g_map.xsize - 1;
   }
   if (g_map.map_mode < 4) {
      xtc = vx >> 4;
      ytc = vy >> 4;
      dx = vx;
      dy = vy;
      v_x1 = view_x1;
      v_y1 = view_y1;
      v_x2 = view_x2;
      v_y2 = view_y2;
   } else {
      dx = vx * g_map.pmult / g_map.pdiv;
      dy = vy * g_map.pmult / g_map.pdiv;
      xtc = dx >> 4;
      ytc = dy >> 4;
      v_x1 = view_x1 * g_map.pmult / g_map.pdiv;
      v_y1 = view_y1 * g_map.pmult / g_map.pdiv;
      v_x2 = view_x2 * g_map.pmult / g_map.pdiv;
      v_y2 = view_y2 * g_map.pmult / g_map.pdiv;
   }
   xofs = 16 - (dx & 15);
   yofs = 16 - (dy & 15);
   for (dy = 0; dy < 16; dy++) {
      if (ytc + dy >= v_y1 && ytc + dy <= v_y2) {
         for (dx = 0; dx < 21; dx++) {
            if (xtc + dx >= v_x1 && xtc + dx <= v_x2) {
               // Used in several places in this loop, so shortened the name
               int here = ((ytc + dy) * g_map.xsize) + xtc + dx;
               pix = f_seg[here];
               draw_sprite (double_buffer, map_icons[tilex[pix]],
                            dx * 16 + xofs, dy * 16 + yofs);
#ifdef DEBUGMODE
               if (debugging > 3) {
                  // Obstacles
                  if (o_seg[here] == 1)
                     draw_sprite (double_buffer, obj_mesh, dx * 16 + xofs,
                                  dy * 16 + yofs);

                  // Zones
#if (ALLEGRO_VERSION >= 4 && ALLEGRO_SUB_VERSION >= 1)
                  if (z_seg[here] == 0) {
                     // Do nothing
                  } else if (z_seg[here] < 10) {
                     /* The zone's number is single-digit, center vert+horiz */
                     textprintf_ex (double_buffer, font, dx * 16 + 4 + xofs,
                                    dy * 16 + 4 + yofs,
                                    makecol (255, 255, 255), 0, "%d",
                                    z_seg[here]);
                  } else if (z_seg[here] < 100) {
                     /* The zone's number is double-digit, center only vert */
                     textprintf_ex (double_buffer, font, dx * 16 + xofs,
                                    dy * 16 + 4 + yofs,
                                    makecol (255, 255, 255), 0, "%d",
                                    z_seg[here]);
                  } else if (z_seg[here] < 10) {
                     /* The zone's number is triple-digit.  Print the 100's
                      * digit in top-center of the square; the 10's and 1's
                      * digits on bottom of the square
                      */
                     textprintf_ex (double_buffer, font, dx * 16 + 4 + xofs,
                                    dy * 16 + yofs, makecol (255, 255, 255), 0,
                                    "%d", (int) (z_seg[here] / 100));
                     textprintf_ex (double_buffer, font, dx * 16 + xofs,
                                    dy * 16 + 8 + yofs,
                                    makecol (255, 255, 255), 0,
                                    "%02d", (int) (z_seg[here] % 100));
                  }
#else
                  if (z_seg[here] == 0) {
                     // Do nothing
                  } else if (z_seg[here] < 10) {
                     /* The zone's number is single-digit, center vert+horiz */
                     textprintf (double_buffer, font, dx * 16 + 4 + xofs,
                                 dy * 16 + 4 + yofs, makecol (255, 255, 255),
                                 "%d", z_seg[here]);
                  } else if (z_seg[here] < 100) {
                     /* The zone's number is double-digit, center only vert */
                     textprintf (double_buffer, font, dx * 16 + xofs,
                                 dy * 16 + 4 + yofs, makecol (255, 255, 255),
                                 "%d", z_seg[here]);
                  } else if (z_seg[here] < 10) {
                     /* The zone's number is triple-digit.  Print the 100's
                      * digit in top-center of the square; the 10's and 1's
                      * digits on bottom of the square
                      */
                     textprintf (double_buffer, font, dx * 16 + 4 + xofs,
                                 dy * 16 + yofs, makecol (255, 255, 255), "%d",
                                 (int) (z_seg[here] / 100));
                     textprintf (double_buffer, font, dx * 16 + xofs,
                                 dy * 16 + 8 + yofs, makecol (255, 255, 255),
                                 "%02d", (int) (z_seg[here] % 100));
                  }
#endif /* (ALLEGRO_VERSION) */
               }
#endif /* DEBUGMODE */
            }
         }
      }
   }
}



/*! \brief Draw shadows
 *
 * Draw the shadow layer... this beats making extra tiles.  This may be
 * moved in the future to fall between the background and foreground layers.
 * Shadows are never parallaxed.
 */
static void draw_shadows (void)
{
   int dx, dy, pix, xtc, ytc;

   if (draw_shadow == 0)
      return;
   if (!view_on) {
      view_y1 = 0;
      view_y2 = g_map.ysize - 1;
      view_x1 = 0;
      view_x2 = g_map.xsize - 1;
   }
   xtc = vx >> 4;
   ytc = vy >> 4;
   xofs = 16 - (vx & 15);
   yofs = 16 - (vy & 15);
   for (dy = 0; dy < 16; dy++) {
      for (dx = 0; dx < 21; dx++) {
         if (ytc + dy >= view_y1 && xtc + dx >= view_x1 && ytc + dy <= view_y2
             && xtc + dx <= view_x2) {
            pix = s_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
            if (pix > 0)
               draw_trans_sprite (double_buffer, shadow[pix], dx * 16 + xofs,
                                  dy * 16 + yofs);
         }
      }
   }
}



/*! \brief Draw border box
 *
 * Draw the fancy-pants border that I use.  I hard-draw the border instead
 * of using bitmaps, because that's just the way I am.  It doesn't degrade
 * performance, so who cares :)
 * Border is about 4 pixels thick, fitting inside (x,y)-(x2,y2)
 *
 * \param   where Bitmap to draw to
 * \param   x Top-left x-coord
 * \param   y Top-left y-coord
 * \param   x2 Bottom-right x-coord
 * \param   y2 Bottom-right y-coord
 */
static void border (BITMAP * where, int x, int y, int x2, int y2)
{
   vline (where, x + 1, y + 3, y2 - 3, GREY2);
   vline (where, x + 2, y + 3, y2 - 3, GREY3);
   vline (where, x + 3, y + 2, y2 - 2, GREY3);
   vline (where, x + 3, y + 5, y2 - 5, WHITE);
   vline (where, x + 4, y + 5, y2 - 5, GREY1);
   vline (where, x2 - 1, y + 3, y2 - 3, GREY2);
   vline (where, x2 - 2, y + 3, y2 - 3, GREY3);
   vline (where, x2 - 3, y + 2, y2 - 2, GREY3);
   vline (where, x2 - 3, y + 5, y2 - 5, WHITE);
   vline (where, x2 - 4, y + 5, y2 - 5, GREY1);
   hline (where, x + 3, y + 1, x2 - 3, GREY2);
   hline (where, x + 3, y + 2, x2 - 3, GREY3);
   hline (where, x + 4, y + 3, x2 - 4, GREY3);
   hline (where, x + 5, y + 3, x2 - 5, WHITE);
   hline (where, x + 5, y + 4, x2 - 5, GREY1);
   hline (where, x + 3, y2 - 1, x2 - 3, GREY2);
   hline (where, x + 3, y2 - 2, x2 - 3, GREY3);
   hline (where, x + 4, y2 - 3, x2 - 4, GREY3);
   hline (where, x + 5, y2 - 3, x2 - 5, WHITE);
   hline (where, x + 5, y2 - 4, x2 - 5, GREY1);
   putpixel (where, x + 2, y + 2, GREY2);
   putpixel (where, x + 2, y2 - 2, GREY2);
   putpixel (where, x2 - 2, y + 2, GREY2);
   putpixel (where, x2 - 2, y2 - 2, GREY2);
   putpixel (where, x + 4, y + 4, WHITE);
   putpixel (where, x + 4, y2 - 4, WHITE);
   putpixel (where, x2 - 4, y + 4, WHITE);
   putpixel (where, x2 - 4, y2 - 4, WHITE);
}



/*! \brief Draw  box, with different backgrounds and borders
 *
 * Draw the box as described. This was suggested by CB as 
 * a better alternative to the old create bitmap/blit trans/destroy bitmap
 * method.
 *
 * \author PH
 * \date 20030616
 *
 * \param   where Bitmap to draw to
 * \param   x1 x-coord of top left
 * \param   y1 y-coord of top left
 * \param   x2 x-coord of bottom right
 * \param   y2 y-coord of bottom right
 * \param   bg Colour/style of background
 * \param   bstyle Style of border
 */
static void draw_kq_box (BITMAP * where, int x1, int y1, int x2, int y2,
                         int bg, int bstyle)
{
   int a;
   /* Draw a maybe-translucent background */
   if (bg == BLUE) {
      drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
   } else {
      bg = (bg == DARKBLUE) ? DBLUE : DRED;
   }
   rectfill (where, x1 + 2, y1 + 2, x2 - 3, y2 - 3, bg);
   drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
   /* Now the border */
   switch (bstyle) {
   case B_TEXT:
   case B_MESSAGE:
      border (where, x1, y1, x2 - 1, y2 - 1);
      break;
   case B_THOUGHT:
      /* top and bottom */
      for (a = x1 + 8; a < x2 - 8; a += 8) {
         draw_sprite (where, bord[1], a, y1);
         draw_sprite (where, bord[6], a, y2 - 8);
      }
      /* sides */
      for (a = y1 + 8; a < y2 - 8; a += 12) {
         draw_sprite (where, bord[3], x1, a);
         draw_sprite (where, bord[4], x2 - 8, a);
      }
      /* corners */
      draw_sprite (where, bord[0], x1, y1);
      draw_sprite (where, bord[2], x2 - 8, y1);
      draw_sprite (where, bord[5], x1, y2 - 8);
      draw_sprite (where, bord[7], x2 - 8, y2 - 8);
      break;
   default:                    /* no border */
      break;
   }
}



/*! \brief Draw menu box
 *
 * Draw a menubox.  This is kinda hacked because of translucency, but it
 * works.  I use the DARKBLUE define to draw a non-translucent box.
 *
 * \param   where Bitmap to draw to
 * \param   x x-coord
 * \param   y y-coord
 * \param   w Width
 * \param   h Height
 * \param   c Colour (see note above)
 */
void menubox (BITMAP * where, int x, int y, int w, int h, int c)
{
   draw_kq_box (where, x, y, x + w * 8 + 16, y + h * 8 + 16, c, B_TEXT);
}



/*! \brief Display string
 *
 * Display a string in a particular font on a bitmap at the specified
 * co-ordinates.
 *
 * \param   where Bitmap to draw to
 * \param   sx x-coord
 * \param   sy y-coord
 * \param   msg String to draw
 * \param   cl Font index (0..6)
 */
void print_font (BITMAP *where, int sx, int sy, const char *msg, int cl)
{
   int z, cc, hgt = 8;

   if (cl < 0 || cl > 6) {
      sprintf (strbuf, "print_font: Bad font index, %d", cl);
      klog (strbuf);
      return;
   }
   if (cl == FBIG)
      hgt = 12;
   for (z = 0; z < (signed int) strlen (msg); z++) {
      cc = msg[z];
      cc -= 32;
      if (cc > 92)
         cc = 92;
      if (cc < 0)
         cc = 0;
      masked_blit (kfonts, where, cc * 8, cl * 8, z * 8 + sx, sy, 8, hgt);
   }
}



/*! \brief Display number
 *
 * Display a number using the small font on a bitmap at the specified
 * co-ordinates and using the specified color.  This still expects the
 * number to be in a string... the function's real purpose is to use
 * a different font for numerical display in combat.
 *
 * \param   where Bitmap to draw to
 * \param   sx x-coord
 * \param   sy y-coord
 * \param   msg String to draw
 * \param   cl Font index (0..4)
 */
void print_num (BITMAP * where, int sx, int sy, char *msg, int cl)
{
   int z, cc;

   if (cl < 0 || cl > 4) {
      sprintf (strbuf, "print_num: Bad font index, %d", cl);
      klog (strbuf);
      return;
   }
   for (z = 0; z < (signed int) strlen (msg); z++) {
      cc = msg[z];
      cc -= 48;
      if (cc >= 0 && cc <= 9)
         masked_blit (sfonts[cl], where, cc * 6, 0, z * 6 + sx, sy, 6, 8);
   }
}



/*! \brief Calculate bubble position
 *
 * The purpose of this function is to calculate where a text bubble
 * should go in relation to the entity who is speaking.
 *
 * \param   who Character that is speaking, or -1 for 'general'
 */
static void set_textpos (int who)
{
   if (who < MAX_ENT && who >= 0) {
      gbx = (g_ent[who].tilex * 16) - vx;
      gby = (g_ent[who].tiley * 16) - vy;
      gbbx = gbx - (gbbw * 4);
      if (gbbx < 8)
         gbbx = 8;
      if (gbbw * 8 + gbbx + 16 > 312)
         gbbx = 296 - (gbbw * 8);
      if (gby > -16 && gby < 240) {
         if (g_ent[who].facing == 1 || g_ent[who].facing == 2) {
            if (gbbh * 12 + gby + 40 <= 232)
               gbby = gby + 24;
            else
               gbby = gby - (gbbh * 12) - 24;
         } else {
            if (gby - (gbbh * 12) - 24 >= 8)
               gbby = gby - (gbbh * 12) - 24;
            else
               gbby = gby + 24;
         }
      } else {
         if (gby < 8)
            gbby = 8;
         if (gbbh * 12 + gby + 16 > 232)
            gbby = 216 - (gbbh * 12);
      }
      if (gbby > gby) {
         gby += 20;
         gbt = (gbx < 152 ? 3 : 2);
      } else {
         gby -= 20;
         gbt = (gbx < 152 ? 1 : 0);
      }
      if (gbx < gbbx + 8)
         gbx = gbbx + 8;
      if (gbx > gbbw * 8 + gbbx - 8)
         gbx = gbbw * 8 + gbbx - 8;
      if (gby < gbby - 4)
         gby = gbby - 4;
      if (gby > gbbh * 12 + gbby + 4)
         gby = gbbh * 12 + gbby + 4;
   } else {
      gbby = 216 - (gbbh * 12);
      gbbx = 152 - (gbbw * 4);
      gbt = -1;
   }
}



/*! \brief Draw text box
 *
 * Hmm... I think this function draws the textbox :p
 *
 * \date 20030417 PH This now draws the text as well as just the box
 * \param   bstyle Style (B_TEXT or B_THOUGHT or B_MESSAGE)
 */
static void draw_textbox (int bstyle)
{
   int wid, hgt, a;
   BITMAP *stem;
/*    BITMAP *tm; */

   wid = gbbw * 8 + 16;
   hgt = gbbh * 12 + 16;

   draw_kq_box (double_buffer, gbbx + xofs, gbby + yofs, gbbx + xofs + wid,
                gbby + yofs + hgt, BLUE, bstyle);
   if (gbt != -1) {
      /* select the correct stem-thingy that comes out of the speech bubble */
      stem = bub[gbt + (bstyle == B_THOUGHT ? 4 : 0)];
      /* and draw it */
      draw_sprite (double_buffer, stem, gbx + xofs, gby + yofs);
   }

   for (a = 0; a < gbbh; a++) {
      print_font (double_buffer, gbbx + 8 + xofs, a * 12 + gbby + 8 + yofs,
                  msgbuf[a], FBIG);
   }
}



/*! \brief Draw text bubble
 *
 * Draw a regular text bubble and display the text.
 *
 * \param   who Entity that is speaking
 * \param   sp1 Line 1 of text
 * \param   sp2 Line 2 of text
 * \param   sp3 Line 3 of text
 * \param   sp4 Line 4 of text
 *
 * \sa bubble_text_ex()
 */
void bubble_text (int who, char *sp1, char *sp2, char *sp3, char *sp4)
{
   strcpy (msgbuf[0], parse_string (sp1));
   strcpy (msgbuf[1], parse_string (sp2));
   strcpy (msgbuf[2], parse_string (sp3));
   strcpy (msgbuf[3], parse_string (sp4));
   generic_text (who, B_TEXT);
}



/*! \brief The internal processing modes during text reformatting
 *
 * \sa relay()
 */
enum m_mode
{ M_UNDEF, M_SPACE, M_NONSPACE, M_END };



/*! \brief Split text into lines
 * \author PH
 * \date 20021220
 *
 *
 * Takes a string and re-formats it to fit into the msgbuf text buffer,
 * for displaying with  generic_text().  Processes as much as it can to
 * fit in one box, and returns a pointer to the next unprocessed character
 *
 * \param   buf The string to reformat
 * \returns the rest of the string that has not been processed, or NULL if
 *          it has all been processed.
 */
static const char *relay (const char *buf)
{
   int lasts, lastc, i, cr, cc;
   char tc;
   enum m_mode state;
   for (i = 0; i < 4; ++i)
      memset (msgbuf[i], 0, MSG_COLS);
   i = 0;
   cc = 0;
   cr = 0;
   lasts = -1;
   lastc = 0;
   state = M_UNDEF;
   while (1) {
      tc = buf[i];
      switch (state) {
      case M_UNDEF:
         switch (tc) {
         case ' ':
            lasts = i;
            lastc = cc;
            state = M_SPACE;
            break;
         case '\0':
            msgbuf[cr][cc] = '\0';
            state = M_END;
            break;
         case '\n':
            msgbuf[cr][cc] = '\0';
            cc = 0;
            ++i;
            if (++cr >= 4)
               return &buf[i];
            break;
         default:
            state = M_NONSPACE;
            break;
         }
         break;
      case M_SPACE:
         switch (tc) {
         case ' ':
            if (cc < MSG_COLS - 1) {
               msgbuf[cr][cc++] = tc;
            } else {
               msgbuf[cr][MSG_COLS - 1] = '\0';
            }
            ++i;
            break;
         default:
            state = M_UNDEF;
            break;
         }
         break;
      case M_NONSPACE:
         switch (tc) {
         case ' ':
         case '\0':
         case '\n':
            state = M_UNDEF;
            break;
         default:
            if (cc < MSG_COLS - 1) {
               msgbuf[cr][cc++] = tc;
            } else {
               msgbuf[cr++][lastc] = '\0';
               cc = 0;
               i = lasts;
               if (cr >= MSG_ROWS) {
                  return &buf[1 + lasts];
               }
            }
            ++i;
         }
         break;
      case M_END:
         return NULL;
         break;
      }
   }
}



/*! \brief Display speech/thought bubble
 * \author PH
 * \date 20021220
 *
 * Displays text, like bubble_text, but passing the args
 * through relay() first
 * \date updated 20030401 merged thought and speech
 * \sa bubble_text()
 * \param   fmt Format, B_TEXT or B_THOUGHT
 * \param   who Character that is speaking
 * \param   s The text to display
 */
void text_ex (int fmt, int who, const char *s)
{
   s = parse_string (s);
/*   if (fmt == B_MESSAGE) {
      fmt = B_TEXT;
      who = 255;
   }*/
   while (s) {
      s = relay (s);
      generic_text (who, fmt);
   }
}





/*! \brief Draw thought bubble
 *
 * Draw a thought bubble and display the text.
 *
 * \sa      thought_text_ex()
 * \param   who Entity that is speaking
 * \param   sp1 Line 1 of text
 * \param   sp2 Line 2 of text
 * \param   sp3 Line 3 of text
 * \param   sp4 Line 4 of text
 */
void thought_text (int who, char *sp1, char *sp2, char *sp3, char *sp4)
{
   strcpy (msgbuf[0], parse_string (sp1));
   strcpy (msgbuf[1], parse_string (sp2));
   strcpy (msgbuf[2], parse_string (sp3));
   strcpy (msgbuf[3], parse_string (sp4));
   generic_text (who, B_THOUGHT);
}



/*! \brief Text box drawing
 *
 * Generic routine to actually display a text box and wait for a keypress.
 *
 * \param   who Character that is speaking/thinking (ignored for B_MESSAGE style)
 * \param   box_style Style (B_TEXT or B_THOUGHT or B_MESSAGE)
 */
static void generic_text (int who, int box_style)
{
   int a, stop = 0;
   int len;
   gbbw = 1;
   gbbh = 0;
   gbbs = 0;
   for (a = 0; a < 4; a++) {
      len = strlen (msgbuf[a]);
      /* FIXME: PH changed >1 to >0 */
      if (len > 0) {
         gbbh = a + 1;
         if ((signed int) len > gbbw)
            gbbw = len;
      }
   }
   set_textpos (box_style == B_MESSAGE ? -1 : who);
   if (gbbw == -1 || gbbh == -1)
      return;
   unpress ();
   timer_count = 0;
   while (!stop) {
      check_animation ();
      drawmap ();
      draw_textbox (box_style);
      blit2screen (xofs, yofs);
      readcontrols ();
      if (balt) {
         unpress ();
         stop = 1;
      }
   }
   timer_count = 0;
}



/*! \brief Insert character names
 *
 * This checks a string for $0, or $1 and replaces with player names.
 *
 * PH 20030107 Increased limit on length of the_string.
 * NB. Values for $ other than $0 or $1 will cause errors.
 *
 * \param   the_string Input string
 * \returns processed string, in a static buffer \p strbuf
 *          or \p the_string, if it had no replacement chars.
 */
const char *parse_string (const char *the_string)
{
   static char strbuf[1024];
   const char *ap;
   char *bp, *name;
   name = NULL;
   memset (strbuf, 0, sizeof (strbuf));
   bp = strbuf;
   for (ap = the_string; *ap; ++ap) {
      if (*ap == '$') {
         for (name = party[pidx[ap[1] - '0']].name; *name; ++name) {
            if (bp < strbuf + sizeof (strbuf))
               *bp++ = *name;
         }
         ++ap;
      } else {
         if (bp < strbuf + sizeof (strbuf))
            *bp++ = *ap;
      }
   }
   return name == NULL ? the_string : strbuf;
}



/*! \brief prompt for user input
 *
 * Present the user with a prompt and a list of options to select from.
 * The prompt is shown, as per text_ex(), and the choices shown in
 * a separate window at the bottom. If the prompt is longer than one
 * box-full, it is shown box-by-box, until the last one, when the choices are shown.
 * If there are more choices than will fit into the box at the bottom, arrows are shown
 * to indicate more pages.
 * Press ALT to select; CTRL does nothing.
 *
 * \author PH
 * \date 20030417
 *
 * \param   who Which character is ASKING the question
 * \param   ptext The prompt test
 * \param   opt An array of options, null terminated
 * \param   n_opt The number of options
 * \return  option selected, 0= first option etc.
 */
int prompt_ex (int who, const char *ptext, char *opt[], int n_opt)
{
   int curopt = 0;
   int topopt = 0;
   int winheight;
   int winwidth = 0;
   int winx, winy;
   int i, w, running;

   ptext = parse_string (ptext);
   while (1) {
      gbbw = 1;
      gbbs = 0;
      ptext = relay (ptext);
      if (ptext) {
         /* print prompt pages prior to the last one */
         generic_text (who, B_TEXT);
      } else {
         /* do prompt and options */
         int a;
         /* calc the size of the prompt box */
         for (a = 0; a < 4; a++) {
            int len = strlen (msgbuf[a]);
            /* FIXME: PH changed >1 to >0 */
            if (len > 0) {
               gbbh = a + 1;
               if ((signed int) len > gbbw)
                  gbbw = len;
            }
         }
         /* calc the size of the options box */
         for (i = 0; i < n_opt; ++i) {
            while (isspace (*opt[i])) {
               ++opt[i];
            }
            w = strlen (opt[i]);
            if (winwidth < w)
               winwidth = w;
         }
         winheight = n_opt > 4 ? 4 : n_opt;
         winx = xofs + (320 - winwidth * 8) / 2;
         winy = yofs + 230 - winheight * 12;
         running = 1;
         while (running) {
            check_animation ();
            drawmap ();
            /* Draw the prompt text */
            set_textpos (who);
            draw_textbox (B_TEXT);
            /* Draw the  options text */
            draw_kq_box (double_buffer, winx - 5, winy - 5,
                         winx + winwidth * 8 + 13,
                         winy + winheight * 12 + 5, BLUE, B_TEXT);
            for (i = 0; i < winheight; ++i) {
               print_font (double_buffer, winx + 8, winy + i * 12,
                           opt[i + topopt], FBIG);
            }
            draw_sprite (double_buffer, menuptr, winx + 8 - menuptr->w,
                         (curopt - topopt) * 12 + winy + 4);
            /* Draw the 'up' and 'down' markers if there are more options than will fit in the window */
            if (topopt > 0)
               draw_sprite (double_buffer, upptr, winx, winy - 8);
            if (topopt < n_opt - winheight)
               draw_sprite (double_buffer, dnptr, winx, winy + 12 * winheight);

            blit2screen (xofs, yofs);

            readcontrols ();
            if (up && curopt > 0) {
               play_effect (SND_CLICK, 128);
               unpress ();
               --curopt;
            } else if (down && curopt < (n_opt - 1)) {
               play_effect (SND_CLICK, 128);
               unpress ();
               ++curopt;
            } else if (balt) {
               /* Selected an option */
               play_effect (SND_CLICK, 128);
               unpress ();
               running = 0;
            } else if (bctrl) {
               /* Just go "ow!" */
               unpress ();
               play_effect (SND_BAD, 128);
            }

            /* Adjust top position so that the current option is always shown */
            if (curopt < topopt) {
               topopt = curopt;
            }
            if (curopt >= topopt + winheight) {
               topopt = curopt - winheight + 1;

            }
         }
         return curopt;
      }
   }
}



/*! \brief Do user prompt
 *
 * Draw a text box and wait for a response.  It is possible to offer up to four
 * choices in a prompt box.
 *
 * \param   who Entity that is speaking
 * \param   numopt Number of choices
 * \param   bstyle Textbox style (B_TEXT or B_THOUGHT)
 * \param   sp1 Line 1 of text
 * \param   sp2 Line 2 of text
 * \param   sp3 Line 3 of text
 * \param   sp4 Line 4 of text
 * \returns index of option chosen (0..numopt-1)
 */
int prompt (int who, int numopt, int bstyle, char *sp1, char *sp2, char *sp3,
            char *sp4)
{
   int ly, stop = 0, ptr = 0, a;

   gbbw = 1;
   gbbh = 0;
   gbbs = 0;
   strcpy (msgbuf[0], parse_string (sp1));
   strcpy (msgbuf[1], parse_string (sp2));
   strcpy (msgbuf[2], parse_string (sp3));
   strcpy (msgbuf[3], parse_string (sp4));
   unpress ();
   for (a = 0; a < 4; a++) {
      if (strlen (msgbuf[a]) > 1) {
         gbbh = a + 1;
         if ((signed int) strlen (msgbuf[a]) > gbbw)
            gbbw = strlen (msgbuf[a]);
      }
   }
   set_textpos (who);
   if (gbbw == -1 || gbbh == -1)
      return -1;
   ly = (gbbh - numopt) * 12 + gbby + 10;
   while (!stop) {
      check_animation ();
      drawmap ();
      draw_textbox (bstyle);
/*           for (a = 0; a < gbbh; a++) */
/*              print_font (double_buffer, gbbx + 8 + xofs, */
/*                          a * 12 + gbby + 8 + yofs, msgbuf[a], FBIG); */
      draw_sprite (double_buffer, menuptr, gbbx + xofs + 8,
                   ptr * 12 + ly + yofs);
      blit2screen (xofs, yofs);

      readcontrols ();
      if (up) {
         unpress ();
         ptr--;
         if (ptr < 0)
            ptr = 0;
         play_effect (SND_CLICK, 128);
      }
      if (down) {
         unpress ();
         ptr++;
         if (ptr > numopt - 1)
            ptr = numopt - 1;
         play_effect (SND_CLICK, 128);
      }
      if (balt) {
         unpress ();
         stop = 1;
      }
   }
   return ptr;
}



/*! \brief Alert player
 *
 * Draw a single-line message in the center of the screen and wait for
 * the confirm key to be pressed or for a specific amount of time.
 *
 * \param   m Message text
 * \param   icn Icon to display or 255 for none
 * \param   delay Time to wait (milliseconds?)
 * \param   x_m X-coord of top-left (like xofs)
 * \param   y_m Y-coord of top-left
 */
void message (char *m, int icn, int delay, int x_m, int y_m)
{
   char msg[1024];
   const char *s;
   int i, num_lines, max_len, len;

   /* Do the $0 replacement stuff */
   memset (msg, 0, sizeof (msg));
   strncpy (msg, parse_string (m), sizeof (msg) - 1);
   s = msg;

   /* Save a copy of the screen */
   blit (double_buffer, back, x_m, y_m, 0, 0, 352, 280);

   /* Loop for each box full of text... */
   while (s != NULL) {
      s = relay (s);
      /* Calculate the box size */
      num_lines = max_len = 0;
      for (i = 0; i < MSG_ROWS; ++i) {
         len = strlen (msgbuf[i]);
         if (len > 0) {
            if (max_len < len)
               max_len = len;
            ++num_lines;
         }
      }
      /* Draw the box and maybe the icon */
      if (icn == 255) {
         /* No icon */
         menubox (double_buffer, 152 - (max_len * 4) + x_m, 108 + y_m, max_len,
                  num_lines, DARKBLUE);
      } else {
         /* There is an icon; make the box a little bit bigger to the left */
         menubox (double_buffer, 144 - (max_len * 4) + x_m, 108 + y_m,
                  max_len + 1, num_lines, DARKBLUE);
         draw_icon (double_buffer, icn, 152 - (max_len * 4) + x_m, 116 + y_m);
      }

      /* Draw the text */
      for (i = 0; i < num_lines; ++i) {
         print_font (double_buffer, 160 - (max_len * 4) + x_m,
                     116 + 8 * i + y_m, msgbuf[i], FNORMAL);
      }
      /* Show it */
      blit2screen (x_m, y_m);
      /* Wait for delay time or key press */
      if (delay == 0)
         wait_enter ();
      else
         wait (delay);
      blit (back, double_buffer, 0, 0, x_m, y_m, 352, 280);
   }
}



/*! \brief Adjust view
 *
 * This merely sets the view variables for use in
 * other functions that rely on the view.
 * The view defines a subset of the map,
 * for example when you move to a house in a town,
 * the view contracts to display only the interior.
 *
 * \param   vw Non-zero to enable view, otherwise show the whole map
 * \param   x1 Top-left of view
 * \param   y1 Top-left of view
 * \param   x2 Bottom-right of view
 * \param   y2 Bottom-right of view
 */
void set_view (int vw, int x1, int y1, int x2, int y2)
{
   view_on = vw;
   if (view_on) {
      view_x1 = x1;
      view_y1 = y1;
      view_x2 = x2;
      view_y2 = y2;
   } else {
      view_x1 = 0;
      view_y1 = 0;
      view_x2 = g_map.xsize - 1;
      view_y2 = g_map.ysize - 1;
   }
}
