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

BITMAP *icons[MAX_TILES];  // Selectable tiles on the right-hand menu
BITMAP *pcx_buffer, *double_buffer; // Image and screen buffers
BITMAP *mesh, *b_mesh, *z_mesh;  // Icons for Attributes (*b_mesh unused)
BITMAP *font6, *mpic;   // Font and mouse images
BITMAP *eframes[MAX_EPICS][12];  // Gee, could it be an Entity frame?
BITMAP *shadow[MAX_SHADOWS];  // This is for a shadow.  It's dark.

/* These are for the Layers 1-3 */
unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
/* These are for the Zone, Shadow and Obstacle Attributes */
unsigned char *z_map, *cz_map, *s_map, *cs_map, *o_map, *co_map;

/* Used for the right-hand menu, plus something for the mouse */
short icon_set = 0, max_sets = 51, nomouse = 0;

/* Stores the name of the currently loaded map */
char map_fname[16] = "";

char *strbuf;  // Used everywhere for strings

short gx = 0, gy = 0, x = 0, y = 0; // View-screen and misc coordinates
unsigned short int cb;
int draw_mode = MAP_LAYER123, curtile = 0, dmode = 0;
int curent = 0, curzone = 0, curshadow = 0, curobs = 0;
int copying = 0, copyx1 = -1, copyx2 = -1, copyy1 = -1, copyy2 = -1;
int clipb = 0, cbh = 0, cbw = 0;

ss_map gmap;
s_entity gent[50];
s_show showing;

int noe = 0, cent = 0;  // Number of entities, index of current entity

// Our undesputed mouse picture.  Wow.
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


// Welcome to Mapdraw, folks!
int main (void)
{
   int stop = 0;

   startup ();
   while (!stop)
     {
        process_controls ();
        draw_map ();
        draw_menubars ();
        if (!nomouse)
           draw_sprite (double_buffer, mpic, mouse_x, mouse_y);
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        if (key[KEY_Q])
           stop = confirm_exit ();
        yield_timeslice ();
     }
   cleanup ();
   return 0;
}

END_OF_MAIN ();


// Report errors and comments through this function
void klog (char *msg)
{
   FILE *ff;

   ff = fopen ("mapdraw.log", "a");
   if (!ff)
      exit (-1);
   fprintf (ff, "%s\n", msg);
   fclose (ff);
}


