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

#include <string.h>
#include <allegro.h>
#include "mapdraw.h"
#include "../include/disk.h"


/* globals */

/* Image and screen buffers */
BITMAP *pcx_buffer;

/* This is for a shadow.  It's dark. */
BITMAP *shadow[MAX_SHADOWS];

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

char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "Incave.pcx", "village.pcx", "mount.pcx"
};

PALETTE pal;


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
         blit (pcx_buffer, icons[p * ICONSET_SIZE + q], q * 16,
               p * 16, 0, 0, 16, 16);
      }
   }

   /* Check for bogus map squares */
   for (p = 0; p < gmap.xsize * gmap.ysize; ++p) {
      /* Shadow layer */
      if (sh_map[p] >= MAX_SHADOWS)
         sh_map[p] = 0;
      /* Background layer */
      if (b_map[p] > MAX_TILES)
         b_map[p] = 0;
      /* Zone layer */
      if (z_map[p] > MAX_ZONES)
         z_map[p] = 0;
   }
   destroy_bitmap (pcx_buffer);

   return;

}                               /* load_map () */


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

            blit (icons[map[w]], bmp, 0, 0, i * 16, j * 16, 16, 16);
            draw_sprite (bmp, icons[b_map[w]], i * 16, j * 16);
            draw_sprite (bmp, icons[f_map[w]], i * 16, j * 16);
            draw_trans_sprite (bmp, shadow[sh_map[w]], i * 16, j * 16);

            /* TT: Modified, because otherwise, we will only show one
             *     number for each zone, regardless of how many "Zone #1"
             *     we may have on an actual map.
             */
            if (z_map[w] > 0 && z_map[w] < MAX_ZONES) {
               if (z_map[w] < 10) {
                  /* The zone's number is single-digit */
                  textprintf (bmp, font, i * 16 + 4, j * 16 + 4, makecol (255, 255, 255), "%d", z_map[w]);
               } else if (z_map[w] < 100) {
                  /* The zone's number is double-digit */
                  textprintf (bmp, font, i * 16, j * 16 + 4, makecol (255, 255, 255), "%d", z_map[w]);
               } else if (i < 1000) {
                  /* The zone's number is triple-digit */
                  textprintf (bmp, font, i * 16 + 4, j * 16, makecol (255, 255, 255), "%d", (int) (z_map[w] / 100));
                  textprintf (bmp, font, i * 16, j * 16 + 8, makecol (255, 255, 255), "%02d", (int) (z_map[w] % 100));
               }
            }
         }
      }

      get_palette (pal);
      save_bitmap (op, bmp, pal);
      destroy_bitmap (bmp);
   }
}                               /* visual_map () */


int main (int argc, char *argv[]) {
   char fn[PATH_MAX];
   int k;
   COLOR_MAP cmap;
   allegro_init();
// TT: Removed to prevent flickering; no window to pop up & destroy
//   set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200,0,0);
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;
   for (k = 0; k < MAX_TILES; k++) {
      icons[k] = create_bitmap (16, 16);
      clear (icons[k]);
   }
   /* Shadow images */
   pcx_buffer = load_pcx ("Misc.pcx", pal);
   for (k = 0; k < MAX_SHADOWS; k++) {
      shadow[k] = create_bitmap (16, 16);
      blit (pcx_buffer, shadow[k], k * 16, 160, 0, 0, 16, 16);
   }
   destroy_bitmap (pcx_buffer);
   while (--argc > 0) {
      replace_extension(fn, argv[argc], "pcx", sizeof(fn));
      load_map_batch(argv[argc]);
      set_palette(pal);
      visual_map_ex(fn);
      TRACE("%s mode %d\n", argv[argc], gmap.map_mode);
   }
// TT: Same as above
//   set_gfx_mode(GFX_TEXT, 0,0,0,0);
   return 0;
} END_OF_MAIN();



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
