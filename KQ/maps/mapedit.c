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


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
/*
 * This is the KQ map editor
 * (Pete's hacked version)
 * (TT's revised hacked version)
 * December 2002 (and Jan 2003)
 */
#include "allegro.h"
#include "mapdraw.h"

/* globals */

/* Selectable tiles on the right-hand menu */
BITMAP *icons[MAX_TILES];

/* Image and screen buffers */
BITMAP *pcx_buffer, *double_buffer;

/* Icons for Attributes (*b_mesh unused) */
BITMAP *mesh, *b_mesh, *z_mesh;

/* Font and mouse images */
BITMAP *font6, *mpic;

/* Gee, could it be an Entity frame? */
BITMAP *eframes[MAX_EPICS][12];

/* This is for a shadow.  It's dark. */
BITMAP *shadow[MAX_SHADOWS];

/* These are for the Layers 1-3 */
unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
/* These are for the Zone, Shadow and Obstacle Attributes */
unsigned char *z_map, *cz_map, *sh_map, *cs_map, *o_map, *co_map;

/* Used for the right-hand menu, plus something for the mouse */
short icon_set = 0, max_sets = 51, nomouse = 0;

/* Stores the name of the currently loaded map */
char map_fname[16] = "";

/* Used everywhere for strings */
char *strbuf;

/* gx and gy are view-window coords; x and y are everything else */
short gx = 0, gy = 0, x = 0, y = 0;

int draw_mode = MAP_LAYER123, curtile = 0, dmode = 0;

int curzone = 0, curshadow = 0, curobs = 0;
int copying = 0, copyx1 = -1, copyx2 = -1, copyy1 = -1, copyy2 = -1;
int clipb = 0, cbh = 0, cbw = 0;

s_map gmap;
s_entity gent[50];
s_show showing;

/* Number of entities, index of currently-selected entity */
int noe = 0, cent = 0;

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
   "Incave.pcx", "village.pcx", "mount.pcx"
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

void preview_map(void);


/* Welcome to Mapdraw, folks! */
int main (void)
{
   int stop = 0;

   startup ();
   while (!stop) {
      process_controls ();
      if (draw_mode == MAP_PREVIEW)
        preview_map();
      else
        draw_map ();
      draw_menubars ();
      if (!nomouse)
         draw_sprite (double_buffer, mpic, mouse_x, mouse_y);
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      if (key[KEY_Q])
         stop = confirm_exit ();
      yield_timeslice ();
   }                            /* while (!stop) */
   cleanup ();
   return 0;
}

END_OF_MAIN ();


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


/*! \brief Mouse and keyboard input
 *
 * Mouse and keyboard inputs (sorry, no joystick support)
 */
