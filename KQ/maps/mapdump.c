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

#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "mapdraw.h"
#include "../include/disk.h"

/* Something for allegro version compatibility */
/* ..can we use the textout_ex() and friends? */
#if (ALLEGRO_VERSION>=4 && ALLEGRO_SUB_VERSION>=1)
#define HAVE_TEXT_EX
#endif

/* globals */

/* Shadows, obstacles, entities */
BITMAP *shadow[MAX_SHADOWS];

/* Selectable tiles on the right-hand menu */
BITMAP *icons[MAX_TILES];

/* Used for the icons */
short max_sets = 51;

s_map gmap;
s_entity gent[50];

/* Show details when parsing MAP files */
int verbose = 0;

/* Default values, incase an option is not specified */
static int d_layer1 = 1, d_layer2 = 1, d_layer3 = 1, d_shadows = 1,
   d_zones = 0, d_obstacles = 0, d_entities = 1;
static int show_layer1, show_layer2, show_layer3, show_shadows,
   show_zones, show_obstacles, show_entities;

char *filenames[PATH_MAX];


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


/*! \brief Code shutdown and memory deallocation
 *
 * Called at the end of main(), closes everything
 */
void cleanup (void)
{
   int i;

   free (map);
   free (b_map);
   free (f_map);
   free (o_map);
   free (sh_map);
   free (z_map);

   for (i = 0; i < MAX_TILES; i++)
      destroy_bitmap (icons[i]);
   for (i = 0; i < MAX_SHADOWS; i++)
      destroy_bitmap (shadow[i]);
   destroy_bitmap (mesh);
}                               /* cleanup () */


/*! \brief Error in loading a map
 *
 * Display an error message for a file that doesn't exist.
 *
 */
void error_load (const char *problem_file)
{
   char err_msg[80];
   ASSERT (problem_file);

   strcat (strncpy (err_msg, problem_file, sizeof (err_msg) - 1), "\n");
   TRACE ("%s: could not load %s\n", allegro_error, problem_file);
   allegro_message (err_msg);
}                               /* error_load () */


/*! \brief Display help on the command syntax */
void usage (const char *argv)
{
   fprintf (stdout, "Map to PCX converter for KQ.\n");
   fprintf (stdout, "Usage: %s [+/-][options] [-v] filename(s)\n", argv);
   fprintf (stdout, "Options:\n");
   fprintf (stdout,
            "   +  includes the option: it WILL appear in the PCX image.\n");
   fprintf (stdout,
            "   -  negates an option: it will not appear in the PCX image.\n");
   fprintf (stdout, "   1  shows layer 1 (default ON)\n");
   fprintf (stdout, "   2  shows layer 2 (default ON)\n");
   fprintf (stdout, "   3  shows layer 3 (default ON)\n");
   fprintf (stdout, "   e  shows the entities (default OFF)\n");
   fprintf (stdout, "   o  shows the obstacles (default OFF)\n");
   fprintf (stdout, "   s  shows the shadows (default ON)\n");
   fprintf (stdout, "   z  shows the zones (default OFF)\n\n");
   fprintf (stdout, "   -v displays %s output in verbose mode\n\n", argv);
   fprintf (stdout, "   filename  is the .MAP file(s) to be used\n");
   fprintf (stdout, "Example: %s +1oz -23es town1.map town2.map\n\n", argv);
   fprintf (stdout,
            "  Output will be `town1.pcx' and `town2.pcx' with only level 1, obstacles,\n");
   fprintf (stdout, "    and zones showing.\n");
   fprintf (stdout,
            "  Layers 2 and 3, entities, and shadows will NOT be included.\n");
}                               /* usage () */


int main (int argc, char *argv[])
{
   char fn[PATH_MAX], *filenames[PATH_MAX];
   int i, k, number_of_files = 0, verbose = 0;
   COLOR_MAP cmap;

   /* Make sure that we have some sort of input; exit with error if not */
   if (argc == 1) {
      usage (argv[0]);
      return 0;
   }
   allegro_init ();
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;

   for (i = 1; i < argc; i++) {
      if (!strcmp (argv[i], "--help") || !strcmp (argv[i], "-h")) {
         usage (argv[0]);
         return 0;
      }
      if (!strcmp (argv[i], "-v") || !strcmp (argv[i], "--verbose"))
         verbose = 1;
   }

   show_layer1 = d_layer1;
   show_layer2 = d_layer2;
   show_layer3 = d_layer3;
   show_entities = d_entities;
   show_obstacles = d_obstacles;
   show_shadows = d_shadows;
   show_zones = d_zones;
   if (verbose)
      fprintf (stdout, "\nStarting %s...\n", argv[0]);
   for (i = 1; i < argc; i++) {
      /* Do not allow "--" options to be passed */
      if (argv[i][0] == '-' && argv[i][1] == '-') {
      } else if (argv[i][0] == '-') {
         /* This means to exclude an effect */
         show_layer1 = strchr (argv[i] + 1, '1') ? 0 : d_layer1;
         show_layer2 = strchr (argv[i] + 1, '2') ? 0 : d_layer2;
         show_layer3 = strchr (argv[i] + 1, '3') ? 0 : d_layer3;
         show_entities = strchr (argv[i] + 1, 'e') ? 0 : d_entities;
         show_obstacles = strchr (argv[i] + 1, 'o') ? 0 : d_obstacles;
         show_shadows = strchr (argv[i] + 1, 's') ? 0 : d_shadows;
         show_zones = strchr (argv[i] + 1, 'z') ? 0 : d_zones;
      } else if (argv[i][0] == '+') {
         /* This means to include an effect */
         show_layer1 = strchr (argv[i] + 1, '1') ? 1 : d_layer1;
         show_layer2 = strchr (argv[i] + 1, '2') ? 1 : d_layer2;
         show_layer3 = strchr (argv[i] + 1, '3') ? 1 : d_layer3;
         show_entities = strchr (argv[i] + 1, 'e') ? 1 : d_entities;
         show_obstacles = strchr (argv[i] + 1, 'o') ? 1 : d_obstacles;
         show_shadows = strchr (argv[i] + 1, 's') ? 1 : d_shadows;
         show_zones = strchr (argv[i] + 1, 'z') ? 1 : d_zones;
      } else {
         if (exists (argv[i]))
            filenames[number_of_files++] = argv[i];
         else
            fprintf (stderr, "Unrecognized argument: %s\n", argv[i]);
      }
   }

   for (k = 0; k < MAX_TILES; k++) {
      icons[k] = create_bitmap (16, 16);
      clear (icons[k]);
   }

   /* Shadow images */
   if (show_shadows) {
      set_pcx (&pcx_buffer, "Misc.pcx", pal, 0);
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

   for (i = 0; i < number_of_files; i++) {
      if (exists (filenames[i])) {
         if (verbose)
            fprintf (stdout, "- Loading file #%d: %s\n", i + 1,
                     (char *) filenames[i]);
         replace_extension (fn, filenames[i], "bmp", sizeof (fn));
         if (verbose)
            fprintf (stdout, "  - %s replaced by extension .PCX: %s\n",
                     filenames[i], fn);
         load_map (filenames[i]);
         if (verbose)
            fprintf (stdout, "  - Setting palette\n");
         set_palette (pal);
         if (verbose)
            fprintf (stdout, "  - Saving %s...\n", fn);
         visual_map (fn);
         if (verbose)
            fprintf (stdout, "  - \"%s\" created with mode \"%d\"\n", fn,
                     gmap.map_mode);
      }
   }

   return 0;
}                               /* main () */

END_OF_MAIN ();
