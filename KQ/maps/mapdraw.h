/*
 * This is the KQ map editor
 * (Pete's hacked version)
 * (TT's edited hacked version)
 * December 2002 (and Jan 2003)
 */

#define MAX_TILES    1024
#define NUM_TILESETS    7
#define MAX_EPICS      40
#define MAX_ZONES     256
#define MAX_SHADOWS    12
#define MAX_OBSTACLES   5
#define SW            640
#define SH            480
#define WBUILD          1

#define MAP_LAYER1  1           /* Map (sea-level) */
#define MAP_LAYER2  2           /* Background (ground-level) */
#define MAP_LAYER3  4           /* Foreground (tree-tops, etc.) */
#define MAP_SHADOWS  8          /* Entities Attribute */
#define MAP_ZONES  16           /* Shadows Attribute */
#define MAP_OBSTACLES  32       /* Obstacles Attribute */
#define MAP_ENTITIES  64        /* Zones Attribute */
#define MAP_LAYER12  (MAP_LAYER1 | MAP_LAYER2)  /* Map + background */
#define MAP_LAYER13  (MAP_LAYER1 | MAP_LAYER3)  /* Map + foreground */
#define MAP_LAYER23  (MAP_LAYER2 | MAP_LAYER3)  /* Background + foreground */
#define MAP_LAYER123 (MAP_LAYER1 | MAP_LAYER2 | MAP_LAYER3)     /* Map + background + foreground */
#define BLOCK_COPY  128         /* Mode to start copying an area */
#define BLOCK_PASTE  256        /* Mode to paste the copied area */
#define GRAB_TILE  512          /* Select a tile in the map to draw */
#define MAP_PREVIEW 1024        /* Draw a proper preview with layer ordering and parallax */

#define ICONSET_SIZE    20      /* Number of icons shown in the icon map */

#include "../include/structs.h"


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



/* From mapedit.c */
void animate (void);
void bufferize (void);
int check_last_zone (void);
void cleanup (void);
void clear_layer (void);
void clear_obstructs (void);
void clear_shadows (void);
void cmessage (char *);
int confirm_exit (void);
void copy_layer (void);
void copy_region (void);
void describe_map (void);
void draw_layer (short *, int);
void draw_map (void);
void draw_menubars (void);
int get_line (int, int, char *, int);
void get_tile (void);
void global_change (void);
void klog (char *);
void normalize_view (void);
void paste_region (int, int);
void paste_region_special (int, int);
void preview_map (void);
void print_sfont (int, int, char *, BITMAP *);
void process_keyboard (int);
void process_menu_bottom (int, int);
void process_menu_right (int, int);
void process_mouse (const int);
void read_controls (void);
void resize_map (const int);
void select_only (const int, const int);
void show_help (void);
void startup (void);
void update_tileset (void);
void visual_map (void);
void wait_enter (void);
void wipe_map (void);
int yninput (void);

/* From mapent.c */
void displace_entities (void);
void draw_entdata (int);
void draw_ents (void);
void erase_entity (int, int);
void init_entities (void);
void place_entity (int, int);
void update_entities (void);

/* From mapfile.c */
void error_load (char *);
void load_map (char *);
void make_mapfrompcx (void);
void maptopcx (void);
void new_map (void);
void prompt_load_map (void);
void save_map (void);

/* From mapdata.c */
void getfont (void);


extern BITMAP *double_buffer, *pcx_buffer;
extern BITMAP *font6;
extern BITMAP *icons[];
extern PALETTE pal;

extern BITMAP *eframes[MAX_EPICS][12];

extern char *icon_files[];
extern char *strbuf;
extern char map_fname[];

extern int draw_mode, htiles, vtiles;
extern int number_of_ents, current_ent;

extern s_entity gent[];
extern s_map gmap;
extern s_show showing;
extern s_anim adata[MAX_ANIM];

extern short window_x, window_y;
extern short icon_set, max_sets;

extern unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
extern unsigned char *z_map, *sh_map, *o_map, *cz_map, *csh_map, *co_map;
