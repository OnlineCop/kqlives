#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
/* 
 * This is the KQ map editor 
 * (Pete's hacked version)
 * December 2002
 */
#include "allegro.h"
#include "mapdraw.h"

/* globals */

BITMAP *icons[MAX_TILES], *pcx_buffer, *double_buffer, *mesh, *b_mesh, *z_mesh;
BITMAP *font6, *mpic;
BITMAP *eframes[MAX_EPICS][12];
BITMAP *shadow[MAX_SHADOWS];
unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
unsigned char *z_map, *cz_map, *s_map, *cs_map, *o_map, *co_map;
short icon_set = 0, nomouse = 0, max_sets = 51;
char map_fname[16] = "", *strbuf;
short gx = 0, gy = 0, x = 0, y = 0;
unsigned short int cb;
int draw_mode = 0, curtile = 0, dmode = 0, curzone =
   0, curshadow = 0, curobs = 0;
int copying = 0, copyx = -1, copyy = -1, clipb = 0, cbh = 0, cbw = 0;
ss_map gmap;
s_entity gent[50];
int cent = 0, noe = 0;
unsigned char mousepic[] = {
   15, 00, 00, 00,
   15, 15, 00, 00,
   15, 15, 15, 00,
   15, 15, 15, 15,
   00, 00, 15, 00,
   00, 00, 00, 15
};
char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "Incave.pcx", "village.pcx", "mount.pcx"
};
int htiles = (SW - 80) / 16;
int vtiles = (SH - 48) / 16;

int main (void)
{
   int stop = 0;

   startup ();
   while (!stop)
     {
        process_controls ();
        draw_map ();
        draw_sidebar ();
        if (!nomouse)
           draw_sprite (double_buffer, mpic, mouse_x, mouse_y);
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        if (key[KEY_Q])
           stop = confirm_exit ();
        yield_timeslice ();
     }
   return 0;
}

END_OF_MAIN ();

void klog (char *msg)
{
   FILE *ff;

   ff = fopen ("mapdraw.log", "a");
   if (!ff)
      exit (-1);
   fprintf (ff, "%s\n", msg);
   fclose (ff);
}

