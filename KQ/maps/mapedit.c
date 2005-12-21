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

/* Something for allegro version compatibility */
/* ..can we use the textout_ex() and friends? */
#if (ALLEGRO_VERSION >= 4 && ALLEGRO_SUB_VERSION >= 1)
#define HAVE_TEXT_EX
#endif

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mapdraw.h"

/* globals */

/* mesh1[MAX_OBSTACLES]: Obstacles
 * mesh2: Highlight
 * mesh3: MapBoundaries
 * shadow: Shadows
 */
BITMAP *mesh2, *mesh3;
static BITMAP *mouse_pic;

/* Used for the right-hand menu, plus something for the mouse */
short nomouse = 0;

/* window_x and window_y are view-window coords
 * x and y are everything else
 */
short window_x = 0, window_y = 0, x = 0, y = 0;

static int draw_mode = MAP_LAYER123, curtile = 0, dmode = 0;
static int curzone = 0, curshadow = 0, curobs = 0;
static int copying = 0, copyx1 = -1, copyx2 = -1, copyy1 = -1, copyy2 = -1;
static int clipb = 0, cbh = 0, cbw = 0;
static int needupdate, highlight, grab_tile = 0;
static const int COLUMN_WIDTH = 80;


/*! Tile animation specifiers for each tile set */
/* Format: {starting_tile, finishing_tile, animation_speed}
 * You may have up to 5 animations per tile set and tiles
 * must be sequential; they cannot jump around on the set.
 */
s_anim tanim[NUM_TILESETS][MAX_ANIM] = {
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

/*! Current sequence position of animated tiles */
unsigned short tilex[MAX_TILES];

const int htiles = (SW - 80) / 16;
const int vtiles = (SH - 48) / 16;
int column[5], row[8];

/*! Markers in use in this map */
int curmarker;
int cpu_usage = 2; // Default to rest(1)


/* Welcome to Mapdraw, folks! */
int main (int argc, char *argv[])
{
   int main_stop = 0, oldmouse_x = 0, oldmouse_y = 0;
   int i;

   row[0] = vtiles * 16 + 6;
   for (i = 1; i < 8; i++) {
      row[i] = row[i - 1] + 6;
   }

   column[0] = 0;
   for (i = 1; i < 5; i++) {
      column[i] = column[i - 1] + COLUMN_WIDTH + 1;
   }

   needupdate = 0;

   if (!startup ())
      return 1;

   if (argc > 1) {
      load_map (argv[1]);
      /* Recount the number of entities on the map */
      number_of_ents = 0;

      for (i = 0; i < 50; i++) {
         if (gent[i].active == 1)
            number_of_ents = i + 1;
      }
      if (gmap.revision == 1) {
         /* copy out the markers */
         num_markers = gmap.num_markers;
         memcpy (markers, gmap.markers, gmap.num_markers * sizeof (s_marker));
         curmarker = 0;
      } else {
         num_markers = 0;
      }
   }

   while (!main_stop) {
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
         }
         blit2screen ();
         if (!nomouse) {
            show_mouse (NULL);
            oldmouse_x = mouse_x;
            oldmouse_y = mouse_y;
            scare_mouse ();
         }
      }

      if (key[KEY_Q])
         main_stop = confirm_exit ();
      kq_yield ();
   }                            /* while (!main_stop) */
   cleanup ();
   return EXIT_SUCCESS;
}                               /* main () */

END_OF_MAIN ();


/*! \brief Animation
 *
 * When the user is in MAP_PREVIEW mode and the user hits the spacebar,
 * animate all appropriate tiles (water, fire, etc.).
 */
void animate (void)
{
   int i, j;

   if (draw_mode != MAP_PREVIEW)
      return;

   for (i = 0; i < MAX_ANIM; i++) {
      if (adata[i].start != 0) {
         for (j = adata[i].start; j <= adata[i].end; j++) {
            if (tilex[j] < adata[i].end)
               tilex[j]++;
            else
               tilex[j] = adata[i].start;
         }                      // for (j)
      }
   }                            // for (i)
}                               /* animate */


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


/*! \brief Return the maximum zone used in the map
 *
 * Count and display the maximum Zone Attributes on the map
 *
 * \returns the maximum zone number in the map
 */
int check_last_zone (void)
{
   int a = 0, p;

   for (p = 0; p < gmap.xsize * gmap.ysize; p++) {
      if (z_map[p] > a)
         a = z_map[p];
   }

   return a;
}                               /* check_last_zone () */


/*! \brief Code shutdown and memory deallocation
 *
 * Called at the end of main(), closes everything
 */
void cleanup (void)
{
   int k, j;

   for (k = 0; k < MAX_EPICS; k++)
      for (j = 0; j < 12; j++)
         destroy_bitmap (eframes[k][j]);
   destroy_bitmap (double_buffer);
   destroy_bitmap (mesh2);
   destroy_bitmap (mesh3);
   destroy_bitmap (mouse_pic);

   free (b_map);
   free (f_map);
   free (map);
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

   shared_cleanup ();
}                               /* cleanup () */


/*! \brief Clears a layer from the map
 *
 * Remove an entire Layer from the map
 */
void clear_layer (void)
{
   int response, i, done;
   int selected_layer = 0;

   make_rect (double_buffer, 2, 15);
   print_sfont (6, 6, "Clear Layer", double_buffer);
   print_sfont (6, 12, "Layer (1-3): ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (84, 12, strbuf, 2);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         selected_layer = atoi (strbuf);

         /* Make sure the value is valid */
         if (selected_layer < 1 || selected_layer > 3) {
            cmessage ("Invalid layer!");
            wait_enter ();
         } else {
            sprintf (strbuf, "Do you want to clear Layer %d? (y/n)",
                     selected_layer);
            cmessage (strbuf);
            if (yninput ())
               done = 1;
         }
      }
   }

   for (i = 0; i < gmap.xsize * gmap.ysize; i++) {
      if (selected_layer == 1)
         map[i] = 0;
      else if (selected_layer == 2)
         b_map[i] = 0;
      else if (selected_layer == 3)
         f_map[i] = 0;
   }
}                               /* clear_layer () */


/*! \brief Clear obstructions on map
 *
 * This goes through the entire map and removes all Obstructions
 */
void clear_obstructs (void)
{
   int i;

   cmessage ("Do you want to clear Obstructions? (y/n)");
   if (yninput ())
      for (i = 0; i < gmap.xsize * gmap.ysize; i++)
         o_map[i] = 0;
}                               /* clear_obstructs () */


/*! \brief Clear shadows on the map
 *
 * This goes through the entire map and removes all Shadows
 */
void clear_shadows (void)
{
   int i;

   cmessage ("Do you want to clear Shadows? (y/n)");
   if (yninput ())
      for (i = 0; i < gmap.xsize * gmap.ysize; i++)
         sh_map[i] = 0;
}                               /* clear_shadows () */


/*! \brief Default method for displaying messages to screen
 *
 * This displays messages onto the screen; used for warnings, reminders, etc.
 *
 * \param   buff String of text to be printed to the screen
 */
void cmessage (char *buff)
{
   int msg_length;

   msg_length = strlen (buff) * 3;
   rectfill (screen, 154 - msg_length, 89, 165 + msg_length, 105, 0);
   rect (screen, 156 - msg_length, 91, 163 + msg_length, 103, 255);
   print_sfont (160 - msg_length, 95, buff, screen);
}                               /* cmessage () */


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


/*! \brief Copy an entire layer into another
 *
 * Copy the contents of one Layer to another
 */
