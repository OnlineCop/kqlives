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

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rectfill (screen, 0, 0, 319, 16, 0);
   rect (screen, 2, 2, 317, 14, 255);

   /* If the line WAS blank, simply reload the current map */
   if (strlen (fname) < 1) {
      strcpy (fname, map_fname);
      sprintf (strbuf, "Reload %s? (y/n)", fname);
   } else
      sprintf (strbuf, "Load %s? (y/n)", fname);

   print_sfont (6, 6, strbuf, screen);

   if (yninput ()) {
      load_map (fname);
   }  /* if (yninput ()) */

   return;
}                               /* prompt_load_map () */


/*! \brief Load a map
 *
 * A very useful function
 */
void load_map (char *fname)
{
   int p, q, i;
   PACKFILE *pf;

   pf = pack_fopen (fname, F_READ_PACKED);
   if (!pf) {
      rectfill (screen, 0, 0, 319, 24, 0);
      rect (screen, 2, 2, 317, 22, 255);
      sprintf (strbuf, "Could not load \"%s\"", fname);
      print_sfont (6, 6, strbuf, screen);
      sprintf (strbuf, "[press enter]");
      print_sfont (6, 14, strbuf, screen);
      wait_enter ();
      return;
   }

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
         blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16,
               p * 16, 0, 0, 16, 16);
      }
   }
   icon_set = 0;

   /* Check for bogus map squares */
   for (p = 0; p < gmap.xsize * gmap.ysize; ++p) {
      /* Shadow layer */
      if (sh_map[p] >= MAX_SHADOWS)
         sh_map[p] = 0;
      /* Background layer */
      if (b_map[p] > MAX_TILES)
         b_map[p] = 0;
   }
   destroy_bitmap (pcx_buffer);

   /* Recount the number of entities on the map */
   number_of_ents = 0;

   for (p = 0; p < 50; p++)
      if (gent[p].active == 1)
         number_of_ents = p + 1;

   return;
}                               /* load_map () */


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
   int  ld;
   int w, h, ax, ay;
   BITMAP *pb;
   short* tm;

   rectfill (screen, 0, 0, 319, 21, 0);
   rect (screen, 2, 2, 317, 19, 255);
   print_sfont (6, 6, "Make map from pcx", screen);
   print_sfont (6, 12, "Filename: ", screen);
   ld = get_line (66, 12, fname, 40);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rectfill (screen, 0, 0, 319, 17, 0);
   rect (screen, 2, 2, 317, 15, 255);
   sprintf (strbuf, "Load %s? (y/n)", fname);
   print_sfont (6, 6, strbuf, screen);

   if (yninput ()) {
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
      tm=NULL;
      do {
	rectfill (screen, 0, 0, 319, 27, 0);
	rect (screen, 2, 2, 317, 25, 255);
	print_sfont (6, 6, "Put to (B)ack (M)id (F)ore?", screen);
	if (get_line(6,16,imp,sizeof(imp))>0) {
	  switch(*imp) {
	  case 'm': case 'M':
	    tm=b_map;
	      break;
	  case 'b': case 'B':
	    tm=map;
	      break;
	  case 'f': case 'F':
	    tm=f_map;
	      break;
	  }
	}
      } while (map==NULL);

      pb = load_bitmap (fname, pal);
      if (pb->w < gmap.xsize)
         w = pb->w;
      else
         w = gmap.xsize;

      if (pb->h < gmap.ysize)
         h = pb->h;
      else
         h = gmap.ysize;

      for (ay = 0; ay < h; ay++) {
         for (ax = 0; ax < w; ax++) {
               tm[ay * gmap.xsize + ax] = pb->line[ay][ax];
         }
      }
      destroy_bitmap (pb);
   }
}                               /* make_mapfrompcx () */


/*! \brief Convert map to PCX images
 *
 * Convert the current map into several mini PCX images
 */
