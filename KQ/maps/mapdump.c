/***************************************************************************\
 * This is part of the KQ game, supported by the kqlives community.        *
 * Comments and suggestions are welcome.  Please join our                  *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/                      *
 *                                                                         *
 * Mapdump.c is a hacked up program by PH to write out some parameters     *
 * from the map files                                                      *
\***************************************************************************/

// TT TODO: We still need to make a cleanup() function

#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "mapdraw.h"
#include "../include/disk.h"


/* globals */

/* Image and screen buffers */
BITMAP *pcx_buffer;
char *strbuf;

/* Shadows, obstacles, entities */
BITMAP *shadow[MAX_SHADOWS];
BITMAP *mesh;
BITMAP *eframes[MAX_EPICS][12];

/* These are for the Layers 1-3 */
unsigned short *map, *b_map, *f_map;
/* These are for the Zone, Shadow and Obstacle Attributes */
unsigned char *z_map, *sh_map, *o_map;

/* Selectable tiles on the right-hand menu */
BITMAP *icons[MAX_TILES];

/* Used for the icons */
short max_sets = 51;

/* Stores the name of the currently loaded map */
char map_fname[16] = "";

s_map gmap;
s_entity gent[50];

/* Number of entities, index of currently-selected entity */
int noe = 0;

/* Default values, incase an option is not specified */
int show_layer1 = 1, show_layer2 = 1, show_layer3 = 1, show_shadows = 1,
   show_zones = 0, show_obstacles = 0, show_entities = 1;

char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "Incave.pcx", "village.pcx", "mount.pcx"
};

PALETTE pal;


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
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
}                               /* bufferize () */

END_OF_FUNCTION (bufferize);


/*! \brief Load map to modify
 *
 * \param   fname Name of the file we will open
 */
void load_map_batch (const char *fname)
{
   int p, q, i;
   PACKFILE *pf;

   pf = pack_fopen (fname, F_READ_PACKED);
   if (!pf)
      return;

   strcpy (map_fname, fname);
   load_s_map (&gmap, pf);

   for (i = 0; i < 50; ++i) {
      load_s_entity (gent + i, pf);
   }
   bufferize ();
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         map[q * gmap.xsize + p] = pack_igetw (pf);
      }
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         b_map[q * gmap.xsize + p] = pack_igetw (pf);
      }
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         f_map[q * gmap.xsize + p] = pack_igetw (pf);
      }
   }

   pack_fread (z_map, (gmap.xsize * gmap.ysize), pf);
   pack_fread (sh_map, (gmap.xsize * gmap.ysize), pf);
   pack_fread (o_map, (gmap.xsize * gmap.ysize), pf);
   pack_fclose (pf);

   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);
   for (p = 0; p < max_sets; p++) {
      for (q = 0; q < ICONSET_SIZE; q++) {
         blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16, p * 16, 0, 0,
               16, 16);
      }
   }

   /* Check for bogus map squares */
   for (p = 0; p < gmap.xsize * gmap.ysize; ++p) {
      /* Mid layer */
      if (map[p] > MAX_TILES)
         map[p] = 0;

      /* Background layer */
      if (b_map[p] > MAX_TILES)
         b_map[p] = 0;

      /* Foreground layer */
      if (f_map[p] > MAX_TILES)
         f_map[p] = 0;

      /* Shadow layer */
      if (sh_map[p] >= MAX_SHADOWS)
         sh_map[p] = 0;

      /* Zone layer */
      if (z_map[p] > MAX_ZONES)
         z_map[p] = 0;

      /* Obstacles layer */
      if (o_map[p] > MAX_OBSTACLES)
         o_map[p] = 0;
   }
   destroy_bitmap (pcx_buffer);

   return;

}                               /* load_map () */

END_OF_FUNCTION (load_map_batch);


/*! \brief Display help on the command syntax */
static void usage (char *argv)
{
   printf ("Map to PCX converter for KQ.\n");
   printf ("Usage: %s [+/-][options] filename(s)\n", argv);
   printf ("Options:\n");
   printf ("   +  includes the option: it WILL appear in the PCX image.\n");
   printf ("   -  negates an option: it will not appear in the PCX image.\n");
   printf ("   1  shows layer 1 (default ON)\n");
   printf ("   2  shows layer 2 (default ON)\n");
   printf ("   3  shows layer 3 (default ON)\n");
   printf ("   e  shows the entities (default OFF)\n");
   printf ("   o  shows the obstacles (default OFF)\n");
   printf ("   s  shows the shadows (default ON)\n");
   printf ("   z  shows the zones (default OFF)\n");
   printf ("   filename  is the .MAP file(s) to be used\n\n");
   printf ("Example: %s +1oz -23es\n", argv);
   printf ("  Output will be a map with only level 1, obstacles, and zones");
   printf (" showing.\n");
   printf ("  Layers 2 and 3, entities, and shadows will not be included.\n");
}                               /* usage () */

END_OF_FUNCTION (void usage);


/*! \brief Perform visual affects to output PCX file according
 *         to arguments passed into main()
 *
 * \param   fname Name of the file we will open
 */