void copy_layer (void)
{
   int response, done, a, b;
   int from_layer = 0, to_layer = 0;

   make_rect (double_buffer, 3, 20);
   print_sfont (6, 6, "Copy Layer to Layer", double_buffer);
   print_sfont (6, 12, "From (1-3):", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (78, 12, strbuf, 2);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure the line isn't blank */
      if (strlen (strbuf) > 0) {
         from_layer = atoi (strbuf);

         /* Make sure the value is valid */
         if (from_layer < 1 || from_layer > 3) {
            cmessage ("Invalid layer!");
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }                            // while ()

   /* This is incase we need to redraw the map, the information will still be
    * visible to the user
    */
   sprintf (strbuf, "%d", from_layer);
   print_sfont (78, 12, strbuf, double_buffer);
   print_sfont (6, 18, "To (1-3):", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (78, 18, strbuf, 2);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure the line isn't blank */
      if (strlen (strbuf) > 0) {
         to_layer = atoi (strbuf);

         /* Make sure the value is valid */
         if (to_layer < 1 || to_layer > 3) {
            cmessage ("Invalid layer!");
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }

   for (a = 0; a < gmap.xsize * gmap.ysize; a++) {
      if (from_layer == 1)
         b = map[a];
      else if (from_layer == 2)
         b = b_map[a];
      else if (from_layer == 3)
         b = f_map[a];

      if (to_layer == 1)
         map[a] = b;
      else if (to_layer == 2)
         b_map[a] = b;
      else if (to_layer == 3)
         f_map[a] = b;
   }
}                               /* copy_layer () */


/*! \brief Copy all the layers in a block area
 *
 * Copy all of the layers from one area to another
 */
void copy_region (void)
{
   /* Area block coords */
   int zx, zy;
   /* Used to swap 'backward' copy areas */
   int swapx, swapy, coord1, coord2;

   clipb = 0;
   cbh = copyy2 - copyy1;
   cbw = copyx2 - copyx1;

   /* Determines if 'copy start coords' < 'copy finish coords' and fixes it */
   if (cbh < 0) {
      /* This swaps the 'start' and 'finish' y-coords if backward */
      swapy = copyy2;
      copyy2 = copyy1;
      copyy1 = swapy;
      cbh = copyy2 - copyy1;
   }

   if (cbw < 0) {
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
         coord1 = zy * gmap.xsize + zx;
         coord2 = (copyy1 + zy) * gmap.xsize + copyx1 + zx;

         c_map[coord1] = map[coord2];
         cb_map[coord1] = b_map[coord2];
         cf_map[coord1] = f_map[coord2];
         co_map[coord1] = o_map[coord2];
         csh_map[coord1] = sh_map[coord2];
         cz_map[coord1] = z_map[coord2];
      }
   }
}                               /* copy_region () */


/*! \brief Return the number of times THIS obstacle is used on the map
 *
 * \returns the number of times the obstacle is used
 */
int count_current_obstacles (void)
{
   int i = 0, j;
   for (j = 0; j < gmap.xsize * gmap.ysize; j++) {
      if (o_map[j] == curobs) {
         i++;
      }
   }
   return i;
}                               /* count_current_obstacles () */


/*! \brief Return the number of times THIS shadow is used on the map
 *
 * \returns the number of times the shadow is used
 */
int count_current_shadows (void)
{
   int i = 0, j;
   for (j = 0; j < gmap.xsize * gmap.ysize; j++) {
      if (sh_map[j] == curshadow) {
         i++;
      }
   }
   return i;
}                               /* count_current_shadows () */


/*! \brief Return the number of times THIS zone is used on the map
 *
 * \returns the number of times the zone is used
 */
int count_current_zones (void)
{
   int i = 0, j;
   for (j = 0; j < gmap.xsize * gmap.ysize; j++) {
      if (z_map[j] == curzone) {
         i++;
      }
   }
   return i;
}                               /* count_current_zones () */


/*! \brief Set the map's description
 *
 * This is what displays in the game when a new map is entered (ie its name)
 */
void describe_map (void)
{
   int response, done;

   make_rect (double_buffer, 3, 57);
   print_sfont (6, 6, "Enter map description", double_buffer);
   sprintf (strbuf, "Current: %s", gmap.map_desc);
   print_sfont (6, 12, strbuf, double_buffer);
   print_sfont (6, 18, "New description: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (108, 18, strbuf, 40);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) == 0) {
         cmessage ("Do you want the description to be blank? (y/n)");
         if (yninput ())
            done = 1;
      } else {
         done = 1;
      }
   }
   strcpy (gmap.map_desc, strbuf);
}                               /* describe_map () */


/*! \brief Update the screen
 *
 * Update the screen after all controls taken care of.
 */
void draw_map (void)
{
   /* Coordinates inside the view-window */
   int dx, dy;
   /* Index for tiles inside the view-window */
   int w;
   /* Size of the map or view-window, whichever is smaller */
   int maxx, maxy;
   int i;

   /* Make sure we reset the visible attributes */
   for (i = 0; i < 3; i++)
      showing.layer[i] = 0;

   /* Clear everything with black */
   rectfill (double_buffer, 0, 0, htiles * 16, vtiles * 16, 0);

   /* The maxx/maxy is used since the map isn't always as large as the
    * view-window; we don't want to check/update anything that would be out of
    * bounds.
    */
   maxx = htiles > gmap.xsize ? gmap.xsize : htiles;
   maxy = vtiles > gmap.ysize ? gmap.ysize : vtiles;

   /* This loop will draw everything within the view-window */
   for (dy = 0; dy < maxy; dy++) {
      for (dx = 0; dx < maxx; dx++) {

         /* This index is the sum of the view-window's coordinates on the map
          * and the current tile's coordinates inside the view-window.
          */
         w = ((window_y + dy) * gmap.xsize) + window_x + dx;

         if (draw_mode & MAP_LAYER1)
            showing.layer[0] = 1;
         if (draw_mode & MAP_LAYER2)
            showing.layer[1] = 1;
         if (draw_mode & MAP_LAYER3)
            showing.layer[2] = 1;

         switch (draw_mode) {
         case BLOCK_COPY:
         case BLOCK_PASTE:
         case MAP_ENTITIES:
         case MAP_SHADOWS:
         case MAP_OBSTACLES:
         case MAP_MARKERS:
         case MAP_ZONES:
            if (showing.last_layer & MAP_LAYER1)
               showing.layer[0] = 1;
            if (showing.last_layer & MAP_LAYER2)
               showing.layer[1] = 1;
            if (showing.last_layer & MAP_LAYER3)
               showing.layer[2] = 1;
            if (showing.last_layer == MAP_PREVIEW) {
               showing.layer[0] = 1;
               showing.layer[1] = 1;
               showing.layer[2] = 1;
            }
            break;
         default:
            break;
         }                      // switch (draw_mode)

         /* Clear the background before drawing */
         if ((draw_mode >= MAP_LAYER1 && draw_mode <= MAP_LAYER3))
            rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                      dy * 16 + 15, 0);

         if (showing.layer[0])
            draw_sprite (double_buffer, icons[map[w]], dx * 16, dy * 16);
         if (showing.layer[1])
            draw_sprite (double_buffer, icons[b_map[w]], dx * 16, dy * 16);
         if (showing.layer[2])
            draw_sprite (double_buffer, icons[f_map[w]], dx * 16, dy * 16);

         /* Draw the Shadows */
         if ((showing.shadows) && (sh_map[w] > 0) && (sh_map[w] < MAX_SHADOWS)) {
            draw_trans_sprite (double_buffer, shadow[sh_map[w]], dx * 16,
                               dy * 16);
         }

         /* Draw the Obstacles */
         if ((showing.obstacles) && (o_map[w] > 0)
             && (o_map[w] <= MAX_OBSTACLES)) {
            draw_sprite (double_buffer, mesh1[o_map[w] - 1], dx * 16, dy * 16);
         }                      // if (showing.obstacles)

         /* Draw the Zones */
         if ((showing.zones) && (z_map[w] > 0) && (z_map[w] < MAX_ZONES)) {
/* This check is here because of the differing versions of the Allegro library */
#ifdef HAVE_TEXT_EX
            if (z_map[w] < 10) {
               /* The zone's number is single-digit, center vert+horiz */
               textprintf_ex (double_buffer, font, dx * 16 + 4, dy * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            } else if (z_map[w] < 100) {
               /* The zone's number is double-digit, center only vert */
               textprintf_ex (double_buffer, font, dx * 16, dy * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            } else if (z_map[w] < 1000) {
               /* The zone's number is triple-digit.  Print the 100's digit in
                * top-center of the square; the 10's and 1's digits on bottom
                * of the square
                */
               textprintf_ex (double_buffer, font, dx * 16 + 4, dy * 16,
                              makecol (255, 255, 255), 0, "%d",
                              (int) (z_map[w] / 100));
               textprintf_ex (double_buffer, font, dx * 16, dy * 16 + 8,
                              makecol (255, 255, 255), 0, "%02d",
                              (int) (z_map[w] % 100));
            }
#else
            if (z_map[w] < 10) {
               /* The zone's number is single-digit, center vert+horiz */
               textprintf (double_buffer, font, dx * 16 + 4, dy * 16 + 4,
                           makecol (255, 255, 255), "%d", z_map[w]);
            } else if (z_map[w] < 100) {
               /* The zone's number is double-digit, center only vert */
               textprintf (double_buffer, font, dx * 16, dy * 16 + 4,
                           makecol (255, 255, 255), "%d", z_map[w]);
            } else if (z_map[w] < 1000) {
               /* The zone's number is triple-digit.  Print the 100's digit in
                * top-center of the square; the 10's and 1's digits on bottom
                * of the square
                */
               textprintf (double_buffer, font, dx * 16 + 4, dy * 16,
                           makecol (255, 255, 255), "%d",
                           (int) (z_map[w] / 100));
               textprintf (double_buffer, font, dx * 16, dy * 16 + 8,
                           makecol (255, 255, 255), "%02d",
                           (int) (z_map[w] % 100));
            }
#endif
         }                      // if (showing.zones)

         /* Highlight each instance of the Attribute */
         if (highlight) {
            if ((draw_mode == MAP_SHADOWS && sh_map[w] == curshadow
                 && curshadow > 0)
                || (draw_mode == MAP_OBSTACLES && o_map[w] == curobs
                    && curobs > 0)
                || (draw_mode == MAP_ZONES && z_map[w] == curzone
                    && curzone > 0))
               draw_sprite (double_buffer, mesh2, dx * 16, dy * 16);
         }
      }                         /* for (dx) */
   }                            /* for (dy) */

   /* Show the map boundary with red diag-lined tiles */
   for (dy = 0; dy < vtiles; dy++) {
      for (dx = 0; dx < htiles; dx++) {
         if ((dy > maxy - 1) || (dx > maxx - 1)) {
            draw_sprite (double_buffer, mesh3, dx * 16, dy * 16);
         }
      }
   }

   /* Draw the Markers */
   if (showing.markers == 1 && num_markers > 0) {
      for (i = 0; i < num_markers; ++i) {
         if ((markers[i].x >= window_x) && (markers[i].x < window_x + htiles)
             && (markers[i].y >= window_y)
             && (markers[i].y < window_y + vtiles)) {
            draw_sprite (double_buffer, marker_image,
                         (markers[i].x - window_x) * 16 + 8,
                         (markers[i].y - window_y) * 16 - 8);
         }                      /* If marker is visible */
      }                         /* For each marker */

      /* Only draw highlight around marker if the mode is correct */
      if (draw_mode == MAP_MARKERS) {
         /* Put a rectangle around the selected one for clarity */
         rect (double_buffer, (markers[curmarker].x - window_x) * 16,
               (markers[curmarker].y - window_y) * 16,
               (markers[curmarker].x - window_x) * 16 + 15,
               (markers[curmarker].y - window_y) * 16 + 15, 25);
      }
   }

   /* If showing markers */
   /* Draw the Entities */
   if (showing.entities == 1) {
      for (i = 0; i < number_of_ents; i++) {
         /* Draw only the entities within the view-screen */
         if ((gent[i].tilex >= window_x) && (gent[i].tilex < window_x + htiles)
             && (gent[i].tiley >= window_y)
             && (gent[i].tiley < window_y + vtiles)) {
            /* Draw either a normal sprite or a translucent one */
            if (gent[i].transl == 0) {
               draw_sprite (double_buffer,
                            eframes[gent[i].chrx][gent[i].facing * 3],
                            (gent[i].tilex - window_x) * 16,
                            (gent[i].tiley - window_y) * 16);
            } else {
               draw_trans_sprite (double_buffer,
                                  eframes[gent[i].chrx][gent[i].facing * 3],
                                  (gent[i].tilex - window_x) * 16,
                                  (gent[i].tiley - window_y) * 16);
            }                   // if..else ()
         }                      // if (gent[i].tilex/tiley)
      }                         // for (i)
   }                            // if (showing.entities)

   /* Displays the rectangle around the Block Copy coords */
   if (draw_mode == BLOCK_COPY && copyx1 != -1 && copyy1 != -1) {
      int rectx1, rectx2, recty1, recty2;
      if (copyx1 < window_x + htiles && copyy1 < window_y + vtiles) {
         rectx1 = (copyx1 - window_x) * 16;
         recty1 = (copyy1 - window_y) * 16;
         rectx2 = copyx2 < window_x + htiles ?
            (copyx2 - window_x) * 16 + 15 : htiles * 16;
         recty2 = copyy2 < window_y + vtiles ?
            (copyy2 - window_y) * 16 + 15 : vtiles * 16;

         if (copying == 0) {
            /* Highlight the selected tile (takes into account window's coords) */
            rect (double_buffer, rectx1, recty1, rectx2, recty2, 25);
         } else {
            /* Only the initial tile is selected */
            rect (double_buffer, rectx1, recty1, rectx1 + 15, recty1 + 15, 25);
         }
      }
   }
}                               /* draw_map () */


/*! \brief Draw the menus
 *
 * Process both the menus on the side and bottom of the screen
 */
void draw_menubars (void)
{
   int p, xp = 0, yp = 0;
   int draw_mode_display, draw_mode_last, draw_the_rect;

   /* Description for the current draw_mode (could use work) */
   char dt[][12] = { "Layer1", "Layer2", "Layer3",
      "View L1+2", "View L1+3", "View L2+3", "View L1+2+3",
      "Shadows", "Zones", "Obstacles", "Entities",
      "Block Copy", "Block Paste", "Preview", "Markers",
   };

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
   case MAP_PREVIEW:
      draw_mode_display = 13;
      break;
   case MAP_MARKERS:
      draw_mode_display = 14;
      break;
   default:
      draw_mode_display = 0;
      break;
   }                            /* switch (draw_mode) */

   draw_mode_last = 0;

   switch (showing.last_layer) {
   case MAP_LAYER1:
      draw_mode_last = 0;
      break;
   case MAP_LAYER2:
      draw_mode_last = 1;
      break;
   case MAP_LAYER3:
      draw_mode_last = 2;
      break;
   case MAP_LAYER12:
      draw_mode_last = 3;
      break;
   case MAP_LAYER13:
      draw_mode_last = 4;
      break;
   case MAP_LAYER23:
      draw_mode_last = 5;
      break;
   case MAP_LAYER123:
      draw_mode_last = 6;
      break;
   case MAP_SHADOWS:
      draw_mode_last = 7;
      break;
   case MAP_ZONES:
      draw_mode_last = 8;
      break;
   case MAP_OBSTACLES:
      draw_mode_last = 9;
      break;
   case MAP_ENTITIES:
      draw_mode_last = 10;
      break;
   case BLOCK_COPY:
      draw_mode_last = 11;
      break;
   case BLOCK_PASTE:
      draw_mode_last = 12;
      break;
   case MAP_PREVIEW:
      draw_mode_last = 13;
      break;
   default:
      draw_mode_last = 0;
      break;
   }                            /* switch (showing.last_layer) */

   /* Clear the bottom and right sides so we don't leave any artifacts */
   rectfill (double_buffer, 0, vtiles * 16 + 2, htiles * 16 + 2, SH - 1, 0);
   rectfill (double_buffer, htiles * 16 + 2, 0, SW - 1, SH - 1, 0);

   /* The white horizontal line that seperates the bottom menu */
   hline (double_buffer, 0, vtiles * 16 + 1, htiles * 16 + 1, 255);

   /* The white verticle bar that seperates the right tileset */
   vline (double_buffer, htiles * 16 + 1, 0, vtiles * 16 + 1, 255);

   /* The name of the current map */
   sprintf (strbuf, "Map:  %s", map_fname);
   print_sfont (column[0], row[0], strbuf, double_buffer);

   /* Show which tileset the map will use */
   sprintf (strbuf, "Icon: %s (%d)", icon_files[gmap.tileset], gmap.tileset);
   print_sfont (column[0], row[1], strbuf, double_buffer);

   /* Give the default song played when the map is entered */
   sprintf (strbuf, "Song: %s", gmap.song_file);
   print_sfont (column[0], row[2], strbuf, double_buffer);

   /* Allow the map to treat Zone 0 like a normal zone */
   if (gmap.zero_zone == 0)
      print_sfont (column[0], row[3], "ZeroZone: NO", double_buffer);
   else
      print_sfont (column[0], row[3], "ZeroZone: YES", double_buffer);

   /* Display the map number */
   sprintf (strbuf, "Map #: %d", gmap.map_no);
   print_sfont (column[0], row[4], strbuf, double_buffer);

   /* The current mode we use (see drawmap() in the main game for info) */
   sprintf (strbuf, "Mode: %s", map_mode_text[gmap.map_mode]);
   print_sfont (column[0], row[5], strbuf, double_buffer);

   /* Can the player save the game here? */
   if (gmap.can_save == 0)
      print_sfont (column[0], row[6], "Save: NO", double_buffer);
   else
      print_sfont (column[0], row[6], "Save: YES", double_buffer);

   /* Can the player use the Warp spell from here? */
   if (gmap.can_warp == 0)
      print_sfont (column[1], row[4], "Warp: NO", double_buffer);
   else
      print_sfont (column[1], row[4], "Warp: YES", double_buffer);

   /* Coordinates where Warp spell takes you to
    * NOTE: only works for main.map apparently...
    */
   sprintf (strbuf, "WarpX: %d", gmap.warpx);
   print_sfont (column[1], row[5], strbuf, double_buffer);
   sprintf (strbuf, "WarpY: %d", gmap.warpy);
   print_sfont (column[1], row[6], strbuf, double_buffer);

   /* Default area to warp to when a map is entered */
   sprintf (strbuf, "Start X: %d", gmap.stx);
   print_sfont (column[2], row[1], strbuf, double_buffer);
   sprintf (strbuf, "Start Y: %d", gmap.sty);
   print_sfont (column[2], row[2], strbuf, double_buffer);

   /* Size of the current map */
   sprintf (strbuf, "Width: %d", gmap.xsize);
   print_sfont (column[2], row[3], strbuf, double_buffer);
   sprintf (strbuf, "Height: %d", gmap.ysize);
   print_sfont (column[2], row[4], strbuf, double_buffer);

   /* Allow the player to use SunStones on this map */
   print_sfont (column[2], row[5],
                gmap.use_sstone == 0 ? "SunStone: NO" : "SunStone: YES",
                double_buffer);

   /* Count Zone attributes on map */
   sprintf (strbuf, "Last zone: %d", check_last_zone ());
   print_sfont (column[2], row[6], strbuf, double_buffer);

   /* Parallaxing multiplication/division values (usually 1:1) */
   sprintf (strbuf, "Mult: %d", gmap.pmult);
   print_sfont (column[3], row[1], strbuf, double_buffer);
   sprintf (strbuf, "Div: %d", gmap.pdiv);
   print_sfont (column[3], row[2], strbuf, double_buffer);

   /* Displays the value of the Obstacle under the mouse */
   if (draw_mode == MAP_OBSTACLES) {
      if (curobs > 0)
         sprintf (strbuf, "Obstacle #%d (found: %d)", curobs,
                  count_current_obstacles ());
      else
         sprintf (strbuf, "Obstacle #%d", curobs);
      print_sfont (column[4], row[0], strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = o_map[((window_y + yp) * gmap.xsize) + window_x + xp];
         sprintf (strbuf, "Current Tile: %d", p);
         print_sfont (column[4], row[1], strbuf, double_buffer);
      }
   }                            // if (draw_mode == MAP_OBSTALES)

   /* Displays the value of the Shadow under the mouse */
   if (draw_mode == MAP_SHADOWS) {
      if (curshadow > 0)
         sprintf (strbuf, "Shadow #%d (found: %d)", curshadow,
                  count_current_shadows ());
      else
         sprintf (strbuf, "Shadow #%d", curshadow);
      print_sfont (column[4], row[0], strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = sh_map[((window_y + yp) * gmap.xsize) + window_x + xp];
         sprintf (strbuf, "Current Tile: %d", p);
         print_sfont (column[4], row[1], strbuf, double_buffer);
      }
   }                            // if (draw_mode == MAP_SHADOWS)

   /* Displays the value of the Zone under the mouse */
   if (draw_mode == MAP_ZONES) {
      if (curzone > 0)
         sprintf (strbuf, "Zone #%d (found: %d)", curzone,
                  count_current_zones ());
      else
         sprintf (strbuf, "Zone #%d", curzone);
      print_sfont (column[4], row[0], strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = z_map[((window_y + yp) * gmap.xsize) + window_x + xp];
         sprintf (strbuf, "Current Tile: %d", p);
         print_sfont (column[4], row[1], strbuf, double_buffer);
      }
   }                            // if (draw_mode == MAP_ZONES)

   /* Displays both the currently selected Marker as well as the
    * Marker under the mouse
    */
   if (draw_mode == MAP_MARKERS && num_markers > 0) {
      sprintf (strbuf, "Marker #%d: %s (%d, %d)", curmarker,
               markers[curmarker].name, markers[curmarker].x,
               markers[curmarker].y);
      print_sfont (column[4], row[0], strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      for (p = 0; p < num_markers; p++) {
         if (markers[p].x == xp + window_x && markers[p].y == yp + window_y) {
            sprintf (strbuf, "Current Marker:");
            print_sfont (column[4], row[1], strbuf, double_buffer);
            sprintf (strbuf, "Marker #%d: \"%s\"", p, markers[p].name);
            print_sfont (column[4] + 24, row[2], strbuf, double_buffer);
         }
      }
   }                            // if (draw_mode == MAP_MARKERS)

   /* Displays the Highlight on the 4 Attributes */
   if (draw_mode >= MAP_OBSTACLES && draw_mode <= MAP_ZONES) {
      if (highlight)
         sprintf (strbuf, "Highlight: ON");
      else
         sprintf (strbuf, "Highlight: OFF");
      print_sfont (column[4], row[2], strbuf, double_buffer);
   }                            // if (draw_mode >= MAP_OBSTACLES)

   if (grab_tile) {
      print_sfont (column[4], row[4], "Currently grabbing:", double_buffer);
      sprintf (strbuf, dt[draw_mode_display]);
      print_sfont (column[4] + 24, row[5], strbuf, double_buffer);
   }                            // if (grab_tile)

   if (draw_mode == BLOCK_COPY || draw_mode == BLOCK_PASTE) {
      sprintf (strbuf, "Copy From:: (%d, %d)", (copyx1 > -1 ? copyx1 : 0),
               (copyy1 > -1 ? copyy1 : 0));
      print_sfont (column[4], row[0], strbuf, double_buffer);

      sprintf (strbuf, "Copy To: (%d, %d)", (copyx2 > -1 ? copyx2 : 0),
               (copyy2 > -1 ? copyy2 : 0));
      print_sfont (column[4], row[1], strbuf, double_buffer);
   }
#define DEBUG
#ifdef DEBUG
   /* Debugging values */
   sprintf (strbuf, "Last Layer: %s", dt[draw_mode_last]);
   print_sfont (column[4], row[6], strbuf, double_buffer);
#endif

   /* Add a border around the iconset on the right */
   for (p = 0; p < 8; p++) {
      rect (double_buffer, htiles * 16 + p + 2, p, (htiles + 5) * 16 - (p + 1),
            173 - p, 24 - p);
   }

   /* Display the iconset in the right menu */
   if (icon_set != 999) {
      for (p = 0; p < (ICONSET_SIZE / 2); p++) {
         /* Left 20 icons */
         blit (icons[icon_set * ICONSET_SIZE + p], double_buffer, 0, 0,
               htiles * 16 + 8 + 1, p * 16 + 8 - 1, 16, 16);
         blit (icons[icon_set * ICONSET_SIZE + p + (ICONSET_SIZE / 2)],
               double_buffer, 0, 0, (htiles + 1) * 16 + 8 + 1, p * 16 + 8 - 1,
               16, 16);
         /* Right 20 icons */
         if (icon_set * ICONSET_SIZE + p + (ICONSET_SIZE) <
             max_sets * ICONSET_SIZE) {
            blit (icons[icon_set * ICONSET_SIZE + p + (ICONSET_SIZE)],
                  double_buffer, 0, 0, (htiles + 2) * 16 + 8 + 1,
                  p * 16 + 8 - 1, 16, 16);
            blit (icons[icon_set * ICONSET_SIZE + p + (ICONSET_SIZE * 3 / 2)],
                  double_buffer, 0, 0, (htiles + 3) * 16 + 8 + 1,
                  p * 16 + 8 - 1, 16, 16);
         } else {
            /* This loops the first 20 icons around when you're at the end of
             * the icon_set
             */
            blit (icons[p], double_buffer, 0, 0, (htiles + 2) * 16 + 8 + 1,
                  p * 16 + 8 - 1, 16, 16);
            blit (icons[ICONSET_SIZE / 2 + p], double_buffer, 0, 0,
                  (htiles + 3) * 16 + 8 + 1, p * 16 + 8 - 1, 16, 16);
         }                      // if..else ()
      }                         // for (p)
   }                            // if (icon_set)

   draw_the_rect = 0;
   /* Calculate from the total 40 icons, which one the user selected */
   if (curtile >= icon_set * ICONSET_SIZE
       && curtile < (icon_set + 2) * ICONSET_SIZE) {
      /* These are the left 20: */
      xp = (curtile - (icon_set * ICONSET_SIZE)) / (ICONSET_SIZE / 2);
      yp = (curtile - (icon_set * ICONSET_SIZE)) % (ICONSET_SIZE / 2);

      draw_the_rect = 1;
   } else if (curtile >= 0 && curtile < ICONSET_SIZE
              && (icon_set == max_sets - 1)) {
      /* These are the right 20: */
      xp = (curtile / (ICONSET_SIZE / 2)) + 2;
      yp = curtile % (ICONSET_SIZE / 2);

      draw_the_rect = 1;
   }                            // if..elseif (curtile)

   /* Draw the rectangle around the selected icon */
   if (draw_the_rect) {
      rect (double_buffer, (xp + htiles) * 16 + 8 + 1, yp * 16 + 8 - 1,
            (xp + htiles + 1) * 16 + 8 + 1, (yp + 1) * 16 + 8 - 1, 255);
   }

   /* Determine which tile is going to be displayed */
   if (draw_mode == MAP_ENTITIES) {
      sprintf (strbuf, "Entity");
      stretch_blit (eframes[current_ent][0], double_buffer, 0, 0, 16, 16,
                    (htiles + 1) * 16 + 8, 250, 32, 32);
   } else if (draw_mode == MAP_OBSTACLES) {
      sprintf (strbuf, "Obstacle");
      if (curobs > 0) {
         stretch_blit (mesh1[curobs - 1], double_buffer, 0, 0, 16, 16,
                       (htiles + 1) * 16 + 8, 250, 32, 32);
      }
   } else if (draw_mode == MAP_SHADOWS) {
      sprintf (strbuf, "Shadow");
      stretch_blit (shadow[curshadow], double_buffer, 0, 0, 16, 16,
                    (htiles + 1) * 16 + 8, 250, 32, 32);
   } else if (draw_mode == MAP_MARKERS) {
      sprintf (strbuf, "Markers");
      stretch_blit (marker_image, double_buffer, 0, 0, 16, 16,
                    (htiles + 1) * 16 + 8, 250, 32, 32);

      /* Align the numbers inside the Preview window */
      p = (htiles + 3) * 16;
      if (curmarker < 10)
         p += 1;
      else if (curmarker < 100)
         p -= 7;
      else if (curmarker < 1000)
         p -= 15;

#ifdef HAVE_TEXT_EX
      textprintf_ex (double_buffer, font, p, 274,
                     makecol (255, 255, 255), 0, "%d", curmarker);
#else
      textprintf (double_buffer, font, p, 274,
                  makecol (255, 255, 255), "%d", curmarker);
#endif

   } else {
      sprintf (strbuf, "Tile");
      stretch_blit (icons[curtile], double_buffer, 0, 0, 16, 16,
                    (htiles + 1) * 16 + 8, 250, 32, 32);
   }
   print_sfont (htiles * 16 + 42 - (strlen (strbuf) * 6 / 2), 234, strbuf,
                double_buffer);
   print_sfont ((htiles + 1) * 16 + 2, 240, "Preview:", double_buffer);
   rect (double_buffer, (htiles + 1) * 16 + 6, 248, (htiles + 3) * 16 + 8 + 1,
         283, 255);

   /* Display the draw_mode */
   print_sfont (htiles * 16 + 8, 176, "Mode:", double_buffer);
   print_sfont (htiles * 16 + 14, 182, dt[draw_mode_display], double_buffer);

   /* Display the iconset and selected icon */
   sprintf (strbuf, "#%d(%d)", icon_set, curtile);
   print_sfont (htiles * 16 + 8, 188, strbuf, double_buffer);

   /* Mouse x/y coordinates on map (if mouse is over map) */
   if ((mouse_x / 16 < htiles) && (mouse_y / 16 < vtiles)) {
      sprintf (strbuf, "x=%d", window_x + x);
      print_sfont (htiles * 16 + 8, 194, strbuf, double_buffer);
      sprintf (strbuf, "y=%d", window_y + y);
      print_sfont (htiles * 16 + 8, 200, strbuf, double_buffer);
   }                            // if (mouse_x, mouse_y)

   /* Show when a user is drawing to the map */
   if (dmode == 1)
      print_sfont ((htiles + 1) * 16, 206, "drawing", double_buffer);

   /* Displays the Entity icon and total and current Entities */
   if (draw_mode == MAP_ENTITIES) {
      sprintf (strbuf, "%d", current_ent);
      print_sfont ((htiles + 4) * 16 + 2, SH - 38, strbuf, double_buffer);
      blit (eframes[current_ent][0], double_buffer, 0, 0, (htiles + 4) * 16,
            (SH - 32), 16, 16);
      sprintf (strbuf, "%d", number_of_ents);
      print_sfont ((htiles + 4) * 16 + 2, SH - 12, strbuf, double_buffer);
   }                            // if (draw_mode == MAP_ENTITIES)

   /* Draw a rectangle around the mouse when it's inside the view-window */
   if (mouse_y / 16 < vtiles && mouse_x / 16 < htiles)
      rect (double_buffer, x * 16, y * 16, x * 16 + 15, y * 16 + 15, 255);

   /* Show the current CPU usage */
   print_sfont (htiles * 16 + 8, 294, "CPU Usage:", double_buffer);
   if (cpu_usage == 0) {
      print_sfont (htiles * 16 + 14, 300, "timeslice", double_buffer);
   } else {
      sprintf (strbuf, "rest(%d)", cpu_usage - 1);
      print_sfont (htiles * 16 + 14, 300, strbuf, double_buffer);
   }

}                               /* draw_menubars () */


/*! \brief Process keyboard input
 *
 * This allows the user to type in values, such as coords, names, etc.
 *
 * \param   line_x X-coord of the input "line"
 * \param   line_y Y-coord of the input "line"
 * \param   buffer Where to sent the result
 * \param   max_len Maximum characters to accept
 * \returns 0 if user hits ESC (cancel)
 * \returns 1 if user hits ENTER
 */
int get_line (const int line_x, const int line_y, char *buffer,
              const int max_len)
{
   int done = 0, index = 0, ch;
   BITMAP *under;

   under = create_bitmap (320, 6);

   blit (screen, under, 0, line_y, 0, 0, 320, 6);
   while (!done) {
      ch = (readkey () & 0xff);

      /* Make sure character entered is valid ASCII */
      if (ch >= 32 && ch <= 126 && index < max_len - 1) {
         buffer[index] = ch;
         buffer[index + 1] = 0;
         print_sfont (line_x, line_y, buffer, screen);
         if (++index == max_len)
            index = max_len - 1;
      } else {
         /* Code for ENTER */
         if (ch == 13) {
            buffer[index] = 0;
            done = 2;
         } else {
            /* Code for BACKSPACE */
            if (ch == 8) {
               if (--index < 0)
                  index = 0;
               buffer[index] = ' ';
               buffer[index + 1] = 0;
               blit (under, screen, 0, 0, 0, line_y, 320, 8);
               print_sfont (line_x, line_y, buffer, screen);
               buffer[index] = 0;
            } else {
               /* Code for ESC */
               if (ch == 27) {
                  done = 1;
               }
            }
         }                      /* if (ch == 13) */
      }                         /* if (ch >= 32 && ch <= 126 && index < max_len - 1) */
   }                            /* while (1) */
   /* Just to make sure, incase something wasn't working right... */
   destroy_bitmap (under);
   return done - 1;
}                               /* get_line () */


/*! \brief Gets the index of the tile under the mouse
 *
 * Grab the currently selected map tile and display it in the iconset
 */
void get_tile (void)
{
   int tile = ((window_y + y) * gmap.xsize) + window_x + x;
   int i;

   switch (draw_mode) {
   case MAP_LAYER1:
      curtile = map[tile];
      break;
   case MAP_LAYER2:
      curtile = b_map[tile];
      break;
   case MAP_LAYER3:
      curtile = f_map[tile];
      break;
   case MAP_ENTITIES:
      for (tile = 0; tile < number_of_ents; tile++)
         if ((gent[tile].tilex == window_x + x)
             && (gent[tile].tiley == window_y + y))
            current_ent = gent[tile].chrx;
      break;
   case MAP_OBSTACLES:
      curobs = o_map[tile];
      break;
   case MAP_SHADOWS:
      curshadow = sh_map[tile];
      break;
   case MAP_ZONES:
      curzone = z_map[tile];
      break;
   case MAP_MARKERS:
      for (i = 0; i < num_markers; i++) {
         if ((markers[i].x == window_x + x)
              && (markers[i].y == window_y + y)) {
            curmarker = i;
         }
      }
   default:
      break;
   }

   if (draw_mode >= MAP_LAYER1 && draw_mode <= MAP_LAYER3)
      icon_set = (curtile / ICONSET_SIZE) - ((curtile / ICONSET_SIZE) % 2);
}                               /* get_tile () */


/*! \brief Change map tiles
 *
 * Change any map tiles from one icon to another throughout the map
 */
void global_change (void)
{
   int response, done;
   int tile_from = 0, tile_to = 0, i;

   /* Layers and attributes */
   int p1 = 0, p2 = 0, p3 = 0, ps = 0, po = 0, pz = 0;

   make_rect (double_buffer, 4, 16);
   print_sfont (6, 6, "From Tile: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (72, 6, strbuf, 4);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         tile_from = atoi (strbuf);

         /* Make sure the value is valid */
         if (!(tile_from >= 0 && tile_from < ICONSET_SIZE * max_sets)) {
            cmessage ("Invalid tile!");
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }                            // while ()

   /* This is incase we need to redraw the map, the information will still be
    * visible to the user
    */
   sprintf (strbuf, "%d", tile_from);
   print_sfont (72, 6, strbuf, double_buffer);
   print_sfont (6, 12, "To Tile:", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (60, 12, strbuf, 4);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         tile_to = atoi (strbuf);

         /* Make sure the value is valid */
         if (!(tile_to >= 0 && tile_to < ICONSET_SIZE * max_sets)) {
            cmessage ("Invalid tile!");
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }                            // while ();

   /* This is incase we need to redraw the map, the information will still be
    * visible to the user
    */
   sprintf (strbuf, "%d", tile_to);
   print_sfont (60, 12, strbuf, double_buffer);
   print_sfont (6, 18, "Layer (123soz):", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (18, 24, strbuf, 7);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure the line isn't blank */
      if (strlen (strbuf) > 0) {
         p1 = strchr (strbuf, '1') ? 1 : 0;
         p2 = strchr (strbuf, '2') ? 1 : 0;
         p3 = strchr (strbuf, '3') ? 1 : 0;
         ps = strchr (strbuf, 's') ? 1 : 0;
         po = strchr (strbuf, 'o') ? 1 : 0;
         pz = strchr (strbuf, 'z') ? 1 : 0;
         done = 1;
      }
   }

   for (i = 0; i < gmap.xsize * gmap.ysize; i++) {
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


/*! \brief Error reporting tool
 *
 * Report errors and comments through this function
 *
 * \param   msg The message you want sent to the LOG file
 */
void klog (char *msg)
{
   char err_msg[80];
   FILE *ff;

   strcat (strncpy (err_msg, msg, sizeof (err_msg) - 1), "\n");
   TRACE ("%s\n%s\n", msg, allegro_error);
   allegro_message (err_msg);

   ff = fopen ("mapdraw.log", "a");
   if (!ff)
      exit (-1);
   fprintf (ff, "%s\n", msg);
   fclose (ff);
}                               /* klog () */


/*! \brief Create a rectangle around the selection
 *
 * This will create an outlined rectangle at the top-left corner of the screen
 * which will ALWAYS be drawn to double_buffer (never screen).
 *
 * \param   rect_height The number of lines we will draw around
 * \param   rect_width The number of CHARACTERS we will draw around
 */
void make_rect (BITMAP * where, const int rect_height, const int rect_width)
{
   if (rect_height < 1 || rect_width < 1)
      return;

   /* Letters are all 6 tiles tall/wide */
   rectfill (where, 0, 0, ((rect_width + 1) * 6) + 4,
             ((rect_height + 1) * 6) + 4, 0);
   rect (where, 2, 2, ((rect_width + 1) * 6) + 2, ((rect_height + 1) * 6) + 2,
         255);
}                               /* make_rect () */


/*! \brief All this does is ensure that we are within the map boundaries */
void normalize_view (void)
{
   /* These prevent the user from moving past the map's edge */
   if (window_x > gmap.xsize - htiles)
      window_x = gmap.xsize - htiles;
   if (window_y > gmap.ysize - vtiles)
      window_y = gmap.ysize - vtiles;
   if (window_x < 0)
      window_x = 0;
   if (window_y < 0)
      window_y = 0;
}                               /* normalize_view () */


/*! \brief Paste the copied selection to all Layers
 *
 * Copy all the layers in a block area to a user-defined point in the map
 *
 * \param   tx Target x-coord
 * \param   ty Target y-coord
 */
void paste_region (const int tx, const int ty)
{
   int zx, zy, coord1, coord2;
   s_marker *m;
   int moved_x, moved_y;

   moved_x = tx - copyx1;       // copyx1 - tx < 0 ? tx - copyx1 : copyx1 - tx;
   moved_y = ty - copyy1;       // copyy1 - ty < 0 ? ty - copyy1 : copyy1 - ty;

   for (m = markers; m < markers + num_markers; ++m) {
      if (!(m->x < copyx1 || m->x > copyx2 || m->y < copyy1 || m->y > copyy2)) {
         /* Move the markers found within the Copy From block */
         m->x = m->x + moved_x;
         m->y = m->y + moved_y;
      }
   }

   /* Set the new coords for the Copy From to the new Copy To
    * location correctly handle moving the markers.
    */
   copyx1 = tx;
   copyx2 = copyx1 + cbw;
   copyy1 = ty;
   copyy2 = copyy1 + cbh;

   if (clipb == 0)
      return;
   for (zy = 0; zy <= cbh; zy++) {
      for (zx = 0; zx <= cbw; zx++) {
         if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) {
            coord1 = (ty + zy) * gmap.xsize + tx + zx;
            coord2 = zy * gmap.xsize + zx;

            map[coord1] = c_map[coord2];
            b_map[coord1] = cb_map[coord2];
            f_map[coord1] = cf_map[coord2];
            z_map[coord1] = cz_map[coord2];
            sh_map[coord1] = csh_map[coord2];
            o_map[coord1] = co_map[coord2];
         }
      }
   }
}                               /* paste_region () */


/*! \brief Pastes user-defined layer(s)
 *
 * This asks the user which layer(s) s/he wants to paste to, then
 * performs the paste operation.
 *
 * \param   tx Target x-coord to paste to
 * \param   ty Target y-coord to paste to
 */
void paste_region_special (const int tx, const int ty)
{
   int response, done;
   int zx, zy, coord1, coord2;

   /* Layers 1-3, Shadows, Obstacles, and Zones */
   int p1, p2, p3, ps, po, pz;

   /* Confirm that there is something in the clipboard; don't attempt if not */
   if (clipb == 0)
      return;

   make_rect (double_buffer, 2, 29);
   print_sfont (6, 6, "Paste which layers? (123soz)", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (6, 12, strbuf, 7);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {

         p1 = strchr (strbuf, '1') ? 1 : 0;
         p2 = strchr (strbuf, '2') ? 1 : 0;
         p3 = strchr (strbuf, '3') ? 1 : 0;
         ps = strchr (strbuf, 's') ? 1 : 0;
         po = strchr (strbuf, 'o') ? 1 : 0;
         pz = strchr (strbuf, 'z') ? 1 : 0;

         for (zy = 0; zy <= cbh; zy++) {
            for (zx = 0; zx <= cbw; zx++) {
               if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) {
                  coord1 = (ty + zy) * gmap.xsize + tx + zx;
                  coord2 = zy * gmap.xsize + zx;

                  if (p1)
                     map[coord1] = c_map[coord2];
                  if (p2)
                     b_map[coord1] = cb_map[coord2];
                  if (p3)
                     f_map[coord1] = cf_map[coord2];
                  if (pz)
                     z_map[coord1] = cz_map[coord2];
                  if (ps)
                     sh_map[coord1] = csh_map[coord2];
                  if (po)
                     o_map[coord1] = co_map[coord2];
               }                /* if () */
            }                   /* for (zx) */
         }                      /* for (zy) */
      }
      done = 1;
   }
}                               /* paste_region_special () */


/*! \brief Draw the shadows
 *
 * Draws the shadows onto the screen and takes into consideration any layer
 * effects that need to take place (see map_mode_text[] for details).
 * This is basically the same as draw_layer().
 * \author  PH
 * \date    20031205
 * \param   parallax 0 draws with parallax off, else parallax on
 */
static void draw_shadow (const int parallax)
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
#ifdef HAVE_TEXT_EX
      textprintf_centre_ex (double_buffer, font, double_buffer->w / 2,
                            double_buffer->h / 2, makecol (255, 255, 255), 0,
                            "Mode %d preview not supported.", gmap.map_mode);
#else
      textprintf_centre (double_buffer, font, double_buffer->w / 2,
                         double_buffer->h / 2, makecol (255, 255, 255),
                         "Mode %d preview not supported.", gmap.map_mode);
#endif
      break;
   }
}                               /* preview_map () */


/*! \brief Draw the layers with parallax
 *
 * Draws a single layer (back, middle or fore) and will compensate for parallax.
 * Drawing is done by draw_sprite() - so the bottom layer needs to go onto a black background.
 * This is called by preview_map().
 * The code is slightly overcomplicated but it is more general, if things need to change later.
 *
 * \author PH
 * \date 20031205
 * \param   layer Pointer to layer data array
 * \param   parallax ==0 draw with parallax off or !=0 on
 */
void draw_layer (short *layer, const int parallax)
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
         draw_sprite (double_buffer,
                      icons[tilex[layer[layer_x + j * gmap.xsize]]],
                      layer_x * 16 - x0, j * 16 - y0);
      }
   }
}                               /* draw_layer () */