// Mouse and keyboard inputs (sorry, no joystick support)
void process_controls (void)
{
   int k;

   if (keypressed ())
     {
        k = (readkey () >> 8);

        /* Note: The only modes that will draw to the map are
                 MAP_LAYER1, MAP_LAYER2, or MAP_LAYER3 */
        if (k == KEY_1) // Layer 1
           draw_mode = MAP_LAYER1;
        if (k == KEY_2) // Layer 2
           draw_mode = MAP_LAYER2;
        if (k == KEY_3) // Layer 3
           draw_mode = MAP_LAYER3;
        if (k == KEY_4) // Show Layers 1+2
           draw_mode = MAP_LAYER12;
        if (k == KEY_5) // Show Layers 1+3
           draw_mode = MAP_LAYER13;
        if (k == KEY_6) // Show Layers 2+3
           draw_mode = MAP_LAYER23;
        if (k == KEY_7) // Show Layers 1+2+3
           draw_mode = MAP_LAYER123;

        if (k >= KEY_1 && k <= KEY_7)  // Turn off Attributes
          {
             showing.entities = 0;
             showing.shadows = 0;
             showing.obstacles = 0;
             showing.zones = 0;
             showing.last_layer = draw_mode;
          }

        if (k == KEY_C) // View Layers 1+2+3, plus Entites and Shadows
          {
             draw_mode = MAP_LAYER123;
             showing.entities = 1;
             showing.shadows = 1;
             showing.obstacles = 0;
             showing.zones = 0;
             showing.last_layer = draw_mode;
          }
        if (k == KEY_A) // Displays Layers 1-3 and all Attributes
          {
             draw_mode = MAP_LAYER123;
             showing.entities = 1;
             showing.shadows = 1;
             showing.obstacles = 1;
             showing.zones = 1;
             showing.last_layer = draw_mode;
          }

        /* Note: The Attribute must be 'Active'
                 to be able to draw to the map */
        if (k == KEY_F11)  // Toggle Entities Attribute
          {
             if (showing.entities == 1)
               {
                  if (draw_mode == MAP_ENTITIES)
                    {
                       draw_mode = showing.last_layer;
                       showing.entities = 0;
                    }
                  else
                     draw_mode = MAP_ENTITIES;
               }
             else
               {
                  draw_mode = MAP_ENTITIES;
                  showing.entities = 1;
               }
          }
        if (k == KEY_S) // Toggle Shadows Attribute
          {
             if (showing.shadows == 1)
               {
                  if (draw_mode == MAP_SHADOWS)
                    {
                       draw_mode = showing.last_layer;
                       showing.shadows = 0;
                    }
                  else
                     draw_mode = MAP_SHADOWS;
               }
             else
               {
                  draw_mode = MAP_SHADOWS;
                  showing.shadows = 1;
               }
          }
        if (k == KEY_O) // Toggle Obstacles Attribute
          {
             if (showing.obstacles == 1)
               {
                  if (draw_mode == MAP_OBSTACLES)
                    {
                       draw_mode = showing.last_layer;
                       showing.obstacles = 0;
                    }
                  else
                     draw_mode = MAP_OBSTACLES;
               }
             else
               {
                  draw_mode = MAP_OBSTACLES;
                  showing.obstacles = 1;
               }
          }
        if (k == KEY_Z) // Toggle Zones Attribute
          {
             if (showing.zones == 1)
               {
                  if (draw_mode == MAP_ZONES)
                    {
                       draw_mode = showing.last_layer;
                       showing.zones = 0;
                    }
                  else
                     draw_mode = MAP_ZONES;
               }
             else
               {
                  draw_mode = MAP_ZONES;
                  showing.zones = 1;
               }
          }
        if (k == KEY_T) // Copy a selection
           draw_mode = BLOCK_COPY;
        if (k == KEY_P) // Paste copied area
           draw_mode = BLOCK_PASTE;
        if (k == KEY_G) // Get the tile under the mouse curser
          {
             if ((draw_mode != MAP_LAYER1) &&
                 (draw_mode != MAP_LAYER2) &&
                 (draw_mode != MAP_LAYER3) &&
                 (showing.last_layer != MAP_LAYER1) &&
                 (showing.last_layer != MAP_LAYER2) &&
                 (showing.last_layer != MAP_LAYER3))
                showing.last_layer = MAP_LAYER1; // The default view mode
             else if (draw_mode != GRAB_TILE)
                showing.last_layer = draw_mode;
             draw_mode = GRAB_TILE;
          }
        if (k == KEY_W) // Clear contents of current map
           wipe_map ();
        if (k == KEY_D) // Move (displace) location of all entities on map
           displace_entities ();
        if (k == KEY_E) // Empty the contents of the clipboard
           clipb = 0;
        if (k == KEY_R) // Resize the map's height and width
           resize_map ();
        if (k == KEY_J) // Copy Layers 1, 2, 3 to mini PCX images
           maptopcx ();
        if (k == KEY_L) // Set Zone to Last Zone
           curzone = check_last_zone () + 1;

        if (k == KEY_N) // Create new map, lets you choose the tileset
           new_map ();
        if (k == KEY_F1)   // Show help
           show_help();
        if (k == KEY_F2)   // Load a saved map
           load_map ();
        if (k == KEY_F3)   // Save the map you are working on
           save_map ();
        if (k == KEY_F4)   // Erase an entire layer from the map
           clear_layer ();
        if (k == KEY_F5)   // Load a pcx file to become a map
           make_mapfrompcx ();
        if (k == KEY_F6)   // Change all instances of one tile to another
           global_change ();
        if (k == KEY_F7)   // Remove all obstructions on the map
           clear_obstructs ();
        if (k == KEY_F8)   // Remove all shadows on the map
           clear_shadows ();
        if (k == KEY_F9)   // Copy from one layer to another
           copy_layer ();
        if (k == KEY_F10)  // Enter a few words to describe the map
           describe_map ();
        if (k == KEY_F12)  // Entity-modification mode
          {
             showing.entities = 1;
             update_entities ();
          }
        if (k == KEY_ESC)  // Clear the current Block Copy coordinates
          {
             if (copying == 1)
               {
                  copying = 0;
                  copyx1 = copyx2 = -1;
                  copyy1 = copyy2 = -1;
               }
          }
        if (k == KEY_MINUS)   // Change values for current mode
          {
             switch (draw_mode)
               {
               case MAP_ENTITIES:  // Select an Entity to draw
                  cent--;
                  if (cent < 0)
                     cent = MAX_EPICS - 1;
                  break;
               case MAP_SHADOWS:   // Select a Shadow to draw
                  curshadow--;
                  if (curshadow < 0)
                     curshadow = MAX_SHADOWS - 1;
                  break;
               case MAP_OBSTACLES: // Select an Obstacle to draw
                  curobs--;
                  if (curobs < 0)
                     curobs = 5;
                  break;
               case MAP_ZONES:  // Select a Zone to draw
                  curzone--;
                  if (curzone < 0)
                     curzone = MAX_ZONES - 1;
                  break;
               default: // Select a tile from the icon set on the right
                  icon_set--;
                  if (icon_set < 0)
                     icon_set = max_sets;
                  break;
               }
          }
        if (k == KEY_EQUALS)  // Change values for current mode
          {
             switch (draw_mode)
               {
               case MAP_ENTITIES:  // Select an Entity to draw
                  cent++;
                  if (cent == MAX_EPICS)
                     cent = 0;
                  break;
               case MAP_SHADOWS:   // Select a Shadow to draw
                  curshadow++;
                  if (curshadow >= MAX_SHADOWS)
                     curshadow = 0;
                  break;
               case MAP_OBSTACLES: // Select an Obstacle to draw
                  curobs++;
                  if (curobs > 5)
                     curobs = 0;
                  break;
               case MAP_ZONES:  // Select a Zone to draw
                  curzone++;
                  if (curzone >= MAX_ZONES)
                     curzone = 0;
                  break;
               default: // Select a tile from the icon set on the right
                  icon_set++;
                  if (icon_set > max_sets)
                     icon_set = 0;
                  break;
               }
          }
        if (k == KEY_UP)   // Move view-window up
           gy--;
        if (k == KEY_DOWN) // Move view-window down
           gy++;
        if (k == KEY_RIGHT)   // Move view-window right
           gx++;
        if (k == KEY_LEFT) // Move view-window left
           gx--;
        if (k == KEY_PGUP) // Move view-window one page up
           gy -= vtiles;
        if (k == KEY_PGDN) // Move view-window one page down
           gy += vtiles;
        if (k == KEY_TAB)  // Move view-window one page right
           gx += htiles;
        if (k == KEY_BACKSPACE)  // Move view-window one page left
           gx -= htiles;
        if (k == KEY_END)  // Move to bottom-right edge of map
          {
             gx = gmap.xsize - htiles;
             gy = gmap.ysize - vtiles;
          }
        if (k == KEY_HOME) // Move to top-left edge of map
          {
             gx = 0;
             gy = 0;
          }
     }

   // These prevent the user from moving past the map's edge
   if (gx > gmap.xsize - htiles)
      gx = gmap.xsize - htiles;
   if (gx < 0)
      gx = 0;
   if (gy > gmap.ysize - vtiles)
      gy = gmap.ysize - vtiles;
   if (gy < 0)
      gy = 0;

   // No mouse buttons pushed means default drawing mode
   if (!(mouse_b & 1))
      dmode = 0;

   // Left mouse button
   if (mouse_b & 1)
     {
        x = mouse_x / 16;
        y = mouse_y / 16;

        // Check if the mouse is over the menu at the bottom
        if (y > (vtiles - 1))
          {
             // This lets you select all the options at the bottom
             //  of the screen to update your map attributes.
             if (dmode == 0)
               {
                  check_mdupdate (mouse_x, mouse_y);
                  return;
               }
             // This serves a dual purpose: First it 'protects' the tile
             //  below the view-screen from being drawn to when the mouse
             //  is over the options-area.  Second, it means that the user
             //  is still holding down the button when the mouse was moved
             //  down there, so do nothing instead of selecting one of the
             //  options accidentally.
             else
                return;
          }

        // Check if the mouse is over the icon set on the right
        if (x > (htiles - 1))
          {
             // This lets you select the tiles from the icon set at
             //  the right of the screen to update your map icons.
             if (dmode == 0)
               {
                  check_tilesel (mouse_x, mouse_y);
                  return;
               }
             // This does the same thing for the icon set as the above
             //  code does for the menu options.
             else
                return;
          }

        // Draw to the map
        if ((draw_mode == MAP_LAYER1) || (draw_mode == MAP_LAYER2) ||
            (draw_mode == MAP_LAYER3) || (draw_mode == MAP_OBSTACLES) ||
            (draw_mode == MAP_ZONES) || (draw_mode == MAP_ENTITIES) ||
            (draw_mode == MAP_SHADOWS) || (draw_mode == BLOCK_PASTE))
           dmode = 1;

        // Draw to Layer 1
        // This must draw only to Layer 1.  We never want to draw to two
        //  layers at once.
        if (draw_mode == MAP_LAYER1)
           map[((gy + y) * gmap.xsize) + gx + x] = curtile;

        // Draw to Layer 2
        // This must draw only to Layer 2.  Same as above.
        if (draw_mode == MAP_LAYER2)
           b_map[((gy + y) * gmap.xsize) + gx + x] = curtile;

        // Draw to Layer 3
        // This must draw only to Layer 3.  Ditto.
        if (draw_mode == MAP_LAYER3)
           f_map[((gy + y) * gmap.xsize) + gx + x] = curtile;

        // Add Obstacle
        if (draw_mode == MAP_OBSTACLES)
           o_map[((gy + y) * gmap.xsize) + gx + x] = curobs;
        // Add Zone
        if (draw_mode == MAP_ZONES)
           z_map[((gy + y) * gmap.xsize) + gx + x] = curzone;
        // Add Entity
        if (draw_mode == MAP_ENTITIES)
           place_entity ((mouse_x / 16) + gx, (mouse_y / 16) + gy);
        // Add Shadow
        if (draw_mode == MAP_SHADOWS)
           s_map[((gy + y) * gmap.xsize) + gx + x] = curshadow;

        // Begin area copy (copies all zones)
        if (draw_mode == BLOCK_COPY && copying == 0)
          {
             copyx1 = gx + x;
             copyy1 = gy + y;
             copyx2 = copyy2 = -1;  // Clear 'end block copy'
             copying = 1;
          }
        // Paste copied region(s) onto the map
        if (draw_mode == BLOCK_PASTE && clipb != 0)
          paste_region (gx + x, gy + y);

        // Select a tile from the map and its icon from the icon map
        if (draw_mode == GRAB_TILE)
          {
             if (showing.last_layer == MAP_LAYER1)
                curtile = map[((gy + y) * gmap.xsize) + gx + x];
             if (showing.last_layer == MAP_LAYER2)
                curtile = b_map[((gy + y) * gmap.xsize) + gx + x];
             if (showing.last_layer == MAP_LAYER3)
                curtile = f_map[((gy + y) * gmap.xsize) + gx + x];

             icon_set = curtile / ICONSET_SIZE; // Update icon map on right
          }
     } // end if (mouse_b & 1)

   /* Paste Layers and/or Attributes */
   if ((mouse_b & 2) && (draw_mode == BLOCK_PASTE) && (clipb != 0))
     paste_region_special (gx + x, gy + y);

   // Handle a right-click for all remaining options
   if (mouse_b & 2)
     {
        x = mouse_x / 16;
        y = mouse_y / 16;

        // This prevents tiles from being erased 'under' the menus.
        if ((y > (vtiles - 1)) || x > (htiles - 1))
          return;

        // Remove Entity
        if (draw_mode == MAP_ENTITIES)
           erase_entity (mouse_x / 16 + gx, mouse_y / 16 + gy);
        // Remove Shadow
        if (draw_mode == MAP_SHADOWS)
           s_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        // Remove Obstacle
        if (draw_mode == MAP_OBSTACLES)
           o_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        // Remove Zone
        if (draw_mode == MAP_ZONES)
           z_map[((gy + y) * gmap.xsize) + gx + x] = 0;

        // Finish area copy
        if (draw_mode == BLOCK_COPY && copying == 1)
          {
             copyx2 = gx + x;
             copyy2 = gy + y;
             copy_region ();
             copying = 0;
          }

        // Erase tile from Layer 1
        if (draw_mode == MAP_LAYER1)
           map[((gy + y) * gmap.xsize) + gx + x] = 0;
        // Erase tile from Layer 2
        if (draw_mode == MAP_LAYER2)
           b_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        // Erase tile from Layer 3
        if (draw_mode == MAP_LAYER3)
           f_map[((gy + y) * gmap.xsize) + gx + x] = 0;
     } // end if (mouse_b & 2)

   x = mouse_x / 16;
   y = mouse_y / 16;
   if (y > (vtiles - 1))
      y = vtiles - 1;
   if (x > (htiles - 1))
      x = htiles - 1;
}