void process_controls (void)
{
   int k;

   if (keypressed ())
     {
        k = (readkey () >> 8);
        if (k == KEY_1)
           draw_mode = 0;
        if (k == KEY_4)
           draw_mode = 1;
        if (k == KEY_5)
           draw_mode = 2;
        if (k == KEY_2)
           draw_mode = 9;
        if (k == KEY_3)
           draw_mode = 10;
        if (k == KEY_O)
           draw_mode = 3;
        if (k == KEY_Z)
           draw_mode = 4;
        if (k == KEY_T)
           draw_mode = 5;
        if (k == KEY_S)
           draw_mode = 8;
        if (k == KEY_P)
           draw_mode = 6;
        if (k == KEY_V)
           draw_mode = 11;
        if (k == KEY_C)
           draw_mode = 12;
        if (k == KEY_W)
           wipe_map ();
        if (k == KEY_D)
           displace_entities ();
        if (k == KEY_E)
           clipb = 0;
        if (k == KEY_R)
           resize_map ();
        if (k == KEY_J)
           maptopcx ();
        if (k == KEY_F1)
           load_map ();
        if (k == KEY_F2)
           new_map ();
        if (k == KEY_F3)
           save_map ();
        if (k == KEY_F4)
           clear_layer ();
        if (k == KEY_F5)
           make_mapfrompcx ();
        if (k == KEY_F6)
           global_change ();
        if (k == KEY_F7)
           clear_obstruct ();
        if (k == KEY_F8)
           clear_foreground ();
        if (k == KEY_F9)
           swap_layers ();
        if (k == KEY_F10)
           describe_map ();
        if (k == KEY_F11)
           draw_mode = 7;
        if (k == KEY_F12)
          {
             update_entities ();
          }
        if (k == KEY_ESC)
          {
             if (copying == 1)
               {
                  copying = 0;
                  copyx = -1;
                  copyy = -1;
               }
          }
        if (k == KEY_MINUS)
          {
             switch (draw_mode)
               {
               case 3:
                  curobs--;
                  if (curobs < 0)
                     curobs = 5;
                  break;
               case 4:
                  curzone--;
                  if (curzone < 0)
                     curzone = MAX_ZONES - 1;
                  break;
               case 7:
                  cent--;
                  if (cent < 0)
                     cent = MAX_EPICS - 1;
                  break;
               case 8:
                  curshadow--;
                  if (curshadow < 0)
                     curshadow = MAX_SHADOWS - 1;
                  break;
               default:
                  icon_set--;
                  if (icon_set < 0)
                     icon_set = max_sets;
                  break;
               }
          }
        if (k == KEY_EQUALS)
          {
             switch (draw_mode)
               {
               case 3:
                  curobs++;
                  if (curobs >= 5)
                     curobs = 0;
                  break;
               case 4:
                  curzone++;
                  if (curzone >= MAX_ZONES)
                     curzone = 0;
                  break;
               case 7:
                  cent++;
                  if (cent == MAX_EPICS)
                     cent = 0;
                  break;
               case 8:
                  curshadow++;
                  if (curshadow >= MAX_SHADOWS)
                     curshadow = 0;
                  break;
               default:
                  icon_set++;
                  if (icon_set > max_sets)
                     icon_set = 0;
                  break;
               }
          }
        if (k == KEY_UP)
           gy--;
        if (k == KEY_DOWN)
           gy++;
        if (k == KEY_RIGHT)
           gx++;
        if (k == KEY_LEFT)
           gx--;
        if (k == KEY_PGUP)
           gy -= vtiles;
        if (k == KEY_PGDN)
           gy += vtiles;
        if (k == KEY_TAB)
           gx += htiles;
        if (k == KEY_BACKSPACE)
           gx -= htiles;
     }
   if (gx > gmap.xsize - htiles)
      gx = gmap.xsize - htiles;
   if (gx < 0)
      gx = 0;
   if (gy > gmap.ysize - vtiles)
      gy = gmap.ysize - vtiles;
   if (gy < 0)
      gy = 0;
   if (!(mouse_b & 1))
      dmode = 0;
   if (mouse_b & 1)
     {
        x = mouse_x / 16;
        y = mouse_y / 16;
        if (y > (vtiles - 1))
          {
             if (dmode == 0)
               {
                  check_mdupdate (mouse_x, mouse_y);
                  return;
               }
             else
                y = vtiles - 1;
          }
        if (x > (htiles - 1) && dmode == 0)
           check_tilesel (mouse_x, mouse_y);
        else
          {
             if (x > (htiles - 1))
                x = htiles - 1;
             dmode = 1;
             if (draw_mode == 0)
                map[((gy + y) * gmap.xsize) + gx + x] = curtile;
             if (draw_mode == 1 || draw_mode == 9)
                b_map[((gy + y) * gmap.xsize) + gx + x] = curtile;
             if (draw_mode == 2 || draw_mode == 10)
                f_map[((gy + y) * gmap.xsize) + gx + x] = curtile;
             if (draw_mode == 3)
                o_map[((gy + y) * gmap.xsize) + gx + x] = curobs;
             if (draw_mode == 4)
                z_map[((gy + y) * gmap.xsize) + gx + x] = curzone;
             if (draw_mode == 5 && copying == 0)
               {
                  copying = 1;
                  copyx = gx + x;
                  copyy = gy + y;
               }
             if (draw_mode == 6 && clipb != 0)
                paste_region (gx + x, gy + y);
             if (draw_mode == 7)
                place_entity (mouse_x / 16 + gx, mouse_y / 16 + gy);
             if (draw_mode == 8)
                s_map[((gy + y) * gmap.xsize) + gx + x] = curshadow;
          }
     }
   /* Selective paste */
   if ((mouse_b & 2) && (draw_mode == 6) && (clipb != 0))
     {
        paste_region_special (gx + x, gy + y);
     }
   if ((mouse_b & 2) && draw_mode > 2 && draw_mode != 6)
     {
        x = mouse_x / 16;
        y = mouse_y / 16;
        if (y > (vtiles - 1))
           y = vtiles - 1;
        if (x > (htiles - 1))
           x = htiles - 1;
        if (draw_mode == 3)
           o_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 4)
           z_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 5 && copying == 1)
          {
             copy_region (gx + x, gy + y);
             copying = 0;
          }
        if (draw_mode == 7)
           erase_entity (mouse_x / 16 + gx, mouse_y / 16 + gy);
        if (draw_mode == 8)
           s_map[((gy + y) * gmap.xsize) + gx + x] = 0;
     }
   if ((mouse_b & 2) && (draw_mode < 3 || draw_mode > 8))
     {
        x = mouse_x / 16;
        y = mouse_y / 16;
        if (y > (vtiles - 1))
           y = vtiles - 1;
        if (x > (htiles - 1))
           x = htiles - 1;
        dmode = 1;
        if (draw_mode == 0)
           map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 1 || draw_mode == 9)
           b_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 2 || draw_mode == 10)
           f_map[((gy + y) * gmap.xsize) + gx + x] = 0;
     }
   x = mouse_x / 16;
   y = mouse_y / 16;
   if (y > (vtiles - 1))
      y = vtiles - 1;
   if (x > (htiles - 1))
      x = htiles - 1;
}

int confirm_exit (void)
{
   cmessage ("Are you sure you want to exit? (y/n)");
   return yninput ();
}

void check_tilesel (int cx, int cy)
{
   int xp, yp;

   if (cx >= (SW - 64) && cx <= (SW - 33) && cy >= 0 && cy <= 159)
     {
        xp = (cx - (SW - 64)) / 16;
        yp = cy / 16;
        curtile = icon_set * 20 + (xp * 10 + yp);
     }
}