/*! \brief Displays the text on the screen
 *
 * Prints the string to the screen using the default font
 *
 * \param   x Where to display the text on the x-coord
 * \param   y Where to display the text on the y-coord
 * \param   string A "thinner" version of yarn, made from cotton or wool
 * \param   where The destination, or where it will be drawn to
 */
void print_sfont (const int print_x, const int print_y, const char *string,
                  BITMAP * where)
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
      masked_blit (font6, where, 0, c * 6, i * 6 + print_x, print_y, 6, 6);
   }
}                               /* print_sfont () */


/*! \brief Keyboard input
 * Keyboard inputs (sorry, no joystick support)
 *
 * \param   k The keyboard key to process
 */
int process_keyboard (const int k)
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
      select_only (1, MAP_LAYER123);
      break;
   case (KEY_A):
      /* Display Layers 1+2+3 and all Attributes */
      draw_mode = MAP_LAYER123;
      showing.entities = 1;
      showing.shadows = 1;
      showing.obstacles = 1;
      showing.zones = 1;
      showing.markers = 1;
      showing.last_layer = draw_mode;
      grab_tile = 0;
      break;
   case (KEY_C):
      /* View Layers 1+2+3, plus Entities and Shadows, as the player would see
       * the map in the game
       */
      draw_mode = MAP_PREVIEW;
      showing.entities = 1;
      showing.shadows = 1;
      showing.obstacles = 0;
      showing.zones = 0;
      showing.markers = 0;
      showing.last_layer = MAP_LAYER123;
      grab_tile = 0;
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
         } else {
            if (draw_mode == MAP_PREVIEW)
               showing.last_layer = MAP_LAYER123;
            draw_mode = MAP_ENTITIES;
         }
      } else {
         if (draw_mode < MAP_ENTITIES)
            showing.last_layer = draw_mode;
         else if (draw_mode == MAP_PREVIEW)
            showing.last_layer = MAP_LAYER123;
         showing.entities = 1;
         draw_mode = MAP_ENTITIES;
      }
      grab_tile = 0;
      break;
   case (KEY_F):
      /* Get the first Zone used and set the indicator to that */
      if (draw_mode == MAP_ZONES)
         curzone = 0;
      break;
   case (KEY_G):
      /* Get the tile under the mouse curser, including all 5 of the
       * Attributes. This will not let you enter grab_tile mode if you are not
       * in a mode which can be drawn onto.
       */
      if ((draw_mode >= MAP_LAYER1 && draw_mode <= MAP_LAYER3)
          || (draw_mode >= MAP_ENTITIES && draw_mode <= MAP_ZONES)
          || draw_mode == MAP_MARKERS)
         grab_tile = 1 - grab_tile;
      else
         grab_tile = 0;
      break;
   case (KEY_H):
      /* Highlight current obstacles, shadows, and zones */
      if (draw_mode >= MAP_OBSTACLES && draw_mode <= MAP_ZONES) {
         highlight = highlight ? 0 : 1;
      }
      break;
   case (KEY_J):
      /* Copy Layers 1, 2, 3 to mini PCX images */
      maptopcx ();
      break;
   case (KEY_L):
      /* Get the last Zone used and set the indicator to that */
      if (draw_mode == MAP_ZONES)
         curzone = check_last_zone ();
      break;
   case (KEY_M):
      /* Show markers or not */
      if (showing.markers == 1) {
         if (draw_mode == MAP_MARKERS) {
            draw_mode = showing.last_layer;
            showing.markers = 0;
         } else {
            if (draw_mode == MAP_PREVIEW)
               showing.last_layer = MAP_LAYER123;
            draw_mode = MAP_MARKERS;
         }
      } else {
         if (draw_mode < MAP_ENTITIES)
            showing.last_layer = draw_mode;
         else if (draw_mode == MAP_PREVIEW)
            showing.last_layer = MAP_LAYER123;
         showing.markers = 1;
         draw_mode = MAP_MARKERS;
      }
      grab_tile = 0;
      break;
   case (KEY_N):
      /* Create a new map; you can choose the tileset to use for it */
      new_map ();
      grab_tile = 0;
      break;
   case (KEY_O):
      /* Toggle whether obstacles should be shown or turned off */
      if (showing.obstacles == 1) {
         if (draw_mode == MAP_OBSTACLES) {
            draw_mode = showing.last_layer;
            showing.obstacles = 0;
         } else {
            if (draw_mode == MAP_PREVIEW)
               showing.last_layer = MAP_LAYER123;
            draw_mode = MAP_OBSTACLES;
         }
      } else {
         if (draw_mode < MAP_ENTITIES)
            showing.last_layer = draw_mode;
         else if (draw_mode == MAP_PREVIEW)
            showing.last_layer = MAP_LAYER123;
         showing.obstacles = 1;
         draw_mode = MAP_OBSTACLES;
      }
      grab_tile = 0;
      break;
   case (KEY_P):
      /* Paste the copied selection area */
      draw_mode = BLOCK_PASTE;
      grab_tile = 0;
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
         } else {
            if (draw_mode == MAP_PREVIEW)
               showing.last_layer = MAP_LAYER123;
            draw_mode = MAP_SHADOWS;
         }
      } else {
         if (draw_mode < MAP_ENTITIES)
            showing.last_layer = draw_mode;
         else if (draw_mode == MAP_PREVIEW)
            showing.last_layer = MAP_LAYER123;
         showing.shadows = 1;
         draw_mode = MAP_SHADOWS;
      }
      grab_tile = 0;
      break;
   case (KEY_T):
      /* Copy a selection */
      draw_mode = BLOCK_COPY;
      grab_tile = 0;
      break;
   case (KEY_U):
      if (++cpu_usage > 2)
         cpu_usage = 0;
   case (KEY_V):
      /* Save whole map as a picture */
      if (draw_mode == MAP_PREVIEW)
      {
         showing.entities   = 1;
         showing.obstacles  = 0;
         showing.shadows    = 1;
         showing.zones      = 0;
         showing.markers    = 0;
         showing.last_layer = 0;
         showing.layer[0]   = 1;
         showing.layer[1]   = 1;
         showing.layer[2]   = 1;
      } else {
         showing.layer[0] = showing.last_layer & MAP_LAYER1;
         showing.layer[1] = showing.last_layer & MAP_LAYER2;
         showing.layer[2] = showing.last_layer & MAP_LAYER3;
      }

      visual_map (showing, "vis_map.pcx");
      break;
   case (KEY_W):
      /* TT TODO: This looks like it does the exact same thing as KEY_N:
       * create a new map.  If it does, let's just get rid of it altogether.
       */
      /* Clear the contents of the current map */
      wipe_map ();
      break;
   case (KEY_Z):
      /* Toggle whether zones should be shown or turned off */
      if (showing.zones == 1) {
         if (draw_mode == MAP_ZONES) {
            draw_mode = showing.last_layer;
            showing.zones = 0;
         } else {
            if (draw_mode == MAP_PREVIEW)
               showing.last_layer = MAP_LAYER123;
            draw_mode = MAP_ZONES;
         }
      } else {
         if (draw_mode < MAP_ENTITIES)
            showing.last_layer = draw_mode;
         else if (draw_mode == MAP_PREVIEW)
            showing.last_layer = MAP_LAYER123;
         showing.zones = 1;
         draw_mode = MAP_ZONES;
      }
      grab_tile = 0;
      break;
   case (KEY_SPACE):
      /* Attempt at giving the user a chance to see the animations */
      if (draw_mode == MAP_PREVIEW)
         animate ();
      break;
   case (KEY_F1):
      /* Display the help screen */
      show_help ();
      break;
   case (KEY_F2):
      /* Load a map */
      prompt_load_map ();
      if (gmap.revision == 1) {
         /* Copy out the markers */
         memcpy (markers, gmap.markers,
                 (num_markers = gmap.num_markers) * sizeof (s_marker));
         curmarker = 0;
      } else {
         num_markers = 0;
      }

      grab_tile = 0;
      break;
   case (KEY_F3):
      /* Save the map you are working on */

      if (gmap.map_no < 0) {
         cmessage ("Don't forget to assign a number to this map!");
         yninput ();
      }

      /* Copy the markers back in */
      gmap.markers = realloc (gmap.markers, num_markers * sizeof (s_marker));
      memcpy (gmap.markers, markers, num_markers * sizeof (s_marker));
      gmap.num_markers = num_markers;
      save_map ();
      break;
   case (KEY_F4):
      /* Erase an entire layer from the map */
      clear_layer ();
      break;
   case (KEY_F5):
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
      /* Enter the Modify Entity mode */
      showing.entities = 1;
      draw_mode = MAP_ENTITIES;
      grab_tile = 0;
      update_entities ();
      break;
   case (KEY_ESC):
      /* Cancel a Block Copy */
      if (copying == 1)
         copying = 0;
      /* Clear coordinates of copy */
      copyx1 = copyx2 = -1;
      copyy1 = copyy2 = -1;
      /* Empty the contents of the clipboard */
      clipb = 0;
      break;
   case (KEY_MINUS):
   case (KEY_MINUS_PAD):
      switch (draw_mode) {

      case MAP_ENTITIES:
         /* Select an Entity to place on the map */
         current_ent--;
         if (current_ent < 0)
            current_ent = MAX_EPICS - 1;
         break;
      case MAP_SHADOWS:
         /* Select the Shadow to place on the map */
         curshadow--;
         if (curshadow < 0)
            curshadow = MAX_SHADOWS - 1;
         break;
      case MAP_OBSTACLES:
         /* Select which Obstacle to set on the map */
         curobs--;
         if (curobs < 0)
            curobs = 5;
         break;
      case MAP_ZONES:
         /* Select a Zone to set on the map */
         curzone--;
         if (curzone < 0)
            curzone = MAX_ZONES - 1;
         break;
      case MAP_MARKERS:
         /* Page through the Markers on the map */
         if (num_markers > 0) {
            curmarker--;
            if (curmarker < 0)
               curmarker = num_markers - 1;
         } else
            break;

         /* Move view-window enough to show marker */
         if (markers[curmarker].x < window_x)
            window_x = markers[curmarker].x;
         else if (markers[curmarker].x > window_x + htiles - 1)
            window_x = markers[curmarker].x - htiles + 1;

         if (markers[curmarker].y < window_y)
            window_y = markers[curmarker].y;
         else if (markers[curmarker].y > window_y + vtiles - 1)
            window_y = markers[curmarker].y - vtiles + 1;

         normalize_view ();
         break;
      default:
         /* Change the iconset's "page" */
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
         if (curobs > MAX_OBSTACLES)
            curobs = 0;
         break;
      case MAP_ZONES:
         /* Select a Zone to set on the map */
         curzone++;
         if (curzone >= MAX_ZONES)
            curzone = 0;
         break;
      case MAP_MARKERS:
         /* Page through the Markers on the map */
         if (num_markers > 0) {
            curmarker++;
            if (curmarker >= num_markers)
               curmarker = 0;
         } else
            break;

         /* Move view-window enough to show marker */
         if (markers[curmarker].x < window_x)
            window_x = markers[curmarker].x;
         else if (markers[curmarker].x > window_x + htiles - 1)
            window_x = markers[curmarker].x - htiles + 1;

         if (markers[curmarker].y < window_y)
            window_y = markers[curmarker].y;
         else if (markers[curmarker].y > window_y + vtiles - 1)
            window_y = markers[curmarker].y - vtiles + 1;

         normalize_view ();
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
   case (KEY_1_PAD):
   case (KEY_2_PAD):
   case (KEY_3_PAD):
   case (KEY_4_PAD):
   case (KEY_6_PAD):
   case (KEY_7_PAD):
   case (KEY_8_PAD):
   case (KEY_9_PAD):
   case (KEY_DOWN):
   case (KEY_LEFT):
   case (KEY_RIGHT):
   case (KEY_UP):
      /* We will evaluate these outside of this switch() statement, but it's
       * important to break out before "default:" returns a zero-value
       */
      break;
   default:
      return 0;
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
   return 1;
}                               /* process_keyboard () */


