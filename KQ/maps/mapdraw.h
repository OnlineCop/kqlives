/*
 * This is the KQ map editor
 * (Pete's hacked version)
 * (TT's edited hacked version)
 * December 2002 (and Jan 2003)
 */

#define MAX_TILES    1024
#define NUM_TILESETS    6
#define MAX_EPICS      40
#define MAX_ZONES     256
#define MAX_SHADOWS    12
#define SW            640
#define SH            480
#define WBUILD          1

#define MAP_LAYER1  1  // map (sea-level)
#define MAP_LAYER2  2  // background (ground-level)
#define MAP_LAYER3  4  // foreground (tree-tops, etc.)
#define MAP_SHADOWS  8  // Entities Attribute
#define MAP_ZONES  16  // Shadows Attribute
#define MAP_OBSTACLES  32  // Obstacles Attribute
#define MAP_ENTITIES  64  // Zones Attribute
#define MAP_LAYER12  (MAP_LAYER1 | MAP_LAYER2)  // map + background
#define MAP_LAYER13  (MAP_LAYER1 | MAP_LAYER3)  // map + foreground
#define MAP_LAYER23  (MAP_LAYER2 | MAP_LAYER3)  // background + foreground
#define MAP_LAYER123 (MAP_LAYER1 | MAP_LAYER2 | MAP_LAYER3)  // map + background + foreground
#define BLOCK_COPY  128  // Mode to start copying an area
#define BLOCK_PASTE  256  // Mode to paste the copied area
#define GRAB_TILE  512  // Select a tile in the map to draw

#define ICONSET_SIZE    20 // Number of icons shown in the tile map

#include "../include/structs.h"

/*
typedef struct
{
   unsigned char chrx;  // Entity's identity (what s/he looks like)
   unsigned short x; // x-coord on map
   unsigned short y; // y-coord on map
   unsigned short tilex;   // x-coord tile that entity is standing on
   unsigned short tiley;   // y-coord tile that entity is standing on
   unsigned char id; // Entity type (fighter, enemy, normal)
   unsigned char active;   // "Alive/Showing on map" or not
   unsigned char facing;   // Direction
   unsigned char moving;   // In the middle of a move
   unsigned char movcnt;   // How far along the move entity is
   unsigned char framectr; // Counter for determining animation frame
   unsigned char movemode; // Stand, wander, script or chasing
   unsigned char obsmode;  // Determine if affected by obstacles or not
   unsigned char delay; // Movement delay (between steps)
   unsigned char delayctr; // Counter for movement delay
   unsigned char speed; // How hyperactive the entity is
   unsigned char scount;
   unsigned char cmd;   // Move/Wait/Facing command
   unsigned char sidx;  // Script ID number
   unsigned char extra;
   unsigned char chasing;  // Entity is following another
   int cmdnum; // Number of times we need to repeat 'cmd'
   unsigned char atype;
   unsigned char snapback; // Snaps back to direction previously facing
   unsigned char facehero; // Look at player when talked to
   unsigned char transl;   // Entity is see-through or not
   char script[60];  // Movement/action script (pacing, etc.)
}
s_entity;

typedef struct
{
   unsigned char map_no, zero_zone, map_mode, can_save;
   unsigned char tileset, use_sstone, can_warp, extra_byte;
   int xsize, ysize;
   int pmult, pdiv;
   int stx, sty;
   int warpx, warpy;
   int extra_sdword1, extra_sdword2;
   char song_file[16];
   char map_desc[40];
}
ss_map;
*/

typedef struct
{
   int entities, shadows, obstacles, zones;
   int last_layer;   // tracks last-used layer
}
s_show;

/*
   A requirement for this program is that all icon-files have a blank icon in entry 0
*/

/* prototypes */
void klog (char *);
void process_controls (void);
int confirm_exit (void);
void check_tilesel (int, int);
void check_mdupdate (int, int);
void draw_map (void);
void draw_menubars (void);
void bufferize (void);
void load_map (void);
void save_map (void);
void make_mapfrompcx (void);
void global_change (void);
void clear_obstructs (void);
void clear_shadows (void);
int get_line (int, int, char *, int);
int yninput (void);
void startup (void);
void cleanup (void);
void new_map (void);
void init_entities (void);
void wait_enter (void);
void cmessage (char *);
void paste_region_special (int, int);
void paste_region (int, int);
void copy_region (void);
void resize_map (void);
void displace_entities (void);
void wipe_map (void);
void maptopcx (void);
void place_entity (int, int);
void erase_entity (int, int);
void update_entities (void);
void draw_entdata (int);
void describe_map (void);
void copy_layer (void);
void clear_layer (void);
void print_sfont (int, int, char *, BITMAP *);
void getfont (void);
void update_tileset (void);
void show_help (void);
int check_last_zone (void);
void get_tile (void);

void visual_map(void);

extern unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
extern unsigned char *z_map, *cz_map, *sh_map, *cs_map, *o_map, *co_map;
extern char *strbuf;
extern s_map gmap;
extern s_entity gent[];
extern s_show showing;
extern PALETTE pal;
extern BITMAP *double_buffer, *pcx_buffer;
extern BITMAP *icons[];
extern char *icon_files[];
extern short icon_set, max_sets;
extern int noe, cent;
extern char map_fname[];
extern short gx, gy;
extern int draw_mode, htiles, vtiles;

extern BITMAP *font6;
