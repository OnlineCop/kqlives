/***************************************************************************\
 * This map editor is for the KQ game, supported by the kqlives community. *
 * Comments and suggestions about the editor are welcome.  Please join our *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapedit.c contains the bulk of the code, with all the routines for the  *
 * map editor itself (looks, layout, feel, etc.).  If the editor has a bug *
 * it would probably be here.                                              *
\***************************************************************************/


#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mapdraw.h"

/* globals */

/* Selectable tiles on the right-hand menu */
BITMAP *icons[MAX_TILES];

/* Image and screen buffers */
BITMAP *pcx_buffer, *double_buffer;

/* White checker-board overlay for obstacle */
BITMAP *mesh;

/* Font and mouse images */
BITMAP *font6, *mouse_pic;

/* Gee, could it be an Entity frame? */
BITMAP *eframes[MAX_EPICS][12];

/* This is for a shadow.  It's dark. */
BITMAP *shadow[MAX_SHADOWS];

/* These are for the Layers 1-3 */
unsigned short *map, *b_map, *f_map, *c_map, *cb_map, *cf_map;

/* These are for the Zone, Shadow and Obstacle Attributes */
unsigned char *o_map, *sh_map, *z_map, *co_map, *csh_map, *cz_map;

/* Used for the right-hand menu, plus something for the mouse */
short icon_set = 0, max_sets = 51, nomouse = 0;

/* Stores the name of the currently loaded map */
char map_fname[16] = "";

/* Used everywhere for strings */
char *strbuf;

/* window_x and window_y are view-window coords
 * x and y are everything else
 */
short window_x = 0, window_y = 0, x = 0, y = 0;

int draw_mode = MAP_LAYER123, curtile = 0, dmode = 0;

int curzone = 0, curshadow = 0, curobs = 0;
int copying = 0, copyx1 = -1, copyx2 = -1, copyy1 = -1, copyy2 = -1;
int clipb = 0, cbh = 0, cbw = 0;
int needupdate;

s_map gmap;
s_entity gent[50];
s_show showing;

/*! Tile animation specifiers for each tile set */
/* Format: {starting_tile, finishing_tile, animation_speed}
 * You may have up to 5 animations per tile set and tiles
 * must be sequential; they cannot jump around on the set.
 */
s_anim tanim[7][MAX_ANIM] = {
   /* land.pcx */
   {{2, 5, 25}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   /* newtown.pcx */
   {{158, 159, 50}, {160, 163, 25}, {176, 179, 25}, {257, 258, 50},
    {262, 263, 25}},
   /* castle.pcx */
   {{57, 58, 50}, {62, 63, 25}, {205, 206, 50}, {250, 253, 25}, {0, 0, 0}},
   /* incave.pcx */
   {{30, 35, 30}, {176, 179, 25}, {323, 328, 40}, {380, 385, 40},
    {360, 365, 30}},
   /* village.pcx */
   {{38, 39, 25}, {80, 83, 25}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   /* mount.pcx */
   {{58, 59, 50}, {40, 42, 50}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   /* shrine.bmp */
   {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};

/*! Tile animation specifiers for the current tileset */
s_anim adata[MAX_ANIM];

/* Number of entities, index of currently-selected entity */
int number_of_ents = 0, current_ent = 0;

/* Our undisputed mouse picture.  Wow. */
unsigned char mousepic[] = {
   15, 00, 00, 00,
   15, 15, 00, 00,
   15, 15, 15, 00,
   15, 15, 15, 15,
   00, 00, 15, 00,
   00, 00, 00, 15
};

/* Tileset images */
char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "Incave.pcx", "village.pcx", "mount.pcx",
   "shrine.pcx"
};

/* The map modes (parallax and drawing order) are listed here in
 * coded format. The layers are listed as 1, 2, 3, E (entity) S (shadow)
 * and a ) or ( marks which layers use the parallax mult/div.
 */
static const char *map_mode_text[] = {
   "12E3S ",
   "1E23S ",
   "1)2E3S",
   "1E2)3S",
   "1(2E3S",
   "12E(3S",
};

int htiles = (SW - 80) / 16;
int vtiles = (SH - 48) / 16;


/* Welcome to Mapdraw, folks! */
int main (int argc, char *argv[])
{
   int stop = 0, oldmouse_x = 0, oldmouse_y = 0;
   needupdate = 0;

   startup ();
   if (argc > 1) {
      load_map (argv[1]);
   }

   while (!stop) {
      read_controls ();
      if (needupdate) {
         if (draw_mode == MAP_PREVIEW)
            preview_map ();
         else
            draw_map ();
         draw_menubars ();
      }

      if ((needupdate) || (mouse_x != oldmouse_x) || (mouse_y != oldmouse_y)) {
         if (!nomouse) {
            unscare_mouse ();
            show_mouse (double_buffer);
//            draw_sprite (double_buffer, mouse_pic, mouse_x, mouse_y);
         }
         blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
         if (!nomouse) {
            show_mouse (NULL);
            oldmouse_x = mouse_x;
            oldmouse_y = mouse_y;
            scare_mouse ();
         }
      }

      if (key[KEY_Q])
         stop = confirm_exit ();
      yield_timeslice ();
   }                            /* while (!stop) */
   cleanup ();
   return 0;
}                               /* main () */

END_OF_MAIN ();


/*! \brief Animation
 *
 * Realise that this is only an ATTEMPT to get this thing to show what an
 * animation would be like.  It is only available for the PREVIEW mode, so we
 * don't confuse the person putting in one graphic, thinking it will be
 * another.
 *
 * This updates tile indexes for animation threads.
 */
void animate (void)
{

   /* TT TODO:
    * Firstly, I will need to declare the animation sequences.
    * Second, I _think_ that tilex[] handles which tile is being shown on-
    * screen.
    */
   int i, j;

   if (draw_mode != MAP_PREVIEW)
      return;

   for (i = 0; i < MAX_ANIM; i++) {
      if (adata[i].start != 0) {
         for (j = adata[i].start; j <= adata[i].end; j++) {
            if (map[j] < adata[i].end)
               map[j]++;
            else
               map[j] = adata[i].start;
         }
      }
   }

   for (i = 0; i < MAX_ANIM; i++)
      adata[i] = tanim[gmap.tileset][i];

}                               /* animate */

END_OF_FUNCTION (animate);


/*! \brief Memory allocation
 *
 * Allocation of memory, etc. for the maps
 */
void bufferize (void)
{
   free (map);
   map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);

   free (b_map);
   b_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);

   free (f_map);
   f_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);

   free (o_map);
   o_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   free (sh_map);
   sh_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   free (c_map);
   c_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);

   free (cb_map);
   cb_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);

   free (co_map);
   co_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   free (cf_map);
   cf_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);

   free (csh_map);
   csh_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   free (cz_map);
   cz_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   memset (csh_map, 0, gmap.xsize * gmap.ysize);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);

   clipb = 0;
}                               /* bufferize () */

END_OF_FUNCTION (bufferize);


/*! \brief Return the maximum zone used in the map
 *
 * Count and display the maximum Zone Attributes on the map
 *
 * \returns the maximum zone number in the map
 */
int check_last_zone (void)
{
   int a = 0, p;
   for (p = 0; p < gmap.xsize * gmap.ysize; p++)
      if (z_map[p] > a)
         a = z_map[p];
   return a;
}                               /* check_last_zone () */

END_OF_FUNCTION (check_last_zone);


/*! \brief Code shutdown and memory deallocation
 *
 * Called at the end of main(), closes everything
 */
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
   free (b_map);
   free (f_map);
   free (o_map);
   free (sh_map);
   free (z_map);
   free (c_map);
   free (cb_map);
   free (cf_map);
   free (co_map);
   free (csh_map);
   free (cz_map);
   free (strbuf);
   destroy_bitmap (mesh);
   for (k = 0; k < MAX_SHADOWS; k++)
      destroy_bitmap (shadow[k]);
}                               /* cleanup () */

END_OF_FUNCTION (cleanup);


/*! \brief Clears a layer from the map
 *
 * Remove an entire Layer from the map
 */