/*! \brief Select an option from the menu
 *
 * Selects one of the options from the menus at the bottom of the screen
 *
 * \param   cx X-coord of the mouse
 * \param   cy Y-coord of the mouse
 */
void process_menu_bottom (const int cx, const int cy)
{
   int a;
   scare_mouse ();

   /* The mouse is over 'Icon:' menu */
   if (cx >= column[0] && cx < column[2] && cy >= row[1] && cy < row[2]) {
      /* This allows the user to select the tileset used for the current map.
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
   if (cx >= column[0] && cx < column[2] && cy >= row[2] && cy < row[3]) {
      rectfill (double_buffer, column[0] + 6 * 6, row[2], column[2] - 1,
                row[3] - 1, 0);
      print_sfont (column[0] + 6 * 5, row[2], ">", double_buffer);
      hline (double_buffer, column[0] + 6 * 6, row[3] - 1,
             column[0] + 6 * 24 - 1, 255);
      blit2screen ();
      a = get_line (column[0] + 6 * 6, row[2], strbuf, 19);

      /* This is kinda hard to error-check... */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      strcpy (gmap.song_file, strbuf);
      return;
   }

   /* The mouse is over 'ZeroZone' menu */
   if (cx >= column[0] && cx < column[1] && cy >= row[3] && cy < row[4]) {
      gmap.zero_zone = 1 - gmap.zero_zone;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Map #' menu */
   if (cx >= column[0] && cx < column[1] && cy >= row[4] && cy < row[5]) {
      rectfill (double_buffer, column[0] + 6 * 7, row[4], column[1] - 1,
                row[5] - 1, 0);
      print_sfont (column[0] + 6 * 6, row[4], ">", double_buffer);
      hline (double_buffer, column[0] + 6 * 7, row[5] - 1,
             column[0] + 6 * 10 - 1, 255);
      blit2screen ();
      a = get_line (column[0] + 6 * 7, row[4], strbuf, 4);

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
      return;
   }

   /* The mouse is over 'Mode' menu */
   if (cx >= column[0] && cx < column[1] && cy >= row[5] && cy < row[6]) {
      gmap.map_mode++;
      if (gmap.map_mode > 5)
         gmap.map_mode = 0;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Save' menu */
   if (cx >= column[0] && cx < column[1] && cy >= row[6] && cy < row[7]) {
      gmap.can_save = 1 - gmap.can_save;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Warp' menu */
   if (cx >= column[1] && cx < column[2] && cy >= row[4]
       && cy < row[5]) {
      gmap.can_warp = 1 - gmap.can_warp;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'WarpX' menu */
   if (cx >= column[1] && cx < column[2] && cy >= row[5] && cy < row[6]) {
      rectfill (double_buffer, column[1] + 6 * 7, row[5], column[2] - 1,
                row[6] - 1, 0);
      print_sfont (column[1] + 6 * 6, row[5], ">", double_buffer);
      hline (double_buffer, column[1] + 6 * 7, row[6] - 1,
             column[1] + 6 * 11 - 1, 255);
      blit2screen ();
      a = get_line (column[1] + 6 * 7, row[5], strbuf, 5);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) < MAX_WIDTH)) {
         cmessage ("Invalid x-coordinate for warp!");
         wait_enter ();
         return;
      }
      gmap.warpx = atoi (strbuf);
      return;
   }

   /* The mouse is over 'WarpY' menu */
   if (cx >= column[1] && cx < column[2] && cy >= row[6] && cy < row[7]) {
      rectfill (double_buffer, column[1] + 6 * 7, row[6], column[2] - 1,
                row[7] - 1, 0);
      print_sfont (column[1] + 6 * 6, row[6], ">", double_buffer);
      hline (double_buffer, column[1] + 6 * 7, row[7] - 1,
             column[1] + 6 * 11 - 1, 255);
      blit2screen ();
      a = get_line (column[1] + 6 * 7, row[6], strbuf, 5);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) < MAX_HEIGHT)) {
         cmessage ("Invalid y-coordinate for warp!");
         wait_enter ();
         return;
      }
      gmap.warpy = atoi (strbuf);
      return;
   }

   /* The mouse is over 'Start X' menu */
   if (cx >= column[2] && cx < column[3] && cy >= row[1] && cy < row[2]) {
      rectfill (double_buffer, column[2] + 6 * 9, row[1], column[3] - 1,
                row[2] - 1, 0);
      print_sfont (column[2] + 6 * 8, row[1], ">", double_buffer);
      hline (double_buffer, column[2] + 6 * 9, row[2] - 1,
             column[2] + 6 * 13 - 1, 255);
      blit2screen ();
      a = get_line (column[2] + 6 * 9, row[1], strbuf, 5);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) < gmap.xsize)) {
         cmessage ("Invalid starting x-position!");
         wait_enter ();
         return;
      }
      gmap.stx = atoi (strbuf);
      return;
   }

   /* The mouse is over 'Start Y' menu */
   if (cx >= column[2] && cx < column[3] && cy >= row[2] && cy < row[3]) {
      rectfill (double_buffer, column[2] + 6 * 9, row[2], column[3] - 1,
                row[3] - 1, 0);
      print_sfont (column[2] + 6 * 8, row[2], ">", double_buffer);
      hline (double_buffer, column[2] + 6 * 9, row[3] - 1,
             column[2] + 6 * 13 - 1, 255);
      blit2screen ();
      a = get_line (column[2] + 6 * 9, row[2], strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0 || strlen (strbuf) < 1)
         return;
      /* Make sure the value is valid */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) < gmap.ysize)) {
         cmessage ("Invalid starting y-position!");
         wait_enter ();
         return;
      }
      gmap.sty = atoi (strbuf);
      return;
   }

   /* Mouse is over 'Mult' menu */
   if (cx >= column[3] && cx < column[4] && cy >= row[1] && cy < row[2]) {
      rectfill (double_buffer, column[3] + 6 * 6, row[1], column[4] - 1,
                row[2] - 1, 0);
      print_sfont (column[3] + 6 * 5, row[1], ">", double_buffer);
      hline (double_buffer, column[3] + 6 * 6, row[2] - 1,
             column[3] + 6 * 9 - 1, 255);
      blit2screen ();
      a = get_line (column[3] + 6 * 6, row[1], strbuf, 4);

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
      return;
   }

   /* The mouse is over 'Div' menu */
   if (cx >= column[3] && cx < column[4] && cy >= row[2] && cy < row[3]) {
      rectfill (double_buffer, column[3] + 6 * 5, row[2], column[4] - 1,
                row[3] - 1, 0);
      print_sfont (column[3] + 6 * 4, row[2], ">", double_buffer);
      hline (double_buffer, column[3] + 6 * 5, row[3] - 1,
             column[3] + 6 * 8 - 1, 255);
      blit2screen ();
      a = get_line (column[3] + 6 * 5, row[2], strbuf, 4);

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
      return;
   }

   /* The mouse is over the 'Width' menu */
   if (cx >= column[2] && cx < column[3] && cy >= row[3] && cy < row[4]) {
      /* 1 means resize width */
      resize_map (1);
      return;
   }

   /* The mouse is over the 'Height' menu */
   if (cx >= column[2] && cx < column[3] && cy >= row[4] && cy < row[5]) {
      /* 2 means resize height */
      resize_map (2);
      return;
   }

   /* The mouse is over 'SunStone' menu */
   if (cx >= column[2] && cx < column[3] && cy >= row[5] && cy < row[6]) {
      gmap.use_sstone = 1 - gmap.use_sstone;
      while (mouse_b & 1);
      return;
   }
}                               /* process_menu_bottom () */


