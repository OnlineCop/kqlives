/***************************************************************************\
 * This map editor is for the KQ game, supported by the kqlives community. *
 * Comments and suggestions about the editor are welcome.  Please join our *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapshared.c contains anything that will need to be shared across more   *
 * than one instance of main(), including disk I/O, etc.                   *
\***************************************************************************/

#include <allegro.h>
#include <string.h>
#include "mapdraw.h"
#include "../include/disk.h"

int show_layer1 = 1, show_layer2 = 1, show_layer3 = 1;
int show_zones = 0, show_obstacles = 0, show_shadows = 1;

char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "Incave.pcx", "village.pcx", "mount.pcx"
};


/*! \brief Load a map
 *
 * \param   filename File to load
 */
void load_map (const char *filename)
{
   int p, q, i;
   PACKFILE *pf;
   char load_fname[PATH_MAX];

   ASSERT (load_fname);
   strcpy (load_fname, filename);

   if (!exists (load_fname)) {
      replace_extension (load_fname, filename, "map", sizeof (load_fname));
      if (!exists (load_fname)) {
         error_load (load_fname);
         return;
      }
   }

   pf = pack_fopen (load_fname, F_READ_PACKED);
   if (!pf) {
      error_load (load_fname);
      return;
   }

   strcpy (map_fname, load_fname);
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

   set_pcx (&pcx_buffer, icon_files[gmap.tileset], pal, 1);
   max_sets = (pcx_buffer->h / 16);

   for (q = 0; q < max_sets; q++)
      for (p = 0; p < ICONSET_SIZE; p++)
         blit (pcx_buffer, icons[q * ICONSET_SIZE + p], p * 16, q * 16, 0, 0,
               16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);

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

      /* Obstacles layer */
      if (o_map[p] > MAX_OBSTACLES)
         o_map[p] = 0;

      /* Shadow layer */
      if (sh_map[p] >= MAX_SHADOWS)
         sh_map[p] = 0;

      /* Zone layer */
      if (z_map[p] > MAX_ZONES)
         z_map[p] = 0;
   }
}                               /* load_map () */


/*! \brief Check to see if a requested PCX file is available
 *
 * Another very useful function
 *
 * /param   pcx_buf Buffer to save the PCX image back into
 * /param   pcx_file PCX file to load
 * /param   pcx_pal Palette to set the image to
 * /param   critical If file cannot be found, exit the program with an error
 */
void set_pcx (BITMAP **pcx_buf, const char *pcx_file, PALETTE pcx_pal, const int critical)
{
   if (exists (pcx_file))
      *pcx_buf = load_pcx (pcx_file, pcx_pal);
   else {
      sprintf (strbuf, "PCX image could not be found: %s", pcx_file);
      allegro_message (strbuf);
      if (critical) {
         /* This means that this file is critical to the program, so we need to
          * exit the program completely, as this cannot be recovered from.
          */
         cleanup ();
         exit (EXIT_FAILURE);
      }
   }
}                               /* set_pcx () */


/*! \brief Save the whole map as a pcx
 *
 * Make one giant bitmap and draw all the layers on it, so you can get an
 * overview of what's going on.  Doesn't draw entities, and doesn't work well
 * with parallax (any ideas how to handle parallax?).
 * \author PH
 * \date 20030412
 * \updated 20040730 by TT
 *
 * \param   save_fname File to save the map to
 */
void visual_map (const char *save_fname)
{
   int i, j, w;
   BITMAP *bmp;

   /* Create a bitmap the same size as the map */
   bmp = create_bitmap (gmap.xsize * 16, gmap.ysize * 16);
   clear (bmp);

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

         if ((show_zones) && (z_map[w] > 0) && (z_map[w] < MAX_ZONES)) {
/* This check is here because of the differing versions of the Allegro library */
#ifdef HAVE_TEXT_EX
            if (z_map[w] < 10) {
               /* The zone's number is single-digit, center vert+horiz */
               textprintf_ex (bmp, font, i * 16 + 4, j * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            } else if (z_map[w] < 100) {
               /* The zone's number is double-digit, center only vert */
               textprintf_ex (bmp, font, i * 16, j * 16 + 4,
                              makecol (255, 255, 255), 0, "%d", z_map[w]);
            } else if (z_map[w] < 1000) {
               /* The zone's number is triple-digit.  Print the 100's digit in
                * top-center of the square; the 10's and 1's digits on bottom
                * of the square
                */
               textprintf_ex (bmp, font, i * 16 + 4, j * 16,
                              makecol (255, 255, 255), 0, "%d",
                              (int) (z_map[w] / 100));
               textprintf_ex (bmp, font, i * 16, j * 16 + 8,
                              makecol (255, 255, 255), 0, "%02d",
                              (int) (z_map[w] % 100));
            }
#else
            if (z_map[w] < 10) {
               /* The zone's number is single-digit, center vert+horiz */
               textprintf (bmp, font, i * 16 + 4, j * 16 + 4,
                           makecol (255, 255, 255), "%d", z_map[w]);
            } else if (z_map[w] < 100) {
               /* The zone's number is double-digit, center only vert */
               textprintf (bmp, font, i * 16, j * 16 + 4,
                           makecol (255, 255, 255), "%d", z_map[w]);
            } else if (z_map[w] < 1000) {
               /* The zone's number is triple-digit.  Print the 100's digit in
                * top-center of the square; the 10's and 1's digits on bottom
                * of the square
                */
               textprintf (bmp, font, i * 16 + 4, j * 16,
                           makecol (255, 255, 255), "%d",
                           (int) (z_map[w] / 100));
               textprintf (bmp, font, i * 16, j * 16 + 8,
                           makecol (255, 255, 255), "%02d",
                           (int) (z_map[w] % 100));
            }
#endif
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
   save_bitmap (save_fname, bmp, pal);
   destroy_bitmap (bmp);
}                               /* visual_map () */
