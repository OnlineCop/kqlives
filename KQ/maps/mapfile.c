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
void error_load (AL_CONST char *fname)
{
   ASSERT (fname);
   rectfill (screen, 0, 0, 319, 24, 0);
   rect (screen, 2, 2, 317, 22, 255);
   sprintf (strbuf, "Could not load \"%s\"", fname);
   print_sfont (6, 6, strbuf, screen);
   sprintf (strbuf, "[press enter]");
   print_sfont (6, 14, strbuf, screen);
   wait_enter ();
   return;
}                               /* error_load () */

END_OF_FUNCTION (error_load);


/*! \brief Load a map
 *
 * A very useful function
 */
void load_map (char *fname)
{
   int p, q, i;
   PACKFILE *pf;

   ASSERT (fname);
   pf = pack_fopen (fname, F_READ_PACKED);
   if (!pf) {
      error_load (fname);
      return;
   }

   strcpy (map_fname, fname);
   load_s_map (&gmap, pf);

   for (i = 0; i < 50; ++i)
      load_s_entity (gent + i, pf);

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

   if (!exists (icon_files[gmap.tileset])) {
      error_load (icon_files[gmap.tileset]);
      sprintf (strbuf, "%s could not be loaded!", icon_files[gmap.tileset]);
      allegro_message (strbuf);
      cleanup ();
      exit (EXIT_FAILURE);
   }

   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);

   for (q = 0; q < max_sets; q++)
      for (p = 0; p < ICONSET_SIZE; p++)
         blit (pcx_buffer, icons[q * ICONSET_SIZE + p], p * 16, q * 16, 0, 0,
               16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);

   /* Check for bogus map squares */
   for (p = 0; p < gmap.xsize * gmap.ysize; ++p) {
      /* Shadow layer */
      if (sh_map[p] >= MAX_SHADOWS)
         sh_map[p] = 0;
      /* Background layer */
      if (b_map[p] > MAX_TILES)
         b_map[p] = 0;
   }

   /* Recount the number of entities on the map */
   number_of_ents = 0;

   for (p = 0; p < 50; p++)
      if (gent[p].active == 1)
         number_of_ents = p + 1;

   return;
}                               /* load_map () */

END_OF_FUNCTION (load_map);


/*! \brief Convert a PCX image to a map
 *
 * Take a PCX image and convert its values to make a map
 * 20040129 PH improved so you can reload pcx onto any layer
 *          used to be only background or foreground
 */
void make_mapfrompcx (void)
{
   char fname[16];
   char imp[16];
   int response, done;
   int w, h, ax, ay;
   BITMAP *pcx_bitmap;
   short *tm;

   draw_map ();
   rectfill (double_buffer, 0, 0, 319, 21, 0);
   rect (double_buffer, 2, 2, 317, 19, 255);
   print_sfont (6, 6, "Make map from pcx", double_buffer);
   print_sfont (6, 12, "Filename: ", double_buffer);

   done = 0;
   while (!done) {
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      response = get_line (66, 12, fname, 40);

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         if (!exists (fname)) {
            error_load (fname);
            return;
            /* TT TODO: move the "Load %s?" (below) here, so the user can type
             * it in correctly this time.
             */
         }
         done = 1;
      }
   }

   draw_map ();
   rectfill (double_buffer, 0, 0, 319, 15, 0);
   rect (double_buffer, 2, 2, 317, 13, 255);
   sprintf (strbuf, "Load %s? (y/n)", fname);
   print_sfont (6, 6, strbuf, screen);

   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   if (!yninput ())
      return;

   draw_map ();
   rectfill (double_buffer, 0, 0, 319, 21, 0);
   rect (double_buffer, 2, 2, 317, 19, 255);
   print_sfont (6, 6, "Put to (B)ack (M)id (F)ore?", double_buffer);

   done = 0;
   while (!done) {
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      tm = NULL;

      response = get_line (6, 12, imp, sizeof (imp));

      /* If the user hits ESC, break out of the function entirely */
      if (response == 0)
         return;

      /* Make sure this line isn't blank */
      if (strlen (strbuf) > 0) {
         if (*imp == 'm' || *imp == 'M') {
            tm = b_map;
            done = 1;
         }
         if (*imp == 'b' || *imp == 'B') {
            tm = map;
            done = 1;
         }
         if (*imp == 'f' || *imp == 'F') {
            tm = f_map;
            done = 1;
         }
      }

      if (!exists (fname)) {
         error_load (fname);
         return;
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

END_OF_FUNCTION (make_mapfrompcx);


/*! \brief Convert map to PCX images
 *
 * Convert the current map into several mini PCX images
 */
void maptopcx (void)
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
         pcx_foreground->line[jy][jx] = b_map[jy * gmap.xsize + jx];
         pcx_foreground->line[jy][jx] = f_map[jy * gmap.xsize + jx];
      }
   }
   save_pcx ("mdback.pcx", pcx_background, pal);
   save_pcx ("mdmid.pcx", pcx_foreground, pal);
   save_pcx ("mdfore.pcx", pcx_foreground, pal);
   destroy_bitmap (pcx_background);
   destroy_bitmap (pcx_foreground);
   destroy_bitmap (pcx_foreground);
}                               /* maptopcx () */