void check_mdupdate (int cx, int cy)
{
   int a;

   if (cx >= 0 && cx <= 159 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        gmap.tileset++;
        if (gmap.tileset >= NUM_TILESETS)
           gmap.tileset = 0;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 0 && cx <= 159 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 36, (SH - 34), 159, (SH - 29), 0);
        print_sfont (30, (SH - 34), ">", screen);
        hline (screen, 36, (SH - 29), 113, 255);
        a = get_line (36, (SH - 34), strbuf, 13);
        if (a != 0)
           strcpy (gmap.song_file, strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 0 && cx <= 159 && cy >= (SH - 28) && cy <= (SH - 23))
     {
        gmap.zero_zone = 1 - gmap.zero_zone;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 0 && cx <= 79 && cy >= (SH - 22) && cy <= (SH - 17))
     {
        rectfill (screen, 42, (SH - 22), 159, (SH - 17), 0);
        print_sfont (36, (SH - 22), ">", screen);
        hline (screen, 42, (SH - 17), 65, 255);
        a = get_line (42, (SH - 22), strbuf, 4);
        if (a != 0)
           gmap.map_no = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 0 && cx <= 79 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        gmap.map_mode++;
        if (gmap.map_mode > 5)
           gmap.map_mode = 0;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 0 && cx <= 79 && cy >= (SH - 10) && cy <= (SH - 5))
     {
        gmap.can_save = 1 - gmap.can_save;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 80 && cx <= 159 && cy >= (SH - 22) && cy <= (SH - 17))
     {
        gmap.can_warp = 1 - gmap.can_warp;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 80 && cx <= 159 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        rectfill (screen, 122, (SH - 16), 159, (SH - 11), 0);
        print_sfont (116, (SH - 16), ">", screen);
        hline (screen, 122, (SH - 11), 145, 255);
        a = get_line (122, (SH - 16), strbuf, 4);
        if (a != 0)
           gmap.warpx = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 80 && cx <= 159 && cy >= (SH - 10) && cy <= (SH - 5))
     {
        rectfill (screen, 122, (SH - 10), 159, (SH - 5), 0);
        print_sfont (116, (SH - 10), ">", screen);
        hline (screen, 122, (SH - 5), 145, 255);
        a = get_line (122, (SH - 10), strbuf, 4);
        if (a != 0)
           gmap.warpy = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 160 && cx <= 239 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        rectfill (screen, 214, (SH - 40), 239, (SH - 35), 0);
        print_sfont (208, (SH - 40), ">", screen);
        hline (screen, 214, (SH - 35), 237, 255);
        a = get_line (214, (SH - 40), strbuf, 4);
        if (a != 0)
           gmap.stx = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 160 && cx <= 239 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 214, (SH - 34), 239, (SH - 29), 0);
        print_sfont (208, (SH - 34), ">", screen);
        hline (screen, 214, (SH - 29), 237, 255);
        a = get_line (214, (SH - 34), strbuf, 4);
        if (a != 0)
           gmap.sty = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 240 && cx <= 319 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        rectfill (screen, 276, (SH - 40), 319, (SH - 35), 0);
        print_sfont (270, (SH - 40), ">", screen);
        hline (screen, 276, (SH - 35), 299, 255);
        a = get_line (276, (SH - 40), strbuf, 4);
        if (a != 0)
           gmap.pmult = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 240 && cx <= 319 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 270, (SH - 34), 319, (SH - 29), 0);
        print_sfont (264, (SH - 34), ">", screen);
        hline (screen, 270, (SH - 29), 293, 255);
        a = get_line (270, (SH - 34), strbuf, 4);
        if (a != 0)
           gmap.pdiv = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 160 && cx <= 319 && cy >= (SH - 28) && cy <= (SH - 17))
      resize_map ();
   if (cx >= 160 && cx <= 239 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        gmap.use_sstone = 1 - gmap.use_sstone;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }

}

