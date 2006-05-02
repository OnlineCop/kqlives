/***************************************************************************\
 * This map editor is for the KQ game, supported by the kqlives community. *
 * Comments and suggestions about the editor are welcome.  Please join our *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapfile.c is the go-between for any disk accessing that needs to take   *
 * place.  All routines that rely on accessing files or directories should *
 * be contained in this file.                                              *
\***************************************************************************/


#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include "mapdraw.h"
#include "../include/disk.h"


/*! \brief Error in loading a map
 *
 * Display an error message for a file that doesn't exist.
 *
 */
void error_load (const char *problem_file)
{
   int i = 0;
   ASSERT (problem_file);

   sprintf (strbuf, "Could not load \"%s\"", problem_file);
   draw_map ();
   blit2screen ();

   i = (strlen (strbuf) + 1) * 6;
   rectfill (screen, 0, 0, i + 4, 22, 0);
   rect (screen, 2, 2, i + 2, 20, 255);
   print_sfont (6, 6, strbuf, screen);
   sprintf (strbuf, "[press enter]");
   print_sfont (6, 12, strbuf, screen);
   wait_enter ();
}                               /* error_load () */


/*! \brief Convert a PCX image to a map
 *
 * Take a PCX image and convert its values to make a map
 * 20040129 PH improved so you can reload pcx onto any layer
 *          used to be only background or foreground
 */
void make_mapfrompcx (void)
{
   /* TT TODO: This is still very buggy, as the PCX image only has 255 values;
    * the particular tile on the map may actually exceed this; when we import
    * the PCX image, anything over the 255 boundary are mistaken for <= 255
    */
   char fname[40];
   int response, destination, done;
   int w, h, ax, ay;
   BITMAP *pcx_bitmap;
   unsigned short *tm;

   draw_map ();
   rectfill (double_buffer, 0, 0, 238, 29, 0);
   rect (double_buffer, 2, 2, 236, 27, 255);
   print_sfont (6, 6, "Make map from pcx", double_buffer);
   print_sfont (6, 12, "Filename: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (66, 12, fname, 40);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (fname) > 0) {
         if (!exists (fname)) {
            replace_extension (fname, fname, "pcx", sizeof (fname));
            if (!exists (fname)) {
               error_load (fname);
               return;
            }
         }

         sprintf (strbuf, "Load %s? (y/n)", fname);
         print_sfont (6, 18, strbuf, screen);
         if (yninput ())
            done = 1;
      }
   }

   draw_map ();
   rectfill (double_buffer, 0, 0, 238, 29, 0);
   rect (double_buffer, 2, 2, 236, 27, 255);
   print_sfont (6, 6, "Which layer do you want to put it to:", double_buffer);
   print_sfont (6, 12, "(1) (2) or (3)?", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      tm = NULL;

      response = get_line (102, 12, strbuf, 2);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         destination = atoi (strbuf);
         if (destination == 1) {
            tm = map;
            done = 1;
         } else if (destination == 2) {
            tm = b_map;
            done = 1;
         } else if (destination == 3) {
            tm = f_map;
            done = 1;
         }
      }
   }

   pcx_bitmap = load_bitmap (fname, pal);
   if (pcx_bitmap->w < gmap.xsize)
      w = pcx_bitmap->w;
   else
      w = gmap.xsize;

   if (pcx_bitmap->h < gmap.ysize)
      h = pcx_bitmap->h;
   else
      h = gmap.ysize;

   for (ay = 0; ay < h; ay++) {
      for (ax = 0; ax < w; ax++) {
         tm[ay * gmap.xsize + ax] = pcx_bitmap->line[ay][ax];
      }
   }
   destroy_bitmap (pcx_bitmap);
}                               /* make_mapfrompcx () */


/*! \brief Convert map to PCX images
 *
 * Convert the current map into several mini PCX images
 *
 * \param   format - Whether a PCX or BMP is desired for output
 *                   1: BMP
 *                   2: PCX
 */
