/* 
 * This is the KQ map editor 
 * (Pete's hacked version)
 * December 2002
 */

#define MAX_TILES    1024
#define NUM_TILESETS    6
#define MAX_EPICS      40
#define MAX_ZONES     256
#define MAX_SHADOWS    12
#define SW            640
#define SH            480
#define WBUILD          1

typedef struct
{
   unsigned char chrx;
   unsigned short x, y;
   unsigned short tilex, tiley;
   unsigned char id;
   unsigned char active;
   unsigned char facing;
   unsigned char moving, movcnt;
   unsigned char framectr;
   unsigned char movemode;
   unsigned char obsmode;
   unsigned char delay, delayctr;
   unsigned char speed, scount;
   unsigned char cmd, sidx;
   unsigned char extra, chasing;
   int cmdnum;
   unsigned char atype;
   unsigned char snapback;
   unsigned char facehero;
   unsigned char transl;
   char script[60];
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

/*
   A requirement for this program is that all icon-files have a blank icon in entry 0
*/

/* prototypes */
void my_counter (void);
void klog (char *);
void process_controls (void);
int confirm_exit (void);
void check_tilesel (int, int);
void check_mdupdate (int, int);
void draw_map (void);
void draw_sidebar (void);
void bufferize (void);
void load_map (void);
void save_map (void);
void make_mapfrompcx (void);
void global_change (void);
void clear_obstruct (void);
void clear_foreground (void);
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
void copy_region (int, int);
void resize_map (void);
void displace_entities (void);
void wipe_map (void);
void maptopcx (void);
void place_entity (int, int);
void erase_entity (int, int);
void update_entities (void);
void draw_entdata (int);
void describe_map (void);
void swap_layers (void);
void clear_layer (void);
void print_sfont (int, int, char *, BITMAP *);
void getfont (void);

extern unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
extern unsigned char *z_map, *cz_map, *s_map, *cs_map, *o_map, *co_map;
extern char *strbuf;
extern ss_map gmap;
extern BITMAP *double_buffer, *pcx_buffer;
extern BITMAP *icons[];
extern s_entity gent[];
extern PALETTE pal;
extern char *icon_files[];
extern short icon_set, max_sets;
extern int noe, cent;
extern char map_fname[];
extern short gx, gy;
extern int draw_mode, htiles, vtiles;

extern BITMAP *font6;