void draw_map (void)
{
   int dx, dy, d, w, maxx, maxy;

   rectfill (double_buffer, 0, 0, (SW - 81), (SH - 49), 0);
   if (vtiles > gmap.ysize)
      maxy = gmap.ysize;
   else
      maxy = vtiles;
   if (htiles > gmap.xsize)
      maxx = gmap.xsize;
   else
      maxx = htiles;
   for (dy = 0; dy < maxy; dy++)
     {
        for (dx = 0; dx < maxx; dx++)
          {
             w = ((gy + dy) * gmap.xsize) + gx + dx;
             if (draw_mode < 9 || draw_mode > 10)
                blit (icons[map[w]], double_buffer, 0, 0, dx * 16, dy * 16,
                      16, 16);
             if (draw_mode > 8 && draw_mode != 11 && draw_mode != 12)
                rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                          dy * 16 + 15, 0);
             if (draw_mode != 0 && draw_mode != 10)
                draw_sprite (double_buffer, icons[b_map[w]], dx * 16, dy * 16);
             if (draw_mode > 1 && draw_mode != 9)
                draw_sprite (double_buffer, icons[f_map[w]], dx * 16, dy * 16);
             if (draw_mode == 3)
               {
                  switch (o_map[w])
                    {
                    case 1:
                       draw_sprite (double_buffer, mesh, dx * 16, dy * 16);
                       break;
                    case 2:
                       hline (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                              255);
                       vline (double_buffer, dx * 16 + 8, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    case 3:
                       hline (double_buffer, dx * 16, dy * 16 + 8,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16 + 15, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    case 4:
                       hline (double_buffer, dx * 16, dy * 16 + 15,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16 + 8, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    case 5:
                       hline (double_buffer, dx * 16, dy * 16 + 8,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16, dy * 16, dy * 16 + 15,
                              255);
                       break;
                    }
               }
             if (draw_mode == 4)
                if (z_map[w] > 0)
                   draw_sprite (double_buffer, z_mesh, dx * 16, dy * 16);
             if (draw_mode == 8)
                if (s_map[w] > 0)
                   draw_trans_sprite (double_buffer, shadow[s_map[w]],
                                      dx * 16, dy * 16);
          }
     }
   if (draw_mode == 7)
     {
        for (d = 0; d < noe; d++)
          {
             if (gent[d].tilex >= gx && gent[d].tilex <= gx + (htiles - 1)
                 && gent[d].tiley >= gy && gent[d].tiley <= gy + (vtiles - 1))
               {
                  if (gent[d].transl == 0)
                     draw_sprite (double_buffer,
                                  eframes[gent[d].chrx][gent[d].facing * 3],
                                  (gent[d].tilex - gx) * 16,
                                  (gent[d].tiley - gy) * 16);
                  else
                     draw_trans_sprite (double_buffer,
                                        eframes[gent[d].chrx][gent[d].facing *
                                                              3],
                                        (gent[d].tilex - gx) * 16,
                                        (gent[d].tiley - gy) * 16);
               }
          }
     }
}

void draw_sidebar (void)
{
   int p, xp, yp, a;
   char dt[11][16] =
      { "Layer1", "Layer1+2", "Layer1+2+3", "Obstacles", "Zones", "Copying",
      "Pasting", "Entity", "Shadows", "Only-2", "Only-3"
   };

   hline (double_buffer, 0, (SH - 48), (SW - 81), 255);
   rectfill (double_buffer, (SW - 72), 0, (SW - 1), (SH - 1), 0);
   rectfill (double_buffer, 0, (SH - 47), (SW - 73), (SH - 1), 0);
   rectfill (double_buffer, (SW - 80), 0, (SW - 73), (SH - 49), 255);
   sprintf (strbuf, "Map: %s", map_fname);
   print_sfont (0, (SH - 46), strbuf, double_buffer);
   sprintf (strbuf, "Icon: %s", icon_files[gmap.tileset]);
   print_sfont (0, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Song: %s", gmap.song_file);
   print_sfont (0, (SH - 34), strbuf, double_buffer);
   if (gmap.zero_zone == 0)
      print_sfont (0, (SH - 28), "ZeroZone: NO", double_buffer);
   else
      print_sfont (0, (SH - 28), "ZeroZone: YES", double_buffer);
   sprintf (strbuf, "Map #: %d", gmap.map_no);
   print_sfont (0, (SH - 22), strbuf, double_buffer);
   sprintf (strbuf, "Mode: %d", gmap.map_mode);
   print_sfont (0, (SH - 16), strbuf, double_buffer);
   if (gmap.can_save == 0)
      print_sfont (0, (SH - 10), "Save: NO", double_buffer);
   else
      print_sfont (0, (SH - 10), "Save: YES", double_buffer);
   if (gmap.can_warp == 0)
      print_sfont (80, (SH - 22), "Warp: NO", double_buffer);
   else
      print_sfont (80, (SH - 22), "Warp: YES", double_buffer);
   sprintf (strbuf, "WarpX: %d", gmap.warpx);
   print_sfont (80, (SH - 16), strbuf, double_buffer);
   sprintf (strbuf, "WarpY: %d", gmap.warpy);
   print_sfont (80, (SH - 10), strbuf, double_buffer);
   sprintf (strbuf, "Start X: %d", gmap.stx);
   print_sfont (160, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Start Y: %d", gmap.sty);
   print_sfont (160, (SH - 34), strbuf, double_buffer);
   sprintf (strbuf, "Width: %d", gmap.xsize);
   print_sfont (160, (SH - 28), strbuf, double_buffer);
   sprintf (strbuf, "Height: %d", gmap.ysize);
   print_sfont (160, (SH - 22), strbuf, double_buffer);
   sprintf (strbuf, "Mult: %d", gmap.pmult);
   print_sfont (240, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Div: %d", gmap.pdiv);
   print_sfont (240, (SH - 34), strbuf, double_buffer);
   print_sfont (160, (SH - 16),
                gmap.use_sstone == 0 ? "SunStone: NO" : "SunStone: YES",
                double_buffer);
   a = 0;
   for (p = 0; p < gmap.xsize * gmap.ysize; p++)
      if (z_map[p] > a)
         a = z_map[p];
   sprintf (strbuf, "Last zone: %d", a);
   print_sfont (160, (SH - 10), strbuf, double_buffer);
   if (icon_set != 999)
     {
        for (p = 0; p < 10; p++)
          {
             blit (icons[icon_set * 20 + p], double_buffer, 0, 0, (SW - 64),
                   p * 16, 16, 16);
             blit (icons[icon_set * 20 + p + 10], double_buffer, 0, 0,
                   (SW - 48), p * 16, 16, 16);
          }
     }
   if (curtile >= icon_set * 20 && curtile <= icon_set * 20 + 19)
     {
        xp = curtile - (icon_set * 20);
        yp = xp;
        xp = xp / 10;
        yp = yp - (xp * 10);
        rect (double_buffer, xp * 16 + (SW - 64), yp * 16,
              xp * 16 + (SW - 49), yp * 16 + 15, 255);
     }
   rectfill (double_buffer, (SW - 72), 164, (SW - 1), (SH - 1), 0);
   print_sfont ((SW - 72), 164, "Mode:", double_buffer);
   print_sfont ((SW - 64), 170, dt[draw_mode], double_buffer);
   sprintf (strbuf, "#%d(%d)", icon_set, curtile);
   print_sfont ((SW - 72), 176, strbuf, double_buffer);
   sprintf (strbuf, "x=%d", gx + x);
   print_sfont ((SW - 72), 182, strbuf, double_buffer);
   sprintf (strbuf, "y=%d", gy + y);
   print_sfont ((SW - 72), 188, strbuf, double_buffer);
   if (dmode == 1)
      print_sfont ((SW - 64), 194, "drawing", double_buffer);
   if (draw_mode == 3)
     {
        sprintf (strbuf, "Obs #%d", curobs);
        print_sfont (104, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = o_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (200, (SH - 46), strbuf, double_buffer);
          }
     }
   if (draw_mode == 4)
     {
        sprintf (strbuf, "Zone #%d", curzone);
        print_sfont (104, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = z_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (200, (SH - 46), strbuf, double_buffer);
          }
     }
   if (draw_mode == 5 && copying == 1)
     {
        sprintf (strbuf, "From: %d,%d", copyx, copyy);
        print_sfont (100, (SH - 46), strbuf, double_buffer);
     }
   if (draw_mode == 7)
     {
        sprintf (strbuf, "%d", cent);
        print_sfont ((SW - 14), (SH - 38), strbuf, double_buffer);
        blit (eframes[cent][0], double_buffer, 0, 0, (SW - 16), (SH - 32), 16,
              16);
        sprintf (strbuf, "%d", noe);
        print_sfont ((SW - 14), (SH - 12), strbuf, double_buffer);
     }
   if (draw_mode == 8)
     {
        sprintf (strbuf, "Shd. #%d", curshadow);
        print_sfont (140, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = s_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (200, (SH - 46), strbuf, double_buffer);
          }
     }
   rect (double_buffer, x * 16, y * 16, x * 16 + 15, y * 16 + 15, 255);
}

void bufferize (void)
{
   free (map);
   map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (b_map);
   b_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (f_map);
   f_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (s_map);
   s_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (o_map);
   o_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (c_map);
   c_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cb_map);
   cb_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cf_map);
   cf_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cz_map);
   cz_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (cs_map);
   cs_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (co_map);
   co_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (s_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   clipb = 0;
}



void global_change (void)
{
   int ld, ft = 0, tt, p;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "From Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld != 0)
      ft = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "To Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld != 0)
     {
        tt = atoi (strbuf);
        for (p = 0; p < (gmap.xsize * gmap.ysize); p++)
           if (map[p] == ft)
              map[p] = tt;
     }
}

void clear_obstruct (void)
{
   int co;

   for (co = 0; co < gmap.xsize * gmap.ysize; co++)
      o_map[co] = 0;
}

void clear_foreground (void)
{
   int co;

   for (co = 0; co < gmap.xsize * gmap.ysize; co++)
      f_map[co] = 0;
}

int get_line (int gx, int gy, char *buffer, int max_len)
{
   int index = 0, ch;
   BITMAP *under;

   under = create_bitmap (320, 6);
   blit (screen, under, 0, gy, 0, 0, 320, 6);
   while (1)
     {
        ch = (readkey () & 0xff);
        if (ch >= 32 && ch <= 126 && index < max_len - 1)
          {
             buffer[index] = ch;
             buffer[index + 1] = 0;
             print_sfont (gx, gy, buffer, screen);
             if (++index == max_len)
                index = max_len - 1;
          }
        else
          {
             if (ch == 13)
               {
                  buffer[index] = 0;
                  destroy_bitmap (under);
                  return (1);
               }
             else
               {
                  if (ch == 8 || ch == 127)
                    {
                       if (--index < 0)
                          index = 0;
                       buffer[index] = ' ';
                       buffer[index + 1] = 0;
                       blit (under, screen, 0, 0, 0, gy, 320, 8);
                       print_sfont (gx, gy, buffer, screen);
                       buffer[index] = 0;
                    }
                  else
                    {
                       if (ch == 27)
                         {
                            destroy_bitmap (under);
                            return (0);
                         }
                    }
               }
          }
     }
}

int yninput (void)
{
   int ch, done = 0;

   while (!done)
     {
        ch = (readkey () >> 8);
        if (ch == KEY_N)
           done = 1;
        if (ch == KEY_Y)
           done = 2;
     }
   return done - 1;
}

void startup (void)
{
   int k, kx, ky, a, p, q;
   COLOR_MAP cmap;

   allegro_init ();
   install_keyboard ();
   install_timer ();
   if (WBUILD == 1)
      set_gfx_mode (GFX_AUTODETECT_WINDOWED, SW, SH, 0, 0);
   else
      set_gfx_mode (GFX_AUTODETECT, SW, SH, 0, 0);
   a = install_mouse ();
   if (a == -1)
     {
        nomouse = 1;
        klog ("Mouse not found!\n");
        rest (1000);
     }
   mpic = create_bitmap (4, 6);
   for (ky = 0; ky < 6; ky++)
      for (kx = 0; kx < 4; kx++)
         mpic->line[ky][kx] = mousepic[ky * 4 + kx];
   set_mouse_speed (4, 4);
   strbuf = (char *) malloc (256);
   double_buffer = create_bitmap (SW, SH);
   clear (double_buffer);
   gmap.map_no = -1;
   gmap.tileset = 0;
   gmap.xsize = htiles;
   gmap.ysize = vtiles;
   set_palette (pal);
   for (k = 0; k < MAX_TILES; k++)
     {
        icons[k] = create_bitmap (16, 16);
        clear (icons[k]);
     }
   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);
   for (p = 0; p < max_sets; p++)
      for (q = 0; q < 20; q++)
         blit (pcx_buffer, icons[p * 20 + q], q * 16, p * 16, 0, 0, 16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
   bufferize ();
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;
   font6 = create_bitmap (6, 546);
   getfont ();
   mesh = create_bitmap (16, 16);
   clear (mesh);
   for (ky = 0; ky < 16; ky += 2)
     {
        for (kx = 0; kx < 16; kx += 2)
           putpixel (mesh, kx, ky, 255);
        for (kx = 1; kx < 16; kx += 2)
           putpixel (mesh, kx, ky + 1, 255);
     }
   b_mesh = create_bitmap (16, 16);
   clear (b_mesh);
   for (ky = 0; ky < 16; ky += 2)
     {
        for (kx = 0; kx < 16; kx += 2)
           putpixel (b_mesh, kx, ky, 1);
        for (kx = 1; kx < 16; kx += 2)
           putpixel (b_mesh, kx, ky + 1, 1);
     }
   z_mesh = create_bitmap (16, 16);
   clear (z_mesh);
   hline (z_mesh, 0, 7, 15, 255);
   hline (z_mesh, 0, 8, 15, 255);
   vline (z_mesh, 7, 0, 15, 255);
   vline (z_mesh, 8, 0, 15, 255);
   print_sfont (5, 5, "Z", z_mesh);
   pcx_buffer = load_pcx ("Misc.pcx", pal);
   for (a = 0; a < MAX_SHADOWS; a++)
     {
        shadow[a] = create_bitmap (16, 16);
        blit (pcx_buffer, shadow[a], a * 16, 160, 0, 0, 16, 16);
     }
   destroy_bitmap (pcx_buffer);
   pcx_buffer = load_pcx ("entities.pcx", pal);
   for (k = 0; k < MAX_EPICS; k++)
     {
        for (a = 0; a < 12; a++)
          {
             eframes[k][a] = create_bitmap (16, 16);
             blit (pcx_buffer, eframes[k][a], a * 16, k * 16, 0, 0, 16, 16);
          }
     }
   destroy_bitmap (pcx_buffer);
   init_entities ();
}

void cleanup (void)
{
   int k, j;

   for (k = 0; k < MAX_TILES; k++)
      destroy_bitmap (icons[k]);
   for (k = 0; k < MAX_EPICS; k++)
      for (j = 0; j < 12; j++)
         destroy_bitmap (eframes[k][j]);
   destroy_bitmap (double_buffer);
   free (map);
   free (f_map);
   free (z_map);
   free (s_map);
   free (o_map);
   free (c_map);
   free (cf_map);
   free (cz_map);
   free (cs_map);
   free (co_map);
   free (strbuf);
   destroy_bitmap (mesh);
   destroy_bitmap (b_mesh);
   destroy_bitmap (z_mesh);
   for (k = 0; k < MAX_SHADOWS; k++)
      destroy_bitmap (shadow[k]);
}


void wait_enter (void)
{
   while ((readkey () >> 8) != KEY_ENTER);
}

void cmessage (char *buff)
{
   int cx;

   cx = strlen (buff) * 3;
   rectfill (screen, 154 - cx, 91, 165 + cx, 108, 0);
   rect (screen, 154 - cx, 91, 165 + cx, 108, 255);
   print_sfont (160 - cx, 97, buff, screen);
}

void paste_region_special (int tx, int ty)
{
   int zx, zy, bf;
   int p1, p2, p3, ps, po, pz;
   char opts[9];
   if (clipb == 0)
      return;
   memset (opts, 0, sizeof (opts));
   rectfill (screen, 138, 89, 300, 120, 0);
   print_sfont (140, 91, "Paste which layers?(123soz)", screen);
   rect (screen, 138, 89, 300, 120, 255);
   get_line (160, 110, opts, sizeof (opts) - 1);
   p1 = strchr (opts, '1') ? 1 : 0;
   p2 = strchr (opts, '2') ? 1 : 0;
   p3 = strchr (opts, '3') ? 1 : 0;
   ps = strchr (opts, 's') ? 1 : 0;
   po = strchr (opts, 'o') ? 1 : 0;
   pz = strchr (opts, 'z') ? 1 : 0;
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             if (ty + zy <= gmap.ysize && tx + zx <= gmap.xsize)
               {
                  if (p1)
                    {
                       bf = c_map[zy * gmap.xsize + zx];
                       map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                    }
                  if (p2)
                    {
                       bf = cb_map[zy * gmap.xsize + zx];
                       b_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                    }
                  if (p3)
                    {
                       bf = cf_map[zy * gmap.xsize + zx];
                       f_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                    }
                  if (pz)
                    {
                       bf = cz_map[zy * gmap.xsize + zx];
                       z_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                    }
                  if (ps)
                    {
                       bf = cs_map[zy * gmap.xsize + zx];
                       s_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                    }
                  if (po)
                    {
                       bf = co_map[zy * gmap.xsize + zx];
                       o_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                    }
               }
          }
     }
}
void paste_region (int tx, int ty)
{
   int zx, zy, bf;

   if (clipb == 0)
      return;
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             if (ty + zy <= gmap.ysize && tx + zx <= gmap.xsize)
               {
                  bf = c_map[zy * gmap.xsize + zx];
                  map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cb_map[zy * gmap.xsize + zx];
                  b_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cf_map[zy * gmap.xsize + zx];
                  f_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cz_map[zy * gmap.xsize + zx];
                  z_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cs_map[zy * gmap.xsize + zx];
                  s_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = co_map[zy * gmap.xsize + zx];
                  o_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
               }
          }
     }
}