void clear_layer (void)
{
   int rt, ml, a;
   char ts[4];

   rectfill (double_buffer, 0, 0, 319, 29, 0);
   rect (double_buffer, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Clear Layer", double_buffer);
   print_sfont (6, 12, "Layer (1-3):", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rt = get_line (84, 12, ts, 2);

   /* Make sure the line isn't blank */
   if (rt == 0 || strlen (ts) < 1)
      return;
   /* Make sure the value is valid */
   if ((atoi (ts) < 1 || atoi (ts) > 3)) {
      cmessage ("Invalid layer!");
      wait_enter ();
      return;
   }

   ml = atoi (ts);
   for (a = 0; a < gmap.xsize * gmap.ysize; a++) {
      if (ml == 1)
         map[a] = 0;
      else if (ml == 2)
         b_map[a] = 0;
      else
         f_map[a] = 0;
   }
}                               /* clear_layer () */

END_OF_FUNCTION (clear_layer);


/*! \brief Clear obstructions on map
 *
 * This goes through the entire map and removes all Obstructions
 */
void clear_obstructs (void)
{
   int co;

   cmessage ("Do you want to clear Obstructions? (y/n)");
   if (yninput ())
      for (co = 0; co < gmap.xsize * gmap.ysize; co++)
         o_map[co] = 0;
}                               /* clear_obstructs () */

END_OF_FUNCTION (clear_obstructs);


/*! \brief Clear shadows on the map
 *
 * This goes through the entire map and removes all Shadows
 */
void clear_shadows (void)
{
   int co;

   cmessage ("Do you want to clear Shadows? (y/n)");
   if (yninput ())
      for (co = 0; co < gmap.xsize * gmap.ysize; co++)
         sh_map[co] = 0;
}                               /* clear_shadows () */

END_OF_FUNCTION (clear_shadows);


/*! \brief Default method for displaying messages to screen
 *
 * This displays messages onto the screen; used for warnings, reminders, etc.
 *
 * \param   buff String of text to be printed to the screen
 */
void cmessage (char *buff)
{
   int cx;

   cx = strlen (buff) * 3;
   rectfill (screen, 154 - cx, 91, 165 + cx, 108, 0);
   rect (screen, 154 - cx, 91, 165 + cx, 108, 255);
   print_sfont (160 - cx, 97, buff, screen);
}                               /* cmessage () */

END_OF_FUNCTION (cmessage);


/*! \brief Confirm exit
 *
 * Confirm that the user wants to exit the map editor
 *
 * \returns 1 for yes, 0 for no
 */
int confirm_exit (void)
{
   cmessage ("Are you sure you want to exit? (y/n)");
   return yninput ();
}                               /* confirm_exit () */

END_OF_FUNCTION (confirm_exit);


/*! \brief Copy an entire layer into another
 *
 * Copy the contents of one Layer to another
 */
void copy_layer (void)
{
   int rt, rt2, ff, tt, a, b;
   char ts[4];

   rectfill (double_buffer, 0, 0, 319, 29, 0);
   rect (double_buffer, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Copy Layer to Layer", double_buffer);
   print_sfont (6, 12, "From (1-3):", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rt = get_line (78, 12, ts, 2);

   /* Make sure the line isn't blank */
   if (rt == 0 || strlen (ts) < 1)
      return;
   /* Make sure the value is valid */
   if ((atoi (ts) < 1 || atoi (ts) > 3)) {
      cmessage ("Invalid layer!");
      wait_enter ();
      return;
   }

   ff = atoi (ts);
   print_sfont (6, 18, "To (1-3):", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rt2 = get_line (78, 18, ts, 2);

   /* Make sure the line isn't blank */
   if (rt2 == 0 || strlen (ts) < 1)
      return;
   /* Make sure the value is valid */
   if ((atoi (ts) < 1 || atoi (ts) > 3)) {
      cmessage ("Invalid layer!");
      wait_enter ();
      return;
   }

   tt = atoi (ts);
   for (a = 0; a < gmap.xsize * gmap.ysize; a++) {
      if (ff == 1)
         b = map[a];
      else
         b = (ff == 2 ? b_map[a] : f_map[a]);

      if (tt == 1)
         map[a] = b;
      else {
         if (tt == 2)
            b_map[a] = b;
         else
            f_map[a] = b;
      }
   }
}                               /* copy_layer () */

END_OF_FUNCTION (copy_layer);


/*! \brief Copy all the layers in a block area
 *
 * Copy all of the layers from one area to another
 */
void copy_region (void)
{
   /* Area block coords */
   int zx, zy;
   /* Used to swap 'backward' copy areas */
   int swapx, swapy;

   clipb = 0;
   cbh = copyy2 - copyy1;
   cbw = copyx2 - copyx1;

   /* Determines if 'copy start coords' < 'copy finish coords' and fixes it */
   if (cbh < 1) {
      /* This swaps the 'start' and 'finish' y-coords if backward */
      swapy = copyy2;
      copyy2 = copyy1;
      copyy1 = swapy;
      cbh = copyy2 - copyy1;
   }
   if (cbw < 1) {
      /* This swaps the x-coords if they are backward */
      swapx = copyx2;
      copyx2 = copyx1;
      copyx1 = swapx;
      cbw = copyx2 - copyx1;
   }

   clipb = 1;

   /* Proceed to copy all the layers in the area block */
   for (zy = 0; zy <= cbh; zy++) {
      for (zx = 0; zx <= cbw; zx++) {
         /* TT: Changed from:
            bf = map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
            c_map[zy * gmap.xsize + zx] = bf;
            .
            .
            .
          */

         c_map[zy * gmap.xsize + zx] =
            map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cb_map[zy * gmap.xsize + zx] =
            b_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cf_map[zy * gmap.xsize + zx] =
            f_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         co_map[zy * gmap.xsize + zx] =
            o_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         csh_map[zy * gmap.xsize + zx] =
            sh_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cz_map[zy * gmap.xsize + zx] =
            z_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];

      }
   }
}                               /* copy_region () */

END_OF_FUNCTION (copy_region);


/*! \brief Set the map's description
 *
 * This is what displays in the game when a new map is entered (ie its name)
 */
void describe_map (void)
{
   int ld;
   char tcd[39];

   rectfill (double_buffer, 0, 0, 319, 29, 0);
   rect (double_buffer, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Enter map description", double_buffer);
   sprintf (strbuf, "Current: %s", gmap.map_desc);
   print_sfont (6, 12, strbuf, double_buffer);
   print_sfont (6, 18, "New description: ", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   ld = get_line (66, 18, tcd, 40);

   /* Make sure the line isn't blank */
   if (ld == 0 || strlen (tcd) < 1)
      return;

   strcpy (gmap.map_desc, tcd);
}                               /* describe_map () */

END_OF_FUNCTION (describe_map);


/*! \brief Update the screen
 *
 * Update the screen after all controls taken care of.
 */
void draw_map (void)
{
   /* Coordinates inside the view-window */
   int dx, dy;
   /* Entity index */
   int d;
   /* Index for tiles inside the view-window */
   int w;
   /* Size of the map or view-window, whichever is smaller */
   int maxx, maxy;

   /* Clear everything with black */
   rectfill (double_buffer, 0, 0, (SW - 81), (SH - 49), 0);

   /* The maxx/maxy is used since the map isn't always as large as the
      view-window, we don't want to check/update anything that would be
      out of bounds.
    */
   if (vtiles > gmap.ysize)
      maxy = gmap.ysize;
   else
      maxy = vtiles;
   if (htiles > gmap.xsize)
      maxx = gmap.xsize;
   else
      maxx = htiles;

   /* This loop will draw everything within the view-window */
   for (dy = 0; dy < maxy; dy++) {
      for (dx = 0; dx < maxx; dx++) {
         /* This index is the sum of the view-window's coordinates on
            the map and the current tile's coordinates inside the
            view-window.
          */
         w = ((window_y + dy) * gmap.xsize) + window_x + dx;

         /* Clears Layer 1 background and then draws */
         if ((draw_mode & MAP_LAYER1) || (draw_mode == BLOCK_COPY)
             || (draw_mode == BLOCK_PASTE)) {
            if (draw_mode == MAP_LAYER1)
               rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                         dy * 16 + 15, 0);
            draw_sprite (double_buffer, icons[map[w]], dx * 16, dy * 16);
         }

         /* This draws Layer 1 only if it was what was showing when
            the user toggled the Attribute.
          */
         if ((draw_mode == MAP_ENTITIES) || (draw_mode == MAP_SHADOWS)
             || (draw_mode == MAP_OBSTACLES) || (draw_mode == MAP_ZONES)
             || (draw_mode == GRAB_TILE)) {
            if (showing.last_layer & MAP_LAYER1)
               blit (icons[map[w]], double_buffer, 0, 0, dx * 16, dy * 16, 16,
                     16);
         }

         /* Clears Layer 2 background and then draws */
         if ((draw_mode & MAP_LAYER2) || (draw_mode == BLOCK_COPY)
             || (draw_mode == BLOCK_PASTE)) {
            if (draw_mode == MAP_LAYER2)
               rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                         dy * 16 + 15, 0);
            draw_sprite (double_buffer, icons[b_map[w]], dx * 16, dy * 16);
         }

         /* This draws Layer 2 only if it was what was showing when
            the user toggled the Attribute.
          */
         if ((draw_mode == MAP_ENTITIES) || (draw_mode == MAP_SHADOWS)
             || (draw_mode == MAP_OBSTACLES) || (draw_mode == MAP_ZONES)
             || (draw_mode == GRAB_TILE)) {
            if (showing.last_layer & MAP_LAYER2)
               draw_sprite (double_buffer, icons[b_map[w]], dx * 16, dy * 16);
         }

         /* Clears Layer 3 background and then draws */
         if ((draw_mode & MAP_LAYER3) || (draw_mode == BLOCK_COPY)
             || (draw_mode == BLOCK_PASTE)) {
            if (draw_mode == MAP_LAYER3)
               rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                         dy * 16 + 15, 0);
            draw_sprite (double_buffer, icons[f_map[w]], dx * 16, dy * 16);
         }

         /* This draws Layer 3 only if it was what was showing when
            the user toggled the Attribute.
          */
         if ((draw_mode == MAP_ENTITIES) || (draw_mode == MAP_SHADOWS)
             || (draw_mode == MAP_OBSTACLES) || (draw_mode == MAP_ZONES)
             || (draw_mode == GRAB_TILE)) {
            if (showing.last_layer & MAP_LAYER3)
               draw_sprite (double_buffer, icons[f_map[w]], dx * 16, dy * 16);
         }

         /* Draw the Shadows */
         if (showing.shadows == 1)
            if (sh_map[w] > 0)
               draw_trans_sprite (double_buffer, shadow[sh_map[w]], dx * 16,
                                  dy * 16);

         /* Draw the Obstacles */
         if (showing.obstacles == 1) {
            switch (o_map[w]) {
               /* Block-all: blocks movement from every direction */
            case 1:
               draw_sprite (double_buffer, mesh, dx * 16, dy * 16);
               break;
               /* North-block: blocks movement up */
            case 2:
               hline (double_buffer, dx * 16, dy * 16, dx * 16 + 15, 255);
               vline (double_buffer, dx * 16 + 8, dy * 16, dy * 16 + 15, 255);
               break;
               /* East-block: blocks movement right */
            case 3:
               hline (double_buffer, dx * 16, dy * 16 + 8, dx * 16 + 15, 255);
               vline (double_buffer, dx * 16 + 15, dy * 16, dy * 16 + 15, 255);
               break;
               /* South-block: blocks movement down */
            case 4:
               hline (double_buffer, dx * 16, dy * 16 + 15, dx * 16 + 15, 255);
               vline (double_buffer, dx * 16 + 8, dy * 16, dy * 16 + 15, 255);
               break;
               /* West-block: blocks movement left */
            case 5:
               hline (double_buffer, dx * 16, dy * 16 + 8, dx * 16 + 15, 255);
               vline (double_buffer, dx * 16, dy * 16, dy * 16 + 15, 255);
               break;
            }
         }

         /* if (showing.obstacles == 1) */
         /* Draw the Zones */
         if (showing.zones == 1 && z_map[w] > 0) {
            if (z_map[w] < 10) {
               /* Center single-digit number vert+horiz */
               textprintf_ex (double_buffer, font, dx * 16 + 4, dy * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            }

            else if (z_map[w] < 100) {
               /* Center double-digit number vert */
               textprintf_ex (double_buffer, font, dx * 16, dy * 16 + 4,
                              makecol (255, 255, 255), 0, "%d",
                              (int) (z_map[w] / 10));
               textprintf_ex (double_buffer, font, dx * 16 + 8, dy * 16 + 4,
                              makecol (255, 255, 255), 0, "%d",
                              (int) (z_map[w] % 10));
            }

            else {
               /* Print 100's digit in top-right corner of square;
                  10's and 1's digits on bottom of square
                */
               textprintf_ex (double_buffer, font, dx * 16 + 8, dy * 16,
                              makecol (255, 255, 255), 0, "%d",
                              (int) (z_map[w] / 100));
               textprintf_ex (double_buffer, font, dx * 16, dy * 16 + 8,
                              makecol (255, 255, 255), 0, "%02d",
                              (int) (z_map[w] % 100));
            }
         }                      /* if (showing.zones == 1 && z_map[w] > 0) */
      }                         /* for (dx = 0; dx < maxx; dx++) */
   }                            /* for (dy = 0; dy < maxy; dy++) */
   /* Draw the Entities */
   if (showing.entities == 1) {
      for (d = 0; d < number_of_ents; d++) {
         /* Draw only the entities within the view-screen */
         if ((gent[d].tilex >= window_x) && (gent[d].tilex < window_x + htiles)
             && (gent[d].tiley >= window_y)
             && (gent[d].tiley < window_y + vtiles)) {
            /* Draw either a normal sprite or a translucent one */
            if (gent[d].transl == 0)
               draw_sprite (double_buffer,
                            eframes[gent[d].chrx][gent[d].facing * 3],
                            (gent[d].tilex - window_x) * 16,
                            (gent[d].tiley - window_y) * 16);
            else
               draw_trans_sprite (double_buffer,
                                  eframes[gent[d].chrx][gent[d].facing * 3],
                                  (gent[d].tilex - window_x) * 16,
                                  (gent[d].tiley - window_y) * 16);
         }
      }
   }                            /* if (showing.entities == 1) */
}                               /* draw_map () */

END_OF_FUNCTION (draw_map);


/*! \brief Draw the menus
 *
 * Process both the menus on the side and bottom of the screen
 */
void draw_menubars (void)
{
   int p, xp, yp, a;
   int draw_mode_display;

   /* Description for the current draw_mode (could use work) */
   char dt[15][12] = { "Layer1", "Layer2", "Layer3",
      "View L1+2", "View L1+3", "View L2+3", "View L1+2+3",
      "Shadows", "Zones", "Obstacles", "Entities",
      "Block Copy", "Block Paste", "Grab Tile", "Preview"
   };

   /* Displays the coordinates of the Block Copy */
   if (draw_mode == BLOCK_COPY && copyx1 != -1 && copyy1 != -1) {
      int rectx1, rectx2, recty1, recty2;

      rectx1 = (copyx1 - window_x) * 16;
      recty1 = (copyy1 - window_y) * 16;
      rectx2 = (copyx2 - window_x) * 16 + 15;
      recty2 = (copyy2 - window_y) * 16 + 15;

      sprintf (strbuf, "Rect: %d,%d", copyx1 * 16, copyy1 * 16);
      print_sfont (320, (SH - 24), strbuf, double_buffer);

      if (copying == 0) {
         /* Highlight the selected tile (takes into account window's coords) */
         rect (double_buffer, rectx1, recty1, rectx2, recty2, 25);
      } else {
         rect (double_buffer, rectx1, recty1, rectx1 + 15, recty1 + 15, 25);
      }
   }

   /* The white line that seperates the bottom menu */
   hline (double_buffer, 0, (SH - 48), (SW - 81), 255);

   /* Fill the area under the right menu with black */
   rectfill (double_buffer, (SW - 72), 0, (SW - 1), (SH - 1), 0);

   /* Fill the area under the bottom menu with black */
   rectfill (double_buffer, 0, (SH - 47), (SW - 73), (SH - 1), 0);

   /* The white verticle bar that seperates the tileset */
   rectfill (double_buffer, (SW - 80), 0, (SW - 73), (SH - 49), 255);

   /* The name of the current map */
   sprintf (strbuf, "Map:  %s", map_fname);
   print_sfont (0, (SH - 46), strbuf, double_buffer);

   /* Show which tileset the map will use */
   sprintf (strbuf, "Icon: %s (%d)", icon_files[gmap.tileset], gmap.tileset);
   print_sfont (0, (SH - 40), strbuf, double_buffer);

   /* Give the default song played when the map is entered */
   sprintf (strbuf, "Song: %s", gmap.song_file);
   print_sfont (0, (SH - 34), strbuf, double_buffer);

   /* Allow the map to treat Zone 0 like a normal zone */
   if (gmap.zero_zone == 0)
      print_sfont (0, (SH - 28), "ZeroZone: NO", double_buffer);
   else
      print_sfont (0, (SH - 28), "ZeroZone: YES", double_buffer);

   /* Display the map number */
   sprintf (strbuf, "Map #: %d", gmap.map_no);
   print_sfont (0, (SH - 22), strbuf, double_buffer);

   /* The current mode we use (see drawmap() in the main game for info) */
   sprintf (strbuf, "Mode: %s", map_mode_text[gmap.map_mode]);
   print_sfont (0, (SH - 16), strbuf, double_buffer);

   /* Can the player save the game here? */
   if (gmap.can_save == 0)
      print_sfont (0, (SH - 10), "Save: NO", double_buffer);
   else
      print_sfont (0, (SH - 10), "Save: YES", double_buffer);

   /* Can the player use the Warp spell from here? */
   if (gmap.can_warp == 0)
      print_sfont (80, (SH - 22), "Warp: NO", double_buffer);
   else
      print_sfont (80, (SH - 22), "Warp: YES", double_buffer);

   /* Coordinates where Warp spell takes you to
      NOTE: only works for main.map apparently...
    */
   sprintf (strbuf, "WarpX: %d", gmap.warpx);
   print_sfont (80, (SH - 16), strbuf, double_buffer);
   sprintf (strbuf, "WarpY: %d", gmap.warpy);
   print_sfont (80, (SH - 10), strbuf, double_buffer);

   /* Default area to warp to when a map is entered */
   sprintf (strbuf, "Start X: %d", gmap.stx);
   print_sfont (160, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Start Y: %d", gmap.sty);
   print_sfont (160, (SH - 34), strbuf, double_buffer);

   /* Size of the current map */
   sprintf (strbuf, "Width: %d", gmap.xsize);
   print_sfont (160, (SH - 28), strbuf, double_buffer);
   sprintf (strbuf, "Height: %d", gmap.ysize);
   print_sfont (160, (SH - 22), strbuf, double_buffer);

   /* Parallaxing multiplication/division values (usually 1:1) */
   sprintf (strbuf, "Mult: %d", gmap.pmult);
   print_sfont (240, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Div: %d", gmap.pdiv);
   print_sfont (240, (SH - 34), strbuf, double_buffer);

   /* Allow the player to use SunStones on this map */
   print_sfont (160, (SH - 16),
                gmap.use_sstone == 0 ? "SunStone: NO" : "SunStone: YES",
                double_buffer);

   /* Count Zone attributes on map */
   a = check_last_zone ();
   sprintf (strbuf, "Last zone: %d", a);
   print_sfont (160, (SH - 10), strbuf, double_buffer);

   /* Display the iconset in the right menu */
   if (icon_set != 999) {
      for (p = 0; p < (ICONSET_SIZE / 2); p++) {
         blit (icons[icon_set * ICONSET_SIZE + p], double_buffer, 0, 0,
               (SW - 64), p * 16, 16, 16);
         blit (icons[(icon_set * ICONSET_SIZE) + p + (ICONSET_SIZE / 2)],
               double_buffer, 0, 0, (SW - 48), p * 16, 16, 16);
      }
   }

   /* Show which icon the user selected with a rectangle */
   if (curtile >= icon_set * ICONSET_SIZE
       && curtile < (icon_set * ICONSET_SIZE) + (ICONSET_SIZE)) {
      xp = curtile - (icon_set * ICONSET_SIZE);
      yp = xp;
      xp = xp / (ICONSET_SIZE / 2);
      yp = yp - (xp * (ICONSET_SIZE / 2));

      /* This draws the rectangle around the selected icon */
      rect (double_buffer, xp * 16 + (SW - 64), yp * 16, xp * 16 + (SW - 49),
            yp * 16 + 15, 255);
   }

   /* Clear everything under the iconset */
   rectfill (double_buffer, (SW - 72), 164, (SW - 1), (SH - 1), 0);

   /* Display the draw_mode */
   print_sfont ((SW - 72), 164, "Mode:", double_buffer);

   /* Determine the views that we're currently using */
   draw_mode_display = 0;
   switch (draw_mode) {
   case MAP_LAYER1:
      draw_mode_display = 0;
      break;
   case MAP_LAYER2:
      draw_mode_display = 1;
      break;
   case MAP_LAYER3:
      draw_mode_display = 2;
      break;
   case MAP_LAYER12:
      draw_mode_display = 3;
      break;
   case MAP_LAYER13:
      draw_mode_display = 4;
      break;
   case MAP_LAYER23:
      draw_mode_display = 5;
      break;
   case MAP_LAYER123:
      draw_mode_display = 6;
      break;
   case MAP_SHADOWS:
      draw_mode_display = 7;
      break;
   case MAP_ZONES:
      draw_mode_display = 8;
      break;
   case MAP_OBSTACLES:
      draw_mode_display = 9;
      break;
   case MAP_ENTITIES:
      draw_mode_display = 10;
      break;
   case BLOCK_COPY:
      draw_mode_display = 11;
      break;
   case BLOCK_PASTE:
      draw_mode_display = 12;
      break;
   case GRAB_TILE:
      draw_mode_display = 13;
      break;
   case MAP_PREVIEW:
      draw_mode_display = 14;
      break;
   default:
      draw_mode_display = 0;
      break;
   }                            /* switch (draw_mode) */
   print_sfont ((SW - 66), 170, dt[draw_mode_display], double_buffer);

   /* Display the iconset and selected icon */
   sprintf (strbuf, "#%d(%d)", icon_set, curtile);
   print_sfont ((SW - 72), 176, strbuf, double_buffer);

   /* Mouse x/y coordinates on map (if mouse is over map) */
   if ((mouse_x / 16 < htiles) && (mouse_y / 16 < vtiles)) {
      sprintf (strbuf, "x=%d", window_x + x);
      print_sfont ((SW - 72), 182, strbuf, double_buffer);
      sprintf (strbuf, "y=%d", window_y + y);
      print_sfont ((SW - 72), 188, strbuf, double_buffer);
   }

   /* Show when a user is drawing to the map */
   if (dmode == 1)
      print_sfont ((SW - 64), 194, "drawing", double_buffer);

   /* Displays the value of the Obstacle under the mouse */
   if (draw_mode == MAP_OBSTACLES) {
      sprintf (strbuf, "Obs. #%d", curobs);
      print_sfont (320, (SH - 46), strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = o_map[((window_y + yp) * gmap.xsize) + window_x + xp];
         sprintf (strbuf, "-> %d", p);
         print_sfont (380, (SH - 46), strbuf, double_buffer);
      }
   }

   /* Displays the value of the Zone under the mouse */
   if (draw_mode == MAP_ZONES) {
      sprintf (strbuf, "Zone #%d", curzone);
      print_sfont (320, (SH - 46), strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = z_map[((window_y + yp) * gmap.xsize) + window_x + xp];
         sprintf (strbuf, "-> %d", p);
         print_sfont (380, (SH - 46), strbuf, double_buffer);
      }
   }

   /* Displays the Entity icon and total and current Entities */
   if (draw_mode == MAP_ENTITIES) {
      sprintf (strbuf, "%d", current_ent);
      print_sfont ((SW - 14), (SH - 38), strbuf, double_buffer);
      blit (eframes[current_ent][0], double_buffer, 0, 0, (SW - 16), (SH - 32),
            16, 16);
      sprintf (strbuf, "%d", number_of_ents);
      print_sfont ((SW - 14), (SH - 12), strbuf, double_buffer);
   }

   /* Displays the value of the Shadow under the mouse */
   if (draw_mode == MAP_SHADOWS) {
      sprintf (strbuf, "Shd. #%d", curshadow);
      print_sfont (320, (SH - 46), strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = sh_map[((window_y + yp) * gmap.xsize) + window_x + xp];
         sprintf (strbuf, "-> %d", p);
         print_sfont (380, (SH - 46), strbuf, double_buffer);
      }
   }

   /* Draw a rectangle around the mouse when it's inside the view-window */
   if (mouse_y / 16 < vtiles && mouse_x / 16 < htiles)
      rect (double_buffer, x * 16, y * 16, x * 16 + 15, y * 16 + 15, 255);

   if (draw_mode == BLOCK_COPY) {
      /* TT: My next project is to check to make sure the coords aren't
       * "backward" like I have for the copy_region() function.  (Could I
       * actually use the code from there to implement this?)
       */
      sprintf (strbuf, "From: %d,%d", (copyx1 > -1 ? copyx1 : 0),
               (copyy1 > -1 ? copyy1 : 0));
      print_sfont (320, (SH - 46), strbuf, double_buffer);
      if ((copying == 0) && ((copyx2 > -1) || (copyy2 > -1))) {
         sprintf (strbuf, "To: %d,%d", copyx2, copyy2);
         print_sfont (332, (SH - 40), strbuf, double_buffer);
      }
   }
}                               /* draw_menubars () */

END_OF_FUNCTION (draw_menubars);


/*! \brief Process keyboard input
 *
 * This allows the user to type in values, such as coords, names, etc.
 *
 * \param   window_x x-coord of the input "line"
 * \param   window_y y-coord of the input "line"
 * \param   buffer Where to sent the result
 * \param   max_len Maximum characters to accept
 * \returns 0 if user hits ESC (cancel)
 * \returns 1 if user hits ENTER
 */
int get_line (int window_x, int window_y, char *buffer, int max_len)
{
   int index = 0, ch;
   BITMAP *under;

   under = create_bitmap (320, 6);
   blit (screen, under, 0, window_y, 0, 0, 320, 6);
   while (1) {
      ch = (readkey () & 0xff);

      /* Make sure character entered is valid ASCII */
      if (ch >= 32 && ch <= 126 && index < max_len - 1) {
         buffer[index] = ch;
         buffer[index + 1] = 0;
         print_sfont (window_x, window_y, buffer, screen);
         if (++index == max_len)
            index = max_len - 1;
      } else {
         /* Code for ENTER */
         if (ch == 13) {
            buffer[index] = 0;
            destroy_bitmap (under);
            return (1);
         } else {
            /* Code for BACKSPACE */
            if (ch == 8) {
               if (--index < 0)
                  index = 0;
               buffer[index] = ' ';
               buffer[index + 1] = 0;
               blit (under, screen, 0, 0, 0, window_y, 320, 8);
               print_sfont (window_x, window_y, buffer, screen);
               buffer[index] = 0;
            } else {
               /* Code for ESC */
               if (ch == 27) {
                  destroy_bitmap (under);
                  return (0);
               }
            }
         }                      /* if (ch == 13) */
      }                         /* if (ch >= 32 && ch <= 126 && index < max_len - 1) */
   }                            /* while (1) */
}                               /* get_line () */

END_OF_FUNCTION (get_line);


/*! \brief Gets the index of the tile under the mouse
 *
 * Grab the currently selected map tile and display it in the iconset
 */
void get_tile (void)
{
   int tile = ((window_y + y) * gmap.xsize) + window_x + x;

   if (showing.last_layer == MAP_LAYER2)
      curtile = b_map[tile];
   else if (showing.last_layer == MAP_LAYER3)
      curtile = f_map[tile];
   else
      curtile = map[tile];

   /* Set the iconset "page" to the one where the selected icon is found */
   icon_set = curtile / ICONSET_SIZE;
}                               /* get_tile () */

END_OF_FUNCTION (get_tile);


/*! \brief Change map tiles
 *
 * Change any map tiles from one icon to another throughout the map
 */
void global_change (void)
{
   /* Keyboard input */
   int user_input;
   /* Current tile to change from */
   int tile_from = 0;
   /* Tile to change to */
   int tile_to;
   /* Temp counter */
   int i;
   /* Layers and attributes */
   int p1, p2, p3, ps, po, pz;
   char opts[9];

   rectfill (double_buffer, 0, 0, 319, 29, 0);
   rect (double_buffer, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "From Tile", double_buffer);
   print_sfont (6, 18, "Tile num:", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   user_input = get_line (66, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (user_input == 0 || strlen (strbuf) < 1)
      return;
   /* Make sure the value is valid */
   if (!(atoi (strbuf) >= 0 && atoi (strbuf) < ICONSET_SIZE * max_sets)) {
      cmessage ("Invalid tile!");
      wait_enter ();
      return;
   }
   tile_from = atoi (strbuf);
   rectfill (double_buffer, 0, 0, 319, 29, 0);
   rect (double_buffer, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "To Tile", double_buffer);
   print_sfont (6, 18, "Tile num:", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   user_input = get_line (66, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (user_input == 0 || strlen (strbuf) < 1)
      return;
   /* Make sure the value is valid */
   if (!(atoi (strbuf) >= 0 && atoi (strbuf) < ICONSET_SIZE * max_sets)) {
      cmessage ("Invalid tile!");
      wait_enter ();
      return;
   }
   tile_to = atoi (strbuf);
   rectfill (double_buffer, 0, 0, 319, 29, 0);
   rect (double_buffer, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Layer", double_buffer);
   print_sfont (6, 18, "Layer num (123soz):", double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   user_input = get_line (126, 18, opts, sizeof (opts) - 1);

   /* Make sure the line isn't blank */
   if (user_input == 0 || strlen (opts) < 1)
      return;

   p1 = strchr (opts, '1') ? 1 : 0;
   p2 = strchr (opts, '2') ? 1 : 0;
   p3 = strchr (opts, '3') ? 1 : 0;
   ps = strchr (opts, 's') ? 1 : 0;
   po = strchr (opts, 'o') ? 1 : 0;
   pz = strchr (opts, 'z') ? 1 : 0;

   for (i = 0; i < (gmap.xsize * gmap.ysize); i++) {
      if (p1)
         if (map[i] == tile_from)
            map[i] = tile_to;
      if (p2)
         if (b_map[i] == tile_from)
            b_map[i] = tile_to;
      if (p3)
         if (f_map[i] == tile_from)
            f_map[i] = tile_to;
      if (ps)
         if (sh_map[i] == tile_from)
            sh_map[i] = tile_to;
      if (po)
         if (o_map[i] == tile_from)
            o_map[i] = tile_to;
      if (pz)
         if (z_map[i] == tile_from)
            z_map[i] = tile_to;
   }
}                               /* global_change () */

END_OF_FUNCTION (global_change);


/*! \brief Error reporting tool
 *
 * Report errors and comments through this function
 *
 * \param   msg The message you want sent to the LOG file
 */
void klog (char *msg)
{
   FILE *ff;

   ff = fopen ("mapdraw.log", "a");
   if (!ff)
      exit (-1);
   fprintf (ff, "%s\n", msg);
   fclose (ff);
}                               /* klog () */

END_OF_FUNCTION (klog);


/*! \brief All this does is ensure that we are within the map boundaries */
void normalize_view (void)
{
   /* These prevent the user from moving past the map's edge */
   if (window_x > gmap.xsize - htiles)
      window_x = gmap.xsize - htiles;
   if (window_x < 0)
      window_x = 0;
   if (window_y > gmap.ysize - vtiles)
      window_y = gmap.ysize - vtiles;
   if (window_y < 0)
      window_y = 0;
}

END_OF_FUNCTION (normalize_view);


/*! \brief Paste the copied selection to all Layers
 *
 * Copy all the layers in a block area to a user-defined point in the map
 *
 * \param   tx Target x-coord
 * \param   ty Target y-coord
 */
void paste_region (int tx, int ty)
{
   int zx, zy;

   if (clipb == 0)
      return;
   for (zy = 0; zy <= cbh; zy++) {
      for (zx = 0; zx <= cbw; zx++) {
         if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) {
            /* TT: Changed from:
               bf = c_map[zy * gmap.xsize + zx];
               map[(ty + zy) * gmap.xsize + tx + zx] = bf;
               .
               .
               .
             */

            map[(ty + zy) * gmap.xsize + tx + zx] =
               c_map[zy * gmap.xsize + zx];
            b_map[(ty + zy) * gmap.xsize + tx + zx] =
               cb_map[zy * gmap.xsize + zx];
            f_map[(ty + zy) * gmap.xsize + tx + zx] =
               cf_map[zy * gmap.xsize + zx];
            z_map[(ty + zy) * gmap.xsize + tx + zx] =
               cz_map[zy * gmap.xsize + zx];
            sh_map[(ty + zy) * gmap.xsize + tx + zx] =
               csh_map[zy * gmap.xsize + zx];
            o_map[(ty + zy) * gmap.xsize + tx + zx] =
               co_map[zy * gmap.xsize + zx];
         }
      }
   }
}                               /* paste_region () */

END_OF_FUNCTION (paste_region);


/*! \brief Pastes user-defined layer(s)
 *
 * This asks the user which layer(s) s/he wants to paste to, then
 * performs the paste operation.
 *
 * \param   tx Target x-coord to paste to
 * \param   ty Target y-coord to paste to
 */
void paste_region_special (int tx, int ty)
{
   int zx, zy;

   /* Layers 1-3, Shadows, Obstacles, and Zones */
   int ld, p1, p2, p3, ps, po, pz;

   /* Holds the user input for which region(s) to paste to */
   char opts[9];

   /* Confirm that there is something in the clipboard; don't attempt if not */
   if (clipb == 0)
      return;

   memset (opts, 0, sizeof (opts));
   rectfill (double_buffer, 138, 89, 306, 120, 0);
   print_sfont (140, 91, "Paste which layers? (123soz)", double_buffer);
   rect (double_buffer, 138, 89, 306, 120, 255);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   ld = get_line (160, 110, opts, sizeof (opts) - 1);

   /* Make sure the line isn't blank */
   if (ld == 0 || strlen (opts) < 1)
      return;

   p1 = strchr (opts, '1') ? 1 : 0;
   p2 = strchr (opts, '2') ? 1 : 0;
   p3 = strchr (opts, '3') ? 1 : 0;
   ps = strchr (opts, 's') ? 1 : 0;
   po = strchr (opts, 'o') ? 1 : 0;
   pz = strchr (opts, 'z') ? 1 : 0;

   for (zy = 0; zy <= cbh; zy++) {
      for (zx = 0; zx <= cbw; zx++) {
         if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) {
            if (p1) {
               map[(ty + zy) * gmap.xsize + tx + zx] =
                  c_map[zy * gmap.xsize + zx];
            }
            if (p2) {
               b_map[(ty + zy) * gmap.xsize + tx + zx] =
                  cb_map[zy * gmap.xsize + zx];
            }
            if (p3) {
               f_map[(ty + zy) * gmap.xsize + tx + zx] =
                  cf_map[zy * gmap.xsize + zx];
            }
            if (pz) {
               z_map[(ty + zy) * gmap.xsize + tx + zx] =
                  cz_map[zy * gmap.xsize + zx];
            }
            if (ps) {
               sh_map[(ty + zy) * gmap.xsize + tx + zx] =
                  csh_map[zy * gmap.xsize + zx];
            }
            if (po) {
               o_map[(ty + zy) * gmap.xsize + tx + zx] =
                  co_map[zy * gmap.xsize + zx];
            }
         }                      /* if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) */
      }                         /* for (zx = 0; zx <= cbw; zx++) */
   }                            /* for (zy = 0; zy <= cbh; zy++) */
}                               /* paste_region_special () */

END_OF_FUNCTION (paste_region_special);


/*! \brief Draw the shadows
 *
 * Draws the shadows onto the screen and takes into consideration any layer
 * effects that need to take place (see map_mode_text[] for details).
 * This is basically the same as draw_layer().
 * \author  PH
 * \date    20031205
 * \param   parallax 0 no parallax
 *                   >0 parallax
 */
static void draw_shadow (int parallax)
{
   int layer_x, j;
   int layer_x1, layer_y1, layer_x2, layer_y2;
   int x0, y0;
   int ss;

   /* Calculate the top left, taking parallax into account */
   layer_x1 = parallax ? window_x * gmap.pmult / gmap.pdiv : window_x;
   layer_y1 = parallax ? window_y * gmap.pmult / gmap.pdiv : window_y;

   /* Calculate bottom right */
   layer_x2 = layer_x1 + htiles;
   layer_y2 = layer_y1 + vtiles;

   /* Make sure these don't step off the edges of the map */
   if (layer_x1 < 0)
      layer_x1 = 0;
   if (layer_y1 < 0)
      layer_y1 = 0;
   if (layer_x2 > gmap.xsize)
      layer_x2 = gmap.xsize;
   if (layer_y2 > gmap.ysize)
      layer_y2 = gmap.ysize;

   /* Calculate the pixel-based coordinate of the top left */
   x0 = window_x * 16;
   y0 = window_y * 16;

   /* ...And draw the tilemap */
   for (j = layer_y1; j < layer_y2; ++j) {
      for (layer_x = layer_x1; layer_x < layer_x2; ++layer_x) {
         ss = sh_map[j * gmap.xsize + layer_x];
         if (ss > 0) {
            draw_trans_sprite (double_buffer, shadow[ss], layer_x * 16 - x0,
                               j * 16 - y0);
         }
      }
   }
}                               /* draw_shadow () */

END_OF_FUNCTION (void draw_shadow);


/*! \brief Preview map
 *
 * Draw the map with all layers on and using parallax/layering
 * like in the game (see map_mode_text[] for details).
 * \author PH
 * \date 20031205
 */
void preview_map (void)
{
   clear_bitmap (double_buffer);
   switch (gmap.map_mode) {
   case 0:
      draw_layer ((short int *) map, 0);
      draw_layer ((short int *) b_map, 0);
      draw_ents ();
      draw_layer ((short int *) f_map, 0);
      draw_shadow (0);
      break;
   case 1:
      draw_layer ((short int *) map, 0);
      draw_ents ();
      draw_layer ((short int *) b_map, 0);
      draw_layer ((short int *) f_map, 0);
      draw_shadow (0);
      break;
   case 2:
      draw_layer ((short int *) map, 1);
      draw_layer ((short int *) b_map, 0);
      draw_ents ();
      draw_layer ((short int *) f_map, 0);
      draw_shadow (0);
      break;
   default:

      sprintf (strbuf, "Mode %d preview not supported, sorry!", gmap.map_mode);
      print_sfont (8, 8, strbuf, double_buffer);

      textprintf_centre_ex (double_buffer, font, double_buffer->w / 2,
                            double_buffer->h / 2, makecol (255, 255, 255), 0,
                            "Mode %d preview not supported.", gmap.map_mode);
      break;
   }
}                               /* preview_map () */

END_OF_FUNCTION (preview_map);


/*! \brief Draw the layers with parallax
 *
 * Draws a single layer (back, middle or fore) and will compensate for parallax.
 * Drawing is done by draw_sprite() - so the bottom layer needs to go onto a black background.
 * This is called by preview_map().
 * The code is slightly overcomplicated but it is more general, if things need to change later.
 *
 * \author PH
 * \date 20031205
 * \param layer pointer to layer data array
 * \param parallax ==0 draw with parallax off or !=0 on
 */
void draw_layer (short *layer, int parallax)
{
   int layer_x, j;
   int layer_x1, layer_x2, layer_y1, layer_y2;
   int x0, y0;

   /* Calculate the top left, taking parallax into account */
   layer_x1 = parallax ? window_x * gmap.pmult / gmap.pdiv : window_x;
   layer_y1 = parallax ? window_y * gmap.pmult / gmap.pdiv : window_y;

   /* Calculate bottom right */
   layer_x2 = layer_x1 + htiles;
   layer_y2 = layer_y1 + vtiles;

   /* Make sure these don't step off the edges of the map */
   if (layer_x1 < 0)
      layer_x1 = 0;
   if (layer_y1 < 0)
      layer_y1 = 0;
   if (layer_x2 > gmap.xsize)
      layer_x2 = gmap.xsize;
   if (layer_y2 > gmap.ysize)
      layer_y2 = gmap.ysize;

   /* Calculate the pixel-based coordinate of the top left */
   x0 = layer_x1 * 16;
   y0 = layer_y1 * 16;

   /* ...And draw the tilemap */
   for (j = layer_y1; j < layer_y2; ++j) {
      for (layer_x = layer_x1; layer_x < layer_x2; ++layer_x) {
         draw_sprite (double_buffer, icons[layer[layer_x + j * gmap.xsize]],
                      layer_x * 16 - x0, j * 16 - y0);
      }
   }
}                               /* draw_layer () */

END_OF_FUNCTION (draw_layer);


/*! \brief Displays the text on the screen
 *
 * Prints the string to the screen using the default font
 *
 * \param   x Where to display the text on the x-coord
 * \param   y Where to display the text on the y-coord
 * \param   string A "thinner" version of yarn, made from cotton or wool
 * \param   where The destination, or where it will be drawn to
 */
void print_sfont (int x, int y, char *string, BITMAP * where)
{
   int i, c;

   for (i = 0; i < (signed) strlen (string); i++) {
      c = string[i];
      /* Convert ascii to the (font's) bitmap equivalent (a is in location 0,
         b in location 1... z in location 25, etc.)
       */
      c -= 32;
      if (c < 0)
         c = 0;
      masked_blit (font6, where, 0, c * 6, i * 6 + x, y, 6, 6);
   }
}                               /* print_sfont () */

END_OF_FUNCTION (print_sfont);


/*! \brief Keyboard input
 * Keyboard inputs (sorry, no joystick support)
 *
 * \param   k The keyboard key to process
 */
void process_keyboard (int k)
{
   /* Process which key was pressed */
   switch (k) {
   case (KEY_1):
      /* Layer 1 */
      select_only (1, MAP_LAYER1);
      break;
   case (KEY_2):
      /* Layer 2 */
      select_only (1, MAP_LAYER2);
      break;
   case (KEY_3):
      /* Layer 3 */
      select_only (1, MAP_LAYER3);
      break;
   case (KEY_4):
      /* Show Layers 1+2 */
      select_only (0, MAP_LAYER12);
      break;
   case (KEY_5):
      /* Show Layers 1+3 */
      select_only (0, MAP_LAYER13);
      break;
   case (KEY_6):
      /* Show Layers 2+3 */
      select_only (0, MAP_LAYER23);
      break;
   case (KEY_7):
      /* Show Layers 1+2+3 */
      select_only (0, MAP_LAYER123);
      break;
   case (KEY_A):
      /* Display Layers 1+2+3 and all Attributes */
      draw_mode = MAP_LAYER123;
      showing.entities = 1;
      showing.shadows = 1;
      showing.obstacles = 1;
      showing.zones = 1;
      showing.last_layer = draw_mode;
      break;
   case (KEY_C):
      /* View Layers 1+2+3, plus Entities and Shadows,
         as the player would see the map in the game */
      draw_mode = MAP_PREVIEW;
      showing.entities = 1;
      showing.shadows = 1;
      showing.obstacles = 0;
      showing.zones = 0;
      showing.last_layer = MAP_LAYER123;
      break;
   case (KEY_D):
      /* Move (displace) the location of all the entities on the map */
      displace_entities ();
      break;
   case (KEY_E):
      /* Toggle whether entities should be shown or turned off */
      if (showing.entities == 1) {
         if (draw_mode == MAP_ENTITIES) {
            draw_mode = showing.last_layer;
            showing.entities = 0;
         } else
            draw_mode = MAP_ENTITIES;
      } else {
         draw_mode = MAP_ENTITIES;
         showing.entities = 1;
      }
      break;
   case (KEY_F):
      /* Get the first Zone used and set the indicator to that */
      if (draw_mode == MAP_ZONES)
         curzone = 0;
      break;
   case (KEY_G):
      /* Get the tile under the mouse curser */
// TT TODO: I think that all the checks except *.last_layer should be removed.
// That way, the user can 'grab' a zone, shadow, or obstacle
      if ((draw_mode != MAP_LAYER1) && (draw_mode != MAP_LAYER2)
          && (draw_mode != MAP_LAYER3) && (showing.last_layer != MAP_LAYER1)
          && (showing.last_layer != MAP_LAYER2)
          && (showing.last_layer != MAP_LAYER3))

         /* The default view mode */
         showing.last_layer = MAP_LAYER1;
      else if (draw_mode != GRAB_TILE)
         /* One of the 3 layers was previously used; switch to that */
         showing.last_layer = draw_mode;
      draw_mode = GRAB_TILE;
      break;
   case (KEY_J):
      /* TT: Do we even want or need this anymore? */
      /* Copy Layers 1, 2, 3 to mini PCX images */
      maptopcx ();
      break;
   case (KEY_L):
      /* Get the last Zone used and set the indicator to that */
      if (draw_mode == MAP_ZONES)
         curzone = check_last_zone ();
      break;
   case (KEY_N):
      /* Create a new map; you can choose the tileset to use for it */
      new_map ();
      break;
   case (KEY_O):
      /* Toggle whether obstacles should be shown or turned off */
      if (showing.obstacles == 1) {
         if (draw_mode == MAP_OBSTACLES) {
            draw_mode = showing.last_layer;
            showing.obstacles = 0;
         } else
            draw_mode = MAP_OBSTACLES;
      } else {
         draw_mode = MAP_OBSTACLES;
         showing.obstacles = 1;
      }
      break;
   case (KEY_P):
      /* Paste the copied selection area */
      draw_mode = BLOCK_PASTE;
      break;
   case (KEY_R):
      /* Resize the map's height and width */
      resize_map (0);
      break;
   case (KEY_S):
      /* Toggle whether shadows should be shown or turned off */
      if (showing.shadows == 1) {
         if (draw_mode == MAP_SHADOWS) {
            draw_mode = showing.last_layer;
            showing.shadows = 0;
         } else
            draw_mode = MAP_SHADOWS;
      } else {
         draw_mode = MAP_SHADOWS;
         showing.shadows = 1;
      }
      break;
   case (KEY_T):
      /* Copy a selection */
      draw_mode = BLOCK_COPY;
      break;
   case (KEY_V):
/* TT: No longer needed: superseded with mapdump */
      /* Save whole map as a picture */
      visual_map ();
      break;
   case (KEY_W):
      /* Clear the contents of the current map */
      wipe_map ();
      break;
   case (KEY_Z):
      /* Toggle whether zones should be shown or turned off */
      if (showing.zones == 1) {
         if (draw_mode == MAP_ZONES) {
            draw_mode = showing.last_layer;
            showing.zones = 0;
         } else
            draw_mode = MAP_ZONES;
      } else {
         draw_mode = MAP_ZONES;
         showing.zones = 1;
      }
      break;
   case (KEY_SPACE):
      /* Attempt at giving the user a chance to see the animations */
      if (draw_mode == MAP_PREVIEW) {
         animate ();
      }
      break;
   case (KEY_F1):
      /* Display the help screen */
      show_help ();
      break;
   case (KEY_F2):
      /* Load a map */
      prompt_load_map ();
      break;
   case (KEY_F3):
      /* Save the map you are working on */
      save_map ();
      break;
   case (KEY_F4):
      /* Erase an entire layer from the map */
      clear_layer ();
      break;
   case (KEY_F5):
      /* TT: Do we even need this anymore? */
      /* Load a [mini] PCX file to become a map */
      make_mapfrompcx ();
      break;
   case (KEY_F6):
      /* Change all instances of one tile in your map to another */
      global_change ();
      break;
   case (KEY_F7):
      /* Remove all Obstructions from the map */
      clear_obstructs ();
      break;
   case (KEY_F8):
      /* Remove all Shadows from the map */
      clear_shadows ();
      break;
   case (KEY_F9):
      /* Copy from one layer to another */
      copy_layer ();
      break;
   case (KEY_F10):
      /* Enter a description that a player sees when entering the map */
      describe_map ();
      break;
   case (KEY_F12):
      /* TT TODO:
       *
       * Change the F12 function to be able to select the current Attribute.
       *
       * This would mean that F12 does not automatically go into the entity-stats
       * mode; it will only go into it if entities was the active draw_mode.
       * If zones were selected, you would be able to move through all of the
       * current zone #s on the screen (meaning, if you had several Zone_5 on the
       * map, pressing KEY_UP would move from the upper-left most Zone_5, to the
       * right, all the way to the bottom of the map, and then repeat itself).
       *
       * This would give a very handy way of tracking down a "stray" zone.
       *
       * The error-check for "no zones" can follow the guidelines of the
       * "no entities" error message.
       *
       */
      /* Enter the Modify Entity mode */
      showing.entities = 1;
      update_entities ();
      break;
   case (KEY_ESC):
      /* TT: This currently cancels a Block Copy, but future use is intended
       * to have it bring up a menu for easier navigation
       */
      cmessage ("Paused!");
      yninput ();

      if (copying == 1) {
         copying = 0;
         copyx1 = copyx2 = -1;
         copyy1 = copyy2 = -1;
      }
      /* Empty the contents of the clipboard */
      clipb = 0;
      break;
   case (KEY_MINUS):
   case (KEY_MINUS_PAD):
      switch (draw_mode) {
         /* Select an Entity to place on the map */
      case MAP_ENTITIES:
         current_ent--;
         if (current_ent < 0)
            current_ent = MAX_EPICS - 1;
         break;
         /* Select the Shadow to place on the map */
      case MAP_SHADOWS:
         curshadow--;
         if (curshadow < 0)
            curshadow = MAX_SHADOWS - 1;
         break;
         /* Select which Obstacle to set on the map */
      case MAP_OBSTACLES:
         curobs--;
         if (curobs < 0)
            curobs = 5;
         break;
         /* Select a Zone to set on the map */
      case MAP_ZONES:
         curzone--;
         if (curzone < 0)
            curzone = MAX_ZONES - 1;
         break;
         /* Change the iconset's "page" */
      default:
         icon_set--;
         if (icon_set < 0)
            icon_set = max_sets - 1;
         break;
      }
      break;
   case (KEY_EQUALS):
   case (KEY_PLUS_PAD):
      /* Change the values for the current mode */
      switch (draw_mode) {
      case MAP_ENTITIES:
         /* Select an Entity to place on the map */
         current_ent++;
         if (current_ent == MAX_EPICS)
            current_ent = 0;
         break;
      case MAP_SHADOWS:
         /* Select the Shadow to place on the map */
         curshadow++;
         if (curshadow >= MAX_SHADOWS)
            curshadow = 0;
         break;
      case MAP_OBSTACLES:
         /* Select which Obstacle to set on the map */
         curobs++;
         if (curobs > 5)
            curobs = 0;
         break;
      case MAP_ZONES:
         /* Select a Zone to set on the map */
         curzone++;
         if (curzone >= MAX_ZONES)
            curzone = 0;
         break;
      default:
         /* Change the iconset's "page" */
         icon_set++;
         if (icon_set > max_sets - 1)
            icon_set = 0;
         break;
      }
      break;
   case (KEY_PGUP):
      /* Move the view-window up one page */
      window_y -= vtiles;
      break;
   case (KEY_PGDN):
      /* Move the view-window down one page */
      window_y += vtiles;
      break;
   case (KEY_TAB):
      /* Move the view-window right one page */
      window_x += htiles;
      break;
   case (KEY_BACKSPACE):
      /* Move the view-window left one page */
      window_x -= htiles;
      break;
   case (KEY_END):
      /* Move the view-window to the bottom-right edge of the map */
      window_x = gmap.xsize - htiles;
      window_y = gmap.ysize - vtiles;
      break;
   case (KEY_HOME):
      /* Move the view-window to the top-left edge of the map */
      window_x = 0;
      window_y = 0;
      break;
   }                            /* switch (k) */
   /* Move the view-window up one tile */
   if (k == KEY_UP || k == KEY_7_PAD || k == KEY_8_PAD || k == KEY_9_PAD)
      window_y--;
   /* Move the view-window down one tile */
   if (k == KEY_DOWN || k == KEY_1_PAD || k == KEY_2_PAD || k == KEY_3_PAD)
      window_y++;
   /* Move the view-window right one tile */
   if (k == KEY_RIGHT || k == KEY_3_PAD || k == KEY_6_PAD || k == KEY_9_PAD)
      window_x++;
   /* Move the view-window left one tile */
   if (k == KEY_LEFT || k == KEY_1_PAD || k == KEY_4_PAD || k == KEY_7_PAD)
      window_x--;

   normalize_view ();
}                               /* process_keyboard () */

END_OF_FUNCTION (process_keyboard);


/*! \brief Select an option from the menu
 *
 * Selects one of the options from the menus at the bottom of the screen
 *
 * \param   cx x-coord of the mouse
 * \param   cy y-coord of the mouse
 */
void process_menu_bottom (int cx, int cy)
{
   int a, column_width = 80;
   scare_mouse ();

   /* The mouse is over 'Icon:' menu */
   if (cx >= 0 && cx < (column_width * 2) && cy >= (SH - 40)
       && cy <= (SH - 35)) {
      /* This allows the user to select the tileset used
         for the current map.
       */
      gmap.tileset++;
      if (gmap.tileset >= NUM_TILESETS)
         gmap.tileset = 0;

      /* See instant results! */
      update_tileset ();

      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Song:' menu */
   if (cx >= 0 && cx < (column_width * 2) && cy >= (SH - 34)
       && cy <= (SH - 29)) {
      rectfill (double_buffer, 36, (SH - 34), (column_width * 2) - 1,
                (SH - 29), 0);
      print_sfont (30, (SH - 34), ">", double_buffer);
      hline (double_buffer, 36, (SH - 29), 113, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (36, (SH - 34), strbuf, 19);

      /* This is kinda hard to error-check... */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      strcpy (gmap.song_file, strbuf);
   }

   /* The mouse is over 'ZeroZone' menu */
   if (cx >= 0 && cx < (column_width) && cy >= (SH - 28) && cy <= (SH - 23)) {
      gmap.zero_zone = 1 - gmap.zero_zone;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Map #' menu */
   if (cx >= 0 && cx < (column_width) && cy >= (SH - 22) && cy <= (SH - 17)) {
      rectfill (double_buffer, 42, (SH - 22), (column_width * 2) - 1,
                (SH - 17), 0);
      print_sfont (36, (SH - 22), ">", double_buffer);
      hline (double_buffer, 42, (SH - 17), 65, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (42, (SH - 22), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= 255)) {
         cmessage ("Invalid map number!");
         wait_enter ();
         return;
      }
      gmap.map_no = atoi (strbuf);
   }

   /* The mouse is over 'Mode' menu */
   if (cx >= 0 && cx < (column_width) && cy >= (SH - 16)
       && cy <= (SH - 11)) {
      gmap.map_mode++;
      if (gmap.map_mode > 5)
         gmap.map_mode = 0;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Save' menu */
   if (cx >= 0 && cx < (column_width) && cy >= (SH - 10) && cy <= (SH - 5)) {
      gmap.can_save = 1 - gmap.can_save;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Warp' menu */
   if (cx >= (column_width) && cx < (column_width * 2) && cy >= (SH - 22)
       && cy <= (SH - 17)) {
      gmap.can_warp = 1 - gmap.can_warp;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'WarpX' menu */
   if (cx >= (column_width) && cx < (column_width * 2) && cy >= (SH - 16)
       && cy <= (SH - 11)) {
      rectfill (double_buffer, 122, (SH - 16), 159, (SH - 11), 0);
      print_sfont (116, (SH - 16), ">", double_buffer);
      hline (double_buffer, 122, (SH - 11), 145, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (122, (SH - 16), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= SW)) {
         cmessage ("Invalid x-coordinate for warp!");
         wait_enter ();
         return;
      }
      gmap.warpx = atoi (strbuf);
   }

   /* The mouse is over 'WarpY' menu */
   if (cx >= (column_width) && cx < (column_width * 2) && cy >= (SH - 10)
       && cy <= (SH - 5)) {
      rectfill (double_buffer, 122, (SH - 10), 159, (SH - 5), 0);
      print_sfont (116, (SH - 10), ">", double_buffer);
      hline (double_buffer, 122, (SH - 5), 145, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (122, (SH - 10), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= SH)) {
         cmessage ("Invalid y-coordinate for warp!");
         wait_enter ();
         return;
      }
      gmap.warpy = atoi (strbuf);
   }

   /* The mouse is over 'Start X' menu */
   if (cx >= (column_width * 2) && cx < (column_width * 3) && cy >= (SH - 40)
       && cy <= (SH - 35)) {
      rectfill (double_buffer, 214, (SH - 40), ((column_width * 3) - 1),
                (SH - 35), 0);
      print_sfont (208, (SH - 40), ">", double_buffer);
      hline (double_buffer, 214, (SH - 35), 237, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (214, (SH - 40), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= SW)) {
         cmessage ("Invalid starting x-position!");
         wait_enter ();
         return;
      }
      gmap.stx = atoi (strbuf);
   }

   /* The mouse is over 'Start Y' menu */
   if (cx >= (column_width * 2) && cx < (column_width * 3) && cy >= (SH - 34)
       && cy <= (SH - 29)) {
      rectfill (double_buffer, 214, (SH - 34), ((column_width * 3) - 1),
                (SH - 29), 0);
      print_sfont (208, (SH - 34), ">", double_buffer);
      hline (double_buffer, 214, (SH - 29), 237, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (214, (SH - 34), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= SH)) {
         cmessage ("Invalid starting y-position!");
         wait_enter ();
         return;
      }
      gmap.sty = atoi (strbuf);
   }

   /* Mouse is over 'Mult' menu */
   if (cx >= (column_width * 3) && cx <= 319 && cy >= (SH - 40)
       && cy <= (SH - 35)) {
      rectfill (double_buffer, 276, (SH - 40), 319, (SH - 35), 0);
      print_sfont (270, (SH - 40), ">", double_buffer);
      hline (double_buffer, 276, (SH - 35), 299, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (276, (SH - 40), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid (999 is a VERY extreme value!).
         Even though you cannot divide by 0 (see below), you CAN multiply
         by 0 to make the background totally stationary.
       */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= 999)) {
         cmessage ("Invalid parallax multiplier!");
         wait_enter ();
         return;
      }
      gmap.pmult = atoi (strbuf);
   }

   /* The mouse is over 'Div' menu */
   if (cx >= (column_width * 3) && cx < (column_width * 4) && cy >= (SH - 34)
       && cy <= (SH - 29)) {
      rectfill (double_buffer, 270, (SH - 34), (column_width * 4) - 1,
                (SH - 29), 0);
      print_sfont (264, (SH - 34), ">", double_buffer);
      hline (double_buffer, 270, (SH - 29), 293, 255);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      a = get_line (270, (SH - 34), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid (999 is a VERY extreme value!) and
         you CANNOT divide by zero.
       */
      if (!(atoi (strbuf) > 0 && atoi (strbuf) <= 999)) {
         cmessage ("Invalid parallax divider!");
         wait_enter ();
         return;
      }
      gmap.pdiv = atoi (strbuf);
   }

   /* The mouse is over the 'Width' menu */
   if (cx >= (column_width * 2) && cx < (column_width * 3) && cy >= (SH - 28)
       && cy <= (SH - 23))
      /* 1 means resize width */
      resize_map (1);

   /* The mouse is over the 'Height' menu */
   if (cx >= (column_width * 2) && cx < (column_width * 3) && cy >= (SH - 22)
       && cy <= (SH - 17))
      /* 2 means resize height */
      resize_map (2);

   /* The mouse is over 'SunStone' menu */
   if (cx >= (column_width * 2) && cx < (column_width * 3) && cy >= (SH - 16)
       && cy <= (SH - 11)) {
      gmap.use_sstone = 1 - gmap.use_sstone;
      while (mouse_b & 1);
      return;
   }
}                               /* process_menu_bottom () */

END_OF_FUNCTION (process_menu_bottom);


/*! \brief Select an icon from the tileset
 *
 * Selects one of the tiles from the menu on the right
 *
 * \param   cx x-coord of the tile
 * \param   cy y-coord of the tile
 */
void process_menu_right (int cx, int cy)
{
   int xp, yp;

   /* Make sure that the mouse is over one if the selectable tiles */
   if (cx >= (SW - 64) && cx < (SW - 32) && cy >= 0 && cy < 160) {
      xp = (cx - (SW - 64)) / 16;
      yp = cy / 16;
      /* Set the tileset to the correct "page" */
      curtile = icon_set * ICONSET_SIZE + (xp * (ICONSET_SIZE / 2) + yp);
   }
}                               /* process_menu_right () */

END_OF_FUNCTION (process_menu_right);


/*! \brief Mouse input
 * Mouse inputs
 *
 * \param   mouse_button 1 for left button
 *                       2 for right button
 */
void process_mouse (const int mouse_button)
{
   /* Left mouse button */
   if (mouse_button == 1) {

      /* Drawing to the map only happen when (dmode == 1) */
      if (dmode == 0) {
         switch (draw_mode) {
         case (BLOCK_COPY):
            /* Begin area copy
             * This copies all Layers and Attributes except Entities
             */
            if (copying == 0) {
               copyx1 = window_x + x;
               copyy1 = window_y + y;

               /* Clear the end coordinates */
               copyx2 = copyy2 = -1;
               copying = 1;
            }
            break;
         case (BLOCK_PASTE):
            /* Paste copied region(s) onto the map
               This pastes ALL Layers/Attributes except Entities (use right-
               click to select the Layers/Attributes you want to paste)
             */
            if (clipb != 0)
               paste_region (window_x + x, window_y + y);
            break;
         case (GRAB_TILE):
            /* Grab the tile; select its icon from the iconset on the right
             */
            get_tile ();
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         /* if (dmode == 0) */

      /* Now draw to the map */
      if (dmode == 1) {
         switch (draw_mode) {
         case (MAP_LAYER1):
            /* Draw to Layer 1 */
            map[((window_y + y) * gmap.xsize) + window_x + x] = curtile;
            break;
         case (MAP_LAYER2):
            /* Draw to Layer 2 */
            b_map[((window_y + y) * gmap.xsize) + window_x + x] = curtile;
            break;
         case (MAP_LAYER3):
            /* Draw to Layer 3 */
            f_map[((window_y + y) * gmap.xsize) + window_x + x] = curtile;
            break;
         case (MAP_ENTITIES):
            /* Draw to Entity layer */
            place_entity ((mouse_x / 16) + window_x,
                          (mouse_y / 16) + window_y);
            break;
         case (MAP_OBSTACLES):
            /* Draw to Obstacle layer */
            o_map[((window_y + y) * gmap.xsize) + window_x + x] = curobs;
            break;
         case (MAP_SHADOWS):
            /* Draw to Shadow layer */
            sh_map[((window_y + y) * gmap.xsize) + window_x + x] = curshadow;
            break;
         case (MAP_ZONES):
            /* Draw to Zone layer */
            z_map[((window_y + y) * gmap.xsize) + window_x + x] = curzone;
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         /* if (dmode == 1) */
   }                            /* if (mouse_button == 1) */

   /* Right mouse button */
   if (mouse_button == 2) {
      if (dmode == 0) {
         switch (draw_mode) {
         case (BLOCK_COPY):
            /* Finish the Block Copy */
            if (copying == 1) {
               copyx2 = window_x + x;
               copyy2 = window_y + y;
               copy_region ();
               copying = 0;
            }
            break;
         case (BLOCK_PASTE):
            /* Paste Layers and/or Attributes */
            if (clipb != 0)
               paste_region_special (window_x + x, window_y + y);
            break;
         case (GRAB_TILE):
            /* Grab the tile and set the mode to a drawing mode */
            get_tile ();
            dmode = 2;
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         /* if (dmode == 0) */
      if (dmode == 1) {
         switch (draw_mode) {
         case (MAP_LAYER1):
            /* Erase a tile from Layer 1 */
            map[((window_y + y) * gmap.xsize) + window_x + x] = 0;
            break;
         case (MAP_LAYER2):
            /* Erase a tile from Layer 2 */
            b_map[((window_y + y) * gmap.xsize) + window_x + x] = 0;
            break;
         case (MAP_LAYER3):
            /* Erase a tile from Layer 3 */
            f_map[((window_y + y) * gmap.xsize) + window_x + x] = 0;
            break;
         case (MAP_ENTITIES):
            /* Remove an Entity */
            erase_entity (mouse_x / 16 + window_x, mouse_y / 16 + window_y);
            break;
         case (MAP_OBSTACLES):
            /* Remove an Obstacle */
            o_map[((window_y + y) * gmap.xsize) + window_x + x] = 0;
            break;
         case (MAP_SHADOWS):
            /* Remove a Shadow */
            sh_map[((window_y + y) * gmap.xsize) + window_x + x] = 0;
            break;
         case (MAP_ZONES):
            /* Remove a Zone */
            z_map[((window_y + y) * gmap.xsize) + window_x + x] = 0;
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         /* if (dmode == 1) */
   }                            /* if (mouse_button == 2) */
   x = mouse_x / 16;
   y = mouse_y / 16;
   if (y > (vtiles - 1))
      y = vtiles - 1;
   if (x > (htiles - 1))
      x = htiles - 1;
}                               /* process_mouse () */

END_OF_FUNCTION (process_mouse);


/*! \brief Mouse and keyboard input
 *
 * Detect input from either the keyboard or mouse (sorry, no joystick support)
 */
void read_controls (void)
{
   int mouse_button, oldx, oldy;
   needupdate = 0;

   /******************************************
    * Check for, and process, keyboard input *
    ******************************************/
   if (keypressed ()) {
      process_keyboard ((int) (readkey () >> 8));
      needupdate = 1;
   }

   /***************************************
    * Check for, and process, mouse input *
    ***************************************/

   /* Update the mouse position */
   oldx = x;
   oldy = y;
   x = mouse_x / 16;
   y = mouse_y / 16;
   if ((oldx != x) || (oldy != y))
      needupdate = 1;

   /* Go back to default drawing mode when mouse buttons released */
   if (!(mouse_b & 1) && !(mouse_b & 2)) {
      /* When draw_mode == GRAB_TILE and the user right-clicks a tile, it
         enters grab mode and continues to "grab" tiles under the mouse
         until the button is released.  Then it automatically changes to
         Layer[1-3] (whichever the last "drawing" mode was) so the user
         can draw that tile onto the map immediately.
       */
      if (dmode == 2) {
         /* Change to a drawing mode, and use the most recently-used Layer. */
         draw_mode = showing.last_layer;
         needupdate = 1;
      }
      mouse_button = 0;
      dmode = 0;
   }

   /* Left mouse button */
   if (mouse_b & 1) {
      mouse_button = 1;
      needupdate = 1;

      /* Check if the mouse is over the menu at the bottom */
      if (y > (vtiles - 1)) {
         /* Check if the user is already holding down the mouse button */
         if (dmode == 0)
            process_menu_bottom (mouse_x, mouse_y);
         return;
      }

      /* Check if the mouse is over the iconset on the right */
      if (x > (htiles - 1)) {
         /* Check if the user is already holding down the mouse button */
         if (dmode == 0)
            process_menu_right (mouse_x, mouse_y);
         return;
      }

      /* The mouse is inside the window.  Draw to the map */
      if ((draw_mode == MAP_LAYER1) || (draw_mode == MAP_LAYER2)
          || (draw_mode == MAP_LAYER3) || (draw_mode == MAP_OBSTACLES)
          || (draw_mode == MAP_ZONES) || (draw_mode == MAP_ENTITIES)
          || (draw_mode == MAP_SHADOWS)) {
         dmode = 1;
      }
      process_mouse (mouse_button);
      return;
   }                            /* if (mouse_b & 1) */

   /* Right mouse button */
   if (mouse_b & 2) {
      mouse_button = 2;
      needupdate = 1;

      /* Check if the mouse is over either (bottom or right) menu */
      if ((y > (vtiles - 1)) || x > (htiles - 1))
         return;

      /* The mouse is inside the window.  Draw to the map */
      if (((draw_mode == MAP_LAYER1) || (draw_mode == MAP_LAYER2)
           || (draw_mode == MAP_LAYER3) || (draw_mode == MAP_OBSTACLES)
           || (draw_mode == MAP_ZONES) || (draw_mode == MAP_ENTITIES)
           || (draw_mode == MAP_SHADOWS)) && (dmode == 0)) {
         dmode = 1;
      }
      process_mouse (mouse_button);
      return;
   }                            /* if (mouse_b & 2) */
   return;
}                               /* read_controls () */

END_OF_FUNCTION (read_controls);


/*! \brief Resize the current map
 * Changes the map's height and width
 *
 * \param   selection 0 means to change both width and height
 *                    1 means to change the width
 *                    2 means to change the height
 */
void resize_map (const int selection)
{
   /* User input */
   int user_input;
   /* New and old widths and heights */
   int neww, oldw, newh, oldh;
   int ix, iy;

   /* Set current and old map sizes, incase one will not change */
   neww = oldw = gmap.xsize;
   newh = oldh = gmap.ysize;

   if (selection == 0 || selection == 1) {
      rectfill (double_buffer, 0, 0, 319, 29, 0);
      rect (double_buffer, 2, 2, 317, 27, 255);
      print_sfont (6, 6, "Resize map", double_buffer);
      print_sfont (6, 18, "Width: ", double_buffer);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      user_input = get_line (48, 18, strbuf, 4);

      /* Make sure the line isn't blank */
      if (user_input == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if ((atoi (strbuf) < 10 || atoi (strbuf) > SW)) {
         cmessage ("Invalid width!");
         wait_enter ();
         return;
      }

      neww = atoi (strbuf);
   }

   if (selection == 0 || selection == 2) {
      rectfill (double_buffer, 0, 0, 319, 29, 0);
      rect (double_buffer, 2, 2, 317, 27, 255);
      print_sfont (6, 6, "Resize map", double_buffer);
      print_sfont (6, 18, "Height: ", double_buffer);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      user_input = get_line (54, 18, strbuf, 4);

      /* Make sure the line isn't blank */
      if (user_input == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if ((atoi (strbuf) < 15 || atoi (strbuf) > SH)) {
         cmessage ("Invalid height!");
         wait_enter ();
         return;
      }

      newh = atoi (strbuf);
   }

   /* Pre-copy the map info */
   for (iy = 0; iy < gmap.ysize; iy++) {
      for (ix = 0; ix < gmap.xsize; ix++) {
         c_map[iy * gmap.xsize + ix] = map[iy * gmap.xsize + ix];
         cb_map[iy * gmap.xsize + ix] = b_map[iy * gmap.xsize + ix];
         cf_map[iy * gmap.xsize + ix] = f_map[iy * gmap.xsize + ix];
         co_map[iy * gmap.xsize + ix] = o_map[iy * gmap.xsize + ix];
         csh_map[iy * gmap.xsize + ix] = sh_map[iy * gmap.xsize + ix];
         cz_map[iy * gmap.xsize + ix] = z_map[iy * gmap.xsize + ix];
      }
   }

   /* Set map to new size */
   gmap.xsize = neww;
   gmap.ysize = newh;

   /* Set memory for new map size */
   free (map);
   map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (b_map);
   b_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (f_map);
   f_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (o_map);
   o_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (sh_map);
   sh_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (z_map, 0, gmap.xsize * gmap.ysize);

   /* Draw all the old map data into the new map size */
   for (iy = 0; iy < oldh; iy++) {
      for (ix = 0; ix < oldw; ix++) {
         if (iy < gmap.ysize && ix < gmap.xsize) {
            map[iy * gmap.xsize + ix] = c_map[iy * oldw + ix];
            b_map[iy * gmap.xsize + ix] = cb_map[iy * oldw + ix];
            f_map[iy * gmap.xsize + ix] = cf_map[iy * oldw + ix];
            o_map[iy * gmap.xsize + ix] = co_map[iy * oldw + ix];
            sh_map[iy * gmap.xsize + ix] = csh_map[iy * oldw + ix];
            z_map[iy * gmap.xsize + ix] = cz_map[iy * oldw + ix];
         }
      }
   }

   /* Free the 'old-map copy' memory */
   free (c_map);
   c_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cb_map);
   cb_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (cf_map);
   cf_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (co_map);
   co_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (csh_map);
   csh_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (cz_map);
   cz_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   /* Re-allocate memory for the 'old-map copy' for next use */
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   memset (csh_map, 0, gmap.xsize * gmap.ysize);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);

   /* Empty the clipboard */
   clipb = 0;
   normalize_view ();
}                               /* resize_map () */

END_OF_FUNCTION (resize_map);


/* When the user presses keys 1-7, we need to remove all the Attributes.
 * \param   lastlayer 1 if this layer can be a "showing.last_layer"
 *                    0 otherwise
 * \param   which_layer Which layer we are going to change to
 */
void select_only (const int lastlayer, const int which_layer)
{
   draw_mode = which_layer;
   showing.entities = 0;
   showing.shadows = 0;
   showing.obstacles = 0;
   showing.zones = 0;
   if (lastlayer)
      showing.last_layer = draw_mode;
}

END_OF_FUNCTION (select_only);


/*! \brief Handy-dandy help screen
 *
 * This is a simple help screen that displays when F1 is pressed
 */
void show_help (void)
{
// This line turns off other/indent.pro indentation settings:
// *INDENT-OFF*
   int this_counter;
   #define NUMBER_OF_ITEMS 55

   char *help_keys[NUMBER_OF_ITEMS] = {
      "               THIS IS THE HELP DIALOG               ",
      "",
      "F2 . . . . . . . . . . . . . . . . . . . . . Load Map",
      "F3 . . . . . . . . . . . . . . . . . . . . . Save Map",
      "N  . . . . . . . . . . . . . . . . . . . . .  New Map",
      "R  . . . . . . . . . . . . . . . . . . . . Resize Map",
      "",
      "1  . . . . . . . . . . . . . . . . . . . Layer 1 Mode",
      "2  . . . . . . . . . . . . . . . . . . . Layer 2 Mode",
      "3  . . . . . . . . . . . . . . . . . . . Layer 3 Mode",
      "4  . . . . . . . . . . . . . . . . .  View Layers 1+2",
      "5  . . . . . . . . . . . . . . . . .  View Layers 1+3",
      "6  . . . . . . . . . . . . . . . . .  View Layers 2+3",
      "7  . . . . . . . . . . . . . . . .  View Layers 1+2+3",
      "A  . . . . . . . . . . . View all Layers + Attributes",
      "C  . . . . . . . . . . Show map preview (w/ parallax)",
      "",
      "G  . . . . . . . . . . . . . . . . . . . .  Grab Tile",
      "L  . . . . . . . . . . . . . . . . . . . .  Last Zone",
      "",
      "E  . . . . . . . . . . . . . . . . . . .  Entity Mode",
      "O  . . . . . . . . . . . . . . . . . .  Obstacle Mode",
      "S  . . . . . . . . . . . . . . . . . . .  Shadow Mode",
      "Z  . . . . . . . . . . . . . . . . . . . .  Zone Mode",
      "T  . . . . . . . . . . . . . . . . .  Block Copy Mode",
      "P  . . . . . . . . . . . . . . . . . Block Paste Mode",
      "ESC  . . . . . . . . . . . . . . .  Cancel Block Copy",
      "",
      "-/+  . . . . . . . . . . . . . . . . Modify Selection",
      "F12  . . . . . . . . . . . . . . . Modify Entity Mode",
      "D  . . . . . . . . . . . . . . . .  Displace Entities",
      "F10  . . . . . . . . . . . . .  Enter Map Description",
      "",
      "F4 . . . . . . . . . . . . . . . . . . .  Clear Layer",
      "F7 . . . . . . . . . . . . . . . . Clear Obstructions",
      "F8 . . . . . . . . . . . . . . . . . .  Clear Shadows",
      "W  . . . . . . . . . . . . . . . . . . . .  Clear Map",
      "",
      "F5 . . . . . . . . . . . . . . . .  Make Map from PCX",
      "J  . . . . . . . . . . . . . . . . Copy Layers to PCX",
      "V  . . . . . . . . . . . . . . .  Visualise whole map",
      "F6 . . . . . . . . . . . . . .  Copy instance of Tile",
      "F9 . . . . . . . . . . . . . . .  Copy Layer to Layer",
      "",
      "F1 . . . . . . . . . . . . . . . . . . This Help Menu",
      "Q  . . . . . . . . . . . . . . . . . . . . . . . Quit",
      "",
      "TAB  . . . . . . . . . . . . . .  Move 1 screen right",
      "BACKSPACE  . . . . . . . . . . . . Move 1 screen left",
      "PGUP . . . . . . . . . . . . . . . . Move 1 screen up",
      "PGDN . . . . . . . . . . . . . . . Move 1 screen down",
      "HOME . . . . . . . . . . . Move to the top of the map",
      "END  . . . . . . . . . . . Move to the end of the map",
      "",
      "                [PRESS ESC OR ENTER]                 "
   };
// This turns the other/indent.pro settings back on:
// *INDENT-ON*

   rectfill (double_buffer, (19 * 6) - 5, 30, (54 * 8) + 5,
             NUMBER_OF_ITEMS * 7 + 40, 0);
   rect (double_buffer, (19 * 6) - 3, 32, (54 * 8) + 3,
         (NUMBER_OF_ITEMS * 7) + 38, 255);

   this_counter = 0;
   for (this_counter = 0; this_counter < NUMBER_OF_ITEMS; this_counter++) {
      sprintf (strbuf, "%s", help_keys[this_counter]);
      print_sfont (19 * 6, (this_counter * 7) + 35, strbuf, double_buffer);
   }
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   yninput ();

}                               /* show_help () */

END_OF_FUNCTION (show_help);


/*! \brief The opposite of shutdown, maybe?
 *
 * Inits everything needed for user input, graphics, etc.
 */
void startup (void)
{
   int k, kx, ky, a, p, q;
   COLOR_MAP cmap;

   allegro_init ();
   install_keyboard ();
   install_timer ();

   /* Determine if user wants to use Windowed or Regular Mode */
   if (WBUILD == 1)
      set_gfx_mode (GFX_AUTODETECT_WINDOWED, SW, SH, 0, 0);
   else
      set_gfx_mode (GFX_AUTODETECT, SW, SH, 0, 0);

   /* Check for the presence of a mouse */
   a = install_mouse ();
   if (a == -1) {
      nomouse = 1;
      klog ("Mouse not found!\n");
      rest (1000);
   }

   /* Create the picture used for the mouse */
   mouse_pic = create_bitmap (4, 6);
   for (ky = 0; ky < 6; ky++)
      for (kx = 0; kx < 4; kx++)
         mouse_pic->line[ky][kx] = mousepic[ky * 4 + kx];
   set_mouse_speed (4, 4);

   /* Buffer for all strings */
   strbuf = (char *) malloc (256);

   /* Screen buffer */
   double_buffer = create_bitmap (SW, SH);
   clear (double_buffer);
   gmap.map_no = -1;
   gmap.tileset = 0;
   gmap.xsize = htiles;
   gmap.ysize = vtiles;

   /* Create a bitmap for each tile/icon used */
   set_palette (pal);
   for (k = 0; k < MAX_TILES; k++) {
      icons[k] = create_bitmap (16, 16);
      clear (icons[k]);
   }
   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);

   /* Determine the number of icons by the size of the PCX image loaded */
   max_sets = (pcx_buffer->h / 16);
   for (p = 0; p < max_sets - 1; p++)
      for (q = 0; q < ICONSET_SIZE; q++)
         blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16, p * 16, 0, 0,
               16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
   bufferize ();
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;
   font6 = create_bitmap (6, 546);
   getfont ();

   /* Used for Obstacles */
   mesh = create_bitmap (16, 16);
   clear (mesh);
   for (ky = 0; ky < 16; ky += 2) {
      for (kx = 0; kx < 16; kx += 2)
         putpixel (mesh, kx, ky, 255);
      for (kx = 1; kx < 16; kx += 2)
         putpixel (mesh, kx, ky + 1, 255);
   }

   /* Shadow images */
   pcx_buffer = load_pcx ("Misc.pcx", pal);
   for (a = 0; a < MAX_SHADOWS; a++) {
      shadow[a] = create_bitmap (16, 16);
      blit (pcx_buffer, shadow[a], a * 16, 160, 0, 0, 16, 16);
   }
   destroy_bitmap (pcx_buffer);

   /* Entity images */
   pcx_buffer = load_pcx ("entities.pcx", pal);
   for (k = 0; k < MAX_EPICS; k++) {
      for (a = 0; a < 12; a++) {
         eframes[k][a] = create_bitmap (16, 16);
         blit (pcx_buffer, eframes[k][a], a * 16, k * 16, 0, 0, 16, 16);
      }
   }
   destroy_bitmap (pcx_buffer);
   init_entities ();
   showing.entities = 0;
   showing.shadows = 0;
   showing.obstacles = 0;
   showing.zones = 0;
   showing.last_layer = draw_mode;
}                               /* startup () */

END_OF_FUNCTION (startup);


/*! \brief Update the tileset
 *
 * This changes the iconset in the menu on the right and then updates
 * the map to show the current tileset
 */
void update_tileset (void)
{
   int tmapx, tmapy;
   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);
   for (tmapy = 0; tmapy < max_sets; tmapy++)
      for (tmapx = 0; tmapx < ICONSET_SIZE; tmapx++)
         blit (pcx_buffer, icons[tmapy * ICONSET_SIZE + tmapx], tmapx * 16,
               tmapy * 16, 0, 0, 16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
}                               /* update_tileset () */

END_OF_FUNCTION (update_tileset);


/* TT: The creation of MAPDUMP supersedes the need for this function.
 * You may now call mapdump and specify which options to display.
 */

/*! \brief Save the whole map as a pcx
 *
 * Make one giant bitmap and draw all the layers on it,
 * so you can get an overview of what's going on.
 * Doesn't draw entities, and doesn't work well with parallax.
 * (any ideas how to handle parallax?)
 * \author PH
 * \date 20030412
 */
void visual_map (void)
{
   int i, j, w;
   BITMAP *bmp;
   PALETTE pal;

   /* Create a bitmap the same size as the map */
   if ((bmp = create_bitmap (gmap.xsize * 16, gmap.ysize * 16)) != NULL) {
      for (j = 0; j < gmap.ysize; j++) {
         for (i = 0; i < gmap.xsize; i++) {
            /* Which tile is currently being evaluated */
            w = gmap.xsize * j + i;

            blit (icons[map[w]], bmp, 0, 0, i * 16, j * 16, 16, 16);
            draw_sprite (bmp, icons[b_map[w]], i * 16, j * 16);
            draw_sprite (bmp, icons[f_map[w]], i * 16, j * 16);
            draw_trans_sprite (bmp, shadow[sh_map[w]], i * 16, j * 16);

            if (z_map[w] > 0 && z_map[w] < MAX_ZONES) {
               if (z_map[w] < 10) {
                  /* The zone's number is single-digit */
                  textprintf_ex (bmp, font, i * 16 + 4, j * 16 + 4,
                                 makecol (255, 255, 255), 0, "%d", z_map[w]);
               } else if (z_map[w] < 100) {
                  /* The zone's number is double-digit */
                  textprintf_ex (bmp, font, i * 16, j * 16 + 4,
                                 makecol (255, 255, 255), 0, "%d", z_map[w]);
               } else if (i < 1000) {
                  /* The zone's number is triple-digit */
                  textprintf_ex (bmp, font, i * 16 + 4, j * 16,
                                 makecol (255, 255, 255), 0, "%d",
                                 (int) (z_map[w] / 100));
                  textprintf_ex (bmp, font, i * 16, j * 16 + 8,
                                 makecol (255, 255, 255), 0, "%02d",
                                 (int) (z_map[w] % 100));
               }
            }
         }
      }

      get_palette (pal);
      save_bitmap ("vis_map.pcx", bmp, pal);
      destroy_bitmap (bmp);
   }
}                               /* visual_map () */

END_OF_FUNCTION (visual_map);


/*! \brief Wait for ENTER key
 *
 * There's almost no point in explaining this function further :-)
 */
void wait_enter (void)
{
   int a, done = 0;

   while (!done) {
      /* TT: Updated so ENTER/ESC function the same as Y/N */
      a = (readkey () >> 8);
      if (a == KEY_ENTER || a == KEY_ENTER_PAD)
         done = 1;
   }
}                               /* wait_enter () */

END_OF_FUNCTION (wait_enter);


/*! \brief Clear the contents of a map
 *
 * This resets each of the map's layers
 */
void wipe_map (void)
{
   cmessage ("Do you want to clear the whole map? (y/n)");
   if (!yninput ())
      return;

   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (z_map, 0, gmap.xsize * gmap.ysize);

   /* TT FIXED: Forgot to get rid of all the entities! */
   init_entities ();
}                               /* wipe_map () */

END_OF_FUNCTION (wipe_map);


/*! \brief Keyboard input for yes/no question
 *
 * Processes input for a yes/no question:
 *   Y or ENTER do the same thing
 *   N or ESC do the same thing
 *
 * \returns 1 for Y/ENTER, 0 for N/ESC
 */
int yninput (void)
{
   int ch, done = 0;

   while (!done) {
      /* TT: Updated so ENTER/ESC function the same as Y/N */
      ch = (readkey () >> 8);
      if (ch == KEY_N || ch == KEY_ESC)
         done = 1;
      if (ch == KEY_Y || ch == KEY_ENTER || ch == KEY_ENTER_PAD)
         done = 2;
   }
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   return done - 1;
}                               /* yninput () */

END_OF_FUNCTION (yninput);
