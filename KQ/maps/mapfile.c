#include <allegro.h>
#include <stdio.h>
#include <string.h>
/*
 * This is the KQ map editor
 * (Pete's hacked version)
 * (TT's edited hacked version)
 * December 2002 (and Jan 2003)
 */
#include "mapdraw.h"
#include "disk.h"

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
   for (jy = 0; jy < gmap.ysize; jy++)
     {
        for (jx = 0; jx < gmap.xsize; jx++)
          {
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
}


/*! \brief Create a new map
 *
 * Create a new, blank map
*/
void new_map (void)
{
   /* Name of the map */
   char fname[16];
   int ld, p, q, ai = 0;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "New map", screen);
   print_sfont (6, 18, "Width: ", screen);
   ld = get_line (48, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   ai = atoi (strbuf);

   /* Make sure the value is valid */
   if (ai < 20 || ai > SW)
     {
        cmessage ("Invalid width!");
        wait_enter ();
        return;
     }

   gmap.xsize = ai;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "New map", screen);
   print_sfont (6, 18, "Height: ", screen);
   ld = get_line (54, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   ai = atoi (strbuf);

   /* Make sure the value is valid */
   if (ai < 15 || ai > SH)
     {
        cmessage ("Invalid height!");
        wait_enter ();
        return;
     }

   gmap.ysize = ai;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "New map", screen);
   print_sfont (6, 12, "Choose a tile set. ", screen);
   print_sfont (6, 18, "TileSet#: ", screen);
   ld = get_line (66, 18, fname, 40);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   gmap.tileset = atoi (fname);

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

   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
   rectfill (screen, 0, 0, 319, 17, 0);
   rect (screen, 2, 2, 317, 15, 255);
   sprintf (strbuf, "Load %s? (y/n)", fname);
   print_sfont (6, 6, strbuf, screen);

   if (yninput ())
     {
        pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
        max_sets = (pcx_buffer->h / 16);
        for (p = 0; p < max_sets; p++)
           for (q = 0; q < ICONSET_SIZE; q++)
              blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16, p * 16,
                    0, 0, 16, 16);
        icon_set = 0;
        destroy_bitmap (pcx_buffer);
        init_entities ();
     }
} /* new_map () */


/*! \brief Convert a PCX image to a map
 *
 * Take a PCX image and convert its values to make a map
*/
void make_mapfrompcx (void)
{
   char fname[16];
   int res2, ld;
   int w, h, ax, ay;
   BITMAP *pb;

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

   if (yninput ())
     {
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        rectfill (screen, 0, 0, 319, 17, 0);
        rect (screen, 2, 2, 317, 15, 255);
        print_sfont (6, 6, "Put to background? (y/n) - n=foreground",
                     screen);
        res2 = yninput ();

        pb = load_pcx (fname, pal);
        if (pb->w < gmap.xsize)
           w = pb->w;
        else
           w = gmap.xsize;

        if (pb->h < gmap.ysize)
           h = pb->h;
        else
           h = gmap.ysize;

        for (ay = 0; ay < h; ay++)
          {
             for (ax = 0; ax < w; ax++)
               {
                  if (res2)
                     map[ay * gmap.xsize + ax] = pb->line[ay][ax];
                  else
                     f_map[ay * gmap.xsize + ax] = pb->line[ay][ax];
               }
          }
        destroy_bitmap (pb);
     }
} /* make_mapfrompcx () */


/*! \brief Load a map
 *
 * A very useful function
*/
void load_map (void)
{
   char fname[16];
   int ld, p, q, i;
   PACKFILE *pf;

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
   if (strlen(fname) < 1)
     {
        strcpy(fname, map_fname);
        sprintf (strbuf, "Reload %s? (y/n)", fname);
     }
   else
      sprintf (strbuf, "Load %s? (y/n)", fname);

   print_sfont (6, 6, strbuf, screen);

   if (yninput ())
     {
        pf = pack_fopen (fname, F_READ_PACKED);
        if (!pf)
          {
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
	/*
        pack_fread (&gmap, sizeof (ss_map), pf);
        pack_fread (&gent, sizeof (s_entity) * 50, pf);
	*/
	load_s_map(&gmap, pf);
	for (i=0; i<50; ++i)
	  {
	    load_s_entity(gent+i, pf);
	  }
        bufferize ();
        pack_fread (map, (gmap.xsize * gmap.ysize * 2), pf);
        pack_fread (b_map, (gmap.xsize * gmap.ysize * 2), pf);
        pack_fread (f_map, (gmap.xsize * gmap.ysize * 2), pf);
        pack_fread (z_map, (gmap.xsize * gmap.ysize), pf);
        pack_fread (sh_map, (gmap.xsize * gmap.ysize), pf);
        pack_fread (o_map, (gmap.xsize * gmap.ysize), pf);

        pack_fclose (pf);
        pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
        max_sets = (pcx_buffer->h / 16);
        for (p = 0; p < max_sets; p++)
           for (q = 0; q < ICONSET_SIZE; q++)
              blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16, p * 16, 0, 0,
                    16, 16);
        icon_set = 0;

        /* Check for bogus map squares */
        for (p = 0; p < gmap.xsize * gmap.ysize; ++p)
          {
             /* Shadow layer */
             if (sh_map[p] >= MAX_SHADOWS)
                sh_map[p] = 0;
          }
        destroy_bitmap (pcx_buffer);
     } /* if (yninput ()) */

   /* Recount the number of entities on the map */
   noe = 0;
   for (p = 0; p < 50; p++)
      if (gent[p].active == 1)
         noe = p + 1;
} /* load_map () */


/*! \brief Save the current map
 *
 * Another very useful function
*/
void save_map (void)
{
   char fname[16];
   int ld;
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
   if (strlen(fname) < 1)
     {
        if (strlen(map_fname) < 1)
           return;
        else
           strcpy(fname, map_fname);
     }
   sprintf (strbuf, "Save %s? (y/n)", fname);
   print_sfont (6, 6, strbuf, screen);

   if (yninput ())
     {
        strcpy (map_fname, fname);
        pf = pack_fopen (fname, F_WRITE_PACKED);
        pack_fwrite (&gmap, sizeof (s_map), pf);
        pack_fwrite (&gent, sizeof (s_entity) * 50, pf);
        pack_fwrite (map, (gmap.xsize * gmap.ysize * 2), pf);
        pack_fwrite (b_map, (gmap.xsize * gmap.ysize * 2), pf);
        pack_fwrite (f_map, (gmap.xsize * gmap.ysize * 2), pf);
        pack_fwrite (z_map, (gmap.xsize * gmap.ysize), pf);
        pack_fwrite (sh_map, (gmap.xsize * gmap.ysize), pf);
        pack_fwrite (o_map, (gmap.xsize * gmap.ysize), pf);
        pack_fclose (pf);

        cmessage ("Map saved!");
        wait_enter ();
     }
} /* save_map () */
