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
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include "mapdraw.h"


/* globals */

/* Selectable tiles on the right-hand menu */
BITMAP *icons[MAX_TILES];

/* Image and screen buffers */
BITMAP *pcx_buffer, *double_buffer;

/* Icons for Attributes (*b_mesh unused) */
BITMAP *mesh, *b_mesh, *z_mesh;

/* Font and mouse images */
BITMAP *font6, *mpic;


/* Gee, could it be an Entity frame? */
BITMAP *eframes[MAX_EPICS][12];

/* This is for a shadow.  It's dark. */
BITMAP *shadow[MAX_SHADOWS];

/* These are for the Layers 1-3 */
unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
/* These are for the Zone, Shadow and Obstacle Attributes */
unsigned char *z_map, *cz_map, *sh_map, *cs_map, *o_map, *co_map;

/* Used for the right-hand menu, plus something for the mouse */
short icon_set = 0, max_sets = 51, nomouse = 0;

/* Stores the name of the currently loaded map */
char map_fname[16] = "";

/* Used everywhere for strings */
char *strbuf;

/* gx and gy are view-window coords; x and y are everything else */
short gx = 0, gy = 0, x = 0, y = 0;

int draw_mode = MAP_LAYER123, curtile = 0, dmode = 0;

int curzone = 0, curshadow = 0, curobs = 0;
int copying = 0, copyx1 = -1, copyx2 = -1, copyy1 = -1, copyy2 = -1;
int clipb = 0, cbh = 0, cbw = 0;

s_map gmap;
s_entity gent[50];
s_show showing;

/* Number of entities, index of currently-selected entity */
int noe = 0, cent = 0;

char *icon_files[NUM_TILESETS] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "Incave.pcx", "village.pcx", "mount.pcx"
};
int htiles = (SW - 80) / 16;
int vtiles = (SH - 48) / 16;

PALETTE pal;

void load_map_batch (const char* fname)
{

   int p, q, i;
   PACKFILE *pf;

   pf = pack_fopen (fname, F_READ_PACKED);
   if (!pf) {
     return;
   }

      strcpy (map_fname, fname);
      /*
         pack_fread (&gmap, sizeof (ss_map), pf);
         pack_fread (&gent, sizeof (s_entity) * 50, pf);
       */
      load_s_map (&gmap, pf);
      for (i = 0; i < 50; ++i) {
         load_s_entity (gent + i, pf);
      }
      bufferize ();
      for (q = 0; q < gmap.ysize; ++q)
         for (p = 0; p < gmap.xsize; ++p)
            map[q * gmap.xsize + p] = pack_igetw (pf);
      for (q = 0; q < gmap.ysize; ++q)
         for (p = 0; p < gmap.xsize; ++p)
            b_map[q * gmap.xsize + p] = pack_igetw (pf);
      for (q = 0; q < gmap.ysize; ++q)
         for (p = 0; p < gmap.xsize; ++p)
            f_map[q * gmap.xsize + p] = pack_igetw (pf);

/*         pack_fread (map, (gmap.xsize * gmap.ysize * 2), pf); */
/*         pack_fread (b_map, (gmap.xsize * gmap.ysize * 2), pf); */
/*         pack_fread (f_map, (gmap.xsize * gmap.ysize * 2), pf); */
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
      for (p = 0; p < gmap.xsize * gmap.ysize; ++p) {
         /* Shadow layer */
         if (sh_map[p] >= MAX_SHADOWS)
            sh_map[p] = 0;
      }
      destroy_bitmap (pcx_buffer);
      
      /* Recount the number of entities on the map */
   noe = 0;
   for (p = 0; p < 50; p++)
      if (gent[p].active == 1)
         noe = p + 1;
}                               /* load_map () */

void visual_map_ex (const char* op)
{
   int i, j, w;
   BITMAP *bmp;
   PALETTE pal;
   int zones[MAX_ZONES][3];

   for (i = 0; i < MAX_ZONES; ++i) {
      /* Clear the zones buffer */
      /* TT fix: Updated to reset all zones */
      zones[i][0] = 0;          /* whether or not there is a "zone" here */
      zones[i][1] = 0;          /* x-coord of zone on bmp */
      zones[i][2] = 0;          /* y-coord of zone on bmp */
   }

   /* Create a bitmap the same size as the map */
   if ((bmp = create_bitmap (gmap.xsize * 16, gmap.ysize * 16)) != NULL) {
      for (j = 0; j < gmap.ysize; j++) {
         /* Which tile is currently being evaluated */
         w = gmap.xsize * j;
         for (i = 0; i < gmap.xsize; i++) {
            blit (icons[map[w]], bmp, 0, 0, i * 16, j * 16, 16, 16);
            draw_sprite (bmp, icons[b_map[w]], i * 16, j * 16);
            draw_sprite (bmp, icons[f_map[w]], i * 16, j * 16);
            draw_trans_sprite (bmp, shadow[sh_map[w]], i * 16, j * 16);
            /* TT: Which is better: set to 1 or increment? */
            // zones[z_map[w]][0]++;
            zones[z_map[w]][0] = 1;     /* ==1 if there is a "zone" here */
            zones[z_map[w]][1] = i;     /* Pass zone's x-coord to array */
            zones[z_map[w]][2] = j;     /* Pass zone's y-coord to array */
            w++;
         }
      }

      /* Print the zone number over the zones */
      for (i = 0; i < MAX_ZONES; i++) {
         if (zones[i][0] == 1) {
            textprintf (bmp, font, zones[i][1] * 16, zones[i][2] * 16,
                        makecol (255, 255, 255), "%d", i);
         }
      }
      get_palette (pal);
      save_bitmap (op, bmp, pal);
      destroy_bitmap (bmp);
   }
}                               /* visual_map () */


int main(int argc, char* argv[]) {
  char fn[PATH_MAX];
  int k;
  COLOR_MAP cmap;
  allegro_init();
  set_gfx_mode(GFX_AUTODETECT, 320, 200,0,0);
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
  while (--argc>0) {
    replace_extension(fn, argv[argc], "pcx", sizeof(fn));
    load_map_batch(argv[argc]);
    set_palette(pal);
    visual_map_ex(fn); 
    TRACE("%s mode %d\n", argv[argc], gmap.map_mode);
  }
  set_gfx_mode(GFX_TEXT, 0,0,0,0);
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
   memset (sh_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   clipb = 0;
}                               /* bufferize () */
