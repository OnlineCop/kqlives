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
 * \brief Character and Map drawing
 *
 * Includes functions to draw characters, text and maps.
 * Also some colour manipulation.
 * \author JB
 * \date ??????
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

/*
   globals
*/
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
 * \param xw x-coordinate in double_buffer of the top-left of the screen
 * \param yw y-coordinate in double_buffer of the top-left of the screen
*/
void blit2screen (int xw, int yw)
{
   if (wait_retrace == 1)
      vsync ();
   if (show_frate == 1)
     {
        sprintf (strbuf, "%d", mfrate);
        print_font (double_buffer, xofs, yofs, strbuf, FNORMAL);
     }
   if (stretch_view == 1)
      stretch_blit (double_buffer, screen, xw, yw, 320, 240, 0, 0, 640, 480);
   else
      blit (double_buffer, screen, xw, yw, 0, 0, 320, 240);
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
 *
 * \param src Source bitmap
 * \param dest Destination bitmap
 * \param st start of output color range
 * \param fn end of output color range
*/
void color_scale (BITMAP * src, BITMAP * dest, int st, int fn)
{
   int ix, iy, z, a;

   clear_bitmap (dest);
   for (iy = 0; iy < src->h; iy++)
     {
        for (ix = 0; ix < src->w; ix++)
          {
             a = src->line[iy][ix];
             if (a > 0)
               {
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
 * \param who Character to convert
 * \param st Start of output range
 * \param fn End of output range
 * \param aflag if ==1 then ignore \p who and covert all
*/
void convert_cframes (int who, int st, int fn, int aflag)
{
   int a, p;

   if (aflag == 1)
     {
        if (who < PSIZE)
          {
             for (a = 0; a < numchrs; a++)
                for (p = 0; p < MAXCFRAMES; p++)
                   color_scale (tcframes[a][p], cframes[a][p], st, fn);
          }
        else
          {
             for (a = PSIZE; a < PSIZE + numens; a++)
                for (p = 0; p < MAXCFRAMES; p++)
                   color_scale (tcframes[a][p], cframes[a][p], st, fn);
          }
     }
   else
     {
        for (p = 0; p < MAXCFRAMES; p++)
           color_scale (tcframes[who][p], cframes[who][p], st, fn);
     }
}

/*! \brief Restore colours
 *
 * Restore specified fighter frames to normal color.
 * \param who Character to restore
 * \param aflag if ==1 then ignore \p who and convert all frames
*/
void revert_cframes (int who, int aflag)
{
   int a, p;

   if (aflag == 1)
     {
        if (who < PSIZE)
          {
             for (a = 0; a < numchrs; a++)
                for (p = 0; p < MAXCFRAMES; p++)
                   blit (tcframes[a][p], cframes[a][p], 0, 0, 0, 0,
                         fighter[a].cw, fighter[a].cl);
          }
        else
          {
             for (a = PSIZE; a < PSIZE + numens; a++)
                for (p = 0; p < MAXCFRAMES; p++)
                   blit (tcframes[a][p], cframes[a][p], 0, 0, 0, 0,
                         fighter[a].cw, fighter[a].cl);
          }
     }
   else
     {
        for (p = 0; p < MAXCFRAMES; p++)
           blit (tcframes[who][p], cframes[who][p], 0, 0, 0, 0,
                 fighter[who].cw, fighter[who].cl);
     }
}

/*! \brief Draw small  icon
 *
 * Just a helper function... reduces the number of places that 'sicons'
 * has to be referenced.
 * Icons are 8x8 sub-bitmaps of sicons, representing items (sword, etc.)
 *
 * \param where bitmap to draw to
 * \param ino icon to draw
 * \param icx x-coord
 * \param icy y-coord
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
 * \param where bitmap to draw to
 * \param cc non-zero if in combat mode (draw
 *        using info  \p fighter[] rather than \p party[]
 * \param who Charcter to draw status for
 * \param inum the maximum number of status icons to draw.
 *        \p inum ==17 when in combat, ==8 otherwise.
 * \param icx x-coord to draw to
 * \param icy y-coord to draw to
*/
void draw_stsicon (BITMAP * where, int cc, int who, int inum, int icx, int icy)
{
   int j, st = 0, s;

   for (j = 0; j < inum; j++)
     {
        if (cc == 0)
           s = party[who].sts[j];
        else
           s = fighter[who].sts[j];
        if (s != 0)
          {
             masked_blit (stspics, where, 0, j * 8 + 8, st * 8 + icx, icy, 8,
                          8);
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
 *  
 * \param xw x-offset - always ==16
 * \param yw y-offset - always ==16
*/
static void drawchar (int xw, int yw)
{
   int fr, dx, dy, i, f, fid;
   int spec = 0;

   for (i = 0; i < PSIZE + noe; i++)
     {
        spec = 0;
        fid = g_ent[i].eid;
        dx = g_ent[i].x - vx + xw;
        dy = g_ent[i].y - vy + yw;
        fr = g_ent[i].facing * 3;
        if (g_ent[i].moving == 0)
           fr = g_ent[i].facing * 3 + 2;
        else
          {
             if (g_ent[i].framectr > 10)
                fr += 1;
          }
        if (i < PSIZE && i < numchrs)
          {
             if (party[fid].sts[S_DEAD] != 0)
                fr = g_ent[i].facing * 3 + 2;
             if (party[fid].sts[S_POISON] != 0)
                color_scale (frames[fid][fr], tc2, 32, 47);
             else
                blit (frames[fid][fr], tc2, 0, 0, 0, 0, 16, 18);
             if (g_map.tileset == 0)
               {
                  if (is_forestsquare (g_ent[i].tilex, g_ent[i].tiley))
                    {
                       f = 0;
                       if (g_ent[i].moving == 0)
                          f = 1;
                       if (g_ent[i].moving == 1
                           && is_forestsquare (g_ent[i].tilex,
                                               g_ent[i].tiley - 1))
                          f = 1;
                       if (g_ent[i].moving == 2
                           && is_forestsquare (g_ent[i].tilex,
                                               g_ent[i].tiley + 1))
                          f = 1;
                       if (g_ent[i].moving == 3
                           && is_forestsquare (g_ent[i].tilex + 1,
                                               g_ent[i].tiley))
                          f = 1;
                       if (g_ent[i].moving == 4
                           && is_forestsquare (g_ent[i].tilex - 1,
                                               g_ent[i].tiley))
                          f = 1;
                       if (f == 1)
                         {
                            clear_to_color (tc, 0);
                            blit (tc2, tc, 0, 0, 0, 0, 16, 6);
                            if (party[pidx[i]].sts[S_DEAD] == 0)
                               draw_sprite (double_buffer, tc, dx, dy);
                            else
                               draw_trans_sprite (double_buffer, tc, dx, dy);
                         }
                       else
                         {
                            if (party[pidx[i]].sts[S_DEAD] == 0)
                               draw_sprite (double_buffer, tc2, dx, dy);
                            else
                               draw_trans_sprite (double_buffer, tc2, dx, dy);
                         }
                    }
                  else
                    {
                       if (party[fid].sts[S_DEAD] == 0)
                          draw_sprite (double_buffer, tc2, dx, dy);
                       else
                          draw_trans_sprite (double_buffer, tc2, dx, dy);
                    }
               }
             else
               {
                  if (party[fid].sts[S_DEAD] == 0)
                     draw_sprite (double_buffer, tc2, dx, dy);
                  else
                     draw_trans_sprite (double_buffer, tc2, dx, dy);
               }
          }
        else
          {
             if (g_ent[i].active)
               {
                  if (g_ent[i].tilex >= view_x1 && g_ent[i].tilex <= view_x2
                      && g_ent[i].tiley >= view_y1 && g_ent[i].tiley <= view_y2)
                    {
                       if (dx >= -16 && dx <= 336 && dy >= -16 && dy <= 256)
                         {
                            if (g_ent[i].eid >= ID_ENEMY)
                              {
                                 if (g_ent[i].transl == 0)
                                    draw_sprite (double_buffer,
                                                 eframes[g_ent[i].chrx][fr],
                                                 dx, dy);
                                 else
                                    draw_trans_sprite (double_buffer,
                                                       eframes[g_ent[i].
                                                               chrx][fr], dx,
                                                       dy);
                              }
                            else
                              {
                                 if (g_ent[i].transl == 0)
                                    draw_sprite (double_buffer,
                                                 frames[g_ent[i].eid][fr], dx,
                                                 dy);
                                 else
                                    draw_trans_sprite (double_buffer,
                                                       frames[g_ent[i].
                                                              eid][fr], dx, dy);
                              }
                         }
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
 * \param fx x-coord to check
 * \param fy y-coord to check
 * \returns 1 if it is a forest square
*/
int is_forestsquare (int fx, int fy)
{
   int f;

   f = map_seg[(fy * g_map.xsize) + fx];
   if (f == 63 || f == 65 || f == 66 || f == 67 || f == 71 || f == 72
       || f == 73 || f == 74)
      return 1;
   else
      return 0;
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
 * Also handles the Repulse indicator and the map description display.
*/
void drawmap (void)
{
   if (g_map.xsize <= 0)
     {
        clear_to_color (double_buffer, 1);
        return;
     }
   draw_backlayer ();
   if (g_map.map_mode == 1 || g_map.map_mode == 3 || g_map.map_mode == 5)
      drawchar (16, 16);
   draw_midlayer ();
   if (g_map.map_mode == 0 || g_map.map_mode == 2 || g_map.map_mode == 4)
      drawchar (16, 16);
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
   if (progress[P_REPULSE] > 0)
     {
        rectfill (b_repulse, 0, 16, 15, 165, 0);
        rectfill (b_repulse, 5, 16, 10, 16 + progress[P_REPULSE], 15);
        draw_trans_sprite (double_buffer, b_repulse, 2 + xofs, 2 + yofs);
     }
   if (display_desc == 1)
     {
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

   if (draw_background == 0)
      return;
   if (view_on == 0)
     {
        view_y1 = 0;
        view_y2 = g_map.ysize - 1;
        view_x1 = 0;
        view_x2 = g_map.xsize - 1;
     }
   if (g_map.map_mode < 2 || g_map.map_mode > 3)
     {
        xtc = vx >> 4;
        ytc = vy >> 4;
        dx = vx;
        dy = vy;
     }
   else
     {
        dx = vx * g_map.pmult / g_map.pdiv;
        dy = vy * g_map.pmult / g_map.pdiv;
        xtc = dx >> 4;
        ytc = dy >> 4;
     }
   xofs = 16 - (dx & 15);
   yofs = 16 - (dy & 15);
   for (dy = 0; dy < 16; dy++)
     {
        for (dx = 0; dx < 21; dx++)
          {
             if (ytc + dy >= view_y1 && xtc + dx >= view_x1
                 && ytc + dy <= view_y2 && xtc + dx <= view_x2)
               {
                  pix = map_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
                  blit (map_icons[tilex[pix]], double_buffer, 0, 0,
                        dx * 16 + xofs, dy * 16 + yofs, 16, 16);
               }
             else
                blit (map_icons[0], double_buffer, 0, 0, dx * 16 + xofs,
                      dy * 16 + yofs, 16, 16);
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

   if (draw_middle == 0)
      return;
   if (view_on == 0)
     {
        view_y1 = 0;
        view_y2 = g_map.ysize - 1;
        view_x1 = 0;
        view_x2 = g_map.xsize - 1;
     }
   if (g_map.map_mode < 3 || g_map.map_mode == 5)
     {
        xtc = vx >> 4;
        ytc = vy >> 4;
        dx = vx;
        dy = vy;
     }
   else
     {
        dx = vx * g_map.pmult / g_map.pdiv;
        dy = vy * g_map.pmult / g_map.pdiv;
        xtc = dx >> 4;
        ytc = dy >> 4;
     }
   xofs = 16 - (dx & 15);
   yofs = 16 - (dy & 15);
   for (dy = 0; dy < 16; dy++)
     {
        for (dx = 0; dx < 21; dx++)
          {
             if (ytc + dy >= view_y1 && xtc + dx >= view_x1
                 && ytc + dy <= view_y2 && xtc + dx <= view_x2)
               {
                  pix = b_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
                  draw_sprite (double_buffer, map_icons[tilex[pix]],
                               dx * 16 + xofs, dy * 16 + yofs);
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

   if (draw_foreground == 0)
      return;
   if (view_on == 0)
     {
        view_y1 = 0;
        view_y2 = g_map.ysize - 1;
        view_x1 = 0;
        view_x2 = g_map.xsize - 1;
     }
   if (g_map.map_mode < 4)
     {
        xtc = vx >> 4;
        ytc = vy >> 4;
        dx = vx;
        dy = vy;
     }
   else
     {
        dx = vx * g_map.pmult / g_map.pdiv;
        dy = vy * g_map.pmult / g_map.pdiv;
        xtc = dx >> 4;
        ytc = dy >> 4;
     }
   xofs = 16 - (dx & 15);
   yofs = 16 - (dy & 15);
   for (dy = 0; dy < 16; dy++)
     {
        for (dx = 0; dx < 21; dx++)
          {
             if (ytc + dy >= view_y1 && xtc + dx >= view_x1
                 && ytc + dy <= view_y2 && xtc + dx <= view_x2)
               {
                  pix = f_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
                  draw_sprite (double_buffer, map_icons[tilex[pix]],
                               dx * 16 + xofs, dy * 16 + yofs);
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
   if (!view_on)
     {
        view_y1 = 0;
        view_y2 = g_map.ysize - 1;
        view_x1 = 0;
        view_x2 = g_map.xsize - 1;
     }
   xtc = vx >> 4;
   ytc = vy >> 4;
   xofs = 16 - (vx & 15);
   yofs = 16 - (vy & 15);
   for (dy = 0; dy < 16; dy++)
     {
        for (dx = 0; dx < 21; dx++)
          {
             if (ytc + dy >= view_y1 && xtc + dx >= view_x1
                 && ytc + dy <= view_y2 && xtc + dx <= view_x2)
               {
                  pix = s_seg[((ytc + dy) * g_map.xsize) + xtc + dx];
                  if (pix > 0)
                     draw_trans_sprite (double_buffer, shadow[pix],
                                        dx * 16 + xofs, dy * 16 + yofs);
               }
          }
     }
}

/*! \brief Draw border box
 *
 * Draw the fancy pants border that I use.  I hard draw the border instead
 * of using bitmaps, because that's just the way I am.  It doesn't degrade
 * performance, so who cares :)
 * Border is about 4 pixels thick, fitting inside (x,y)-(x2,y2)
 * \param where bitmap to draw to
 * \param x top-left x-coord
 * \param y top-left y-coord
 * \param x2 bottom-right x-coord
 * \param y2  bottom-right y-coord
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

/*! \briefDraw menu box
 *
 * Draw a menubox.  This is kinda hacked because of translucency, but it
 * works.  I use the DARKBLUE define to draw a non-translucent box.
 *
 * \param where bitmap to draw to
 * \param x x-coord
 * \param y y-coord
 * \param w width
 * \param h height
 * \param c colour (see note above)
*/
void menubox (BITMAP * where, int x, int y, int w, int h, int c)
{
   int wid, hgt;
   BITMAP *tm;

   wid = (w + 2) * 8;
   hgt = (h + 2) * 8;
   if (c == BLUE)
     {
        tm = create_bitmap (wid - 5, hgt - 5);
        rectfill (tm, 0, 0, wid - 5, hgt - 5, c);
        draw_trans_sprite (where, tm, x + 2, y + 2);
        border (where, x, y, x + wid - 1, y + hgt - 1);
        destroy_bitmap (tm);
     }
   else
     {
        if (c == DARKBLUE)
           rectfill (where, x + 2, y + 2, x + wid - 3, y + hgt - 3, DBLUE);
        else
           rectfill (where, x + 2, y + 2, x + wid - 3, y + hgt - 3, DRED);
        border (where, x, y, x + wid - 1, y + hgt - 1);
     }
}

/*! \brief Display string
 *
 * Display a string in a particular font on a bitmap at the specified
 * co-ordinates.
 *
 * \param where bitmap to draw to
 * \param sx x-coord
 * \param sy y-coord
 * \param msg string to draw
 * \param cl font index (0..6)
*/
void print_font (BITMAP * where, int sx, int sy, char *msg, int cl)
{
   int z, cc, hgt = 8;

   if (cl < 0 || cl > 6)
     {
        sprintf (strbuf, "print_font: Bad font index, %d", cl);
        klog (strbuf);
        return;
     }
   if (cl == FBIG)
      hgt = 12;
   for (z = 0; z < (signed int) strlen (msg); z++)
     {
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
 * number to be in a string... the functions real purpose is to use
 * a different font for numerical display in combat.
 *
 * \param where bitmap to draw to
 * \param sx x-coord
 * \param sy y-coord
 * \param msg string to draw
 * \param cl font index (0..4)
*/
void print_num (BITMAP * where, int sx, int sy, char *msg, int cl)
{
   int z, cc;

   if (cl < 0 || cl > 4)
     {
        sprintf (strbuf, "print_num: Bad font index, %d", cl);
        klog (strbuf);
        return;
     }
   for (z = 0; z < (signed int) strlen (msg); z++)
     {
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
 * \param who character that is speaking
*/
static void set_textpos (int who)
{
   if (who < MAX_ENT)
     {
        gbx = (g_ent[who].tilex * 16) - vx;
        gby = (g_ent[who].tiley * 16) - vy;
        gbbx = gbx - (gbbw * 4);
        if (gbbx < 8)
           gbbx = 8;
        if (gbbw * 8 + gbbx + 16 > 312)
           gbbx = 296 - (gbbw * 8);
        if (gby > -16 && gby < 240)
          {
             if (g_ent[who].facing == 1 || g_ent[who].facing == 2)
               {
                  if (gbbh * 12 + gby + 40 <= 232)
                     gbby = gby + 24;
                  else
                     gbby = gby - (gbbh * 12) - 24;
               }
             else
               {
                  if (gby - (gbbh * 12) - 24 >= 8)
                     gbby = gby - (gbbh * 12) - 24;
                  else
                     gbby = gby + 24;
               }
          }
        else
          {
             if (gby < 8)
                gbby = 8;
             if (gbbh * 12 + gby + 16 > 232)
                gbby = 216 - (gbbh * 12);
          }
        if (gbby > gby)
          {
             gby += 20;
             if (gbx < 152)
                gbt = 3;
             else
                gbt = 2;
          }
        else
          {
             gby -= 20;
             if (gbx < 152)
                gbt = 1;
             else
                gbt = 0;
          }
        if (gbx < gbbx + 8)
           gbx = gbbx + 8;
        if (gbx > gbbw * 8 + gbbx - 8)
           gbx = gbbw * 8 + gbbx - 8;
        if (gby < gbby - 4)
           gby = gbby - 4;
        if (gby > gbbh * 12 + gbby + 4)
           gby = gbbh * 12 + gbby + 4;
     }
   else
     {
        gbby = 216 - (gbbh * 12);
        gbbx = 152 - (gbbw * 4);
        gbt = -1;
     }
}

/*! \brief Draw text box
 *
 * Hmm... I think this function draws the textbox :p
 * \param bstyle style (B_TEXT or B_THOUGHT)
*/
static void draw_textbox (int bstyle)
{
   int wid, hgt, a;
   BITMAP *tm;

   wid = gbbw * 8 + 16;
   hgt = gbbh * 12 + 16;
   if (bstyle == B_TEXT)
     {
        tm = create_bitmap (wid - 5, hgt - 5);
        rectfill (tm, 0, 0, wid - 5, hgt - 5, BLUE);
        draw_trans_sprite (double_buffer, tm, gbbx + 2 + xofs, gbby + 2 + yofs);
        border (double_buffer, gbbx + xofs, gbby + yofs,
                gbbx + xofs + wid - 1, gbby + yofs + hgt - 1);
        destroy_bitmap (tm);
        if (gbt != -1)
           draw_sprite (double_buffer, bub[gbt], gbx + xofs, gby + yofs);
     }
   else
     {
        tm = create_bitmap (wid - 5, hgt - 5);
        rectfill (tm, 0, 0, wid - 5, hgt - 5, BLUE);
        draw_trans_sprite (double_buffer, tm, gbbx + 2 + xofs, gbby + 2 + yofs);
        draw_sprite (double_buffer, bord[0], gbbx + xofs, gbby + yofs);
        draw_sprite (double_buffer, bord[2], gbbx + xofs + wid - 8,
                     gbby + yofs);
        draw_sprite (double_buffer, bord[5], gbbx + xofs,
                     gbby + yofs + hgt - 8);
        draw_sprite (double_buffer, bord[7], gbbx + xofs + wid - 8,
                     gbby + yofs + hgt - 8);
        for (a = 0; a < gbbw; a++)
          {
             draw_sprite (double_buffer, bord[1], a * 8 + gbbx + xofs + 8,
                          gbby + yofs);
             draw_sprite (double_buffer, bord[6], a * 8 + gbbx + xofs + 8,
                          gbby + yofs + hgt - 8);
          }
        for (a = 0; a < gbbh; a++)
          {
             draw_sprite (double_buffer, bord[3], gbbx + xofs,
                          a * 12 + gbby + yofs + 8);
             draw_sprite (double_buffer, bord[4], gbbx + xofs + wid - 8,
                          a * 12 + gbby + yofs + 8);
          }
        destroy_bitmap (tm);
        if (gbt != -1)
           draw_sprite (double_buffer, bub[gbt + 4], gbx + xofs, gby + yofs);
     }
}

/*! \brief Draw text bubble
 *
 * Draw a regular text bubble and display the text.
 *
 * \param who entity that is speaking
 * \param sp1 Line 1 of text
 * \param sp2 Line 2 of text
 * \param sp3 Line 3 of text
 * \param sp4 Line 4 of text
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
 *
 * Takes a string and re-formats it to fit into the 
 * msgbuf text buffer, for displaying with 
 * generic_text(). 
 * Processes as much as it can to fit in one
 * box, and returns a pointer to the next unprocessed
 * character
 * \author PH
 * \date 20021220
 *
 * \param buf The string to reformat
 * \returns The rest of the string that has not been processed, or NULL if it has all been processed.
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
   while (1)
     {
        tc = buf[i];
        switch (state)
          {
          case M_UNDEF:
             switch (tc)
               {
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
             switch (tc)
               {
               case ' ':
                  if (cc < MSG_COLS)
                    {
                       msgbuf[cr][cc++] = tc;
                       ++i;
                    }
                  break;
               default:
                  state = M_UNDEF;
                  break;
               }
             break;
          case M_NONSPACE:
             switch (tc)
               {
               case ' ':
               case '\0':
               case '\n':
                  state = M_UNDEF;
                  break;
               default:
                  if (cc < MSG_COLS)
                    {
                       msgbuf[cr][cc++] = tc;
                    }
                  else
                    {
                       msgbuf[cr++][lastc] = '\0';
                       cc = 1;
                       i = lasts;
                       if (cr >= MSG_ROWS)
                         {
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

/*! \brief display speech bubble
 *
 * Displays text, like bubble_text, but passing the args
 * through relay() first
 * \author PH
 * \date 20021220
 * \param who Character that is speaking
 * \param s the text to display
 * \sa bubble_text()
 */
void bubble_text_ex (int who, const char *s)
{
   while (s)
     {
        s = relay (s);
        generic_text (who, B_TEXT);
     }
}

/*! \brief display thought bubble
 *
 * Displays text, like thought_text, but passing the args
 * through relay() first
 * \author PH
 * \date 20021220
 * \param who Character that is speaking
 * \param s the text to display
 * \sa thought_text()
 */
void thought_text_ex (int who, const char *s)
{
   while (s)
     {
        s = relay (s);
        generic_text (who, B_THOUGHT);
     }
}

/*! \brief Draw thought bubble
 *
 *  Draw a thought bubble and display the text.
 *
 * \param who entity that is speaking
 * \param sp1 Line 1 of text
 * \param sp2 Line 2 of text
 * \param sp3 Line 3 of text
 * \param sp4 Line 4 of text
 *
 * \sa thought_text_ex()
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
 * \param who character that is speaking/thinking
 * \param box_style style (B_TEXT or B_THOUGHT)
*/
static void generic_text (int who, int box_style)
{
   int a, stop = 0;
   int len;
   gbbw = 1;
   gbbh = 0;
   gbbs = 0;
   for (a = 0; a < 4; a++)
     {
        len = strlen (msgbuf[a]);
        /* FIXME: PH changed >1 to >0 */
        if (len > 0)
          {
             gbbh = a + 1;
             if ((signed int) len > gbbw)
                gbbw = len;
          }
     }
   set_textpos (who);
   if (gbbw == -1 || gbbh == -1)
      return;
   unpress ();
   timer_count = 0;
   while (!stop)
     {
        while (timer_count > 0)
          {
             timer_count--;
             check_animation ();
          }
        drawmap ();
        draw_textbox (box_style);
        for (a = 0; a < gbbh; a++)
          {
             for (a = 0; a < gbbh; a++)
                print_font (double_buffer, gbbx + 8 + xofs,
                            a * 12 + gbby + 8 + yofs, msgbuf[a], FBIG);
          }
        blit2screen (xofs, yofs);
        readcontrols ();
        if (balt)
          {
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
 * \param the_string input string
 * \returns processed string, in a static buffer \p strbuf
 *          or \p the_string, if it had no replacement chars.
*/
const char *parse_string (const char *the_string)
{
   int a, who = -1, flag = 0;
   char crud[255];
   int z;

   if (strlen (the_string) > 35)
      z = 35;
   else
      z = strlen (the_string);
   for (a = 0; a < z; a++)
     {
        if (flag != 1)
          {
             if (the_string[a] != '$')
                crud[a] = the_string[a];
             else
               {
                  flag = 1;
                  crud[a] = '%';
               }
          }
        else
          {
             flag = 2;
             crud[a] = 's';
             who = the_string[a] - 48;
          }
     }
   crud[a] = 0;
   if (flag == 2)
     {
        sprintf (strbuf, crud, party[pidx[who]].name);
        return strbuf;
     }
   else
      return the_string;
}

/*! \brief Do user prompt
 *
 * Draw a text box and wait for a response.  It is possible to offer up to four
 * choices in a prompt box.
 *
 * \param who entity that is speaking
 * \param numopt number of choices
 * \param bstyle textbox style (B_TEXT or B_THOUGHT)
 * \param sp1 Line 1 of text
 * \param sp2 Line 2 of text
 * \param sp3 Line 3 of text
 * \param sp4 Line 4 of text
 * \returns index of option chosen (0..numopt-1)
*/
int prompt (int who, int numopt, int bstyle, char *sp1, char *sp2, char *sp3,
            char *sp4)
{
   int ly, stop = 0, ptr = 0, rd = 1, a;

   gbbw = 1;
   gbbh = 0;
   gbbs = 0;
   strcpy (msgbuf[0], parse_string (sp1));
   strcpy (msgbuf[1], parse_string (sp2));
   strcpy (msgbuf[2], parse_string (sp3));
   strcpy (msgbuf[3], parse_string (sp4));
   unpress ();
   for (a = 0; a < 4; a++)
     {
        if (strlen (msgbuf[a]) > 1)
          {
             gbbh = a + 1;
             if ((signed int) strlen (msgbuf[a]) > gbbw)
                gbbw = strlen (msgbuf[a]);
          }
     }
   set_textpos (who);
   if (gbbw == -1 || gbbh == -1)
      return -1;
   ly = (gbbh - numopt) * 12 + gbby + 10;
   while (!stop)
     {
        if (rd == 1)
          {
             drawmap ();
             draw_textbox (bstyle);
             for (a = 0; a < gbbh; a++)
                print_font (double_buffer, gbbx + 8 + xofs,
                            a * 12 + gbby + 8 + yofs, msgbuf[a], FBIG);
             draw_sprite (double_buffer, menuptr, gbbx + xofs + 8,
                          ptr * 12 + ly + yofs);
             blit2screen (xofs, yofs);
          }
        rd = 0;
        readcontrols ();
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr > numopt - 1)
                ptr = numopt - 1;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             stop = 1;
          }
     }
   return ptr;
}

/*! \brief Alert player
 *
 * Draw a single line message in the center of the screen and wait for
 * the confirm key to be pressed or for a specific amount of time.
 * 
 * \param m message text
 * \param icn icon to display
 * \param delay time to wait (milliseconds?)
 * \param x_m x-coord
 * \param y_m y-coord
*/
void message (char *m, int icn, int delay, int x_m, int y_m)
{
   char msg[41];

   blit (double_buffer, back, x_m, y_m, 0, 0, 352, 280);
   strcpy (msg, parse_string (m));
   if (icn == 255)
     {
        menubox (double_buffer, 152 - (strlen (msg) * 4) + x_m, 108 + y_m,
                 strlen (msg), 1, DARKBLUE);
        print_font (double_buffer, 160 - (strlen (msg) * 4) + x_m, 116 + y_m,
                    msg, FNORMAL);
     }
   else
     {
        menubox (double_buffer, 148 - (strlen (msg) * 4) + x_m, 108 + y_m,
                 strlen (msg) + 1, 1, DARKBLUE);
        draw_icon (double_buffer, icn, 156 - (strlen (msg) * 4) + x_m,
                   116 + y_m);
        print_font (double_buffer, 164 - (strlen (msg) * 4) + x_m, 116 + y_m,
                    msg, FNORMAL);
     }
   blit2screen (x_m, y_m);
   if (delay == 0)
      wait_enter ();
   else
      wait (delay);
   blit (back, double_buffer, 0, 0, x_m, y_m, 352, 280);
}

/*! \brief Adjust view
 *
 * This merely sets the view variables for use in
 * other functions that rely on the view.
 * The view defines a subset of the map,
 * for example when you move to a house in a town,
 * the view contracts to display only the interior.
 *
 * \param vw Non-zero to enable view, otherwise show the whole
 *        map
 * \param x1 top-left of view
 * \param y1 top-left of view
 * \param x2 bottom-right of view
 * \param y2 bottom-right of view
*/
void set_view (int vw, int x1, int y1, int x2, int y2)
{
   view_on = vw;
   if (view_on)
     {
        view_x1 = x1;
        view_y1 = y1;
        view_x2 = x2;
        view_y2 = y2;
     }
   else
     {
        view_y1 = 0;
        view_y2 = g_map.ysize - 1;
        view_x1 = 0;
        view_x2 = g_map.xsize - 1;
     }
}