void process_controls (void)
{
   int k;

   if (keypressed ()) {
      k = (readkey () >> 8);

      /* Note: The only modes that will draw to the map are
         MAP_LAYER1, MAP_LAYER2, or MAP_LAYER3 */
      /* Layer 1 */
      if (k == KEY_1)
         draw_mode = MAP_LAYER1;
      /* Layer 2 */
      if (k == KEY_2)
         draw_mode = MAP_LAYER2;
      /* Layer 3 */
      if (k == KEY_3)
         draw_mode = MAP_LAYER3;
      /* Show Layers 1+2 */
      if (k == KEY_4)
         draw_mode = MAP_LAYER12;
      /* Show Layers 1+3 */
      if (k == KEY_5)
         draw_mode = MAP_LAYER13;
      /* Show Layers 2+3 */
      if (k == KEY_6)
         draw_mode = MAP_LAYER23;
      /* Show Layers 1+2+3 */
      if (k == KEY_7)
         draw_mode = MAP_LAYER123;

      /* Turn off Attributes when selecting one or more layers */
      if (k >= KEY_1 && k <= KEY_7) {
         showing.entities = 0;
         showing.shadows = 0;
         showing.obstacles = 0;
         showing.zones = 0;
         showing.last_layer = draw_mode;
      }

      /* View Layers 1+2+3, plus Entities and Shadows */
      if (k == KEY_C) {
         draw_mode = MAP_PREVIEW;
         showing.entities = 1;
         showing.shadows = 1;
         showing.obstacles = 0;
         showing.zones = 0;
         showing.last_layer = draw_mode;
      }

      /* Display Layers 1+2+3 and all Attributes */
      if (k == KEY_A) {
         draw_mode = MAP_LAYER123;
         showing.entities = 1;
         showing.shadows = 1;
         showing.obstacles = 1;
         showing.zones = 1;
         showing.last_layer = draw_mode;
      }

      /* TT: The Attribute must be 'Active' to be able to draw to the map */

      /* Toggle Entities Attribute */
      if (k == KEY_E) {
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
      }

      /* Toggle Shadows Attribute */
      if (k == KEY_S) {
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
      }

      /* Toggle Obstacles attribute */
      if (k == KEY_O) {
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
      }

      /* Toggle Zone attribute */
      if (k == KEY_Z) {
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
      }

      /* Copy a selection */
      if (k == KEY_T)
         draw_mode = BLOCK_COPY;

      /* Paste the copied selection area */
      if (k == KEY_P)
         draw_mode = BLOCK_PASTE;

      /* Get the tile under the mouse curser */
      if (k == KEY_G) {
         if ((draw_mode != MAP_LAYER1) &&
             (draw_mode != MAP_LAYER2) &&
             (draw_mode != MAP_LAYER3) &&
             (showing.last_layer != MAP_LAYER1) &&
             (showing.last_layer != MAP_LAYER2) &&
             (showing.last_layer != MAP_LAYER3))

            /* The default view mode */
            showing.last_layer = MAP_LAYER1;
         else if (draw_mode != GRAB_TILE)
            /* One of the 3 layers was previously used; switch to that */
            showing.last_layer = draw_mode;
         draw_mode = GRAB_TILE;
      }

      /* Clear the contents of the current map */
      if (k == KEY_W)
         wipe_map ();

      /* Move (displace) the location of all the entities on the map */
      if (k == KEY_D)
         displace_entities ();

      /* Resize the map's height and width */
      if (k == KEY_R)
         resize_map ();

      /* Copy Layers 1, 2, 3 to mini PCX images */
      if (k == KEY_J)
         maptopcx ();

      /* Save whole map as a picture */
      if (k == KEY_V)
         visual_map ();

      /* Get the last Zone used and set the indicator to that */
      if (k == KEY_L)
         curzone = check_last_zone ();

      /* Create a new map; you can choose the tileset to use for it */
      if (k == KEY_N)
         new_map ();

      /* Display the help screen */
      if (k == KEY_F1)
         show_help ();

      /* Load a map */
      if (k == KEY_F2)
         load_map ();

      /* Save the map you are working on */
      if (k == KEY_F3)
         save_map ();

      /* Erase an entire layer from the map */
      if (k == KEY_F4)
         clear_layer ();

      /* Load a [mini] PCX file to become a map */
      if (k == KEY_F5)
         make_mapfrompcx ();

      /* Change all instances of one tile in your map to another */
      if (k == KEY_F6)
         global_change ();

      /* Remove all Obstructions from the map */
      if (k == KEY_F7)
         clear_obstructs ();

      /* Remove all Shadows from the map */
      if (k == KEY_F8)
         clear_shadows ();

      /* Copy from one layer to another */
      if (k == KEY_F9)
         copy_layer ();

      /* Enter a description that a player sees when entering the map */
      if (k == KEY_F10)
         describe_map ();

      /* Enter the Modify Entity mode */
      if (k == KEY_F12) {
         showing.entities = 1;
         update_entities ();
      }

      /* TT: Currently cancels a Block Copy, but future use is intended
         to have it bring up a menu for easier navigation
       */
      if (k == KEY_ESC) {
         if (copying == 1) {
            copying = 0;
            copyx1 = copyx2 = -1;
            copyy1 = copyy2 = -1;
         }
         /* Empty the contents of the clipboard */
         clipb = 0;
      }

      /* Change the values for the current mode */
      if (k == KEY_MINUS || k == KEY_MINUS_PAD) {
         switch (draw_mode) {
            /* Select an Entity to place on the map */
         case MAP_ENTITIES:
            cent--;
            if (cent < 0)
               cent = MAX_EPICS - 1;
            break;
            /* Select the Shadow to place on the map */
         case MAP_SHADOWS:
            curshadow--;
            if (curshadow < 0)
               curshadow = MAX_SHADOWS - 1;
/* TT TODO: If we create a light tile somewhere on the side or bottom and put a
   shadow over the top of it so the user can see what it looks like, it might
   make it easier on the user.
 */
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
      }

      /* if (k == KEY_MINUS) */
      /* Change the values for the current mode */
      if (k == KEY_EQUALS || k == KEY_PLUS_PAD) {
         switch (draw_mode) {
            /* Select an Entity to place on the map */
         case MAP_ENTITIES:
            cent++;
            if (cent == MAX_EPICS)
               cent = 0;
            break;
            /* Select the Shadow to place on the map */
         case MAP_SHADOWS:
            curshadow++;
            if (curshadow >= MAX_SHADOWS)
               curshadow = 0;
            break;
            /* Select which Obstacle to set on the map */
         case MAP_OBSTACLES:
            curobs++;
            if (curobs > 5)
               curobs = 0;
            break;
            /* Select a Zone to set on the map */
         case MAP_ZONES:
            curzone++;
            if (curzone >= MAX_ZONES)
               curzone = 0;
            break;
            /* Change the iconset's "page" */
         default:
            icon_set++;
            if (icon_set > max_sets - 1)
               icon_set = 0;
            break;
         }
      }

      /* if (k == KEY_EQUALS) */
      /* Move the view-window up one tile */
      if (k == KEY_UP || k == KEY_7_PAD || k == KEY_8_PAD || k == KEY_9_PAD)
         gy--;
      /* Move the view-window down one tile */
      if (k == KEY_DOWN || k == KEY_1_PAD || k == KEY_2_PAD || k == KEY_3_PAD)
         gy++;
      /* Move the view-window right one tile */
      if (k == KEY_RIGHT || k == KEY_3_PAD || k == KEY_6_PAD || k == KEY_9_PAD)
         gx++;
      /* Move the view-window left one tile */
      if (k == KEY_LEFT || k == KEY_1_PAD || k == KEY_4_PAD || k == KEY_7_PAD)
         gx--;
      /* Move the view-window up one page */
      if (k == KEY_PGUP)
         gy -= vtiles;
      /* Move the view-window down one page */
      if (k == KEY_PGDN)
         gy += vtiles;
      /* Move the view-window right one page */
      if (k == KEY_TAB)
         gx += htiles;
      /* Move the view-window left one page */
      if (k == KEY_BACKSPACE)
         gx -= htiles;
      /* Move the view-window to the bottom-right edge of the map */
      if (k == KEY_END) {
         gx = gmap.xsize - htiles;
         gy = gmap.ysize - vtiles;
      }
      /* Move the view-window to the top-left edge of the map */
      if (k == KEY_HOME) {
         gx = 0;
         gy = 0;
      }
   }

   /* if (keypressed ()) */
   /* These prevent the user from moving past the map's edge */
   if (gx > gmap.xsize - htiles)
      gx = gmap.xsize - htiles;
   if (gx < 0)
      gx = 0;
   if (gy > gmap.ysize - vtiles)
      gy = gmap.ysize - vtiles;
   if (gy < 0)
      gy = 0;

   /* No mouse buttons pushed means default drawing mode */
   if (!(mouse_b & 1) && !(mouse_b & 2)) {
      /* When draw_mode == GRAB_TILE and the user right-clicks a tile, it
         enters grab mode and continues to "grab" tiles under the mouse
         until the button is released.  Then it automatically changes to
         Layer[1-3] (whichever the last "drawing" mode was) so the user
         can draw that tile onto the map immediately.
       */
      if (dmode == 2)
         draw_mode = showing.last_layer;
      dmode = 0;
   }

   /* Left mouse button */
   if (mouse_b & 1) {
      x = mouse_x / 16;
      y = mouse_y / 16;

      /* Check if the mouse is over the menu at the bottom */
      if (y > (vtiles - 1)) {
         /* This lets you select all the options at the bottom
            of the screen to update your map attributes.
          */
         if (dmode == 0) {
            check_mdupdate (mouse_x, mouse_y);
            return;
         }

         /* This serves a dual purpose: First it 'protects' the tile
            below the view-screen from being drawn to when the mouse
            is over the options-area.  Second, it means that the user
            is still holding down the button when the mouse was moved
            down there, so do nothing instead of selecting one of the
            options accidentally.
          */
         else
            return;
      }

      /* Check if the mouse is over the iconset on the right */
      if (x > (htiles - 1)) {
         /* This lets you select the tiles from the iconset at
            the right of the screen to update your map icons.
          */
         if (dmode == 0) {
            check_tilesel (mouse_x, mouse_y);
            return;
         }

         /* This does the same thing for the iconset as the above
            code does for the menu options.
          */
         else
            return;
      }

      /* Draw to the map */
      if ((draw_mode == MAP_LAYER1) || (draw_mode == MAP_LAYER2) ||
          (draw_mode == MAP_LAYER3) || (draw_mode == MAP_OBSTACLES) ||
          (draw_mode == MAP_ZONES) || (draw_mode == MAP_ENTITIES) ||
          (draw_mode == MAP_SHADOWS))
         dmode = 1;

      /* Changes to the map only happen when (dmode == 1) */
      if (dmode == 1) {
         /* Draw to Layer 1
            This must draw only to Layer 1.  We never want to draw to two
            layers at once.
          */
         if (draw_mode == MAP_LAYER1)
            map[((gy + y) * gmap.xsize) + gx + x] = curtile;

         /* Draw to Layer 2
            This must draw only to Layer 2.
          */
         if (draw_mode == MAP_LAYER2)
            b_map[((gy + y) * gmap.xsize) + gx + x] = curtile;

         /* Draw to Layer 3
            This must draw only to Layer 3.
          */
         if (draw_mode == MAP_LAYER3)
            f_map[((gy + y) * gmap.xsize) + gx + x] = curtile;

         /* Add an Obstacle to the map */
         if (draw_mode == MAP_OBSTACLES)
            o_map[((gy + y) * gmap.xsize) + gx + x] = curobs;

         /* Add a Zone to the map */
         if (draw_mode == MAP_ZONES)
            z_map[((gy + y) * gmap.xsize) + gx + x] = curzone;

         /* Add an Entity to the map */
         if (draw_mode == MAP_ENTITIES)
            place_entity ((mouse_x / 16) + gx, (mouse_y / 16) + gy);

         /* Add a Shadow to the map */
         if (draw_mode == MAP_SHADOWS)
            sh_map[((gy + y) * gmap.xsize) + gx + x] = curshadow;
      }

      /* if (dmode == 1) */
      /* Begin area copy
         This copies all Layers and Attributes except Entities
       */
      if (draw_mode == BLOCK_COPY && copying == 0) {
         copyx1 = gx + x;
         copyy1 = gy + y;

         /* Make sure to clear the end coordinates */
         copyx2 = copyy2 = -1;
         copying = 1;
      }

      /* Paste copied region(s) onto the map
         Since it is a left-click, we'll paste ALL Layers/Attributes
       */
      if (draw_mode == BLOCK_PASTE && clipb != 0)
         paste_region (gx + x, gy + y);

      /* Get the index of the tile under the mouse and select
         its icon from the tileset on the right
       */
      if (draw_mode == GRAB_TILE)
         get_tile ();

   }

   /* if (mouse_b & 1) */
   /* Right-click handler */
   if (mouse_b & 2) {
      x = mouse_x / 16;
      y = mouse_y / 16;

      /* This prevents tiles from being erased 'under' the menus */
      if ((y > (vtiles - 1)) || x > (htiles - 1))
         return;

      /* Paste Layers and/or Attributes */
      if ((draw_mode == BLOCK_PASTE) && (clipb != 0))
         paste_region_special (gx + x, gy + y);

      /* Draw to the map */
      if (((draw_mode == MAP_LAYER1) || (draw_mode == MAP_LAYER2) ||
           (draw_mode == MAP_LAYER3) || (draw_mode == MAP_OBSTACLES) ||
           (draw_mode == MAP_ZONES) || (draw_mode == MAP_ENTITIES) ||
           (draw_mode == MAP_SHADOWS)) && (dmode == 0))
         dmode = 1;

      /* Grab the tile and set the mode to a drawing mode */
      if (draw_mode == GRAB_TILE) {
         get_tile ();
         dmode = 2;
      }

      /* Changes to the map only happen when (dmode == 1) */
      if (dmode == 1) {

         /* Remove an Entity */
         if (draw_mode == MAP_ENTITIES)
            erase_entity (mouse_x / 16 + gx, mouse_y / 16 + gy);

         /* Remove a Shadow */
         if (draw_mode == MAP_SHADOWS)
            sh_map[((gy + y) * gmap.xsize) + gx + x] = 0;

         /* Remove an Obstacle */
         if (draw_mode == MAP_OBSTACLES)
            o_map[((gy + y) * gmap.xsize) + gx + x] = 0;

         /* Remove a Zone */
         if (draw_mode == MAP_ZONES)
            z_map[((gy + y) * gmap.xsize) + gx + x] = 0;

         /* Erase a tile from Layer 1 */
         if (draw_mode == MAP_LAYER1)
            map[((gy + y) * gmap.xsize) + gx + x] = 0;

         /* Erase a tile from Layer 2 */
         if (draw_mode == MAP_LAYER2)
            b_map[((gy + y) * gmap.xsize) + gx + x] = 0;

         /* Erase a tile from Layer 3 */
         if (draw_mode == MAP_LAYER3)
            f_map[((gy + y) * gmap.xsize) + gx + x] = 0;
      }

      /* if (dmode == 1) */
      /* Finish the Block Copy */
      if (draw_mode == BLOCK_COPY && copying == 1) {
         copyx2 = gx + x;
         copyy2 = gy + y;
         copy_region ();
         copying = 0;
      }

   }
   /* if (mouse_b & 2) */
   x = mouse_x / 16;
   y = mouse_y / 16;
   if (y > (vtiles - 1))
      y = vtiles - 1;
   if (x > (htiles - 1))
      x = htiles - 1;
}                               /* process_controls () */


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


/*! \brief Select an icon from the tileset
 *
 * Selects one of the tiles from the menu on the right
 *
 * \param   cx x-coord of the tile
 * \param   cy y-coord of the tile
 */
void check_tilesel (int cx, int cy)
{
   int xp, yp;

   /* Make sure that the mouse is over one if the selectable tiles */
   if (cx >= (SW - 64) && cx < (SW - 32) && cy >= 0 && cy < 160) {
      xp = (cx - (SW - 64)) / 16;
      yp = cy / 16;
      /* Set the tileset to the correct "page" */
      curtile = icon_set * ICONSET_SIZE + (xp * (ICONSET_SIZE / 2) + yp);
   }
}                               /* check_tilesel () */


/*! \brief Select an option from the menu
 *
 * Selects one of the options from the menus at the bottom of the screen
 *
 * \param   cx x-coord of the mouse
 * \param   cy y-coord of the mouse
 */
void check_mdupdate (int cx, int cy)
{
   int a;

   /* The mouse is over 'Icon:' menu */
   if (cx >= 0 && cx <= 159 && cy >= (SH - 40) && cy <= (SH - 35)) {
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
   if (cx >= 0 && cx <= 159 && cy >= (SH - 34) && cy <= (SH - 29)) {
      rectfill (screen, 36, (SH - 34), 159, (SH - 29), 0);
      print_sfont (30, (SH - 34), ">", screen);
      hline (screen, 36, (SH - 29), 113, 255);
      a = get_line (36, (SH - 34), strbuf, 19);

      /* This is kinda hard to error-check... */
      if (a == 0)
         return;
      strcpy (gmap.song_file, strbuf);
   }

   /* The mouse is over 'ZeroZone' menu */
   if (cx >= 0 && cx <= 159 && cy >= (SH - 28) && cy <= (SH - 23)) {
      gmap.zero_zone = 1 - gmap.zero_zone;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Map #' menu */
   if (cx >= 0 && cx <= 79 && cy >= (SH - 22) && cy <= (SH - 17)) {
      rectfill (screen, 42, (SH - 22), 159, (SH - 17), 0);
      print_sfont (36, (SH - 22), ">", screen);
      hline (screen, 42, (SH - 17), 65, 255);
      a = get_line (42, (SH - 22), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
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
   if (cx >= 0 && cx <= 79 && cy >= (SH - 16) && cy <= (SH - 11)) {
      gmap.map_mode++;
      if (gmap.map_mode > 5)
         gmap.map_mode = 0;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Save' menu */
   if (cx >= 0 && cx <= 79 && cy >= (SH - 10) && cy <= (SH - 5)) {
      gmap.can_save = 1 - gmap.can_save;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'Warp' menu */
   if (cx >= 80 && cx <= 159 && cy >= (SH - 22) && cy <= (SH - 17)) {
      gmap.can_warp = 1 - gmap.can_warp;
      while (mouse_b & 1);
      return;
   }

   /* The mouse is over 'WarpX' menu */
   if (cx >= 80 && cx <= 159 && cy >= (SH - 16) && cy <= (SH - 11)) {
      rectfill (screen, 122, (SH - 16), 159, (SH - 11), 0);
      print_sfont (116, (SH - 16), ">", screen);
      hline (screen, 122, (SH - 11), 145, 255);
      a = get_line (122, (SH - 16), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
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
   if (cx >= 80 && cx <= 159 && cy >= (SH - 10) && cy <= (SH - 5)) {
      rectfill (screen, 122, (SH - 10), 159, (SH - 5), 0);
      print_sfont (116, (SH - 10), ">", screen);
      hline (screen, 122, (SH - 5), 145, 255);
      a = get_line (122, (SH - 10), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
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
   if (cx >= 160 && cx <= 239 && cy >= (SH - 40) && cy <= (SH - 35)) {
      rectfill (screen, 214, (SH - 40), 239, (SH - 35), 0);
      print_sfont (208, (SH - 40), ">", screen);
      hline (screen, 214, (SH - 35), 237, 255);
      a = get_line (214, (SH - 40), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
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
   if (cx >= 160 && cx <= 239 && cy >= (SH - 34) && cy <= (SH - 29)) {
      rectfill (screen, 214, (SH - 34), 239, (SH - 29), 0);
      print_sfont (208, (SH - 34), ">", screen);
      hline (screen, 214, (SH - 29), 237, 255);
      a = get_line (214, (SH - 34), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
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
   if (cx >= 240 && cx <= 319 && cy >= (SH - 40) && cy <= (SH - 35)) {
      rectfill (screen, 276, (SH - 40), 319, (SH - 35), 0);
      print_sfont (270, (SH - 40), ">", screen);
      hline (screen, 276, (SH - 35), 299, 255);
      a = get_line (276, (SH - 40), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
         return;
      /* Make sure the value is valid (999 is a VERY extreme value!) */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= 999)) {
         cmessage ("Invalid parallax multiplier!");
         wait_enter ();
         return;
      }
      gmap.pmult = atoi (strbuf);
   }

   /* The mouse is over 'Div' menu */
   if (cx >= 240 && cx <= 319 && cy >= (SH - 34) && cy <= (SH - 29)) {
      rectfill (screen, 270, (SH - 34), 319, (SH - 29), 0);
      print_sfont (264, (SH - 34), ">", screen);
      hline (screen, 270, (SH - 29), 293, 255);
      a = get_line (270, (SH - 34), strbuf, 4);

      /* Make sure the line isn't blank */
      if (a == 0)
         return;
      /* Make sure the value is valid (999 is a VERY extreme value!) */
      if (!(atoi (strbuf) >= 0 && atoi (strbuf) <= 999)) {
         cmessage ("Invalid parallax divider!");
         wait_enter ();
         return;
      }
      gmap.pdiv = atoi (strbuf);
   }

   /* The mouse is over either 'Width' or 'Height' menus */
   if (cx >= 160 && cx <= 319 && cy >= (SH - 28) && cy <= (SH - 17))
      resize_map ();

   /* The mouse is over 'SunStone' menu */
   if (cx >= 160 && cx <= 239 && cy >= (SH - 16) && cy <= (SH - 11)) {
      gmap.use_sstone = 1 - gmap.use_sstone;
      while (mouse_b & 1);
      return;
   }
}                               /* check_mupdate () */


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
   int zones[MAX_ZONES][3];

   for (i = 0; i < MAX_ZONES; ++i) {
      /* Clear the zones buffer */
      /* TT fix: Updated to reset all zones */
      zones[i][0] = 0;          /* whether or not there is a "zone" here */
      zones[i][1] = 0;          /* x-coord of zone on bmp */
      zones[i][2] = 0;          /* y-coord of zone on bmp */
   }

   /* Create a bitmap the same size as the map */
   if ((bmp = create_bitmap (gmap.xsize * 16, gmap.ysize * 16)) != NULL) {
      for (j = 0; j < gmap.ysize; j++) {
         /* Which tile is currently being evaluated */
         w = gmap.xsize * j;
         for (i = 0; i < gmap.xsize; i++) {
            blit (icons[map[w]], bmp, 0, 0, i * 16, j * 16, 16, 16);
            draw_sprite (bmp, icons[b_map[w]], i * 16, j * 16);
            draw_sprite (bmp, icons[f_map[w]], i * 16, j * 16);
            draw_trans_sprite (bmp, shadow[sh_map[w]], i * 16, j * 16);
            /* TT: Which is better: set to 1 or increment? */
            // zones[z_map[w]][0]++;
            zones[z_map[w]][0] = 1;     /* ==1 if there is a "zone" here */
            zones[z_map[w]][1] = i;     /* Pass zone's x-coord to array */
            zones[z_map[w]][2] = j;     /* Pass zone's y-coord to array */
            w++;
         }
      }

      /* Print the zone number over the zones */
      for (i = 0; i < MAX_ZONES; i++) {
         if (zones[i][0] == 1) {
            textprintf (bmp, font, zones[i][1] * 16, zones[i][2] * 16,
                        makecol (255, 255, 255), "%d", i);
         }
      }
      get_palette (pal);
      save_bitmap ("vis_map.pcx", bmp, pal);
      destroy_bitmap (bmp);
   }
}                               /* visual_map () */


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
 * \param pr ==0 draw with parallax off or !=0 on
 */
static void draw_layer (short *layer, int pr)
{
   int i, j;
   int i0, j0, i1, j1;
   int x0, y0;
   /* calculate the top left, taking parallax into account */
   i0 = pr ? gx * gmap.pmult / gmap.pdiv : gx;
   j0 = pr ? gy * gmap.pmult / gmap.pdiv : gy;
   /* calculate bottom right */
   i1 = i0 + htiles;
   j1 = j0 + vtiles;
   /* make sure these don't step off the edges of the map */
   if (i0 < 0)
      i0 = 0;
   if (j0 < 0)
      j0 = 0;
   if (i1 > gmap.xsize)
      i1 = gmap.xsize;
   if (j1 > gmap.ysize)
      j1 = gmap.ysize;
   /* calculate the pixel-based coordinate of the top left */
   x0 = i0 * 16;
   y0 = j0 * 16;
   /* ..and draw the tilemap */
   for (j = j0; j < j1; ++j) {
      for (i = i0; i < i1; ++i) {
         draw_sprite(double_buffer, icons[layer[i + j * gmap.xsize]],
                     i * 16 - x0, j * 16 - y0);
      }
   }
}


/*! \brief Draw the shadows
 *
 * Draws the shadows onto the screen and takes into consideration any layer
 * effects that need to take place (see map_mode_text[] for details).
 * This is basically the same as draw_layer().
 * \param pr ==0 draw with parallax off or !=0 on
 * \author PH
 * \date 20031205
 */
static void draw_shadow (int pr)
{
   int i, j;
   int i0, j0, i1, j1;
   int x0, y0;
   int ss;
   i0 = pr ? gx * gmap.pmult / gmap.pdiv : gx;
   j0 = pr ? gy * gmap.pmult / gmap.pdiv : gy;
   i1 = i0 + htiles;
   j1 = j0 + vtiles;
   if (i0 < 0)
      i0 = 0;
   if (j0 < 0)
      j0 = 0;
   if (i1 > gmap.xsize)
      i1 = gmap.xsize;
   if (j1 > gmap.ysize)
      j1 = gmap.ysize;
   x0 = gx * 16;
   y0 = gy * 16;
   for (j = j0; j < j1; ++j) {
      for (i = i0; i < i1; ++i) {
         ss = sh_map[j * gmap.xsize + i];
         if (ss > 0) {
            draw_trans_sprite (double_buffer, shadow[ss], i * 16-x0, j * 16-y0);
         }
      }
   }
}


/*! \brief Draw the entities
 *
 * Draws the entities onto the screen and takes into consideration any layer
 * effects that need to take place (see map_mode_text[] for details).
 * \author PH
 * \date 20031205
 */
static void draw_ents(void) {
   int d, x0, y0;
   BITMAP *e;
   x0 = gx * 16;
   y0 = gy * 16;
   for (d = 0; d < noe; d++) {
      /* Draw only the entities within the view-screen */
      if ((gent[d].tilex >= gx) && (gent[d].tilex < gx + htiles) &&
         (gent[d].tiley >= gy) && (gent[d].tiley < gy + vtiles)) {
            e = eframes[gent[d].chrx][gent[d].facing * 3];
            /* Draw either a normal sprite or a translucent one */
            if (gent[d].transl == 0)
               draw_sprite (double_buffer, e, gent[d].tilex * 16 - x0, gent[d].tiley * 16 - y0);
            else
               draw_trans_sprite (double_buffer, e, gent[d].tilex * 16 - x0, gent[d].tiley * 16 - y0);
      }
   }
}


/*! \brief Preview map
 *
 * Draw the map with all layers on and using parallax/layering
 * like in the game (see map_mode_text[] for details).
 * \author PH
 * \date 20031205
 */
void preview_map(void)
{
   clear_bitmap(double_buffer);
   switch (gmap.map_mode) {
   case 0:
      draw_layer(map, 0);
      draw_layer(b_map, 0);
      draw_ents();
      draw_layer(f_map, 0);
      draw_shadow(0);
      break;
   case 1:
      draw_layer(map, 0);
      draw_ents();
      draw_layer(b_map, 0);
      draw_layer(f_map, 0);
      draw_shadow(0);
      break;
   case 2:
      draw_layer(map, 1);
      draw_layer(b_map, 0);
      draw_ents();
      draw_layer(f_map, 0);
      draw_shadow(0);
      break;
   default:
      textprintf_centre(double_buffer, font, double_buffer->w / 2,
                        double_buffer->h / 2, makecol(255, 255, 255),
                        "Mode %d preview not supported.", gmap.map_mode);
      break;
   }
}


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
         w = ((gy + dy) * gmap.xsize) + gx + dx;

         /* Clears Layer 1 background and then draws */
         if ((draw_mode & MAP_LAYER1) ||
             (draw_mode == BLOCK_COPY) || (draw_mode == BLOCK_PASTE)) {
            if (draw_mode == MAP_LAYER1)
               rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                         dy * 16 + 15, 0);
            blit (icons[map[w]], double_buffer, 0, 0, dx * 16, dy * 16, 16, 16);
         }

         /* This draws Layer 1 only if it was what was showing when
            the user toggled the Attribute.
          */
         if ((draw_mode == MAP_ENTITIES) ||
             (draw_mode == MAP_SHADOWS) ||
             (draw_mode == MAP_OBSTACLES) ||
             (draw_mode == MAP_ZONES) || (draw_mode == GRAB_TILE)) {
            if (showing.last_layer & MAP_LAYER1)
               blit (icons[map[w]], double_buffer, 0, 0, dx * 16,
                     dy * 16, 16, 16);
         }


         /* Clears Layer 2 background and then draws */
         if ((draw_mode & MAP_LAYER2) ||
             (draw_mode == BLOCK_COPY) || (draw_mode == BLOCK_PASTE)) {
            if (draw_mode == MAP_LAYER2)
               rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                         dy * 16 + 15, 0);
            draw_sprite (double_buffer, icons[b_map[w]], dx * 16, dy * 16);
         }

         /* This draws Layer 2 only if it was what was showing when
            the user toggled the Attribute.
          */
         if ((draw_mode == MAP_ENTITIES) ||
             (draw_mode == MAP_SHADOWS) ||
             (draw_mode == MAP_OBSTACLES) ||
             (draw_mode == MAP_ZONES) || (draw_mode == GRAB_TILE)) {
            if (showing.last_layer & MAP_LAYER2)
               draw_sprite (double_buffer, icons[b_map[w]], dx * 16, dy * 16);
         }


         /* Clears Layer 3 background and then draws */
         if ((draw_mode & MAP_LAYER3) ||
             (draw_mode == BLOCK_COPY) || (draw_mode == BLOCK_PASTE)) {
            if (draw_mode == MAP_LAYER3)
               rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                         dy * 16 + 15, 0);
            draw_sprite (double_buffer, icons[f_map[w]], dx * 16, dy * 16);
         }

         /* This draws Layer 3 only if it was what was showing when
            the user toggled the Attribute.
          */
         if ((draw_mode == MAP_ENTITIES) ||
             (draw_mode == MAP_SHADOWS) ||
             (draw_mode == MAP_OBSTACLES) ||
             (draw_mode == MAP_ZONES) || (draw_mode == GRAB_TILE)) {
            if (showing.last_layer & MAP_LAYER3)
               draw_sprite (double_buffer, icons[f_map[w]], dx * 16, dy * 16);
         }

         /* Draw the Shadows */
         if (showing.shadows == 1)
            if (sh_map[w] > 0)
               draw_trans_sprite (double_buffer, shadow[sh_map[w]],
                                  dx * 16, dy * 16);

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
            /* We used to draw a cross-hair; PH invented a nifty
               zone-numbering system to replace it:

               draw_sprite (double_buffer, z_mesh, dx * 16, dy * 16);
             */

            if (z_map[w] < 10) {
               /* Center single-digit number vert+horiz */
               textprintf (double_buffer, font, dx * 16 + 4,
                           dy * 16 + 4, makecol (255, 255, 255), "%d",
                           z_map[w]);
            }

            else if (z_map[w] < 100) {
               /* Center double-digit number vert */
               textprintf (double_buffer, font, dx * 16,
                           dy * 16 + 4, makecol (255, 255, 255), "%d",
                           (int) (z_map[w] / 10));
               textprintf (double_buffer, font, dx * 16 + 8,
                           dy * 16 + 4, makecol (255, 255, 255), "%d",
                           (int) (z_map[w] % 10));
            }

            else {
               /* Print 100's digit in top-right corner of square;
                  10's and 1's digits on bottom of square
                */
               textprintf (double_buffer, font, dx * 16 + 8,
                           dy * 16, makecol (255, 255, 255), "%d",
                           (int) (z_map[w] / 100));
               textprintf (double_buffer, font, dx * 16,
                           dy * 16 + 8, makecol (255, 255, 255), "%02d",
                           (int) (z_map[w] % 100));
            }
         }                      /* if (showing.zones == 1 && z_map[w] > 0) */
      }                         /* for (dx = 0; dx < maxx; dx++) */
   }                            /* for (dy = 0; dy < maxy; dy++) */

   /* Draw the Entities */
   if (showing.entities == 1) {
      for (d = 0; d < noe; d++) {
         /* Draw only the entities within the view-screen */
         if ((gent[d].tilex >= gx) && (gent[d].tilex < gx + htiles) &&
             (gent[d].tiley >= gy) && (gent[d].tiley < gy + vtiles)) {
            /* Draw either a normal sprite or a translucent one */
            if (gent[d].transl == 0)
               draw_sprite
                  (double_buffer,
                   eframes[gent[d].chrx][gent[d].facing * 3],
                   (gent[d].tilex - gx) * 16, (gent[d].tiley - gy) * 16);
            else
               draw_trans_sprite
                  (double_buffer,
                   eframes[gent[d].chrx][gent[d].facing * 3],
                   (gent[d].tilex - gx) * 16, (gent[d].tiley - gy) * 16);
         }
      }
   }                            /* if (showing.entities == 1) */
}                               /* draw_map () */


/*! \brief Draw the menus
 *
 * Process both the menus on the side and bottom of the screen
 */
void draw_menubars (void)
{
   int p, xp, yp, a;
   int draw_mode_display;

   /* Description for the current draw_mode (could use work) */
   char dt[14][12] = { "Layer1", "Layer2", "Layer3",
      "View L1+2", "View L1+3", "View L2+3", "View L1+2+3",
      "Shadows", "Zones", "Obstacles", "Entities",
      "Block Copy", "Block Paste",
      "Grab Tile"
   };

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
   sprintf (strbuf, "Icon: %s", icon_files[gmap.tileset]);
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

   /* Display the tileset in the right menu */
   if (icon_set != 999) {
      for (p = 0; p < (ICONSET_SIZE / 2); p++) {
         blit (icons[icon_set * ICONSET_SIZE + p],
               double_buffer, 0, 0, (SW - 64), p * 16, 16, 16);
         blit (icons[(icon_set * ICONSET_SIZE) + p +
                     (ICONSET_SIZE / 2)],
               double_buffer, 0, 0, (SW - 48), p * 16, 16, 16);
      }
   }

   /* Show which icon the user selected with a rectangle */
   if (curtile >= icon_set * ICONSET_SIZE &&
       curtile < (icon_set * ICONSET_SIZE) + (ICONSET_SIZE)) {
      xp = curtile - (icon_set * ICONSET_SIZE);
      yp = xp;
      xp = xp / (ICONSET_SIZE / 2);
      yp = yp - (xp * (ICONSET_SIZE / 2));

      /* This draws the rectangle around the selected icon */
      rect (double_buffer, xp * 16 + (SW - 64), yp * 16,
            xp * 16 + (SW - 49), yp * 16 + 15, 255);
   }

   /* Clear everything under the tileset */
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
   default:
      draw_mode_display = 0;
      break;
   }                            /* switch (draw_mode) */
   print_sfont ((SW - 66), 170, dt[draw_mode_display], double_buffer);

   /* Display the tileset and selected icon */
   sprintf (strbuf, "#%d(%d)", icon_set, curtile);
   print_sfont ((SW - 72), 176, strbuf, double_buffer);

   /* Mouse x/y coordinates on map (if mouse is over map) */
   if ((mouse_x / 16 < htiles) && (mouse_y / 16 < vtiles)) {
      sprintf (strbuf, "x=%d", gx + x);
      print_sfont ((SW - 72), 182, strbuf, double_buffer);
      sprintf (strbuf, "y=%d", gy + y);
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
         p = o_map[((gy + yp) * gmap.xsize) + gx + xp];
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
         p = z_map[((gy + yp) * gmap.xsize) + gx + xp];
         sprintf (strbuf, "-> %d", p);
         print_sfont (380, (SH - 46), strbuf, double_buffer);
      }
   }

   /* Displays the coordinates of the Block Copy */
   if (draw_mode == BLOCK_COPY && copyx1 != -1 && copyy1 != -1) {
      int rectx1, rectx2, recty1, recty2;

      rectx1 = (copyx1 + gx) * 16;
      recty1 = (copyy1 + gy) * 16;
      rectx2 = (copyx2 + gx) * 16 + 15;
      recty2 = (copyy2 + gy) * 16 + 15;

      // TT: Highlight the selected tile (takes into account window's coords)
      rect (double_buffer, rectx1, recty1, rectx2, recty2, 25);

      /* TT: My next project is to select the top-left coords, then when
       * the user right-clicks and finishes the copy-selection, the rect
       * stretches to surround the selected area.  This would mean that I
       * would have to also check to make sure the coords aren't "backward"
       * like I have for the copy_region() function.  (Could I actually use
       * the code from there to implement this?)
       */
      sprintf (strbuf, "From: %d,%d", copyx1, copyy1);
      print_sfont (320, (SH - 46), strbuf, double_buffer);
      if (copying == 0) {
         sprintf (strbuf, "To: %d,%d", copyx2, copyy2);
         print_sfont (332, (SH - 40), strbuf, double_buffer);
      }
   }

   /* Displays the Entity icon and total and current Entities */
   if (draw_mode == MAP_ENTITIES) {
      sprintf (strbuf, "%d", cent);
      print_sfont ((SW - 14), (SH - 38), strbuf, double_buffer);
      blit (eframes[cent][0], double_buffer, 0, 0, (SW - 16), (SH - 32), 16,
            16);
      sprintf (strbuf, "%d", noe);
      print_sfont ((SW - 14), (SH - 12), strbuf, double_buffer);
   }

   /* Displays the value of the Shadow under the mouse */
   if (draw_mode == MAP_SHADOWS) {
      sprintf (strbuf, "Shd. #%d", curshadow);
      print_sfont (320, (SH - 46), strbuf, double_buffer);
      xp = mouse_x / 16;
      yp = mouse_y / 16;
      if (xp < htiles && yp < vtiles) {
         p = sh_map[((gy + yp) * gmap.xsize) + gx + xp];
         sprintf (strbuf, "-> %d", p);
         print_sfont (380, (SH - 46), strbuf, double_buffer);
      }
   }

   /* Draw a rectangle around the mouse when it's inside the view-window */
   if (mouse_y / 16 < vtiles && mouse_x / 16 < htiles)
      rect (double_buffer, x * 16, y * 16, x * 16 + 15, y * 16 + 15, 255);
}                               /* draw_menubars () */


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
   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (sh_map);
   sh_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
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
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   clipb = 0;
}                               /* bufferize () */


/*! \brief Change map tiles
 *
 * Change any map tiles from one icon to another throughout the map
 */
void global_change (void)
{
   int ld, ft = 0, tt, p;
   /* Layers and attributes */
   int p1, p2, p3, ps, po, pz;
   char opts[9];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "From Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;
   /* Make sure the value is valid */
   if (!(atoi (strbuf) >= 0 && atoi (strbuf) < ICONSET_SIZE * max_sets)) {
      cmessage ("Invalid tile!");
      wait_enter ();
      return;
   }
   ft = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "To Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;
   /* Make sure the value is valid */
   if (!(atoi (strbuf) >= 0 && atoi (strbuf) < ICONSET_SIZE * max_sets)) {
      cmessage ("Invalid tile!");
      wait_enter ();
      return;
   }
   tt = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Layer", screen);
   print_sfont (6, 18, "Layer num (123soz):", screen);
   ld = get_line (126, 18, opts, sizeof (opts) - 1);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   p1 = strchr (opts, '1') ? 1 : 0;
   p2 = strchr (opts, '2') ? 1 : 0;
   p3 = strchr (opts, '3') ? 1 : 0;
   ps = strchr (opts, 's') ? 1 : 0;
   po = strchr (opts, 'o') ? 1 : 0;
   pz = strchr (opts, 'z') ? 1 : 0;

   for (p = 0; p < (gmap.xsize * gmap.ysize); p++) {
      if (p1)
         if (map[p] == ft)
            map[p] = tt;
      if (p2)
         if (b_map[p] == ft)
            b_map[p] = tt;
      if (p3)
         if (f_map[p] == ft)
            f_map[p] = tt;
      if (ps)
         if (sh_map[p] == ft)
            sh_map[p] = tt;     /* Is this supposed to be 'tt'? PH Yes I think so */
      if (po)
         if (o_map[p] == ft)
            o_map[p] = tt;      /* ...same... */
      if (pz)
         if (z_map[p] == ft)
            z_map[p] = tt;      /* ...same... */
   }
}                               /* global_change () */


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


/*! \brief Process keyboard input
 *
 * This allows the user to type in values, such as coords, names, etc.
 *
 * \param   gx x-coord of the input "line"
 * \param   gy y-coord of the input "line"
 * \param   buffer Where to sent the result
 * \param   max_len Maximum characters to accept
 * \returns 0 if user hits ESC (cancel)
 * \returns 1 if user hits ENTER
 */
int get_line (int gx, int gy, char *buffer, int max_len)
{
   int index = 0, ch;
   BITMAP *under;

   under = create_bitmap (320, 6);
   blit (screen, under, 0, gy, 0, 0, 320, 6);
   while (1) {
      ch = (readkey () & 0xff);

      /* Make sure character entered is valid ASCII */
      if (ch >= 32 && ch <= 126 && index < max_len - 1) {
         buffer[index] = ch;
         buffer[index + 1] = 0;
         print_sfont (gx, gy, buffer, screen);
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
            if (ch == 8 || ch == 127) {
               if (--index < 0)
                  index = 0;
               buffer[index] = ' ';
               buffer[index + 1] = 0;
               blit (under, screen, 0, 0, 0, gy, 320, 8);
               print_sfont (gx, gy, buffer, screen);
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
   return done - 1;
}                               /* yninput () */


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
   mpic = create_bitmap (4, 6);
   for (ky = 0; ky < 6; ky++)
      for (kx = 0; kx < 4; kx++)
         mpic->line[ky][kx] = mousepic[ky * 4 + kx];
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
         blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16, p * 16,
               0, 0, 16, 16);
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

   /* Unused mesh */
   b_mesh = create_bitmap (16, 16);
   clear (b_mesh);
   for (ky = 0; ky < 16; ky += 2) {
      for (kx = 0; kx < 16; kx += 2)
         putpixel (b_mesh, kx, ky, 1);
      for (kx = 1; kx < 16; kx += 2)
         putpixel (b_mesh, kx, ky + 1, 1);
   }

   /* Used for Zones */
   z_mesh = create_bitmap (16, 16);
   clear (z_mesh);
   hline (z_mesh, 0, 7, 15, 255);
   hline (z_mesh, 0, 8, 15, 255);
   vline (z_mesh, 7, 0, 15, 255);
   vline (z_mesh, 8, 0, 15, 255);

   /* Print a pretty 'Z' in the middle of the Zone cross-hair */
   print_sfont (5, 5, "Z", z_mesh);

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
   free (f_map);
   free (z_map);
   free (sh_map);
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
}                               /* cleanup () */


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


/*! \brief Handy-dandy help screen
 *
 * This is a simple help screen that displays when F1 is pressed
 */
void show_help (void)
{
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
      "C  . . . . . . . . . View Layers 1-3 + S/E Attributes",
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

   rectfill (screen, (19 * 6) - 5, 30, (54 * 8) + 5,
             NUMBER_OF_ITEMS * 7 + 40, 0);
   rect (screen, (19 * 6) - 3, 32, (54 * 8) + 3, (NUMBER_OF_ITEMS * 7) + 38,
         255);

   this_counter = 0;
   for (this_counter = 0; this_counter < NUMBER_OF_ITEMS; this_counter++) {
      sprintf (strbuf, "%s", help_keys[this_counter]);
      print_sfont (19 * 6, (this_counter * 7) + 35, strbuf, screen);
   }
   yninput ();

}                               /* show_help () */


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
   int zx, zy, bf;

   /* Layers 1-3, Shadows, Obstacles, and Zones */
   int ld, p1, p2, p3, ps, po, pz;

   /* Holds the user input for which region(s) to paste to */
   char opts[9];

   /* Confirm that there is something in the clipboard; don't attempt if not */
   if (clipb == 0)
      return;

   memset (opts, 0, sizeof (opts));
   rectfill (screen, 138, 89, 306, 120, 0);
   print_sfont (140, 91, "Paste which layers? (123soz)", screen);
   rect (screen, 138, 89, 306, 120, 255);
   ld = get_line (160, 110, opts, sizeof (opts) - 1);

   /* Make sure the line isn't blank */
   if (ld == 0)
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
               bf = c_map[zy * gmap.xsize + zx];
               map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            }
            if (p2) {
               bf = cb_map[zy * gmap.xsize + zx];
               b_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            }
            if (p3) {
               bf = cf_map[zy * gmap.xsize + zx];
               f_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            }
            if (pz) {
               bf = cz_map[zy * gmap.xsize + zx];
               z_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            }
            if (ps) {
               bf = cs_map[zy * gmap.xsize + zx];
               sh_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            }
            if (po) {
               bf = co_map[zy * gmap.xsize + zx];
               o_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            }
         }                      /* if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) */
      }                         /* for (zx = 0; zx <= cbw; zx++) */
   }                            /* for (zy = 0; zy <= cbh; zy++) */
}                               /* paste_region_special () */