void copy_region (int tx, int ty)
{
   int zx, zy, bf;

   clipb = 0;
   cbh = ty - copyy;
   cbw = tx - copyx;
   if (cbh < 1 && cbw < 1)
      return;
   clipb = 1;
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             bf = map[(copyy + zy) * gmap.xsize + copyx + zx];
             c_map[zy * gmap.xsize + zx] = bf;
             bf = b_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cb_map[zy * gmap.xsize + zx] = bf;
             bf = f_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cf_map[zy * gmap.xsize + zx] = bf;
             bf = z_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cz_map[zy * gmap.xsize + zx] = bf;
             bf = s_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cs_map[zy * gmap.xsize + zx] = bf;
             bf = o_map[(copyy + zy) * gmap.xsize + copyx + zx];
             co_map[zy * gmap.xsize + zx] = bf;
          }
     }
}

void resize_map (void)
{
   int ld, nw, nh, zx, zy, bf;
   int ow, oh;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Width: ", screen);
   ld = get_line (48, 18, strbuf, 4);
   if (ld != 0)
      nw = atoi (strbuf);
   else
      return;
   if (nw < 20 || nw > 640)
     {
        cmessage ("Illegal width!");
        wait_enter ();
        return;
     }
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Height: ", screen);
   ld = get_line (54, 18, strbuf, 4);
   if (ld != 0)
      nh = atoi (strbuf);
   else
      return;
   if (nh < 15 || nh > 480)
     {
        cmessage ("Illegal height!");
        wait_enter ();
        return;
     }
   for (zy = 0; zy < gmap.ysize; zy++)
     {
        for (zx = 0; zx < gmap.xsize; zx++)
          {
             bf = map[zy * gmap.xsize + zx];
             c_map[zy * gmap.xsize + zx] = bf;
             bf = b_map[zy * gmap.xsize + zx];
             cb_map[zy * gmap.xsize + zx] = bf;
             bf = f_map[zy * gmap.xsize + zx];
             cf_map[zy * gmap.xsize + zx] = bf;
             bf = z_map[zy * gmap.xsize + zx];
             cz_map[zy * gmap.xsize + zx] = bf;
             bf = s_map[zy * gmap.xsize + zx];
             cs_map[zy * gmap.xsize + zx] = bf;
             bf = o_map[zy * gmap.xsize + zx];
             co_map[zy * gmap.xsize + zx] = bf;
          }
     }
   ow = gmap.xsize;
   oh = gmap.ysize;
   gmap.xsize = nw;
   gmap.ysize = nh;
   free (map);
   map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (f_map);
   f_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (s_map);
   s_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (o_map);
   o_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (s_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   for (zy = 0; zy < oh; zy++)
     {
        for (zx = 0; zx < ow; zx++)
          {
             if (zy < gmap.ysize && zx < gmap.xsize)
               {
                  bf = c_map[zy * ow + zx];
                  map[zy * gmap.xsize + zx] = bf;
                  bf = cb_map[zy * ow + zx];
                  b_map[zy * gmap.xsize + zx] = bf;
                  bf = cf_map[zy * ow + zx];
                  f_map[zy * gmap.xsize + zx] = bf;
                  bf = cz_map[zy * ow + zx];
                  z_map[zy * gmap.xsize + zx] = bf;
                  bf = cs_map[zy * ow + zx];
                  s_map[zy * gmap.xsize + zx] = bf;
                  bf = co_map[zy * ow + zx];
                  o_map[zy * gmap.xsize + zx] = bf;
               }
          }
     }
   free (c_map);
   c_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cb_map);
   cb_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cf_map);
   cf_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cz_map);
   cz_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (cs_map);
   cs_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (co_map);
   co_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   clipb = 0;
}



