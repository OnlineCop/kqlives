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

s_anim tanim[NUM_TILESETS][MAX_ANIM];
s_anim adata[MAX_ANIM];
unsigned short tilex[MAX_TILES];

/* Show details when parsing MAP files */
int verbose = 0;

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
   free (map);
   free (b_map);
   free (f_map);
   free (o_map);
   free (sh_map);
   free (z_map);

   shared_cleanup ();
}                               /* cleanup () */


/*! \brief Error in loading a map
 *
 * Display an error message for a file that doesn't exist.
 *
 */
void error_load (const char *problem_file)
{
   ASSERT (problem_file);
   char err_msg[80];

   strcat (strncpy (err_msg, problem_file, sizeof (err_msg) - 1), "\n");
   TRACE ("%s: could not load %s\n", allegro_error, problem_file);
   allegro_message (err_msg);
}                               /* error_load () */


/*! \brief Display help on the command syntax */
void usage (const char *argv)
{
   fprintf (stdout, "Map to image converter for KQ.\n");
   fprintf (stdout, "Usage: %s [+/-][options] [-v] [-b] [-f] filename(s)\n",
            argv);
   fprintf (stdout, "Options:\n");
   fprintf (stdout,
            "   +  includes the option: it WILL appear in the image.\n");
   fprintf (stdout,
            "   -  negates an option: it WILL NOT appear in the image.\n");
   fprintf (stdout, "   1  shows layer 1 (default ON)\n");
   fprintf (stdout, "   2  shows layer 2 (default ON)\n");
   fprintf (stdout, "   3  shows layer 3 (default ON)\n");
   fprintf (stdout, "   e  shows the Entities (default OFF)\n");
   fprintf (stdout, "   o  shows the Obstacles (default OFF)\n");
   fprintf (stdout, "   s  shows the Shadows (default ON)\n");
   fprintf (stdout, "   z  shows the Zones (default OFF)\n");
   fprintf (stdout, "   m  shows the Markers (default OFF)\n\n");
   fprintf (stdout, "   -v displays %s output in verbose mode\n\n", argv);
   fprintf (stdout, "   -b output as Windows bitmap instead of PCX\n\n");
   fprintf (stdout, "   -f overwrite image, even if it already exists\n\n");
   fprintf (stdout, "   filename  is the .MAP file(s) to be used\n");
   fprintf (stdout, "Example: %s +1oz -23es town1.map town2.map\n\n", argv);
   fprintf (stdout,
            "  Output will be `town1.pcx' and `town2.pcx' with only Layer 1, Obstacles,\n");
   fprintf (stdout, "    and Zones showing.\n");
   fprintf (stdout,
            "  Layers 2 and 3, Entities, and Shadows will NOT be included.\n");
}                               /* usage () */