/*! \brief Paste the copied selection to all Layers
 *
 * Copy all the layers in a block area to a user-defined point in the map
 *
 * \param   tx Target x-coord
 * \param   ty Target y-coord
 */
void paste_region (int tx, int ty)
{
   int zx, zy, bf;

   if (clipb == 0)
      return;
   for (zy = 0; zy <= cbh; zy++) {
      for (zx = 0; zx <= cbw; zx++) {
         if (ty + zy < gmap.ysize && tx + zx < gmap.xsize) {
            bf = c_map[zy * gmap.xsize + zx];
            map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            bf = cb_map[zy * gmap.xsize + zx];
            b_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            bf = cf_map[zy * gmap.xsize + zx];
            f_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            bf = cz_map[zy * gmap.xsize + zx];
            z_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            bf = cs_map[zy * gmap.xsize + zx];
            sh_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
            bf = co_map[zy * gmap.xsize + zx];
            o_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
         }
      }
   }
}                               /* paste_region () */


/*! \brief Copy all the layers in a block area
 *
 * Copy all of the layers from one area to another
 */
void copy_region (void)
{
   /* Area block coords */
   int zx, zy;
   /* Temp file used in copying 'copy from' to 'copy to */
   int bf;

   /* Used to swap 'backward' copy areas */
   int swapx, swapy;

   clipb = 0;
   cbh = copyy2 - copyy1;
   cbw = copyx2 - copyx1;

   /* Determine if 'copy start coords' < 'copy finish coords' and fixes it */
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
      copyx2 = swapx;
      copyx1 = swapx;
      cbw = copyx2 - copyx1;
   }

   clipb = 1;

   /* Proceed to copy all the layers in the area block */
   for (zy = 0; zy <= cbh; zy++) {
      for (zx = 0; zx <= cbw; zx++) {
         /* TT: Is there a reason we don't just write:

            c_map[zy * gmap.xsize + zx] =
            map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
            cb_map[zy * gmap.xsize + zx] =
            b_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
            .
            .
            .
          */

         bf = map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         c_map[zy * gmap.xsize + zx] = bf;
         bf = b_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cb_map[zy * gmap.xsize + zx] = bf;
         bf = f_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cf_map[zy * gmap.xsize + zx] = bf;
         bf = z_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cz_map[zy * gmap.xsize + zx] = bf;
         bf = sh_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         cs_map[zy * gmap.xsize + zx] = bf;
         bf = o_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
         co_map[zy * gmap.xsize + zx] = bf;
      }
   }
}                               /* copy_region () */