void maptopcx (int format)
{
   /* Foreground, middle, and background */
   BITMAP *pcx_foreground, *pcx_middleground, *pcx_background;
   int jx, jy;

   /* Background PCX image */
   pcx_background = create_bitmap (gmap.xsize, gmap.ysize);
   /* Middle PCX image */
   pcx_middleground = create_bitmap (gmap.xsize, gmap.ysize);
   /* Foreground PCX image */
   pcx_foreground = create_bitmap (gmap.xsize, gmap.ysize);
   for (jy = 0; jy < gmap.ysize; jy++) {
      for (jx = 0; jx < gmap.xsize; jx++) {
         pcx_background->line[jy][jx] = map[jy * gmap.xsize + jx];
         pcx_middleground->line[jy][jx] = b_map[jy * gmap.xsize + jx];
         pcx_foreground->line[jy][jx] = f_map[jy * gmap.xsize + jx];
      }
   }

   if (format == 1)
   {
      save_pcx ("mdback.bmp", pcx_background, pal);
      save_pcx ("mdmid.bmp", pcx_middleground, pal);
      save_pcx ("mdfore.bmp", pcx_foreground, pal);

      /* Confirmation message */
      cmessage ("Saved to BMP!");
      wait_enter ();
   }
   else if (format == 2)
   {
      save_pcx ("mdback.pcx", pcx_background, pal);
      save_pcx ("mdmid.pcx", pcx_middleground, pal);
      save_pcx ("mdfore.pcx", pcx_foreground, pal);

      /* Confirmation message */
      cmessage ("Saved to PCX!");
      wait_enter ();
   }

   destroy_bitmap (pcx_background);
   destroy_bitmap (pcx_middleground);
   destroy_bitmap (pcx_foreground);
}                               /* maptopcx () */


/*! \brief Create a new map
 *
 * Create a new, blank map
 */