END_OF_FUNCTION (maptopcx);


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
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
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
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
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
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
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
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);

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
   gmap.extra_sdword1 = 0;
   gmap.extra_sdword2 = 0;
   gmap.song_file[0] = 0;
   gmap.map_desc[0] = 0;

   bufferize ();
   update_tileset ();
   init_entities ();
}                               /* new_map () */

END_OF_FUNCTION (new_map);


/*! \brief Confirm before loading a map
 *
 *
 */
void prompt_load_map (void)
{
   char fname[16];
   int ld;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Load a map", screen);
   sprintf (strbuf, "Current: %s", map_fname);
   print_sfont (6, 12, strbuf, screen);
   print_sfont (6, 18, "Filename: ", screen);
   ld = get_line (66, 18, fname, 40);

   /* Return if the user hit ESC to cancel the dialog box */
   if (ld == 0)
      return;

   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rectfill (screen, 0, 0, 319, 16, 0);
   rect (screen, 2, 2, 317, 14, 255);

   /* If the line was blank, simply reload the current map */
   if (strlen (fname) < 1) {
      strcpy (fname, map_fname);
      if (exists (fname))
         sprintf (strbuf, "Reload %s? (y/n)", fname);
      else {
         error_load (fname);
         return;
      }
   } else {
      if (exists (fname))
         sprintf (strbuf, "Load %s? (y/n)", fname);
      else {
         error_load (fname);
         return;
      }
   }

   print_sfont (6, 6, strbuf, screen);

   if (yninput ()) {
      load_map (fname);
   }                            // if (yninput ())

   return;
}                               /* prompt_load_map () */

END_OF_FUNCTION (prompt_load_map);


/*! \brief Save the current map
 *
 * Another very useful function
 */
void save_map (void)
{
   char fname[16];
   int ld, p, q;
   PACKFILE *pf;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Save a map", screen);
   sprintf (strbuf, "Current: %s", map_fname);
   print_sfont (6, 12, strbuf, screen);
   print_sfont (6, 18, "Filename: ", screen);
   ld = get_line (66, 18, fname, 40);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rectfill (screen, 0, 0, 319, 17, 0);
   rect (screen, 2, 2, 317, 15, 255);

   /* If the line WAS blank, simply save as current map's name */
   if (strlen (fname) < 1) {
      if (strlen (map_fname) < 1)
         return;
      else
         strcpy (fname, map_fname);
   }
   sprintf (strbuf, "Save %s? (y/n)", fname);
   print_sfont (6, 6, strbuf, screen);

   if (yninput ()) {
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
   }
}                               /* save_map () */

END_OF_FUNCTION (save_map);


/*! \brief Check to see if a requested PCX file is available
 *
 * Another very useful function
 * 
 * /param   pcx_buf Buffer to save the PCX image back into
 * /param   pcx_file PCX file to load
 * /param   pcx_pal Palette to set the image to
 * /param   critical If file cannot be found, exit the program with an error
 */
void set_pcx (BITMAP **pcx_buf, char *pcx_file, PALETTE pcx_pal, int critical)
{
   if (exists (pcx_file))
      *pcx_buf = load_pcx (pcx_file, pcx_pal);
   else if (critical) {
      /* This means that this file is critical to the program, so we need to
       * exit the program completely, as this cannot be recovered from.
       */
      sprintf (strbuf, "PCX image could not be found: %s", pcx_file);
      allegro_message (strbuf);
      cleanup ();
      exit (EXIT_FAILURE);
   }
   return;
}                               /* set_pcx () */

END_OF_FUNCTION (set_pcx)