/*! \brief Select an icon from the tileset
 *
 * Selects one of the tiles from the menu on the right
 *
 * \param   cx X-coord of the tile
 * \param   cy Y-coord of the tile
 */
void process_menu_right (const int cx, const int cy)
{
   int xp, yp;

   /* Make sure that the mouse is over one if the selectable tiles */
   if (cy > 8 && cy < 168 && cx >= htiles * 16 + 8
       && cx < (htiles + 4) * 16 + 8) {
      xp = ((cx - 8) - htiles * 16) / 16;
      yp = (cy - 8) / 16;

      /* Set the tileset to the correct "page" */
      if (icon_set + (xp / 2) > max_sets - 1)
         curtile = ((xp - 2) * (ICONSET_SIZE / 2) + yp);
      else
         curtile = icon_set * ICONSET_SIZE + (xp * (ICONSET_SIZE / 2) + yp);
   }

   /* Show the correct tileset "page" when Tile Preview is clicked on */
   if (cx > (htiles + 1) * 16 + 6 && cx <= (htiles + 3) * 16 + 9 && cy > 248 && cy < 284) {
      icon_set = (curtile / ICONSET_SIZE) - ((curtile / ICONSET_SIZE) % 2);
   }

}                               /* process_menu_right () */


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

      /* The 3 settings for dmode are:
       *   0: We are in a non-edit mode, meaning that we are not going to draw
       *      to the map on Layers 1-3, or the 4 Attributes
       *   1: We are in edit mode and can only draw to a Layer or Attribute
       *   2: This is only when grab_tile is 1, and the user has the right-
       *      mouse button clicked and HELD. When released, dmode changes to 1
       *      again so we can draw to the screen.
       */
      if (dmode == 0) {
         switch (draw_mode) {
         case BLOCK_COPY:
            /* Begin area copy
             * This copies all Layers and Attributes, including Entities
             */
            if (copying == 0) {
               copyx1 = window_x + x;
               copyy1 = window_y + y;

               /* Clear the end coordinates */
               copyx2 = copyy2 = -1;
               copying = 1;
            }
            break;
         case BLOCK_PASTE:
            /* Paste copied region(s) onto the map
             * This pastes ALL Layers/Attributes except Entities (use right-
             * click to select the Layers/Attributes you want to paste)
             */
            if (clipb != 0)
               paste_region (window_x + x, window_y + y);
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         // if (dmode == 0)

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
         case MAP_MARKERS:
            /* Add or change a marker */
            add_change_marker (x + window_x, y + window_y, mouse_button);
            /* This isn't ideal, but just wait for the button to be released */
            while (mouse_b)
               kq_yield ();
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         /* if (dmode) */
   }                            // if (mouse_button == 1)

   /* Right mouse button */
   if (mouse_button == 2) {
      /* Drawing to the map only happen when (dmode == 1) */
      if (dmode == 0) {
         switch (draw_mode) {
         case BLOCK_COPY:
            /* Finish the Block Copy */
            if (copying == 1) {
               copyx2 = window_x + x;
               copyy2 = window_y + y;
               copy_region ();
               copying = 0;
            }
            break;
         case BLOCK_PASTE:
            /* Paste Layers and/or Attributes */
            if (clipb != 0)
               paste_region_special (window_x + x, window_y + y);
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         // if (dmode)

      /* Now draw to the map */
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
         case MAP_MARKERS:
            /* Remove a marker */
            add_change_marker (x + window_x, y + window_y, mouse_button);
            break;
         default:
            break;
         }                      /* switch (draw_mode) */
      }                         /* if (dmode) */
   }                            // if (mouse_button == 2)

   x = mouse_x / 16;
   y = mouse_y / 16;
   if (y > (vtiles - 1))
      y = vtiles - 1;
   if (x > (htiles - 1))
      x = htiles - 1;
}                               /* process_mouse () */


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
      mouse_button = 0;
      /* When grab_tile == 1 and the user right-clicks a tile, it
       * enters grab mode and continues to "grab" tiles under the mouse until
       * the button is released.  Then it automatically changes to whichever
       * Layer or Attribute (any one except Entities) the last "drawing" mode
       * was, so the user can draw that tile onto the map immediately.
       */
      if (dmode == 2) {
         grab_tile = 0;
         needupdate = 1;
      }
      dmode = 0;
      return;
   }                            // if (!mouse_b)

   /* Left mouse button */
   if (mouse_b & 1) {
      mouse_button = 1;
      needupdate = 1;

      /* Check if the mouse is over the menu at the bottom */
      if (y > vtiles - 1) {
         /* Check if the user is already holding down the mouse button */
         if (dmode == 0)
            process_menu_bottom (mouse_x, mouse_y);
         return;
      }

      /* Check if the mouse is over the iconset on the right */
      if (x > htiles - 1) {
         /* Check if the user is already holding down the mouse button */
         if (dmode == 0)
            process_menu_right (mouse_x, mouse_y);
         return;
      }

      if ((y > gmap.ysize - 1) || (x > gmap.xsize - 1))
         return;

      /* The mouse is inside the window */
      if (grab_tile) {
         /* Grab the tile under the mouse curser */
         get_tile ();
      } else {
         /* Draw to the map */
         if ((draw_mode >= MAP_LAYER1 && draw_mode <= MAP_ZONES)
             || (draw_mode == MAP_MARKERS))
            dmode = 1;

         /* Ensures that the user doesn't draw outside the map's height/width */
         if (x < gmap.xsize && y < gmap.ysize)
            process_mouse (mouse_button);
      }
      return;
   }                            // if (mouse_b & 1)

   /* Right mouse button */
   if (mouse_b & 2) {
      /* Right-clicking over either (bottom or right) menus does nothing */
      if ((y > vtiles - 1) || (x > htiles - 1))
         return;

      mouse_button = 2;
      needupdate = 1;

      /* The mouse is inside the window */
      if (grab_tile) {
         dmode = 2;
         get_tile ();
      } else {
         /* Draw to the map */
         if ((draw_mode >= MAP_LAYER1 && draw_mode <= MAP_ZONES)
             || (draw_mode == MAP_MARKERS))
            dmode = 1;

         /* Ensures that the user doesn't draw outside the map's height/width */
         if (x < gmap.xsize && y < gmap.ysize)
            process_mouse (mouse_button);
      }
      return;
   }                            // if (mouse_b & 2)
}                               /* read_controls () */