void visual_map_ex (const char *op)
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

            if (show_layer1)
               blit (icons[map[w]], bmp, 0, 0, i * 16, j * 16, 16, 16);
            if (show_layer2)
               draw_sprite (bmp, icons[b_map[w]], i * 16, j * 16);
            if (show_layer3)
               draw_sprite (bmp, icons[f_map[w]], i * 16, j * 16);
            if (show_shadows)
               draw_trans_sprite (bmp, shadow[sh_map[w]], i * 16, j * 16);

            if (show_zones) {
               if (z_map[w] > 0 && z_map[w] < MAX_ZONES) {
                  if (z_map[w] < 10) {
                     /* The zone's number is single-digit */
                     textprintf_ex (bmp, font, i * 16 + 4, j * 16 + 4,
                                    makecol (255, 255, 255), 0, "%d",
                                    z_map[w]);
                  } else if (z_map[w] < 100) {
                     /* The zone's number is double-digit */
                     textprintf_ex (bmp, font, i * 16, j * 16 + 4,
                                    makecol (255, 255, 255), 0, "%d",
                                    z_map[w]);
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

            if (show_obstacles) {
               switch (o_map[w]) {
               case 1:
                  /* Block-all: blocks movement from every direction */
                  draw_sprite (bmp, mesh, i * 16, j * 16);
                  break;
               case 2:
                  /* North-block: blocks movement up */
                  hline (bmp, i * 16, j * 16, i * 16 + 15, 255);
                  vline (bmp, i * 16 + 8, j * 16, j * 16 + 15, 255);
                  break;
               case 3:
                  /* East-block: blocks movement right */
                  hline (bmp, i * 16, j * 16 + 8, i * 16 + 15, 255);
                  vline (bmp, i * 16 + 15, j * 16, j * 16 + 15, 255);
                  break;
               case 4:
                  /* South-block: blocks movement down */
                  hline (bmp, i * 16, j * 16 + 15, i * 16 + 15, 255);
                  vline (bmp, i * 16 + 8, j * 16, j * 16 + 15, 255);
                  break;
               case 5:
                  /* West-block: blocks movement left */
                  hline (bmp, i * 16, j * 16 + 8, i * 16 + 15, 255);
                  vline (bmp, i * 16, j * 16, j * 16 + 15, 255);
                  break;
               }
            }
         }
      }

      get_palette (pal);
      save_bitmap (op, bmp, pal);
      destroy_bitmap (bmp);
   }
}                               /* visual_map () */

END_OF_FUNCTION (visual_map_ex);


int main (int argc, char *argv[])
{
   // Make sure that we have some sort of input; exit with error if not
   if (argc < 2) {
      usage (*argv);
      return 1;
   }

   char fn[PATH_MAX];
   int i, k;
   COLOR_MAP cmap;
   allegro_init ();
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;

   for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
         if (strcmp (argv[i] + 1, "-help") == 0) {
            usage (*argv);
            return 0;
         }
         /* TT: This means to exclude an effect */
         show_layer1 = strchr (argv[i] + 1, '1') ? 0 : show_layer1;
         show_layer2 = strchr (argv[i] + 1, '2') ? 0 : show_layer2;
         show_layer3 = strchr (argv[i] + 1, '3') ? 0 : show_layer3;
         show_entities = strchr (argv[i] + 1, 'e') ? 0 : show_entities;
         show_obstacles = strchr (argv[i] + 1, 'o') ? 0 : show_obstacles;
         show_shadows = strchr (argv[i] + 1, 's') ? 0 : show_shadows;
         show_zones = strchr (argv[i] + 1, 'z') ? 0 : show_zones;
      } else if (argv[i][0] == '+') {
         /* TT: This means to include an effect */
         show_layer1 = strchr (argv[i] + 1, '1') ? 1 : show_layer1;
         show_layer2 = strchr (argv[i] + 1, '2') ? 1 : show_layer2;
         show_layer3 = strchr (argv[i] + 1, '3') ? 1 : show_layer3;
         show_entities = strchr (argv[i] + 1, 'e') ? 1 : show_entities;
         show_obstacles = strchr (argv[i] + 1, 'o') ? 1 : show_obstacles;
         show_shadows = strchr (argv[i] + 1, 's') ? 1 : show_shadows;
         show_zones = strchr (argv[i] + 1, 'z') ? 1 : show_zones;
      }
   }

   for (k = 0; k < MAX_TILES; k++) {
      icons[k] = create_bitmap (16, 16);
      clear (icons[k]);
   }

   /* Shadow images */
   if (show_shadows) {
      pcx_buffer = load_pcx ("Misc.pcx", pal);
      for (k = 0; k < MAX_SHADOWS; k++) {
         shadow[k] = create_bitmap (16, 16);
         blit (pcx_buffer, shadow[k], k * 16, 160, 0, 0, 16, 16);
      }
      destroy_bitmap (pcx_buffer);
   }

   /* Obstacles */
   if (show_obstacles) {
      mesh = create_bitmap (16, 16);
      clear (mesh);
      for (i = 0; i < 16; i += 2) {
         for (k = 0; k < 16; k += 2)
            putpixel (mesh, k, i, 255);
         for (k = 1; k < 16; k += 2)
            putpixel (mesh, k, i + 1, 255);
      }
   }

   for (i = 1; i < argc; i++) {
      if (argv[i][0] != '-' && argv[i][0] != '+') {
         replace_extension (fn, argv[i], "pcx", sizeof (fn));
         load_map_batch (argv[i]);
         set_palette (pal);
         visual_map_ex (fn);
         TRACE ("%s mode %d\n", argv[i], gmap.map_mode);
      }
   }

   return 0;
}                               /* main () */

END_OF_MAIN ();