void wipe_map (void)
{
   int r;

   rectfill (screen, 0, 0, 319, 17, 0);
   rect (screen, 2, 2, 317, 15, 255);
   print_sfont (6, 6, "Really clear the whole map? (y/n)", screen);
   r = yninput ();
   if (r)
     {
        memset (map, 0, gmap.xsize * gmap.ysize * 2);
        memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
        memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
        memset (z_map, 0, gmap.xsize * gmap.ysize);
        memset (s_map, 0, gmap.xsize * gmap.ysize);
        memset (o_map, 0, gmap.xsize * gmap.ysize);
     }
}



void describe_map (void)
{
   int ld;
   char tcd[39];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Enter map description", screen);
   sprintf (strbuf, "Current: %s", gmap.map_desc);
   print_sfont (6, 12, strbuf, screen);
   print_sfont (6, 18, "Filename: ", screen);
   ld = get_line (66, 18, tcd, 38);
   if (ld != 0)
      strcpy (gmap.map_desc, tcd);
}

void swap_layers (void)
{
   int rt, rt2, ff, tt, a, b;
   char ts[4];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Copy Layer to Layer", screen);
   print_sfont (6, 12, "From (1-3):", screen);
   rt = get_line (78, 12, ts, 2);
   if (rt != 0)
     {
        ff = atoi (ts);
        print_sfont (6, 18, "To (1-3):", screen);
        rt2 = get_line (78, 18, ts, 2);
        if (rt2 != 0)
          {
             tt = atoi (ts);
             for (a = 0; a < gmap.xsize * gmap.ysize; a++)
               {
                  if (ff == 1)
                     b = map[a];
                  else
                    {
                       if (ff == 2)
                          b = b_map[a];
                       else
                          b = f_map[a];
                    }
                  if (tt == 1)
                     map[a] = b;
                  else
                    {
                       if (tt == 2)
                          b_map[a] = b;
                       else
                          f_map[a] = b;
                    }
               }
          }
     }
}

void clear_layer (void)
{
   int rt, ml, a;
   char ts[4];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Clear Layer", screen);
   print_sfont (6, 12, "Layer (1-3):", screen);
   rt = get_line (84, 12, ts, 2);
   if (rt != 0)
     {
        ml = atoi (ts);
        for (a = 0; a < gmap.xsize * gmap.ysize; a++)
          {
             if (ml == 1)
                map[a] = 0;
             else
               {
                  if (ml == 2)
                     b_map[a] = 0;
                  else
                     f_map[a] = 0;
               }
          }
     }
}
void print_sfont (int x, int y, char *string, BITMAP * where)
{
   int i, c;

   for (i = 0; i < (signed) strlen (string); i++)
     {
        c = string[i];
        c -= 32;
        if (c < 0)
           c = 0;
        masked_blit (font6, where, 0, c * 6, i * 6 + x, y, 6, 6);
     }
}