// Confirm exit with user
int confirm_exit (void)
{
   cmessage ("Are you sure you want to exit? (y/n)");
   return yninput ();
}


// Select a tile from the menu on the right
void check_tilesel (int cx, int cy)
{
   int xp, yp;

   // Make sure that the mouse is over one if the selectable tiles
   if (cx >= (SW - 64) && cx < (SW - 32) && cy >= 0 && cy < 160)
     {
        xp = (cx - (SW - 64)) / 16;
        yp = cy / 16;
        curtile = icon_set * ICONSET_SIZE + (xp * (ICONSET_SIZE / 2) + yp);
     }
}


// Select an option from the menus at the bottom of the screen
void check_mdupdate (int cx, int cy)
{
   int a;

   // Mouse is over 'Icon:' menu
   if (cx >= 0 && cx <= 159 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        // This allows the user to select the tileset used for the
        //  current map
        gmap.tileset++;
        if (gmap.tileset >= NUM_TILESETS)
           gmap.tileset = 0;
        update_tileset (); // See instant results!
        draw_menubars ();
        while (mouse_b & 1);
        return;
     }
   // Mouse is over 'Song:' menu
   if (cx >= 0 && cx <= 159 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 36, (SH - 34), 159, (SH - 29), 0);
        print_sfont (30, (SH - 34), ">", screen);
        hline (screen, 36, (SH - 29), 113, 255);
        a = get_line (36, (SH - 34), strbuf, 19);
        if (a != 0)
           strcpy (gmap.song_file, strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'ZeroZone' menu
   if (cx >= 0 && cx <= 159 && cy >= (SH - 28) && cy <= (SH - 23))
     {
        gmap.zero_zone = 1 - gmap.zero_zone;
        draw_menubars ();
        while (mouse_b & 1);
        return;
     }
   // Mouse is over 'Map #' menu
   if (cx >= 0 && cx <= 79 && cy >= (SH - 22) && cy <= (SH - 17))
     {
        rectfill (screen, 42, (SH - 22), 159, (SH - 17), 0);
        print_sfont (36, (SH - 22), ">", screen);
        hline (screen, 42, (SH - 17), 65, 255);
        a = get_line (42, (SH - 22), strbuf, 4);
        if (a != 0)
           gmap.map_no = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'Mode' menu
   if (cx >= 0 && cx <= 79 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        gmap.map_mode++;
        if (gmap.map_mode > 5)
           gmap.map_mode = 0;
        draw_menubars ();
        while (mouse_b & 1);
        return;
     }
   // Mouse is over 'Save' menu
   if (cx >= 0 && cx <= 79 && cy >= (SH - 10) && cy <= (SH - 5))
     {
        gmap.can_save = 1 - gmap.can_save;
        draw_menubars ();
        while (mouse_b & 1);
        return;
     }
   // Mouse is over 'Warp' menu
   if (cx >= 80 && cx <= 159 && cy >= (SH - 22) && cy <= (SH - 17))
     {
        gmap.can_warp = 1 - gmap.can_warp;
        draw_menubars ();
        while (mouse_b & 1);
        return;
     }
   // Mouse is over 'WarpX' menu
   if (cx >= 80 && cx <= 159 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        rectfill (screen, 122, (SH - 16), 159, (SH - 11), 0);
        print_sfont (116, (SH - 16), ">", screen);
        hline (screen, 122, (SH - 11), 145, 255);
        a = get_line (122, (SH - 16), strbuf, 4);
        if (a != 0)
           gmap.warpx = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'WarpY' menu
   if (cx >= 80 && cx <= 159 && cy >= (SH - 10) && cy <= (SH - 5))
     {
        rectfill (screen, 122, (SH - 10), 159, (SH - 5), 0);
        print_sfont (116, (SH - 10), ">", screen);
        hline (screen, 122, (SH - 5), 145, 255);
        a = get_line (122, (SH - 10), strbuf, 4);
        if (a != 0)
           gmap.warpy = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'Start X' menu
   if (cx >= 160 && cx <= 239 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        rectfill (screen, 214, (SH - 40), 239, (SH - 35), 0);
        print_sfont (208, (SH - 40), ">", screen);
        hline (screen, 214, (SH - 35), 237, 255);
        a = get_line (214, (SH - 40), strbuf, 4);
        if (a != 0)
           gmap.stx = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'Start Y' menu
   if (cx >= 160 && cx <= 239 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 214, (SH - 34), 239, (SH - 29), 0);
        print_sfont (208, (SH - 34), ">", screen);
        hline (screen, 214, (SH - 29), 237, 255);
        a = get_line (214, (SH - 34), strbuf, 4);
        if (a != 0)
           gmap.sty = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'Mult' menu
   if (cx >= 240 && cx <= 319 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        rectfill (screen, 276, (SH - 40), 319, (SH - 35), 0);
        print_sfont (270, (SH - 40), ">", screen);
        hline (screen, 276, (SH - 35), 299, 255);
        a = get_line (276, (SH - 40), strbuf, 4);
        if (a != 0)
           gmap.pmult = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over 'Div' menu
   if (cx >= 240 && cx <= 319 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 270, (SH - 34), 319, (SH - 29), 0);
        print_sfont (264, (SH - 34), ">", screen);
        hline (screen, 270, (SH - 29), 293, 255);
        a = get_line (270, (SH - 34), strbuf, 4);
        if (a != 0)
           gmap.pdiv = atoi (strbuf);
        else
           draw_menubars ();
        return;
     }
   // Mouse is over either 'Width' or 'Height' menus
   if (cx >= 160 && cx <= 319 && cy >= (SH - 28) && cy <= (SH - 17))
      resize_map ();
   // Mouse is over 'SunStone' menu
   if (cx >= 160 && cx <= 239 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        gmap.use_sstone = 1 - gmap.use_sstone;
        draw_menubars ();
        while (mouse_b & 1);
        return;
     }
}


// Update the screen after all controls taken care of
void draw_map (void)
{
   int dx, dy; // Coordinates inside the view-window
   int d;   // Entity index
   int w;   // Index for tiles inside the view-window
   int maxx, maxy;   // Size of the map or view-window, whichever is smaller

   // Clear everything with black
   rectfill (double_buffer, 0, 0, (SW - 81), (SH - 49), 0);

   // The maxx/maxy is used since the map isn't always as large as the
   //  view-window, we don't want to check/update anything that would be
   //  out of bounds.
   if (vtiles > gmap.ysize)
      maxy = gmap.ysize;
   else
      maxy = vtiles;
   if (htiles > gmap.xsize)
      maxx = gmap.xsize;
   else
      maxx = htiles;

   // This loop will draw everything within the view-window
   for (dy = 0; dy < maxy; dy++)
     {
        for (dx = 0; dx < maxx; dx++)
          {
             // This index is the sum of the view-window's coordinates on
             //  the map and the current tile's coordinates inside the
             //  view-window.
             w = ((gy + dy) * gmap.xsize) + gx + dx;

             // Clears Layer 1 background and then draws
             if ((draw_mode & MAP_LAYER1) ||
                 (draw_mode == BLOCK_COPY) ||
                 (draw_mode == BLOCK_PASTE))
               {
                  if (draw_mode == MAP_LAYER1)
                     rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                               dy * 16 + 15, 0);
                     blit (icons[map[w]], double_buffer, 0, 0, dx * 16,
                           dy * 16, 16, 16);
               }

             // This draws Layer 1 only if it was what was showing when
             //  the user toggled the Attribute
             if ((draw_mode == MAP_ENTITIES) ||
                 (draw_mode == MAP_SHADOWS) ||
                 (draw_mode == MAP_OBSTACLES) ||
                 (draw_mode == MAP_ZONES) ||
                 (draw_mode == GRAB_TILE))
               {
                  if (showing.last_layer & MAP_LAYER1)
                     blit (icons[map[w]], double_buffer, 0, 0, dx * 16,
                           dy * 16, 16, 16);
               }


             // Clears Layer 2 background and then draws
             if ((draw_mode & MAP_LAYER2) ||
                 (draw_mode == BLOCK_COPY) ||
                 (draw_mode == BLOCK_PASTE))
               {
                  if (draw_mode == MAP_LAYER2)
                     rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                               dy * 16 + 15, 0);
                  draw_sprite (double_buffer, icons[b_map[w]], dx * 16,
                               dy * 16);
               }

             // This draws Layer 2 only if it was what was showing when
             //  the user toggled the Attribute
             if ((draw_mode == MAP_ENTITIES) ||
                 (draw_mode == MAP_SHADOWS) ||
                 (draw_mode == MAP_OBSTACLES) ||
                 (draw_mode == MAP_ZONES) ||
                 (draw_mode == GRAB_TILE))
               {
                  if (showing.last_layer & MAP_LAYER2)
                     draw_sprite (double_buffer, icons[b_map[w]], dx * 16,
                                  dy * 16);
               }


             // Clears Layer 3 background and then draws
             if ((draw_mode & MAP_LAYER3) ||
                 (draw_mode == BLOCK_COPY) ||
                 (draw_mode == BLOCK_PASTE))
               {
                  if (draw_mode == MAP_LAYER3)
                     rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                               dy * 16 + 15, 0);
                  draw_sprite (double_buffer, icons[f_map[w]], dx * 16,
                               dy * 16);
               }

             // This draws Layer 3 only if it was what was showing when
             //  the user toggled the Attribute
             if ((draw_mode == MAP_ENTITIES) ||
                 (draw_mode == MAP_SHADOWS) ||
                 (draw_mode == MAP_OBSTACLES) ||
                 (draw_mode == MAP_ZONES) ||
                 (draw_mode == GRAB_TILE))
               {
                  if (showing.last_layer & MAP_LAYER3)
                     draw_sprite (double_buffer, icons[f_map[w]], dx * 16,
                                  dy * 16);
               }

             // Draw the Shadows
             if (showing.shadows == 1)
                if (s_map[w] > 0)
                   draw_trans_sprite (double_buffer, shadow[s_map[w]],
                                    dx * 16, dy * 16);

             // Draw the Obstacles
             if (showing.obstacles == 1)
               {
                  switch (o_map[w])
                    {
                    // Block-all (square) obstacle
                    case 1:
                       draw_sprite (double_buffer, mesh, dx * 16, dy * 16);
                       break;
                    // North-block: blocks movement up
                    case 2:
                       hline (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                              255);
                       vline (double_buffer, dx * 16 + 8, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    // East-block: blocks movement right
                    case 3:
                       hline (double_buffer, dx * 16, dy * 16 + 8,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16 + 15, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    // South-block: blocks movement down
                    case 4:
                       hline (double_buffer, dx * 16, dy * 16 + 15,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16 + 8, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    // West-block: blocks movement left
                    case 5:
                       hline (double_buffer, dx * 16, dy * 16 + 8,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16, dy * 16, dy * 16 + 15,
                              255);
                       break;
                    }
               }

             // Draw the Zones
             if (showing.zones == 1)
                if (z_map[w] > 0)
                   draw_sprite (double_buffer, z_mesh, dx * 16, dy * 16);
          }
     }

   // Draw the Entities
   if (showing.entities == 1)
     {
        for (d = 0; d < noe; d++)
          {
             // Draw only the entities within the view-screen
             if ((gent[d].tilex >= gx) && (gent[d].tilex < gx + htiles) &&
                 (gent[d].tiley >= gy) && (gent[d].tiley < gy + vtiles))
               {
                  // Draw either a normal sprite or a translucent one
                  if (gent[d].transl == 0)
                     draw_sprite
                           (double_buffer,
                            eframes[gent[d].chrx][gent[d].facing * 3],
                            (gent[d].tilex - gx) * 16,
                            (gent[d].tiley - gy) * 16);
                  else
                     draw_trans_sprite
                           (double_buffer,
                            eframes[gent[d].chrx][gent[d].facing * 3],
                            (gent[d].tilex - gx) * 16,
                            (gent[d].tiley - gy) * 16);
               }
          }
     }
}


// Process both the menus on the side and bottom of the screen
void draw_menubars (void)
{
   int p, xp, yp, a;
/* TODO TT BUG:
   Wrote into the other kqlives guys; I can't figure the algorythm to
   get the following code to work: Since draw_mode now references as:
   1, 2, 4, 8, 16, 32, 64, ...
   it won't print correctly.  It needs to be 0, 1, 2, 3, 4, ...
   so until they update that, this will have a problem, at least when
   displaying the Active draw_mode.
*/
   // Description for the current draw_mode (could use work)
   char dt[14][12] =
      { "Layer1", "Layer2", "Layer3",
        "View L1+2", "View L1+3", "View L2+3", "View L1+2+3",
        "Shadows", "Zones", "Obstacles", "Entities",
        "Block Copy", "Block Paste",
        "Grab Tile"
      };

   // The white line that seperates the bottom menu
   hline (double_buffer, 0, (SH - 48), (SW - 81), 255);
   // Fill the area under the right menu with black
   rectfill (double_buffer, (SW - 72), 0, (SW - 1), (SH - 1), 0);
   // Fill the area under the bottom menu with black
   rectfill (double_buffer, 0, (SH - 47), (SW - 73), (SH - 1), 0);
   // The white verticle bar that seperates the icon-menu
   rectfill (double_buffer, (SW - 80), 0, (SW - 73), (SH - 49), 255);

   // Give the name of the current map
   sprintf (strbuf, "Map: %s", map_fname);
   print_sfont (0, (SH - 46), strbuf, double_buffer);
   // Show which tileset the map will use
   sprintf (strbuf, "Icon: %s", icon_files[gmap.tileset]);
   print_sfont (0, (SH - 40), strbuf, double_buffer);
   // Give the default song played when the map is entered
   sprintf (strbuf, "Song: %s", gmap.song_file);
   print_sfont (0, (SH - 34), strbuf, double_buffer);
   // Allow the map to use Zero Zone or not
   if (gmap.zero_zone == 0)
      print_sfont (0, (SH - 28), "ZeroZone: NO", double_buffer);
   else
      print_sfont (0, (SH - 28), "ZeroZone: YES", double_buffer);
   // Display the map number
   sprintf (strbuf, "Map #: %d", gmap.map_no);
   print_sfont (0, (SH - 22), strbuf, double_buffer);
   // The current mode we use (see drawmap() for info)
   sprintf (strbuf, "Mode: %d", gmap.map_mode);
   print_sfont (0, (SH - 16), strbuf, double_buffer);
   // Can the player save the game here?
   if (gmap.can_save == 0)
      print_sfont (0, (SH - 10), "Save: NO", double_buffer);
   else
      print_sfont (0, (SH - 10), "Save: YES", double_buffer);
   // Can the player use the Warp spell from here?
   if (gmap.can_warp == 0)
      print_sfont (80, (SH - 22), "Warp: NO", double_buffer);
   else
      print_sfont (80, (SH - 22), "Warp: YES", double_buffer);

   // Coordinates where Warp spell takes you to
   // NOTE: only works for main.map apparently...
   sprintf (strbuf, "WarpX: %d", gmap.warpx);
   print_sfont (80, (SH - 16), strbuf, double_buffer);
   sprintf (strbuf, "WarpY: %d", gmap.warpy);
   print_sfont (80, (SH - 10), strbuf, double_buffer);
   // Default area to warp to when a map is entered
   sprintf (strbuf, "Start X: %d", gmap.stx);
   print_sfont (160, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Start Y: %d", gmap.sty);
   print_sfont (160, (SH - 34), strbuf, double_buffer);
   // Size of the current map
   sprintf (strbuf, "Width: %d", gmap.xsize);
   print_sfont (160, (SH - 28), strbuf, double_buffer);
   sprintf (strbuf, "Height: %d", gmap.ysize);
   print_sfont (160, (SH - 22), strbuf, double_buffer);
   // Parallaxing multiplication/division values (usually 1:1)
   sprintf (strbuf, "Mult: %d", gmap.pmult);
   print_sfont (240, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Div: %d", gmap.pdiv);
   print_sfont (240, (SH - 34), strbuf, double_buffer);
   // Allow the player to use SunStones on this map
   print_sfont (160, (SH - 16),
                gmap.use_sstone == 0 ? "SunStone: NO" : "SunStone: YES",
                double_buffer);

   // Count Zone Attributes on map
   a = check_last_zone ();
   sprintf (strbuf, "Last zone: %d", a);
   print_sfont (160, (SH - 10), strbuf, double_buffer);

   // Display the icon set in the right menu
   if (icon_set != 999)
     {
        for (p = 0; p < (ICONSET_SIZE / 2); p++)
          {
             blit (icons[icon_set * ICONSET_SIZE + p],
                   double_buffer, 0, 0, (SW - 64), p * 16, 16, 16);
             blit (icons[(icon_set * ICONSET_SIZE) + p +
                         (ICONSET_SIZE / 2)], double_buffer, 0, 0,
                   (SW - 48), p * 16, 16, 16);
          }
     }

   // Show which icon the user selected with a rectangle
   if (curtile >= icon_set * ICONSET_SIZE &&
       curtile < (icon_set * ICONSET_SIZE) + (ICONSET_SIZE))
     {
        xp = curtile - (icon_set * ICONSET_SIZE);
        yp = xp;
        xp = xp / (ICONSET_SIZE / 2);
        yp = yp - (xp * (ICONSET_SIZE / 2));
        // This draws the rectangle around the selected icon
        rect (double_buffer, xp * 16 + (SW - 64), yp * 16,
              xp * 16 + (SW - 49), yp * 16 + 15, 255);
     }
   // Clear everything under the icon set
   rectfill (double_buffer, (SW - 72), 164, (SW - 1), (SH - 1), 0);
   // Display the draw_mode (see beginning of function for descriptions)
   print_sfont ((SW - 72), 164, "Mode:", double_buffer);

// TT hack:
/* { */ // <-- This brace is just to help localize the 'hack code'

   int draw_mode_display = 0;
   switch (draw_mode)
     {
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
     }
      print_sfont ((SW - 66), 170, dt[draw_mode_display], double_buffer);
#if 0
/* Okay, this is how the conversion works:
     log(draw_mode) / log(2)
   Of course, we need to make it display correctly:
     (int)(ceil(log(draw_mode) / log(2)))
   The only problem that we have is of: LAYER12, LAYER13, LAYER23, LAYER123
   So, after much fussing over it, I decided to keep it as-is above
*/
      print_sfont ((SW - 66), 170, dt[(int)(ceil(log(draw_mode) / log(2)))], double_buffer);
#endif

/* } */
// TT: end hack

   // Display icon set and selected tile
   sprintf (strbuf, "#%d(%d)", icon_set, curtile);
   print_sfont ((SW - 72), 176, strbuf, double_buffer);
   // Mouse x/y coordinates on map (if mouse is over map)
   if ((mouse_x / 16 < htiles) && (mouse_y / 16 < vtiles))
     {
        sprintf (strbuf, "x=%d", gx + x);
        print_sfont ((SW - 72), 182, strbuf, double_buffer);
        sprintf (strbuf, "y=%d", gy + y);
        print_sfont ((SW - 72), 188, strbuf, double_buffer);
     }
   // Show when a user is drawing to the map
   if (dmode == 1)
      print_sfont ((SW - 64), 194, "drawing", double_buffer);

   // Displays the value of the Obstacle under the mouse
   if (draw_mode == MAP_OBSTACLES)
     {
        sprintf (strbuf, "Obs. #%d", curobs);
        print_sfont (320, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = o_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (380, (SH - 46), strbuf, double_buffer);
          }
     }
   // Displays the value of the Zone under the mouse
   if (draw_mode == MAP_ZONES)
     {
        sprintf (strbuf, "Zone #%d", curzone);
        print_sfont (320, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = z_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (380, (SH - 46), strbuf, double_buffer);
          }
     }
   // Displays the coordinates of the Block Copy
   if (draw_mode == BLOCK_COPY && copyx1 != -1 && copyy1 != -1)
     {
        sprintf (strbuf, "From: %d,%d", copyx1, copyy1);
        print_sfont (320, (SH - 46), strbuf, double_buffer);
        if (copying == 0)
          {
             sprintf (strbuf, "To: %d,%d", copyx2, copyy2);
             print_sfont (332, (SH - 40), strbuf, double_buffer);
          }
     }
   // Displays the Entity icon and total and current Entities
   if (draw_mode == MAP_ENTITIES)
     {
        sprintf (strbuf, "%d", cent);
        print_sfont ((SW - 14), (SH - 38), strbuf, double_buffer);
        blit (eframes[cent][0], double_buffer, 0, 0, (SW - 16), (SH - 32), 16,
              16);
        sprintf (strbuf, "%d", noe);
        print_sfont ((SW - 14), (SH - 12), strbuf, double_buffer);
     }
   // Displays the value of the Shadow under the mouse
   if (draw_mode == MAP_SHADOWS)
     {
        sprintf (strbuf, "Shd. #%d", curshadow);
        print_sfont (320, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = s_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (380, (SH - 46), strbuf, double_buffer);
          }
     }
   // Draw a rectangle around the mouse when it's inside the view-window
   if (mouse_y / 16 < vtiles && mouse_x / 16 < htiles)
     rect (double_buffer, x * 16, y * 16, x * 16 + 15, y * 16 + 15, 255);
}


// Allocation of memory, etc. for the maps
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


// Change any map tiles from one icon to another
void global_change (void)
{
   int ld, ft = 0, tt, p;
   int p1, p2, p3, ps, po, pz;   // Layers and Attributes
   char opts[9];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "From Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld == 0)
      return;
   ft = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "To Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld == 0)
      return;
   tt = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Layer", screen);
   print_sfont (6, 18, "Layer num (123soz):", screen);
   ld = get_line (126, 18, opts, sizeof (opts) - 1);
   if (ld == 0)
      return;
   p1 = strchr (opts, '1') ? 1 : 0;
   p2 = strchr (opts, '2') ? 1 : 0;
   p3 = strchr (opts, '3') ? 1 : 0;
   ps = strchr (opts, 's') ? 1 : 0;
   po = strchr (opts, 'o') ? 1 : 0;
   pz = strchr (opts, 'z') ? 1 : 0;

   for (p = 0; p < (gmap.xsize * gmap.ysize); p++)
     {
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
           if (s_map[p] == ft)
              s_map[p] = ft;
        if (po)
           if (o_map[p] == ft)
              o_map[p] = ft;
        if (pz)
           if (z_map[p] == ft)
              z_map[p] = ft;
     }
}


// Get rid of all Obstructions on the map
void clear_obstructs (void)
{
   int co;

   cmessage ("Do you want to clear Obstructions? (y/n)");
   if (yninput ())
      for (co = 0; co < gmap.xsize * gmap.ysize; co++)
         o_map[co] = 0;
}


// Get rid of all Shadows on the map
void clear_shadows (void)
{
   int co;

   cmessage ("Do you want to clear Shadows? (y/n)");
   if (yninput ())
      for (co = 0; co < gmap.xsize * gmap.ysize; co++)
         s_map[co] = 0;
}


// Process keyboard input
int get_line (int gx, int gy, char *buffer, int max_len)
{
   int index = 0, ch;
   BITMAP *under;

   under = create_bitmap (320, 6);
   blit (screen, under, 0, gy, 0, 0, 320, 6);
   while (1)
     {
        ch = (readkey () & 0xff);
        // Make sure character entered is valid ASCII
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
             if (ch == 13) // ENTER key pressed
               {
                  buffer[index] = 0;
                  destroy_bitmap (under);
                  return (1);
               }
             else
               {
                  if (ch == 8 || ch == 127)  // BACKSPACE pressed
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
                       if (ch == 27)   // ESC key pressed
                         {
                            destroy_bitmap (under);
                            return (0);
                         }
                    }
               }
          }
     }
}


// Process keyboard input for a yes/no question (ENTER key not required)
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


// The opposite of shutdown, maybe?
void startup (void)
{
   int k, kx, ky, a, p, q;
   COLOR_MAP cmap;

   allegro_init ();
   install_keyboard ();
   install_timer ();
   // Determine if user wants to use Windowed Mode
   if (WBUILD == 1)
      set_gfx_mode (GFX_AUTODETECT_WINDOWED, SW, SH, 0, 0);
   else
      set_gfx_mode (GFX_AUTODETECT, SW, SH, 0, 0);
   // Check for presence of mouse
   a = install_mouse ();
   if (a == -1)
     {
        nomouse = 1;
        klog ("Mouse not found!\n");
        rest (1000);
     }
   // Create the picture used for the mouse
   mpic = create_bitmap (4, 6);
   for (ky = 0; ky < 6; ky++)
      for (kx = 0; kx < 4; kx++)
         mpic->line[ky][kx] = mousepic[ky * 4 + kx];
   set_mouse_speed (4, 4);

   // Buffer for all strings
   strbuf = (char *) malloc (256);
   // Screen buffer
   double_buffer = create_bitmap (SW, SH);
   clear (double_buffer);
   gmap.map_no = -1;
   gmap.tileset = 0;
   gmap.xsize = htiles;
   gmap.ysize = vtiles;
   // Create a bitmap for each tile/icon used
   set_palette (pal);
   for (k = 0; k < MAX_TILES; k++)
     {
        icons[k] = create_bitmap (16, 16);
        clear (icons[k]);
     }
   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   // Determine the number of icons by the size of the PCX image loaded
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

   // Used for Obstacles
   mesh = create_bitmap (16, 16);
   clear (mesh);
   for (ky = 0; ky < 16; ky += 2)
     {
        for (kx = 0; kx < 16; kx += 2)
           putpixel (mesh, kx, ky, 255);
        for (kx = 1; kx < 16; kx += 2)
           putpixel (mesh, kx, ky + 1, 255);
     }
   // Unused mesh
   b_mesh = create_bitmap (16, 16);
   clear (b_mesh);
   for (ky = 0; ky < 16; ky += 2)
     {
        for (kx = 0; kx < 16; kx += 2)
           putpixel (b_mesh, kx, ky, 1);
        for (kx = 1; kx < 16; kx += 2)
           putpixel (b_mesh, kx, ky + 1, 1);
     }
   // Used for Zones
   z_mesh = create_bitmap (16, 16);
   clear (z_mesh);
   hline (z_mesh, 0, 7, 15, 255);
   hline (z_mesh, 0, 8, 15, 255);
   vline (z_mesh, 7, 0, 15, 255);
   vline (z_mesh, 8, 0, 15, 255);
   // Print a pretty 'Z' in the middle of the cross-hair
   print_sfont (5, 5, "Z", z_mesh);

   // Shadow images
   pcx_buffer = load_pcx ("Misc.pcx", pal);
   for (a = 0; a < MAX_SHADOWS; a++)
     {
        shadow[a] = create_bitmap (16, 16);
        blit (pcx_buffer, shadow[a], a * 16, 160, 0, 0, 16, 16);
     }
   destroy_bitmap (pcx_buffer);

   // Entities images
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
   showing.entities = 0;
   showing.shadows = 0;
   showing.obstacles = 0;
   showing.zones = 0;
   showing.last_layer = draw_mode;
}


// Used to shut down the map editor
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


// Wait for ENTER key
void wait_enter (void)
{
   while ((readkey () >> 8) != KEY_ENTER);
}


// Handy-dandy help screen
void show_help (void)
{
   #define NUMBER_OF_ITEMS 54

   char *help_keys[NUMBER_OF_ITEMS] =
     {
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
      "L  . . . . . . . . . . . . . . . . . . . . Last Layer",
      "",
      "F11  . . . . . . . . . . . . . . . . . .  Entity Mode",
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

   int this_counter = 0;
   for (this_counter = 0; this_counter < NUMBER_OF_ITEMS; this_counter++) {
      sprintf (strbuf, "%s", help_keys[this_counter]);
      print_sfont (19*6, (this_counter * 7) + 35, strbuf, screen);
   }
   while(!key[KEY_ENTER] && !key[KEY_ESC]);

}


// Message displayed on the screen used for warnings, etc.
void cmessage (char *buff)
{
   int cx;

   cx = strlen (buff) * 3;
   rectfill (screen, 154 - cx, 91, 165 + cx, 108, 0);
   rect (screen, 154 - cx, 91, 165 + cx, 108, 255);
   print_sfont (160 - cx, 97, buff, screen);
}


// Paste only the Layer specified by the user
void paste_region_special (int tx, int ty)
{
   int zx, zy, bf;
   int p1, p2, p3, ps, po, pz;   // Layers and Attributes
   char opts[9];
   if (clipb == 0)
      return;
   memset (opts, 0, sizeof (opts));
   rectfill (screen, 138, 89, 306, 120, 0);
   print_sfont (140, 91, "Paste which layers? (123soz)", screen);
   rect (screen, 138, 89, 306, 120, 255);
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
             if (ty + zy < gmap.ysize && tx + zx < gmap.xsize)
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


// Paste the copied selection to all Layers
void paste_region (int tx, int ty)
{
   int zx, zy, bf;

   if (clipb == 0)
      return;
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             if (ty + zy < gmap.ysize && tx + zx < gmap.xsize)
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


// Copy all the layers in a block area
void copy_region (void)
{
   int zx, zy, bf;
   int swapx, swapy; // Used to swap 'backward' copy areas

   clipb = 0;
   cbh = copyy2 - copyy1;
   cbw = copyx2 - copyx1;

   // If the user selected the tiles 'backward', fix it
   if (cbh < 1)
     {
        // This swaps the 'start' and 'finish' points (y-coord)
        swapy = copyy2;
        copyy2 = copyy1;
        copyy1 = swapy;
        cbh = copyy2 - copyy1;
     }
   if (cbw < 1)
     {
        // This swaps the x-coords
        swapx = copyx2;
        copyx2 = swapx;
        copyx1 = swapx;
        cbw = copyx2 - copyx1;
     }

   clipb = 1;

   // Proceed to copy all the layers in the area block
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             bf = map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
             c_map[zy * gmap.xsize + zx] = bf;
             bf = b_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
             cb_map[zy * gmap.xsize + zx] = bf;
             bf = f_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
             cf_map[zy * gmap.xsize + zx] = bf;
             bf = z_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
             cz_map[zy * gmap.xsize + zx] = bf;
             bf = s_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
             cs_map[zy * gmap.xsize + zx] = bf;
             bf = o_map[(copyy1 + zy) * gmap.xsize + copyx1 + zx];
             co_map[zy * gmap.xsize + zx] = bf;
          }
     }
}


// Resize the current map
void resize_map (void)
{
   int ld;     // User input
   int neww, newh; // New width and height
   int zx, zy; // Zone width and height
   int bf;     // Used in copying 'old size' information to 'new size'
   int oldw, oldh; // Old width and height

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Width: ", screen);
   ld = get_line (48, 18, strbuf, 4);
   if (ld != 0)
      neww = atoi (strbuf);
   else
      return;
   if (neww < 20 || neww > 640)
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
      newh = atoi (strbuf);
   else
      return;
   if (newh < 15 || newh > 480)
     {
        cmessage ("Illegal height!");
        wait_enter ();
        return;
     }
   // Pre-copy the map info
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
   oldw = gmap.xsize;  // Save old width
   oldh = gmap.ysize;  // Save old height
   gmap.xsize = neww;
   gmap.ysize = newh;
   // Set memory for new map size
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
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (s_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   // Draw all the old map data into the new map size
   for (zy = 0; zy < oldh; zy++)
     {
        for (zx = 0; zx < oldw; zx++)
          {
             if (zy < gmap.ysize && zx < gmap.xsize)
               {
                  bf = c_map[zy * oldw + zx];
                  map[zy * gmap.xsize + zx] = bf;
                  bf = cb_map[zy * oldw + zx];
                  b_map[zy * gmap.xsize + zx] = bf;
                  bf = cf_map[zy * oldw + zx];
                  f_map[zy * gmap.xsize + zx] = bf;
                  bf = cz_map[zy * oldw + zx];
                  z_map[zy * gmap.xsize + zx] = bf;
                  bf = cs_map[zy * oldw + zx];
                  s_map[zy * gmap.xsize + zx] = bf;
                  bf = co_map[zy * oldw + zx];
                  o_map[zy * gmap.xsize + zx] = bf;
               }
          }
     }
   // Free the 'old-map copy' memory
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

   // Empty the clipboard
   clipb = 0;
}


// Clear the contents of a map
void wipe_map (void)
{
   cmessage ("Do you want to clear the whole map? (y/n)");
   if (yninput ())
     {
        memset (map, 0, gmap.xsize * gmap.ysize * 2);
        memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
        memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
        memset (z_map, 0, gmap.xsize * gmap.ysize);
        memset (s_map, 0, gmap.xsize * gmap.ysize);
        memset (o_map, 0, gmap.xsize * gmap.ysize);
     }
}


// This is what displays in the game when a new map is entered
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
   if (ld != 0)
      strcpy (gmap.map_desc, tcd);
}


// Copy the contents of one Layer to another
void copy_layer (void)
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
                     b = (ff == 2 ? b_map[a] : f_map[a]);
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


// Remove an entire Layer from the map
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
             else if (ml == 2)
                b_map[a] = 0;
             else
                f_map[a] = 0;
          }
     }
}


// Print the string to the screen with mapdraw's font
void print_sfont (int x, int y, char *string, BITMAP * where)
{
   int i, c;

   for (i = 0; i < (signed) strlen (string); i++)
     {
        c = string[i];
        c -= 32;  // Convert ascii to the bitmap equivalent
        if (c < 0)
           c = 0;
        masked_blit (font6, where, 0, c * 6, i * 6 + x, y, 6, 6);
     }
}


// Hopefully, this will change the tile map in the menu on the right
void update_tileset (void)
{
   int tmapx, tmapy;
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);
   for (tmapy = 0; tmapy < max_sets; tmapy++)
      for (tmapx = 0; tmapx < 20; tmapx++)
         blit (pcx_buffer, icons[tmapy * 20 + tmapx], tmapx * 16, tmapy * 16, 0, 0, 16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
}

// Count and display the maximum Zone Attributes on the map
int check_last_zone (void)
{
   int a = 0, p;
   for (p = 0; p < gmap.xsize * gmap.ysize; p++)
      if (z_map[p] > a)
         a = z_map[p];
   return a;
}