/*! \brief Resize the current map
 * Changes the map's height and width
 *
 * \param   selection 0 means to change both width and height
 *                    1 means to change the width
 *                    2 means to change the height
 */
void resize_map (const int selection)
{
   int response, done;
   int old_height, old_width, new_height, new_width;
   int i, ix, iy, coord1, coord2;

   /* Set current and old map sizes, incase one will not change */
   new_width = old_width = gmap.xsize;
   new_height = old_height = gmap.ysize;

   if (selection == 0 || selection == 1) {
      make_rect (double_buffer, 3, 11);
      print_sfont (6, 6, "Resize map", double_buffer);
      print_sfont (6, 18, "Width: ", double_buffer);

      done = 0;
      while (!done) {
         blit2screen ();
         response = get_line (48, 18, strbuf, 4);

         /* If the user hits ESC, break out of the function entirely */
         if (response == 0)
            return;

         /* Make sure the line isn't blank */
         if (strlen (strbuf) > 0) {
            new_width = atoi (strbuf);

            /* Make sure the value is valid */
            if (new_width < 10 || new_width > MAX_WIDTH) {
               cmessage ("Invalid width!");
               wait_enter ();
            } else {
               done = 1;
            }
         }
      }
   }

   if (selection == 0 || selection == 2) {
      make_rect (double_buffer, 3, 11);
      print_sfont (6, 6, "Resize map", double_buffer);
      print_sfont (6, 18, "Height: ", double_buffer);

      done = 0;
      while (!done) {
         blit2screen ();
         response = get_line (54, 18, strbuf, 4);

         /* If the user hits ESC, break out of the function entirely */
         if (response == 0)
            return;

         /* Make sure the line isn't blank */
         if (strlen (strbuf) > 0) {
            new_height = atoi (strbuf);

            /* Make sure the value is valid */
            if (new_height < 15 || new_height > MAX_HEIGHT) {
               cmessage ("Invalid height!");
               wait_enter ();
            } else {
               done = 1;
            }                   // if..else ()
         }                      // if (strlen())
      }                         // while ()
   }                            // if (selection)

   /* Pre-copy the map info */
   for (iy = 0; iy < gmap.ysize; iy++) {
      for (ix = 0; ix < gmap.xsize; ix++) {
         coord1 = iy * gmap.xsize + ix;
         c_map[coord1] = map[coord1];
         cb_map[coord1] = b_map[coord1];
         cf_map[coord1] = f_map[coord1];
         co_map[coord1] = o_map[coord1];
         csh_map[coord1] = sh_map[coord1];
         cz_map[coord1] = z_map[coord1];
      }
   }

   /* Set map to new size */
   gmap.xsize = new_width;
   gmap.ysize = new_height;

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
   for (iy = 0; iy < old_height; iy++) {
      for (ix = 0; ix < old_width; ix++) {
         if (iy < gmap.ysize && ix < gmap.xsize) {
            coord1 = iy * gmap.xsize + ix;
            coord2 = iy * old_width + ix;

            map[coord1] = c_map[coord2];
            b_map[coord1] = cb_map[coord2];
            f_map[coord1] = cf_map[coord2];
            o_map[coord1] = co_map[coord2];
            sh_map[coord1] = csh_map[coord2];
            z_map[coord1] = cz_map[coord2];
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

   /* Make sure entitites aren't outside the map's coords */
   for (i = 0; i < number_of_ents; i++) {
      if (gent[i].tilex >= gmap.xsize)
         gent[i].tilex = gmap.xsize - 1;
      if (gent[i].tiley >= gmap.ysize)
         gent[i].tiley = gmap.ysize - 1;
   }
}                               /* resize_map () */


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
   grab_tile = 0;
}                               /* select_only () */


/*! \brief Handy-dandy help screen
 *
 * This is a simple help screen that displays when F1 is pressed
 */
void show_help (void)
{
   /* Fonts are 6x6; calculate 7x7 to include some whitespace between them. */
   int FH = 6, FW = 6;

// This line turns off other/indent.pro indentation settings:
// *INDENT-OFF*
   int this_counter, i, j;
   #define NUMBER_OF_ITEMS 35

   /* The first line in the help menu needs to be the total width, for correct
    * calculation later on
    */
   char *help_keys[NUMBER_OF_ITEMS] = {
      "                              THIS IS THE HELP DIALOG (F1)                              ",
      "                              ============================",
      "",
      "F2 . . . . . . . . . . . . . . . . Load Map  G  . . . . . . . . . . . . . . .  Grab Tile",
      "F3 . . . . . . . . . . . . . . . . Save Map  F  . . . . . . . . . . . . . . . First Zone",
      "N  . . . . . . . . . . . . . . . .  New Map  L  . . . . . . . . . . . . . . .  Last Zone",
      "R  . . . . . . . . . . . . . . . Resize Map  H  . . . . . . . . . . Highlight Attributes",
      "",
      "1  . . . . . . . . . . . . . . Layer 1 Mode  O  . . . . . . . . . . . . .  Obstacle Mode",
      "2  . . . . . . . . . . . . . . Layer 2 Mode  S  . . . . . . . . . . . . . .  Shadow Mode",
      "3  . . . . . . . . . . . . . . Layer 3 Mode  Z  . . . . . . . . . . . . . . .  Zone Mode",
      "4  . . . . . . . . . . . .  View Layers 1+2  E  . . . . . . . . . . . . . .  Entity Mode",
      "5  . . . . . . . . . . . .  View Layers 1+3  F12  . . . . . . . . . . Modify Entity Mode",
      "6  . . . . . . . . . . . .  View Layers 2+3  D  . . . . . . . . . . .  Displace Entities",
      "7  . . . . . . . . . . .  View Layers 1+2+3  M  . . . . . . . . . . . . . .  Marker Mode",
      "A  . . . . . . View all Layers + Attributes  T  . . . . . . . . . . . .  Block Copy Mode",
      "C  . . . . . Show map preview (w/ parallax)  P  . . . . . . . . . . . . Block Paste Mode",
      "                                             ESC  . . . . . . . . . .  Cancel Block Copy",
      "-/+  . . . . . . . . . . . Modify Selection",
      "F10  . . . . . . . .  Enter Map Description  F5 . . . . . . . . . . .  Make Map from PCX",
      "                                             J  . . . . . . . . . . . Copy Layers to PCX",
      "F4 . . . . . . . . . . . . . .  Clear Layer  V  . . . . . . . . . .  Visualise whole map",
      "F7 . . . . . . . . . . . Clear Obstructions  F6 . . . . . . . . .  Copy instance of Tile",
      "F8 . . . . . . . . . . . . .  Clear Shadows  F9 . . . . . Copy from one Layer to another",
      "W  . . . . . . . . . . . . . . .  Clear Map",
      "                                             UP ARROW . . . . . . . . .  Move up 1 space",
      "PGUP . . . . . . . . . . . Move 1 screen up  DOWN ARROW . . . . . . .  Move down 1 space",
      "PGDN . . . . . . . . . . Move 1 screen down  LEFT ARROW . . . . . . .  Move left 1 space",
      "BACKSPACE  . . . . . . . Move 1 screen left  RIGHT ARROW  . . . . . . Move right 1 space",
      "TAB  . . . . . . . . .  Move 1 screen right",
      "                                             U  . . . . . . . . . . . . Change CPU Usage",
      "Q  . . . . . . . . . . . . . . . . . . Quit  HOME . . . . . . Move to the top of the map",
      "                                             END  . . . . . . Move to the end of the map",
      "",
      "                                  [PRESS ESC OR ENTER]"
   };
// This turns the other/indent.pro settings back on:
// *INDENT-ON*

   i = (htiles * 16 - strlen (*help_keys) * FW) / 2;
   j = (vtiles * 16 - NUMBER_OF_ITEMS * 8) / 2;

   rectfill (double_buffer, i - 5, j - 5,
             i + (strlen (*help_keys) * FW) + 4,
             j + (NUMBER_OF_ITEMS * (FH + 1)) + 4, 0);
   rect (double_buffer, i - 3, j - 3,
         i + (strlen (*help_keys) * FW) + 2,
         j + (NUMBER_OF_ITEMS * (FH + 1)) + 2, 255);

   this_counter = 0;
   for (this_counter = 0; this_counter < NUMBER_OF_ITEMS; this_counter++) {
      sprintf (strbuf, "%s", help_keys[this_counter]);
      print_sfont (i, (this_counter * (FH + 1)) + j, strbuf, double_buffer);
   }
   vline (double_buffer,
          i + (strlen (*help_keys) * FW / 2) - 1,
          j + ((FH + 1) * 3) - 1,
          j + (NUMBER_OF_ITEMS - 2) * (FH + 1) - 2, 255);
   blit2screen ();
   yninput ();

}                               /* show_help () */


/*! \brief The opposite of shutdown, maybe?
 *
 * Inits everything needed for user input, graphics, etc.
 */
int startup (void)
{
   int k, kx, ky, a;
   COLOR_MAP cmap;

   if (allegro_init () != 0)
      return 0;
   install_keyboard ();
   install_timer ();

   /* Determine if user wants to use Windowed or Regular Mode */
   if (WBUILD == 1) {
      if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, SW, SH, 0, 0) != 0) {
         if (set_gfx_mode (GFX_SAFE, SW, SH, 0, 0) != 0) {
            set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
            allegro_message ("Unable to set any graphic mode\n%s\n",
                             allegro_error);
            return 0;
         }
      }
   } else {
      if (set_gfx_mode (GFX_AUTODETECT, SW, SH, 0, 0) != 0) {
         if (set_gfx_mode (GFX_SAFE, SW, SH, 0, 0) != 0) {
            set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
            allegro_message ("Unable to set any graphic mode\n%s\n",
                             allegro_error);
            return 0;
         }
      }                         // if (WBUILD)
   }

   /* Check for the presence of a mouse */
   a = install_mouse ();
   if (a == -1) {
      nomouse = 1;
      klog ("Mouse not found!\n");
      rest (1000);
   }

   shared_startup ();

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

   bufferize ();

   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;

   font6 = create_bitmap (6, 546);
   getfont ();

   /* Used for highlighting */
   static unsigned char hilite[] = {
      00, 00, 00, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 00, 00, 00,
      00, 00, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 00, 00,
      00, 25, 25, 25, 45, 45, 45, 45, 45, 45, 45, 45, 25, 25, 25, 00,
      25, 25, 25, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 25, 25, 25,
      25, 25, 45, 45, 45, 25, 25, 25, 25, 25, 25, 45, 45, 45, 25, 25,
      25, 25, 45, 45, 25, 25, 25, 25, 25, 25, 25, 25, 45, 45, 25, 25,
      25, 25, 45, 45, 25, 25, 25, 00, 00, 25, 25, 25, 45, 45, 25, 25,
      25, 25, 45, 45, 25, 25, 00, 00, 00, 00, 25, 25, 45, 45, 25, 25,
      25, 25, 45, 45, 25, 25, 00, 00, 00, 00, 25, 25, 45, 45, 25, 25,
      25, 25, 45, 45, 25, 25, 25, 00, 00, 25, 25, 25, 45, 45, 25, 25,
      25, 25, 45, 45, 25, 25, 25, 25, 25, 25, 25, 25, 45, 45, 25, 25,
      25, 25, 45, 45, 45, 25, 25, 25, 25, 25, 25, 45, 45, 45, 25, 25,
      25, 25, 25, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 25, 25, 25,
      00, 25, 25, 25, 45, 45, 45, 45, 45, 45, 45, 45, 25, 25, 25, 00,
      00, 00, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 00, 00,
      00, 00, 00, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 00, 00, 00,
   };

   mesh2 = create_bitmap (16, 16);
   clear (mesh2);
   for (ky = 0; ky < 16; ky++)
      for (kx = 0; kx < 16; kx++)
         mesh2->line[ky][kx] = hilite[ky * 16 + kx];

   /* Used to show map boundaries */
   static unsigned char diag_bars[] = {
      25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00,
      25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25,
      00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25,
      00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00,
      00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00,
      00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00,
      00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00,
      00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00,
      25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00,
      25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25,
      00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25,
      00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00,
      00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00,
      00, 00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00,
      00, 00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00,
      00, 25, 25, 00, 00, 00, 00, 00, 00, 25, 25, 00, 00, 00, 00, 00,
   };

   mesh3 = create_bitmap (16, 16);
   clear (mesh3);
   for (ky = 0; ky < 16; ky++)
      for (kx = 0; kx < 16; kx++)
         mesh3->line[ky][kx] = diag_bars[ky * 16 + kx];

   /* Entity images */
   set_pcx (&pcx_buffer, "entities.pcx", pal, 1);

   for (k = 0; k < MAX_EPICS; k++) {
      for (a = 0; a < 12; a++) {
         eframes[k][a] = create_bitmap (16, 16);
         blit (pcx_buffer, eframes[k][a], a * 16, k * 16, 0, 0, 16, 16);
      }
   }
   destroy_bitmap (pcx_buffer);

   init_entities ();
   showing.entities = 0;
   showing.obstacles = 0;
   showing.shadows = 0;
   showing.zones = 0;
   showing.markers = 0;
   showing.last_layer = draw_mode;
   showing.layer[0] = 1;
   showing.layer[1] = 1;
   showing.layer[2] = 1;

   icon_set = 0;

   return 1;
}                               /* startup () */


