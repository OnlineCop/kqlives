/***************************************************************************\
 * This is the header file for the KQ map editor, suppored by the kqlives  *
 * community.  Questions and suggestions to the group are welcome.  Please *
 * join our mailing list: kqlives-main@lists.sourceforge.net               *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapdraw.h contains everything that needs to be shared across files.     *
\***************************************************************************/

#ifndef __MAPDRAW_H
#define __MAPDRAW_H

#define MAX_TILES    1024
#define NUM_TILESETS    7
#define MAX_EPICS      40
#define MAX_ZONES     256
#define MAX_SHADOWS    12
#define MAX_OBSTACLES   5
#define SW            640
#define SH            480
#define WBUILD          1

#define MAP_LAYER1      1       /* Map (sea-level) */
#define MAP_LAYER2      2       /* Background (ground-level) */
#define MAP_LAYER3      4       /* Foreground (tree-tops, etc.) */
#define MAP_LAYER12  (MAP_LAYER1 | MAP_LAYER2)  /* Map + background */
#define MAP_LAYER13  (MAP_LAYER1 | MAP_LAYER3)  /* Map + foreground */
#define MAP_LAYER23  (MAP_LAYER2 | MAP_LAYER3)  /* Background + foreground */
#define MAP_LAYER123  (MAP_LAYER1 | MAP_LAYER2 | MAP_LAYER3)    /* Map + background + foreground */
#define MAP_ENTITIES    8       /* Entities Attribute */
#define MAP_OBSTACLES  16       /* Obstacles Attribute */
#define MAP_SHADOWS    32       /* Shadows Attribute */
#define MAP_ZONES      64       /* Zones Attribute */
#define BLOCK_COPY    128       /* Mode to start copying an area */
#define BLOCK_PASTE   256       /* Mode to paste the copied area */
#define MAP_PREVIEW   512       /* Draw a proper preview with layer ordering and parallax */

#define ICONSET_SIZE   20       /* Number of icons shown in the icon map */

#include "../include/structs.h"

/* Something for allegro version compatibility */
/* ..can we use the textout_ex() and friends? */
#if (ALLEGRO_VERSION>=4 && ALLEGRO_SUB_VERSION>=1)
#define HAVE_TEXT_EX
#endif

typedef struct
{
   int entities, shadows, obstacles, zones;
   int last_layer;              /* Tracks last-used layer */
}
s_show;


/*
 * A requirement for this program is that all icon-files have a blank icon in
 * entry 0
 */

/* From mapdata.c */
void getfont (void);

/* From mapdump.c */
void error_load (const char *);
void usage (const char *);

/* From mapedit.c */
void animate (void);
extern void bufferize (void);
int check_last_zone (void);
extern void cleanup (void);
void clear_layer (void);
void clear_obstructs (void);
void clear_shadows (void);
void cmessage (char *);
int confirm_exit (void);
void copy_layer (void);
void copy_region (void);
int count_current_obstacles (void);
int count_current_shadows (void);
int count_current_zones (void);
void describe_map (void);
void draw_layer (short *, const int);
void draw_map (void);
void draw_menubars (void);
int get_line (const int, const int, char *, const int);
void get_tile (void);
void global_change (void);
void klog (char *);
void make_rect (BITMAP *, const int, const int);
void normalize_view (void);
void paste_region (const int, const int);
void paste_region_special (const int, const int);
void preview_map (void);
void print_sfont (const int, const int, const char *, BITMAP *);
void process_keyboard (const int);
void process_menu_bottom (const int, const int);
void process_menu_right (const int, const int);
void process_mouse (const int);
void read_controls (void);
void resize_map (const int);
void select_only (const int, const int);
void show_help (void);
void startup (void);
void update_tileset (void);
void wait_enter (void);
void wipe_map (void);
int yninput (void);

/* From mapent.c */
void displace_entities (void);
void draw_entdata (const int);
void draw_ents (void);
void erase_entity (const int, const int);
void init_entities (void);
void place_entity (int, int);
void update_entities (void);

/* From mapfile.c */
void error_load (const char *);
void make_mapfrompcx (void);
void maptopcx (void);
void new_map (void);
void prompt_load_map (void);
void save_map (void);

/* From mapshared.c */
void load_map (const char *);
void set_pcx (BITMAP **, const char *, PALETTE, const int);
void visual_map (const char *);


extern BITMAP *double_buffer, *pcx_buffer, *icons[MAX_TILES], *eframes[MAX_EPICS][12];
extern BITMAP *font6, *mesh;
extern PALETTE pal;

extern char map_fname[40], *strbuf;
extern short icon_set;

extern char *icon_files[NUM_TILESETS];

extern const int htiles, vtiles;
extern int number_of_ents, current_ent;

extern s_entity gent[];
extern s_map gmap;
extern s_show showing;

extern unsigned short tilex[MAX_TILES];
extern s_anim tanim[NUM_TILESETS][MAX_ANIM];
extern s_anim adata[MAX_ANIM];

extern short window_x, window_y;
extern short max_sets;

extern unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
extern unsigned char *z_map, *sh_map, *o_map, *cz_map, *csh_map, *co_map;

#endif