void new_map (void)
{
   int response;
   int new_height = 0, new_width = 0, new_tileset = 0;
   int done;

   cmessage ("Do you want to create a new map?");
   if (!yninput ())
      return;

   draw_map ();
   rect (double_buffer, 2, 2, 317, 43, 255);
   rectfill (double_buffer, 3, 3, 316, 42, 0);
   print_sfont (6, 6, "New map", double_buffer);
   print_sfont (6, 18, "Width: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (48, 18, strbuf, 4);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         new_width = atoi (strbuf);

         /* Make sure the value is valid */
         if (new_width < 20 || new_width > SW) {
            cmessage ("Invalid width!");
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }
   /* This is so incase we need to redraw the map, it'll have the information
    * that the user passed into it
    */
   sprintf (strbuf, "%d", new_width);
   print_sfont (48, 18, strbuf, double_buffer);
   print_sfont (6, 26, "Height: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (54, 26, strbuf, 4);

      /* If the user hits ESC, break out of the new_map() loop entirely */
      if (response == 0)
         return;

      /* Make sure the line isn't blank */
      if (strlen (strbuf) > 0) {
         new_height = atoi (strbuf);

         /* Make sure the value is valid */
         if (new_height < 15 || new_height > SH) {
            cmessage ("Invalid height!");
            wait_enter ();
         } else {
            done = 1;
         }
      }
   }
   /* This is incase we need to redraw the map, the information will still be
    * visible to the user
    */
   sprintf (strbuf, "%d", new_height);
   print_sfont (54, 26, strbuf, double_buffer);
   sprintf (strbuf, "Choose a tile set (0-%d):", NUM_TILESETS - 1);
   print_sfont (6, 34, strbuf, double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (156, 34, strbuf, 2);

      /* If the user hits ESC, break out of the new_map() loop entirely */
      if (response == 0)
         return;

      /* Make sure the line isn't blank */
      if (strlen (strbuf) > 0) {
         new_tileset = atoi (strbuf);

         /* Make sure the value is valid */
         if (new_tileset < 0 || new_tileset > NUM_TILESETS - 1) {
            /* TT TODO: The only bug I see so far is if the user enters a "bad"
             * value, such as "Q" or "-"... it thinks the user entered "0"
             */
            cmessage ("Invalid tileset!");
            wait_enter ();
         } else {
            sprintf (strbuf, "Load %s? (y/n)", icon_files[new_tileset]);
            cmessage (strbuf);
            if (yninput ()) {
               done = 1;
            }
         }
      }
   }

   /* Redraw the screen, blank */
   blit2screen ();

   /* Default values for the new map */
   gmap.map_no = 0;
   gmap.zero_zone = 0;
   gmap.map_mode = 0;
   gmap.can_save = 0;
   gmap.tileset = new_tileset;
   gmap.use_sstone = 1;
   gmap.can_warp = 0;
   gmap.extra_byte = 0;
   gmap.xsize = new_width;
   gmap.ysize = new_height;
   gmap.pmult = 1;
   gmap.pdiv = 1;
   gmap.stx = 0;
   gmap.sty = 0;
   gmap.warpx = 0;
   gmap.warpy = 0;
   gmap.revision = 1;
   gmap.extra_sdword2 = 0;
   gmap.song_file[0] = 0;
   gmap.map_desc[0] = 0;
   gmap.num_markers = 0;
   gmap.markers = NULL;
   bufferize ();
   update_tileset ();
   init_entities ();
}                               /* new_map () */


/*! \brief Confirm before loading a map
 *
 *
 */
void prompt_load_map (void)
{
   char fname[40];
   int response, done, i;

   make_rect (double_buffer, 3, 50);
   print_sfont (6, 6, "Load a map", double_buffer);
   sprintf (strbuf, "Current: %s", map_fname);
   print_sfont (6, 12, strbuf, double_buffer);
   print_sfont (6, 18, "Filename: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (66, 18, fname, sizeof (fname));

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* If the line was blank, simply reload the current map */
      if (strlen (fname) < 1) {
         strcpy (fname, map_fname);

         if (!exists (fname)) {
            replace_extension (fname, fname, "map", sizeof (fname));
            if (!exists (fname)) {
               error_load (fname);
               return;
            }
         }
         sprintf (strbuf, "Reload %s? (y/n)", fname);
      } else {
         if (!exists (fname)) {
            replace_extension (fname, fname, "map", sizeof (fname));
            if (!exists (fname)) {
               error_load (fname);
               return;
            }
         }
         sprintf (strbuf, "Load %s? (y/n)", fname);
      }

      cmessage (strbuf);

      if (yninput ()) {
         draw_map ();
         load_map (fname);

         /* Recount the number of entities on the map */
         number_of_ents = 0;

         for (i = 0; i < 50; i++) {
            if (gent[i].active == 1)
               number_of_ents = i + 1;
         }
         done = 1;
      }                         /* if (yninput ()) */
   }
}                               /* prompt_load_map () */


/*! \brief Save the current map
 *
 * Another very useful function
 */
void save_map (void)
{
   char fname[40];
   int response, done;
   int p, q;
   PACKFILE *pf;

   make_rect (double_buffer, 3, 49);
   print_sfont (6, 6, "Save a map", double_buffer);
   sprintf (strbuf, "Current: %s", map_fname);
   print_sfont (6, 12, strbuf, double_buffer);
   print_sfont (6, 18, "Filename: ", double_buffer);

   done = 0;
   while (!done) {
      blit2screen ();
      response = get_line (66, 18, fname, sizeof (fname));

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* If the line WAS blank, simply save as current map's name */
      if (strlen (fname) < 1) {
         if (strlen (map_fname) < 1)
            return;
         else
            strcpy (fname, map_fname);
      }

      replace_extension (fname, fname, "map", sizeof (fname));

      sprintf (strbuf, "Filename: %s", fname);
      print_sfont (6, 18, strbuf, double_buffer);

      sprintf (strbuf, "Save %s? (y/n)", fname);
      cmessage (strbuf);

      if (yninput ())
         done = 1;
   }

   strcpy (map_fname, fname);
   pf = pack_fopen (fname, F_WRITE_PACKED);
   save_s_map (&gmap, pf);

   for (q = 0; q < 50; ++q) {
      save_s_entity (&gent[q], pf);
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         pack_iputw (map[q * gmap.xsize + p], pf);
      }
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         pack_iputw (b_map[q * gmap.xsize + p], pf);
      }
   }
   for (q = 0; q < gmap.ysize; ++q) {
      for (p = 0; p < gmap.xsize; ++p) {
         pack_iputw (f_map[q * gmap.xsize + p], pf);
      }
   }

   pack_fwrite (z_map, (gmap.xsize * gmap.ysize), pf);
   pack_fwrite (sh_map, (gmap.xsize * gmap.ysize), pf);
   pack_fwrite (o_map, (gmap.xsize * gmap.ysize), pf);
   pack_fclose (pf);

   cmessage ("Map saved!");
   wait_enter ();
}                               /* save_map () */