/*! \brief Update the tileset
 *
 * This changes the iconset in the menu on the right and then updates
 * the map to show the current tileset
 */
void update_tileset (void)
{
   int tmapx, tmapy, i;
   int tileset_start;

   /* This will try to use the selected tileset; however, if there's a coding
    * problem, or the file is missing, this will try the NEXT available one.
    * If it loops through all the available tilesets and none are available,
    * stop after an entire loop and exit the program with an error.
    */
   tileset_start = gmap.tileset;

   while (!exists (icon_files[gmap.tileset])) {
      error_load (icon_files[gmap.tileset]);
      gmap.tileset++;
      if (gmap.tileset >= NUM_TILESETS)
         gmap.tileset = 0;

      /* This is just to ensure that we don't try looping forever */
      if (tileset_start == gmap.tileset) {
         sprintf (strbuf, "None of the tilesets could be located!");
         allegro_message (strbuf);
         cleanup ();
         exit (EXIT_FAILURE);
      }
   }

   set_pcx (&pcx_buffer, icon_files[gmap.tileset], pal, 1);
   max_sets = (pcx_buffer->h / 16);

   for (tmapy = 0; tmapy < max_sets; tmapy++)
      for (tmapx = 0; tmapx < ICONSET_SIZE; tmapx++)
         blit (pcx_buffer, icons[tmapy * ICONSET_SIZE + tmapx], tmapx * 16,
               tmapy * 16, 0, 0, 16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);

   /* Update the animation tile sequences */
   for (i = 0; i < MAX_TILES; i++)
      tilex[i] = i;
   for (i = 0; i < MAX_ANIM; i++) {
      adata[i] = tanim[gmap.tileset][i];
   }

}                               /* update_tileset () */