void maptopcx (void)
{
   /* Foreground, middle, and background */
   BITMAP *pf, *pm, *pb;
   int jx, jy;

   /* Background PCX image */
   pb = create_bitmap (gmap.xsize, gmap.ysize);
   /* Middle PCX image */
   pm = create_bitmap (gmap.xsize, gmap.ysize);
   /* Foreground PCX image */
   pf = create_bitmap (gmap.xsize, gmap.ysize);
   for (jy = 0; jy < gmap.ysize; jy++) {
      for (jx = 0; jx < gmap.xsize; jx++) {
         pb->line[jy][jx] = map[jy * gmap.xsize + jx];
         pm->line[jy][jx] = b_map[jy * gmap.xsize + jx];
         pf->line[jy][jx] = f_map[jy * gmap.xsize + jx];
      }
   }
   save_pcx ("mdback.pcx", pb, pal);
   save_pcx ("mdmid.pcx", pm, pal);
   save_pcx ("mdfore.pcx", pf, pal);
   destroy_bitmap (pb);
   destroy_bitmap (pm);
   destroy_bitmap (pf);
}                               /* maptopcx () */


/*! \brief Create a new map
 *
 * Create a new, blank map
 */
void new_map (void)
{
   /* Name of the map */
   int response;
   int new_height = 0, new_width = 0, new_tileset = 0;
   int p, q;

   cmessage("Do you want to create a new map?");
   if (!yninput ())
      return;

   // Redraw the screen, blank
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);

   rect (screen, 2, 2, 317, 43, 255);
   rectfill (screen, 3, 3, 316, 42, 0);
   print_sfont (6, 6, "New map", screen);

   print_sfont (6, 18, "Width: ", screen);
   response = get_line (48, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (response == 0)
      return;

   new_width = atoi (strbuf);

   /* Make sure the value is valid */
   if (new_width < 20 || new_width > SW) {
      cmessage ("Invalid width!");
      wait_enter ();
      return;
   }

   print_sfont (6, 26, "Height: ", screen);
   response = get_line (54, 26, strbuf, 4);

   /* Make sure the line isn't blank */
   if (response == 0)
      return;

   new_height = atoi (strbuf);

   /* Make sure the value is valid */
   if (new_height < 15 || new_height > SH) {
      cmessage ("Invalid height!");
      wait_enter ();
      return;
   }

   int loop = 1;
   while (loop) {

      rect (screen, 2, 2, 317, 43, 255);
      rectfill (screen, 3, 3, 316, 42, 0);
      print_sfont (6, 6, "New map", screen);

      sprintf(strbuf, "Width: %d", new_width);
      print_sfont (6, 18, strbuf, screen);
      sprintf(strbuf, "Height: %d", new_height);
      print_sfont (6, 26, strbuf, screen);

      rectfill (screen, 3, 33, 316, 39, 0);
      print_sfont (6, 34, "Choose a tile set (0-5): ", screen);
      response = get_line (156, 34, strbuf, 2);

      /* Make sure the line isn't blank */
      if (response == 0)
         return;

      new_tileset = atoi (strbuf);

      if (new_tileset > 5) {
         cmessage ("Invalid tileset!");
         wait_enter ();
      } else {
         sprintf (strbuf, "Load %s? (y/n)", icon_files[new_tileset]);
         cmessage (strbuf);
         if (yninput ()) {
            loop = 0;
         }
      }

      // Redraw the screen, blank
      blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);

   }

   gmap.tileset = new_tileset;
   gmap.xsize = new_width;
   gmap.ysize = new_height;

   /* Default values for a map */
   gmap.map_no = 0;
   gmap.zero_zone = 0;
   gmap.map_mode = 0;
   gmap.can_save = 1;
   gmap.use_sstone = 1;
   gmap.pmult = 1;
   gmap.pdiv = 1;
   gmap.song_file[0] = 0;
   gmap.map_desc[0] = 0;
   gmap.stx = 0;
   gmap.sty = 0;

   bufferize ();

   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);
   for (p = 0; p < max_sets; p++)
      for (q = 0; q < ICONSET_SIZE; q++)
         blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16, p * 16,
               0, 0, 16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
   init_entities ();

/* TT TODO:
 * We need to clear 'map_fname'
 */
   //*map_fname = "";
}                               /* new_map () */


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