/*! \brief Resize the current map
 *
 * Changes the map's height and width
 */
void resize_map (void)
{
   /* User input */
   int ld;
   /* New width and height */
   int neww, newh;
   /* Zone width and height */
   int zx, zy;
   /* Used in copying 'old size' info to 'new size' */
   int bf;
   /* Old width and height */
   int oldw, oldh;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Width: ", screen);
   ld = get_line (48, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;
   /* Make sure the value is valid */
   if ((atoi (strbuf) < 10 || atoi (strbuf) > SW)) {
      cmessage ("Invalid width!");
      wait_enter ();
      return;
   }

   neww = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Height: ", screen);
   ld = get_line (54, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;
   /* Make sure the value is valid */
   if ((atoi (strbuf) < 15 || atoi (strbuf) > SH)) {
      cmessage ("Invalid height!");
      wait_enter ();
      return;
   }

   newh = atoi (strbuf);

   /* Pre-copy the map info */
   for (zy = 0; zy < gmap.ysize; zy++) {
      for (zx = 0; zx < gmap.xsize; zx++) {
         bf = map[zy * gmap.xsize + zx];
         c_map[zy * gmap.xsize + zx] = bf;
         bf = b_map[zy * gmap.xsize + zx];
         cb_map[zy * gmap.xsize + zx] = bf;
         bf = f_map[zy * gmap.xsize + zx];
         cf_map[zy * gmap.xsize + zx] = bf;
         bf = z_map[zy * gmap.xsize + zx];
         cz_map[zy * gmap.xsize + zx] = bf;
         bf = sh_map[zy * gmap.xsize + zx];
         cs_map[zy * gmap.xsize + zx] = bf;
         bf = o_map[zy * gmap.xsize + zx];
         co_map[zy * gmap.xsize + zx] = bf;
      }
   }

   /* Save the old map's width and height */
   oldw = gmap.xsize;
   oldh = gmap.ysize;
   gmap.xsize = neww;
   gmap.ysize = newh;

   /* Set memory for new map size */
   free (map);
   map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (b_map);
   b_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (f_map);
   f_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (sh_map);
   sh_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (o_map);
   o_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);

   /* Draw all the old map data into the new map size */
   for (zy = 0; zy < oldh; zy++) {
      for (zx = 0; zx < oldw; zx++) {
         if (zy < gmap.ysize && zx < gmap.xsize) {
            bf = c_map[zy * oldw + zx];
            map[zy * gmap.xsize + zx] = bf;
            bf = cb_map[zy * oldw + zx];
            b_map[zy * gmap.xsize + zx] = bf;
            bf = cf_map[zy * oldw + zx];
            f_map[zy * gmap.xsize + zx] = bf;
            bf = cz_map[zy * oldw + zx];
            z_map[zy * gmap.xsize + zx] = bf;
            bf = cs_map[zy * oldw + zx];
            sh_map[zy * gmap.xsize + zx] = bf;
            bf = co_map[zy * oldw + zx];
            o_map[zy * gmap.xsize + zx] = bf;
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
   free (cz_map);
   cz_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (cs_map);
   cs_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (co_map);
   co_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);

   /* Re-allocate memory for the 'old-map copy' for next use */
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);

   /* Empty the clipboard */
   clipb = 0;
}                               /* resize_map () */


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
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);

   /* TT FIXED: Forgot to get rid of all the entities! */
   init_entities ();
}                               /* wipe_map () */