/*! \brief Wait for ENTER key
 *
 * There's almost no point in explaining this function further :-)
 */
void wait_enter (void)
{
   int a, done;

   done = 0;
   while (!done) {
      a = (readkey () >> 8);
      if (a == KEY_ENTER || a == KEY_ENTER_PAD)
         done = 1;
   }
}                               /* wait_enter () */


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

   init_entities ();
}                               /* wipe_map () */


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
   int ch, done;

   done = 0;
   while (!done) {
      /* ENTER/ESC functions the same as Y/N */
      ch = (readkey () >> 8);
      if (ch == KEY_N || ch == KEY_ESC)
         done = 1;
      if (ch == KEY_Y || ch == KEY_ENTER || ch == KEY_ENTER_PAD)
         done = 2;
   }
   blit2screen ();
   return done - 1;
}                               /* yninput () */


/* Action handler for clicking in marker mode button b; */
void add_change_marker (int marker_x, int marker_y, int mouse_button)
{
   s_marker *m;
   s_marker *found = NULL;

   /* Does a marker exist here? */
   for (m = markers; m < markers + num_markers; ++m) {
      if (m->x == marker_x && m->y == marker_y) {
         found = m;
         break;
      }
   }

   if (found) {
      /* There is a marker here */
      if (mouse_button == 1) {
         /* Rename it */
         rename_marker (found);
      } else if (mouse_button == 2) {
         /* Delete it */

         /* Move the selector to the previous marker if this was the last
          * marker on the map
          */
         num_markers--;
         if (curmarker == num_markers)
            curmarker = num_markers - 1;
         memcpy (found, found + 1,
                 (&markers[num_markers] - found) * sizeof (s_marker));
         while (mouse_b & 2);
      }
   } else {
      /* There is no marker here */
      if (mouse_button == 1) {
         /* Add a marker with default name */
         if (num_markers < MAX_MARKERS) {
            curmarker = num_markers;
            m = &markers[num_markers++];
            m->x = marker_x;
            m->y = marker_y;
            sprintf (m->name, "Marker #%d: (%d, %d)", num_markers, m->x, m->y);
         }
      }
   }
}


void rename_marker (s_marker *found)
{
   int response, done;
   s_marker *m;

   make_rect (double_buffer, 2, 32);
   print_sfont (6, 6, found->name, double_buffer);
   print_sfont (6, 12, ">", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (12, 12, strbuf, 31);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) == 0) {
         cmessage ("Do you want to clear the name of this marker? (y/n)");
         if (yninput ())
            done = 1;
      } else {
         done = 1;
      }

      /* Make sure no other markers have the same name */
      for (m = markers; m < markers + num_markers; ++m) {
         if (!strcmp (strbuf, m->name) && m != found) {
            cmessage ("Another marker has that name. Use another name.");
            yninput ();
            done = 0;
            break;
         }
      }
   }
   strcpy (found->name, strbuf);
}                               /* rename_marker () */


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
   /* TT: If this breaks stuff, let me know. I _know_ it complains that
    * yield_timeslice() is deprecated; until I know everyone is using the most
    * recent version of Allegro, though, we may need to keep this in here for
    * compatibility reasons.
    * Question: Does rest(0) or rest(1) break anything on anyone's machine?
    */
   if (cpu_usage == 0)
      yield_timeslice ();
   else
      rest(cpu_usage - 1);

// TT REMOVE:
//#if (ALLEGRO_VERSION >= 4 && ALLEGRO_SUB_VERSION >= 2)
//   rest (0);
//#else
//   yield_timeslice ();
//#endif

}