int main (int argc, char *argv[])
{
   char fn[PATH_MAX], *filenames[PATH_MAX];
   int i, number_of_files = 0, verbose = 0;
   int force_overwrite = 0;
   char *output_ext = "pcx";
   COLOR_MAP cmap;
   /* Regular and default values (incase an option is not specified) */
   s_show showing, d_showing;

   /* Make sure that we have some sort of input; exit with error if not */
   if (argc == 1) {
      usage (argv[0]);
      return 0;
   }

   install_allegro (SYSTEM_NONE, &errno, atexit);
   set_color_conversion (COLORCONV_TOTAL);
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;

   shared_startup ();

   /* Initialize standard "unchanged" settings */
   showing.entities = -1;
   showing.obstacles = -1;
   showing.shadows = -1;
   showing.zones = -1;
   showing.markers = -1;
   showing.layer[0] = -1;
   showing.layer[1] = -1;
   showing.layer[2] = -1;

   /* Initialize default settings */
   d_showing.entities = 1;
   d_showing.obstacles = 0;
   d_showing.shadows = 1;
   d_showing.zones = 0;
   d_showing.markers = 0;
   d_showing.last_layer = 0;
   d_showing.layer[0] = 1;
   d_showing.layer[1] = 1;
   d_showing.layer[2] = 1;

   /* Some command-line switches must have preference, so sweep twice */
   for (i = 1; i < argc; i++) {
      if (!strcmp (argv[i], "--help") || !strcmp (argv[i], "-h")) {
         usage (argv[0]);
         return 0;
      }
      if (!strcmp (argv[i], "-v") || !strcmp (argv[i], "--verbose"))
         verbose = 1;
      if (!strcmp (argv[i], "-b"))
         output_ext = "bmp";
      if (!strcmp (argv[i], "-f"))
         force_overwrite = 1;
   }

   if (verbose)
      fprintf (stdout, "\nStarting %s...\n", argv[0]);
   for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
         /* This means to exclude an effect */
         showing.entities = strchr (argv[i], 'e') ? 0 : showing.entities;
         showing.obstacles = strchr (argv[i], 'o') ? 0 : showing.obstacles;
         showing.shadows = strchr (argv[i], 's') ? 0 : showing.shadows;
         showing.zones = strchr (argv[i], 'z') ? 0 : showing.zones;
         showing.markers = strchr (argv[i], 'm') ? 0 : showing.zones;
         showing.layer[0] = strchr (argv[i], '1') ? 0 : showing.layer[0];
         showing.layer[1] = strchr (argv[i], '2') ? 0 : showing.layer[1];
         showing.layer[2] = strchr (argv[i], '3') ? 0 : showing.layer[2];
      } else if (argv[i][0] == '+') {
         /* This means to include an effect */
         showing.entities = strchr (argv[i], 'e') ? 1 : showing.entities;
         showing.obstacles = strchr (argv[i], 'o') ? 1 : showing.obstacles;
         showing.shadows = strchr (argv[i], 's') ? 1 : showing.shadows;
         showing.zones = strchr (argv[i], 'z') ? 1 : showing.zones;
         showing.markers = strchr (argv[i], 'm') ? 1 : showing.zones;
         showing.layer[0] = strchr (argv[i], '1') ? 1 : showing.layer[0];
         showing.layer[1] = strchr (argv[i], '2') ? 1 : showing.layer[1];
         showing.layer[2] = strchr (argv[i], '3') ? 1 : showing.layer[2];
      } else {
         if (exists (argv[i])) {
            if (number_of_files < PATH_MAX)
               filenames[number_of_files++] = argv[i];
            else {
               fprintf (stderr,
                        "Too many files specified on command line (max %d)\n",
                        PATH_MAX);
               return 1;
            }
         } else
            fprintf (stderr, "Unrecognized argument: %s\n", argv[i]);
      }
   }

   if (showing.entities == -1)
      showing.entities = d_showing.entities;
   if (showing.obstacles == -1)
      showing.obstacles = d_showing.obstacles;
   if (showing.shadows == -1)
      showing.shadows = d_showing.shadows;
   if (showing.zones == -1)
      showing.zones = d_showing.zones;
   if (showing.markers == -1)
      showing.markers = d_showing.markers;
   if (showing.layer[0] == -1)
      showing.layer[0] = d_showing.layer[0];
   if (showing.layer[1] == -1)
      showing.layer[1] = d_showing.layer[1];
   if (showing.layer[2] == -1)
      showing.layer[2] = d_showing.layer[2];

   if (verbose) {
      fprintf (stdout, "You have set the following: \n");
      fprintf (stdout, "- Entities: %s", showing.entities ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Obstacles: %s", showing.obstacles ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Shadows: %s", showing.shadows ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Zones: %s", showing.zones ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Markers: %s", showing.markers ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Layer1: %s", showing.layer[0] ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Layer2: %s", showing.layer[1] ? "ON\n" : "OFF\n");
      fprintf (stdout, "- Layer3: %s", showing.layer[2] ? "ON\n" : "OFF\n");
   }

   for (i = 0; i < number_of_files; i++) {
      if (exists (filenames[i])) {
         if (verbose)
            fprintf (stdout, "- Loading file #%d: %s\n", i + 1,
                     (char *) filenames[i]);
         replace_extension (fn, filenames[i], output_ext, sizeof (fn));
         if (verbose)
            fprintf (stdout, "  - %s replaced by extension .%s: %s\n",
                     filenames[i], output_ext, fn);
         load_map (filenames[i]);
         if (!exists (fn) || force_overwrite) {
            if (verbose)
               fprintf (stdout, "  - Saving %s...\n", fn);

            visual_map (showing, fn);

            if (verbose)
               fprintf (stdout, "  - \"%s\" created with mode \"%d\"\n", fn,
                        gmap.map_mode);
         } else {
            fprintf (stdout,
                     "Warning: the file %s already exists. Use the -f option to force overwrite.\n",
                     fn);
         }
      }
   }

   cleanup ();
   return 0;
}                               /* main () */

END_OF_MAIN ();