/*! \brief Set the map's description
 *
 * This is what displays in the game when a new map is entered (ie its name)
 */
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
   ld = get_line (66, 18, tcd, 40);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   strcpy (gmap.map_desc, tcd);
}                               /* describe_map () */


/*! \brief Copy an entire layer into another
 *
 * Copy the contents of one Layer to another
 */
void copy_layer (void)
{
   int rt, rt2, ff, tt, a, b;
   char ts[4];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Copy Layer to Layer", screen);
   print_sfont (6, 12, "From (1-3):", screen);
   rt = get_line (78, 12, ts, 2);

   /* Make sure the line isn't blank */
   if (rt == 0)
      return;
   /* Make sure the value is valid */
   if ((atoi (ts) < 1 || atoi (ts) > 3)) {
      cmessage ("Invalid layer!");
      wait_enter ();
      return;
   }

   ff = atoi (ts);
   print_sfont (6, 18, "To (1-3):", screen);
   rt2 = get_line (78, 18, ts, 2);

   /* Make sure the line isn't blank */
   if (rt2 == 0)
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


/*! \brief Clears a layer from the map
 *
 * Remove an entire Layer from the map
 */
void clear_layer (void)
{
   int rt, ml, a;
   char ts[4];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Clear Layer", screen);
   print_sfont (6, 12, "Layer (1-3):", screen);
   rt = get_line (84, 12, ts, 2);

   /* Make sure the line isn't blank */
   if (rt == 0)
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


/*! \brief Update the tileset
 *
 * This changes the tileset in the menu on the right and then updates
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


/*! \brief Gets the index of the tile under the mouse
 *
 * Grab the currently selected map tile and display it in the tileset
 */
void get_tile (void)
{
   if (showing.last_layer == MAP_LAYER1)
      curtile = map[((gy + y) * gmap.xsize) + gx + x];
   if (showing.last_layer == MAP_LAYER2)
      curtile = b_map[((gy + y) * gmap.xsize) + gx + x];
   if (showing.last_layer == MAP_LAYER3)
      curtile = f_map[((gy + y) * gmap.xsize) + gx + x];

   /* Set the tileset "page" to the one where the selected icon is found */
   icon_set = curtile / ICONSET_SIZE;
}                               /* get_tile () */
