
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "allegro.h"

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

/* globals */
PALETTE pal = { {0, 0, 0, 0}, {0, 0, 0, 0}, {8, 8, 8, 0}, {12, 12, 12, 0},
{16, 16, 16, 0}, {20, 20, 20, 0}, {24, 24, 24, 0}, {28, 28, 28, 0},
{33, 33, 33, 0}, {38, 38, 38, 0}, {43, 43, 43, 0}, {47, 47, 47, 0},
{51, 51, 51, 0}, {55, 55, 55, 0}, {59, 59, 59, 0}, {63, 63, 63, 0},

{7, 0, 0, 0}, {15, 0, 0, 0}, {23, 0, 0, 0}, {31, 0, 0, 0},
{39, 0, 0, 0}, {47, 0, 0, 0}, {55, 0, 0, 0}, {63, 0, 0, 0},
{63, 7, 7, 0}, {63, 14, 14, 0}, {63, 21, 21, 0}, {63, 29, 29, 0},
{63, 36, 36, 0}, {63, 44, 44, 0}, {63, 51, 51, 0}, {63, 59, 59, 0},

{0, 7, 0, 0}, {0, 11, 0, 0}, {0, 15, 0, 0}, {0, 19, 0, 0},
{0, 23, 0, 0}, {0, 27, 0, 0}, {0, 31, 0, 0}, {0, 39, 0, 0},
{0, 47, 0, 0}, {0, 55, 0, 0}, {0, 63, 0, 0}, {22, 63, 22, 0},
{37, 63, 37, 0}, {47, 63, 47, 0}, {53, 63, 53, 0}, {58, 63, 58, 0},

{0, 0, 7, 0}, {0, 0, 15, 0}, {0, 0, 23, 0}, {0, 0, 31, 0},
{0, 0, 39, 0}, {0, 0, 47, 0}, {0, 0, 55, 0}, {0, 0, 63, 0},
{7, 7, 63, 0}, {15, 15, 63, 0}, {22, 22, 63, 0}, {30, 30, 63, 0},
{37, 37, 63, 0}, {45, 45, 63, 0}, {52, 52, 63, 0}, {58, 58, 63, 0},

{7, 0, 7, 0}, {15, 0, 15, 0}, {23, 0, 23, 0}, {31, 0, 31, 0},
{39, 0, 39, 0}, {47, 0, 47, 0}, {55, 0, 55, 0}, {63, 0, 63, 0},
{63, 7, 63, 0}, {63, 15, 63, 0}, {63, 22, 63, 0}, {63, 30, 63, 0},
{63, 37, 63, 0}, {63, 45, 63, 0}, {63, 52, 63, 0}, {63, 58, 63, 0},

{7, 1, 0, 0}, {15, 2, 0, 0}, {23, 4, 0, 0}, {31, 7, 0, 0},
{39, 9, 0, 0}, {46, 12, 0, 0}, {54, 15, 0, 0}, {63, 18, 0, 0},
{63, 21, 4, 0}, {63, 25, 8, 0}, {63, 29, 12, 0}, {63, 34, 16, 0},
{63, 38, 22, 0}, {63, 44, 28, 0}, {63, 49, 34, 0}, {63, 55, 41, 200},

{7, 7, 0, 200}, {11, 11, 0, 0}, {15, 15, 0, 0}, {19, 19, 0, 200},
{23, 23, 0, 0}, {27, 27, 0, 0}, {31, 31, 0, 0}, {39, 39, 0, 0},
{47, 47, 0, 0}, {54, 54, 0, 0}, {63, 63, 0, 0}, {63, 63, 23, 0},
{63, 63, 35, 0}, {63, 63, 47, 0}, {63, 63, 53, 0}, {63, 63, 58, 0},

{0, 7, 7, 0}, {0, 11, 11, 0}, {0, 15, 15, 0}, {0, 19, 19, 0},
{0, 23, 23, 0}, {0, 27, 27, 0}, {0, 31, 31, 0}, {0, 39, 39, 0},
{0, 47, 47, 0}, {0, 55, 55, 0}, {0, 63, 63, 0}, {16, 63, 63, 0},
{32, 63, 63, 0}, {47, 63, 63, 0}, {51, 63, 63, 0}, {58, 63, 63, 0},

{3, 1, 0, 0}, {7, 3, 0, 0}, {11, 5, 0, 0}, {15, 8, 0, 0},
{19, 11, 0, 0}, {23, 14, 0, 0}, {27, 17, 0, 0}, {31, 20, 0, 105},
{35, 24, 0, 93}, {39, 27, 0, 0}, {43, 32, 0, 0}, {47, 35, 0, 0},
{51, 38, 0, 0}, {55, 42, 0, 0}, {59, 46, 0, 0}, {63, 55, 22, 0},

{6, 3, 3, 0}, {12, 6, 6, 0}, {18, 9, 9, 0}, {24, 12, 12, 0},
{30, 15, 15, 0}, {36, 18, 18, 0}, {42, 21, 21, 0}, {48, 24, 24, 0},
{54, 27, 27, 0}, {60, 30, 30, 0}, {63, 33, 33, 0}, {63, 36, 36, 0},
{63, 39, 39, 0}, {63, 43, 43, 0}, {63, 47, 47, 0}, {63, 51, 51, 0},

{3, 6, 3, 0}, {6, 12, 6, 0}, {9, 18, 9, 0}, {12, 24, 12, 0},
{15, 30, 15, 0}, {18, 36, 18, 0}, {21, 42, 21, 0}, {24, 48, 24, 0},
{27, 54, 27, 0}, {30, 60, 30, 0}, {33, 63, 33, 0}, {37, 63, 37, 0},
{41, 63, 41, 0}, {45, 63, 45, 0}, {49, 63, 49, 0}, {53, 63, 53, 0},

{4, 2, 1, 0}, {8, 4, 2, 0}, {12, 6, 3, 0}, {16, 8, 4, 0},
{20, 10, 5, 0}, {24, 16, 7, 0}, {28, 20, 10, 0}, {32, 24, 16, 0},
{36, 28, 20, 0}, {40, 32, 25, 0}, {44, 36, 28, 0}, {48, 40, 32, 0},
{52, 44, 36, 0}, {56, 48, 40, 0}, {60, 52, 44, 0}, {63, 56, 48, 0},

{6, 3, 6, 0}, {12, 6, 12, 0}, {18, 9, 18, 0}, {24, 12, 24, 0},
{30, 15, 30, 0}, {36, 18, 36, 0}, {42, 21, 42, 0}, {48, 24, 48, 0},
{54, 27, 54, 0}, {60, 30, 60, 0}, {63, 34, 63, 0}, {63, 38, 63, 0},
{63, 42, 63, 0}, {63, 46, 63, 0}, {63, 50, 63, 0}, {63, 54, 63, 0},

{6, 3, 2, 79}, {12, 6, 3, 0}, {18, 9, 4, 0}, {24, 12, 5, 0},
{30, 15, 6, 0}, {36, 18, 7, 0}, {42, 21, 8, 0}, {48, 24, 9, 0},
{54, 27, 11, 0}, {60, 30, 13, 0}, {63, 33, 15, 0}, {63, 36, 17, 0},
{63, 39, 19, 80}, {63, 42, 21, 71}, {63, 46, 24, 0}, {63, 50, 27, 0},

{6, 6, 3, 0}, {12, 12, 6, 87}, {18, 18, 9, 0}, {24, 24, 12, 0},
{30, 30, 15, 0}, {36, 36, 18, 0}, {42, 42, 21, 0}, {48, 48, 24, 0},
{54, 54, 27, 0}, {60, 60, 30, 0}, {63, 63, 34, 0}, {63, 63, 38, 0},
{63, 63, 42, 0}, {63, 63, 46, 0}, {63, 63, 50, 0}, {63, 63, 54, 0},

{3, 6, 6, 0}, {6, 12, 12, 0}, {9, 18, 18, 0}, {12, 24, 24, 0},
{15, 30, 30, 0}, {18, 36, 36, 0}, {21, 42, 42, 0}, {24, 48, 48, 0},
{27, 54, 54, 0}, {30, 60, 60, 0}, {34, 63, 63, 0}, {38, 63, 63, 0},
{42, 63, 63, 0}, {46, 63, 63, 0}, {50, 63, 63, 0}, {63, 63, 63, 0}
};

BITMAP *icons[MAX_TILES], *pcx_buffer, *double_buffer, *mesh, *b_mesh,
   *z_mesh;
BITMAP *font6, *mpic;
BITMAP *eframes[MAX_EPICS][12];
BITMAP *shadow[MAX_SHADOWS];
unsigned short *map, *b_map, *f_map, *c_map, *cf_map, *cb_map;
unsigned char *z_map, *cz_map, *s_map, *cs_map, *o_map, *co_map;
short icon_set = 0, nomouse = 0;
char map_fname[16] = { "" }, *strbuf;
short gx = 0, gy = 0, x = 0, y = 0;
unsigned short int cb;
int max_sets = 51, draw_mode = 0, curtile = 0, dmode = 0, curzone =
   0, curshadow = 0, curobs = 0;
int copying = 0, copyx = -1, copyy = -1, clipb = 0, cbh = 0, cbw = 0;
ss_map gmap;
s_entity gent[50];
int cent = 0, noe = 0;
unsigned char mousepic[] = {
   15, 00, 00, 00,
   15, 15, 00, 00,
   15, 15, 15, 00,
   15, 15, 15, 15,
   00, 00, 15, 00,
   00, 00, 00, 15
};
char icon_files[NUM_TILESETS][16] = {
   "land.pcx", "newtown.pcx", "castle.pcx",
   "incave.pcx", "village.pcx", "mount.pcx"
};
int htiles = (SW - 80) / 16;
int vtiles = (SH - 48) / 16;

int main (void)
{
   int stop = 0;

   startup ();
   while (!stop)
     {
        process_controls ();
        draw_map ();
        draw_sidebar ();
        if (!nomouse)
           draw_sprite (double_buffer, mpic, mouse_x, mouse_y);
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        if (key[KEY_Q])
           stop = confirm_exit ();
     }
   return 0;
}

END_OF_MAIN ();

void klog (char *msg)
{
   FILE *ff;

   ff = fopen ("mapdraw.log", "a");
   if (!ff)
      exit (-1);
   fprintf (ff, "%s\n", msg);
   fclose (ff);
}

void process_controls (void)
{
   int k;

   if (keypressed ())
     {
        k = (readkey () >> 8);
        if (k == KEY_1)
           draw_mode = 0;
        if (k == KEY_4)
           draw_mode = 1;
        if (k == KEY_5)
           draw_mode = 2;
        if (k == KEY_2)
           draw_mode = 9;
        if (k == KEY_3)
           draw_mode = 10;
        if (k == KEY_O)
           draw_mode = 3;
        if (k == KEY_Z)
           draw_mode = 4;
        if (k == KEY_T)
           draw_mode = 5;
        if (k == KEY_S)
           draw_mode = 8;
        if (k == KEY_P)
           draw_mode = 6;
        if (k == KEY_V)
           draw_mode = 11;
        if (k == KEY_C)
           draw_mode = 12;
        if (k == KEY_W)
           wipe_map ();
        if (k == KEY_D)
           displace_entities ();
        if (k == KEY_E)
           clipb = 0;
        if (k == KEY_R)
           resize_map ();
        if (k == KEY_J)
           maptopcx ();
        if (k == KEY_F1)
           load_map ();
        if (k == KEY_F2)
           new_map ();
        if (k == KEY_F3)
           save_map ();
        if (k == KEY_F4)
           clear_layer ();
        if (k == KEY_F5)
           make_mapfrompcx ();
        if (k == KEY_F6)
           global_change ();
        if (k == KEY_F7)
           clear_obstruct ();
        if (k == KEY_F8)
           clear_foreground ();
        if (k == KEY_F9)
           swap_layers ();
        if (k == KEY_F10)
           describe_map ();
        if (k == KEY_F11)
           draw_mode = 7;
        if (k == KEY_F12)
          {
             update_entities ();
          }
        if (k == KEY_ESC)
          {
             if (copying == 1)
               {
                  copying = 0;
                  copyx = -1;
                  copyy = -1;
               }
          }
        if (k == KEY_MINUS)
          {
             switch (draw_mode)
               {
               case 3:
                  curobs--;
                  if (curobs < 0)
                     curobs = 5;
                  break;
               case 4:
                  curzone--;
                  if (curzone < 0)
                     curzone = MAX_ZONES - 1;
                  break;
               case 7:
                  cent--;
                  if (cent < 0)
                     cent = MAX_EPICS - 1;
                  break;
               case 8:
                  curshadow--;
                  if (curshadow < 0)
                     curshadow = MAX_SHADOWS - 1;
                  break;
               default:
                  icon_set--;
                  if (icon_set < 0)
                     icon_set = max_sets;
                  break;
               }
          }
        if (k == KEY_EQUALS)
          {
             switch (draw_mode)
               {
               case 3:
                  curobs++;
                  if (curobs >= 5)
                     curobs = 0;
                  break;
               case 4:
                  curzone++;
                  if (curzone >= MAX_ZONES)
                     curzone = 0;
                  break;
               case 7:
                  cent++;
                  if (cent == MAX_EPICS)
                     cent = 0;
                  break;
               case 8:
                  curshadow++;
                  if (curshadow >= MAX_SHADOWS)
                     curshadow = 0;
                  break;
               default:
                  icon_set++;
                  if (icon_set > max_sets)
                     icon_set = 0;
                  break;
               }
          }
        if (k == KEY_UP)
           gy--;
        if (k == KEY_DOWN)
           gy++;
        if (k == KEY_RIGHT)
           gx++;
        if (k == KEY_LEFT)
           gx--;
        if (k == KEY_PGUP)
           gy -= vtiles;
        if (k == KEY_PGDN)
           gy += vtiles;
        if (k == KEY_TAB)
           gx += htiles;
        if (k == KEY_BACKSPACE)
           gx -= htiles;
     }
   if (gx > gmap.xsize - htiles)
      gx = gmap.xsize - htiles;
   if (gx < 0)
      gx = 0;
   if (gy > gmap.ysize - vtiles)
      gy = gmap.ysize - vtiles;
   if (gy < 0)
      gy = 0;
   if (!(mouse_b & 1))
      dmode = 0;
   if (mouse_b & 1)
     {
        x = mouse_x / 16;
        y = mouse_y / 16;
        if (y > (vtiles - 1))
          {
             if (dmode == 0)
               {
                  check_mdupdate (mouse_x, mouse_y);
                  return;
               }
             else
                y = vtiles - 1;
          }
        if (x > (htiles - 1) && dmode == 0)
           check_tilesel (mouse_x, mouse_y);
        else
          {
             if (x > (htiles - 1))
                x = htiles - 1;
             dmode = 1;
             if (draw_mode == 0)
                map[((gy + y) * gmap.xsize) + gx + x] = curtile;
             if (draw_mode == 1 || draw_mode == 9)
                b_map[((gy + y) * gmap.xsize) + gx + x] = curtile;
             if (draw_mode == 2 || draw_mode == 10)
                f_map[((gy + y) * gmap.xsize) + gx + x] = curtile;
             if (draw_mode == 3)
                o_map[((gy + y) * gmap.xsize) + gx + x] = curobs;
             if (draw_mode == 4)
                z_map[((gy + y) * gmap.xsize) + gx + x] = curzone;
             if (draw_mode == 5 && copying == 0)
               {
                  copying = 1;
                  copyx = gx + x;
                  copyy = gy + y;
               }
             if (draw_mode == 6 && clipb != 0)
                paste_region (gx + x, gy + y);
             if (draw_mode == 7)
                place_entity (mouse_x / 16 + gx, mouse_y / 16 + gy);
             if (draw_mode == 8)
                s_map[((gy + y) * gmap.xsize) + gx + x] = curshadow;
          }
     }
   if ((mouse_b & 2) && draw_mode > 2 && draw_mode != 6)
     {
        x = mouse_x / 16;
        y = mouse_y / 16;
        if (y > (vtiles - 1))
           y = vtiles - 1;
        if (x > (htiles - 1))
           x = htiles - 1;
        if (draw_mode == 3)
           o_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 4)
           z_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 5 && copying == 1)
          {
             copy_region (gx + x, gy + y);
             copying = 0;
          }
        if (draw_mode == 7)
           erase_entity (mouse_x / 16 + gx, mouse_y / 16 + gy);
        if (draw_mode == 8)
           s_map[((gy + y) * gmap.xsize) + gx + x] = 0;
     }
   if ((mouse_b & 2) && (draw_mode < 3 || draw_mode > 8))
     {
        x = mouse_x / 16;
        y = mouse_y / 16;
        if (y > (vtiles - 1))
           y = vtiles - 1;
        if (x > (htiles - 1))
           x = htiles - 1;
        dmode = 1;
        if (draw_mode == 0)
           map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 1 || draw_mode == 9)
           b_map[((gy + y) * gmap.xsize) + gx + x] = 0;
        if (draw_mode == 2 || draw_mode == 10)
           f_map[((gy + y) * gmap.xsize) + gx + x] = 0;
     }
   x = mouse_x / 16;
   y = mouse_y / 16;
   if (y > (vtiles - 1))
      y = vtiles - 1;
   if (x > (htiles - 1))
      x = htiles - 1;
}

int confirm_exit (void)
{
   cmessage ("Are you sure you want to exit? (y/n)");
   return yninput ();
}

void check_tilesel (int cx, int cy)
{
   int xp, yp;

   if (cx >= (SW - 64) && cx <= (SW - 33) && cy >= 0 && cy <= 159)
     {
        xp = (cx - (SW - 64)) / 16;
        yp = cy / 16;
        curtile = icon_set * 20 + (xp * 10 + yp);
     }
}

void check_mdupdate (int cx, int cy)
{
   int a;

   if (cx >= 0 && cx <= 159 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        gmap.tileset++;
        if (gmap.tileset >= NUM_TILESETS)
           gmap.tileset = 0;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 0 && cx <= 159 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 36, (SH - 34), 159, (SH - 29), 0);
        print_sfont (30, (SH - 34), ">", screen);
        hline (screen, 36, (SH - 29), 113, 255);
        a = get_line (36, (SH - 34), strbuf, 13);
        if (a != 0)
           strcpy (gmap.song_file, strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 0 && cx <= 159 && cy >= (SH - 28) && cy <= (SH - 23))
     {
        gmap.zero_zone = 1 - gmap.zero_zone;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 0 && cx <= 79 && cy >= (SH - 22) && cy <= (SH - 17))
     {
        rectfill (screen, 42, (SH - 22), 159, (SH - 17), 0);
        print_sfont (36, (SH - 22), ">", screen);
        hline (screen, 42, (SH - 17), 65, 255);
        a = get_line (42, (SH - 22), strbuf, 4);
        if (a != 0)
           gmap.map_no = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 0 && cx <= 79 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        gmap.map_mode++;
        if (gmap.map_mode > 5)
           gmap.map_mode = 0;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 0 && cx <= 79 && cy >= (SH - 10) && cy <= (SH - 5))
     {
        gmap.can_save = 1 - gmap.can_save;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 80 && cx <= 159 && cy >= (SH - 22) && cy <= (SH - 17))
     {
        gmap.can_warp = 1 - gmap.can_warp;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }
   if (cx >= 80 && cx <= 159 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        rectfill (screen, 122, (SH - 16), 159, (SH - 11), 0);
        print_sfont (116, (SH - 16), ">", screen);
        hline (screen, 122, (SH - 11), 145, 255);
        a = get_line (122, (SH - 16), strbuf, 4);
        if (a != 0)
           gmap.warpx = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 80 && cx <= 159 && cy >= (SH - 10) && cy <= (SH - 5))
     {
        rectfill (screen, 122, (SH - 10), 159, (SH - 5), 0);
        print_sfont (116, (SH - 10), ">", screen);
        hline (screen, 122, (SH - 5), 145, 255);
        a = get_line (122, (SH - 10), strbuf, 4);
        if (a != 0)
           gmap.warpy = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 160 && cx <= 239 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        rectfill (screen, 214, (SH - 40), 239, (SH - 35), 0);
        print_sfont (208, (SH - 40), ">", screen);
        hline (screen, 214, (SH - 35), 237, 255);
        a = get_line (214, (SH - 40), strbuf, 4);
        if (a != 0)
           gmap.stx = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 160 && cx <= 239 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 214, (SH - 34), 239, (SH - 29), 0);
        print_sfont (208, (SH - 34), ">", screen);
        hline (screen, 214, (SH - 29), 237, 255);
        a = get_line (214, (SH - 34), strbuf, 4);
        if (a != 0)
           gmap.sty = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 240 && cx <= 319 && cy >= (SH - 40) && cy <= (SH - 35))
     {
        rectfill (screen, 276, (SH - 40), 319, (SH - 35), 0);
        print_sfont (270, (SH - 40), ">", screen);
        hline (screen, 276, (SH - 35), 299, 255);
        a = get_line (276, (SH - 40), strbuf, 4);
        if (a != 0)
           gmap.pmult = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 240 && cx <= 319 && cy >= (SH - 34) && cy <= (SH - 29))
     {
        rectfill (screen, 270, (SH - 34), 319, (SH - 29), 0);
        print_sfont (264, (SH - 34), ">", screen);
        hline (screen, 270, (SH - 29), 293, 255);
        a = get_line (270, (SH - 34), strbuf, 4);
        if (a != 0)
           gmap.pdiv = atoi (strbuf);
        else
           draw_sidebar ();
        return;
     }
   if (cx >= 160 && cx <= 319 && cy >= (SH - 28) && cy <= (SH - 17))
      resize_map ();
   if (cx >= 160 && cx <= 239 && cy >= (SH - 16) && cy <= (SH - 11))
     {
        gmap.use_sstone = 1 - gmap.use_sstone;
        draw_sidebar ();
        while (mouse_b & 1);
        return;
     }

}

void draw_map (void)
{
   int dx, dy, d, w, maxx, maxy;

   rectfill (double_buffer, 0, 0, (SW - 81), (SH - 49), 0);
   if (vtiles > gmap.ysize)
      maxy = gmap.ysize;
   else
      maxy = vtiles;
   if (htiles > gmap.xsize)
      maxx = gmap.xsize;
   else
      maxx = htiles;
   for (dy = 0; dy < maxy; dy++)
     {
        for (dx = 0; dx < maxx; dx++)
          {
             w = ((gy + dy) * gmap.xsize) + gx + dx;
             if (draw_mode < 9 || draw_mode > 10)
                blit (icons[map[w]], double_buffer, 0, 0, dx * 16, dy * 16,
                      16, 16);
             if (draw_mode > 8 && draw_mode != 11 && draw_mode != 12)
                rectfill (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                          dy * 16 + 15, 0);
             if (draw_mode != 0 && draw_mode != 10)
                draw_sprite (double_buffer, icons[b_map[w]], dx * 16,
                             dy * 16);
             if (draw_mode > 1 && draw_mode != 9)
                draw_sprite (double_buffer, icons[f_map[w]], dx * 16,
                             dy * 16);
             if (draw_mode == 3)
               {
                  switch (o_map[w])
                    {
                    case 1:
                       draw_sprite (double_buffer, mesh, dx * 16, dy * 16);
                       break;
                    case 2:
                       hline (double_buffer, dx * 16, dy * 16, dx * 16 + 15,
                              255);
                       vline (double_buffer, dx * 16 + 8, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    case 3:
                       hline (double_buffer, dx * 16, dy * 16 + 8,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16 + 15, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    case 4:
                       hline (double_buffer, dx * 16, dy * 16 + 15,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16 + 8, dy * 16,
                              dy * 16 + 15, 255);
                       break;
                    case 5:
                       hline (double_buffer, dx * 16, dy * 16 + 8,
                              dx * 16 + 15, 255);
                       vline (double_buffer, dx * 16, dy * 16, dy * 16 + 15,
                              255);
                       break;
                    }
               }
             if (draw_mode == 4)
                if (z_map[w] > 0)
                   draw_sprite (double_buffer, z_mesh, dx * 16, dy * 16);
             if (draw_mode == 8)
                if (s_map[w] > 0)
                   draw_trans_sprite (double_buffer, shadow[s_map[w]],
                                      dx * 16, dy * 16);
          }
     }
   if (draw_mode == 7)
     {
        for (d = 0; d < noe; d++)
          {
             if (gent[d].tilex >= gx && gent[d].tilex <= gx + (htiles - 1)
                 && gent[d].tiley >= gy && gent[d].tiley <= gy + (vtiles - 1))
               {
                  if (gent[d].transl == 0)
                     draw_sprite (double_buffer,
                                  eframes[gent[d].chrx][gent[d].facing * 3],
                                  (gent[d].tilex - gx) * 16,
                                  (gent[d].tiley - gy) * 16);
                  else
                     draw_trans_sprite (double_buffer,
                                        eframes[gent[d].chrx][gent[d].facing *
                                                              3],
                                        (gent[d].tilex - gx) * 16,
                                        (gent[d].tiley - gy) * 16);
               }
          }
     }
}

void draw_sidebar (void)
{
   int p, xp, yp, a;
   char dt[11][16] =
      { "Layer1", "Layer1+2", "Layer1+2+3", "Obstacles", "Zones", "Copying",
      "Pasting", "Entity", "Shadows", "Only-2", "Only-3"
   };

   hline (double_buffer, 0, (SH - 48), (SW - 81), 255);
   rectfill (double_buffer, (SW - 72), 0, (SW - 1), (SH - 1), 0);
   rectfill (double_buffer, 0, (SH - 47), (SW - 73), (SH - 1), 0);
   rectfill (double_buffer, (SW - 80), 0, (SW - 73), (SH - 49), 255);
   sprintf (strbuf, "Map: %s", map_fname);
   print_sfont (0, (SH - 46), strbuf, double_buffer);
   sprintf (strbuf, "Icon: %s", icon_files[gmap.tileset]);
   print_sfont (0, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Song: %s", gmap.song_file);
   print_sfont (0, (SH - 34), strbuf, double_buffer);
   if (gmap.zero_zone == 0)
      print_sfont (0, (SH - 28), "ZeroZone: NO", double_buffer);
   else
      print_sfont (0, (SH - 28), "ZeroZone: YES", double_buffer);
   sprintf (strbuf, "Map #: %d", gmap.map_no);
   print_sfont (0, (SH - 22), strbuf, double_buffer);
   sprintf (strbuf, "Mode: %d", gmap.map_mode);
   print_sfont (0, (SH - 16), strbuf, double_buffer);
   if (gmap.can_save == 0)
      print_sfont (0, (SH - 10), "Save: NO", double_buffer);
   else
      print_sfont (0, (SH - 10), "Save: YES", double_buffer);
   if (gmap.can_warp == 0)
      print_sfont (80, (SH - 22), "Warp: NO", double_buffer);
   else
      print_sfont (80, (SH - 22), "Warp: YES", double_buffer);
   sprintf (strbuf, "WarpX: %d", gmap.warpx);
   print_sfont (80, (SH - 16), strbuf, double_buffer);
   sprintf (strbuf, "WarpY: %d", gmap.warpy);
   print_sfont (80, (SH - 10), strbuf, double_buffer);
   sprintf (strbuf, "Start X: %d", gmap.stx);
   print_sfont (160, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Start Y: %d", gmap.sty);
   print_sfont (160, (SH - 34), strbuf, double_buffer);
   sprintf (strbuf, "Width: %d", gmap.xsize);
   print_sfont (160, (SH - 28), strbuf, double_buffer);
   sprintf (strbuf, "Height: %d", gmap.ysize);
   print_sfont (160, (SH - 22), strbuf, double_buffer);
   sprintf (strbuf, "Mult: %d", gmap.pmult);
   print_sfont (240, (SH - 40), strbuf, double_buffer);
   sprintf (strbuf, "Div: %d", gmap.pdiv);
   print_sfont (240, (SH - 34), strbuf, double_buffer);
   if (gmap.use_sstone == 0)
      print_sfont (160, (SH - 16), "SunStone: NO", double_buffer);
   else
      print_sfont (160, (SH - 16), "SunStone: YES", double_buffer);
   a = 0;
   for (p = 0; p < gmap.xsize * gmap.ysize; p++)
      if (z_map[p] > a)
         a = z_map[p];
   sprintf (strbuf, "Last zone: %d", a);
   print_sfont (160, (SH - 10), strbuf, double_buffer);
   if (icon_set != 999)
     {
        for (p = 0; p < 10; p++)
          {
             blit (icons[icon_set * 20 + p], double_buffer, 0, 0, (SW - 64),
                   p * 16, 16, 16);
             blit (icons[icon_set * 20 + p + 10], double_buffer, 0, 0,
                   (SW - 48), p * 16, 16, 16);
          }
     }
   if (curtile >= icon_set * 20 && curtile <= icon_set * 20 + 19)
     {
        xp = curtile - (icon_set * 20);
        yp = xp;
        xp = xp / 10;
        yp = yp - (xp * 10);
        rect (double_buffer, xp * 16 + (SW - 64), yp * 16,
              xp * 16 + (SW - 49), yp * 16 + 15, 255);
     }
   rectfill (double_buffer, (SW - 72), 164, (SW - 1), (SH - 1), 0);
   print_sfont ((SW - 72), 164, "Mode:", double_buffer);
   print_sfont ((SW - 64), 170, dt[draw_mode], double_buffer);
   sprintf (strbuf, "#%d(%d)", icon_set, curtile);
   print_sfont ((SW - 72), 176, strbuf, double_buffer);
   sprintf (strbuf, "x=%d", gx + x);
   print_sfont ((SW - 72), 182, strbuf, double_buffer);
   sprintf (strbuf, "y=%d", gy + y);
   print_sfont ((SW - 72), 188, strbuf, double_buffer);
   if (dmode == 1)
      print_sfont ((SW - 64), 194, "drawing", double_buffer);
   if (draw_mode == 3)
     {
        sprintf (strbuf, "Obs #%d", curobs);
        print_sfont (104, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = o_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (200, (SH - 46), strbuf, double_buffer);
          }
     }
   if (draw_mode == 4)
     {
        sprintf (strbuf, "Zone #%d", curzone);
        print_sfont (104, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = z_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (200, (SH - 46), strbuf, double_buffer);
          }
     }
   if (draw_mode == 5 && copying == 1)
     {
        sprintf (strbuf, "From: %d,%d", copyx, copyy);
        print_sfont (100, (SH - 46), strbuf, double_buffer);
     }
   if (draw_mode == 7)
     {
        sprintf (strbuf, "%d", cent);
        print_sfont ((SW - 14), (SH - 38), strbuf, double_buffer);
        blit (eframes[cent][0], double_buffer, 0, 0, (SW - 16), (SH - 32), 16,
              16);
        sprintf (strbuf, "%d", noe);
        print_sfont ((SW - 14), (SH - 12), strbuf, double_buffer);
     }
   if (draw_mode == 8)
     {
        sprintf (strbuf, "Shd. #%d", curshadow);
        print_sfont (140, (SH - 46), strbuf, double_buffer);
        xp = mouse_x / 16;
        yp = mouse_y / 16;
        if (xp < htiles && yp < vtiles)
          {
             p = s_map[((gy + yp) * gmap.xsize) + gx + xp];
             sprintf (strbuf, "-> %d", p);
             print_sfont (200, (SH - 46), strbuf, double_buffer);
          }
     }
   rect (double_buffer, x * 16, y * 16, x * 16 + 15, y * 16 + 15, 255);
}

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
   free (s_map);
   s_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
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
   memset (s_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   clipb = 0;
}

void load_map (void)
{
   char fname[16];
   int response, ld, p, q;
   PACKFILE *pf;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Load a map", screen);
   sprintf (strbuf, "Current: %s", map_fname);
   print_sfont (6, 12, strbuf, screen);
   print_sfont (6, 18, "Filename: ", screen);
   ld = get_line (66, 18, fname, 13);
   if (ld != 0)
     {
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        rectfill (screen, 0, 0, 319, 17, 0);
        rect (screen, 2, 2, 317, 15, 255);
        sprintf (strbuf, "Load %s? (y/n)", fname);
        print_sfont (6, 6, strbuf, screen);
        response = yninput ();
        if (response)
          {
             strcpy (map_fname, fname);
             pf = pack_fopen (fname, F_READ_PACKED);
             pack_fread (&gmap, sizeof (ss_map), pf);
             pack_fread (&gent, sizeof (s_entity) * 50, pf);
             bufferize ();
             pack_fread (map, (gmap.xsize * gmap.ysize * 2), pf);
             pack_fread (b_map, (gmap.xsize * gmap.ysize * 2), pf);
             pack_fread (f_map, (gmap.xsize * gmap.ysize * 2), pf);
             pack_fread (z_map, (gmap.xsize * gmap.ysize), pf);
             pack_fread (s_map, (gmap.xsize * gmap.ysize), pf);
             pack_fread (o_map, (gmap.xsize * gmap.ysize), pf);
             pack_fclose (pf);
             pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
             max_sets = (pcx_buffer->h / 16);
             for (p = 0; p < max_sets; p++)
                for (q = 0; q < 20; q++)
                   blit (pcx_buffer, icons[p * 20 + q], q * 16, p * 16, 0, 0,
                         16, 16);
             icon_set = 0;
             destroy_bitmap (pcx_buffer);
          }
     }
   noe = 0;
   for (p = 0; p < 50; p++)
      if (gent[p].active == 1)
         noe = p + 1;
}

void save_map (void)
{
   char fname[16];
   int response, ld;
   PACKFILE *pf;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Save a map", screen);
   sprintf (strbuf, "Current: %s", map_fname);
   print_sfont (6, 12, strbuf, screen);
   print_sfont (6, 18, "Filename: ", screen);
   ld = get_line (66, 18, fname, 13);
   if (ld != 0)
     {
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        rectfill (screen, 0, 0, 319, 17, 0);
        rect (screen, 2, 2, 317, 15, 255);
        sprintf (strbuf, "Save %s? (y/n)", fname);
        print_sfont (6, 6, strbuf, screen);
        response = yninput ();
        if (response)
          {
             strcpy (map_fname, fname);
             pf = pack_fopen (fname, F_WRITE_PACKED);
             pack_fwrite (&gmap, sizeof (ss_map), pf);
             pack_fwrite (&gent, sizeof (s_entity) * 50, pf);
             pack_fwrite (map, (gmap.xsize * gmap.ysize * 2), pf);
             pack_fwrite (b_map, (gmap.xsize * gmap.ysize * 2), pf);
             pack_fwrite (f_map, (gmap.xsize * gmap.ysize * 2), pf);
             pack_fwrite (z_map, (gmap.xsize * gmap.ysize), pf);
             pack_fwrite (s_map, (gmap.xsize * gmap.ysize), pf);
             pack_fwrite (o_map, (gmap.xsize * gmap.ysize), pf);
             pack_fclose (pf);
             cmessage ("Map saved!");
             wait_enter ();
          }
     }
}

void make_mapfrompcx (void)
{
   char fname[16];
   int response, res2, ld;
   int w, h, ax, ay;
   BITMAP *pb;

   rectfill (screen, 0, 0, 319, 21, 0);
   rect (screen, 2, 2, 317, 19, 255);
   print_sfont (6, 6, "Make map from pcx", screen);
   print_sfont (6, 12, "Filename: ", screen);
   ld = get_line (66, 12, fname, 13);
   if (ld != 0)
     {
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        rectfill (screen, 0, 0, 319, 17, 0);
        rect (screen, 2, 2, 317, 15, 255);
        sprintf (strbuf, "Load %s? (y/n)", fname);
        print_sfont (6, 6, strbuf, screen);
        response = yninput ();
        if (response)
          {
             blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
             rectfill (screen, 0, 0, 319, 17, 0);
             rect (screen, 2, 2, 317, 15, 255);
             print_sfont (6, 6, "Put to background? (y/n) - n=foreground",
                          screen);
             res2 = yninput ();
             pb = load_pcx (fname, pal);
             if (pb->w <= gmap.xsize)
                w = pb->w;
             else
                w = gmap.xsize;
             if (pb->h <= gmap.ysize)
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
     }
}

void global_change (void)
{
   int ld, ft = 0, tt, p;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "From Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld != 0)
      ft = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "To Tile", screen);
   print_sfont (6, 18, "Tile num:", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld != 0)
     {
        tt = atoi (strbuf);
        for (p = 0; p < (gmap.xsize * gmap.ysize); p++)
           if (map[p] == ft)
              map[p] = tt;
     }
}

void clear_obstruct (void)
{
   int co;

   for (co = 0; co < gmap.xsize * gmap.ysize; co++)
      o_map[co] = 0;
}

void clear_foreground (void)
{
   int co;

   for (co = 0; co < gmap.xsize * gmap.ysize; co++)
      f_map[co] = 0;
}

int get_line (int gx, int gy, char *buffer, int max_len)
{
   int index = 0, ch;
   BITMAP *under;

   under = create_bitmap (320, 6);
   blit (screen, under, 0, gy, 0, 0, 320, 6);
   while (1)
     {
        ch = (readkey () & 0xff);
        if (ch >= 32 && ch <= 126 && index < max_len - 1)
          {
             buffer[index] = ch;
             buffer[index + 1] = 0;
             print_sfont (gx, gy, buffer, screen);
             if (++index == max_len)
                index = max_len - 1;
          }
        else
          {
             if (ch == 13)
               {
                  buffer[index] = 0;
                  destroy_bitmap (under);
                  return (1);
               }
             else
               {
                  if (ch == 8 || ch == 127)
                    {
                       if (--index < 0)
                          index = 0;
                       buffer[index] = ' ';
                       buffer[index + 1] = 0;
                       blit (under, screen, 0, 0, 0, gy, 320, 8);
                       print_sfont (gx, gy, buffer, screen);
                       buffer[index] = 0;
                    }
                  else
                    {
                       if (ch == 27)
                         {
                            destroy_bitmap (under);
                            return (0);
                         }
                    }
               }
          }
     }
}

int yninput (void)
{
   int ch, done = 0;

   while (!done)
     {
        ch = (readkey () >> 8);
        if (ch == KEY_N)
           done = 1;
        if (ch == KEY_Y)
           done = 2;
     }
   return done - 1;
}

void startup (void)
{
   int k, kx, ky, a, p, q;
   COLOR_MAP cmap;

   allegro_init ();
   install_keyboard ();
   install_timer ();
   if (WBUILD == 1)
      set_gfx_mode (GFX_AUTODETECT_WINDOWED, SW, SH, 0, 0);
   else
      set_gfx_mode (GFX_AUTODETECT, SW, SH, 0, 0);
   a = install_mouse ();
   if (a == -1)
     {
        nomouse = 1;
        klog ("Mouse not found!\n");
        rest (1000);
     }
   mpic = create_bitmap (4, 6);
   for (ky = 0; ky < 6; ky++)
      for (kx = 0; kx < 4; kx++)
         mpic->line[ky][kx] = mousepic[ky * 4 + kx];
   set_mouse_speed (4, 4);
   strbuf = (char *) malloc (256);
   double_buffer = create_bitmap (SW, SH);
   clear (double_buffer);
   gmap.map_no = -1;
   gmap.tileset = 0;
   gmap.xsize = htiles;
   gmap.ysize = vtiles;
   set_palette (pal);
   for (k = 0; k < MAX_TILES; k++)
     {
        icons[k] = create_bitmap (16, 16);
        clear (icons[k]);
     }
   pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
   max_sets = (pcx_buffer->h / 16);
   for (p = 0; p < max_sets; p++)
      for (q = 0; q < 20; q++)
         blit (pcx_buffer, icons[p * 20 + q], q * 16, p * 16, 0, 0, 16, 16);
   icon_set = 0;
   destroy_bitmap (pcx_buffer);
   bufferize ();
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;
   font6 = create_bitmap (6, 546);
   getfont ();
   mesh = create_bitmap (16, 16);
   clear (mesh);
   for (ky = 0; ky < 16; ky += 2)
     {
        for (kx = 0; kx < 16; kx += 2)
           putpixel (mesh, kx, ky, 255);
        for (kx = 1; kx < 16; kx += 2)
           putpixel (mesh, kx, ky + 1, 255);
     }
   b_mesh = create_bitmap (16, 16);
   clear (b_mesh);
   for (ky = 0; ky < 16; ky += 2)
     {
        for (kx = 0; kx < 16; kx += 2)
           putpixel (b_mesh, kx, ky, 1);
        for (kx = 1; kx < 16; kx += 2)
           putpixel (b_mesh, kx, ky + 1, 1);
     }
   z_mesh = create_bitmap (16, 16);
   clear (z_mesh);
   hline (z_mesh, 0, 7, 15, 255);
   hline (z_mesh, 0, 8, 15, 255);
   vline (z_mesh, 7, 0, 15, 255);
   vline (z_mesh, 8, 0, 15, 255);
   print_sfont (5, 5, "Z", z_mesh);
   pcx_buffer = load_pcx ("Misc.pcx", pal);
   for (a = 0; a < MAX_SHADOWS; a++)
     {
        shadow[a] = create_bitmap (16, 16);
        blit (pcx_buffer, shadow[a], a * 16, 160, 0, 0, 16, 16);
     }
   destroy_bitmap (pcx_buffer);
   pcx_buffer = load_pcx ("entities.pcx", pal);
   for (k = 0; k < MAX_EPICS; k++)
     {
        for (a = 0; a < 12; a++)
          {
             eframes[k][a] = create_bitmap (16, 16);
             blit (pcx_buffer, eframes[k][a], a * 16, k * 16, 0, 0, 16, 16);
          }
     }
   destroy_bitmap (pcx_buffer);
   init_entities ();
}

void cleanup (void)
{
   int k, j;

   for (k = 0; k < MAX_TILES; k++)
      destroy_bitmap (icons[k]);
   for (k = 0; k < MAX_EPICS; k++)
      for (j = 0; j < 12; j++)
         destroy_bitmap (eframes[k][j]);
   destroy_bitmap (double_buffer);
   free (map);
   free (f_map);
   free (z_map);
   free (s_map);
   free (o_map);
   free (c_map);
   free (cf_map);
   free (cz_map);
   free (cs_map);
   free (co_map);
   free (strbuf);
   destroy_bitmap (mesh);
   destroy_bitmap (b_mesh);
   destroy_bitmap (z_mesh);
   for (k = 0; k < MAX_SHADOWS; k++)
      destroy_bitmap (shadow[k]);
}

void new_map (void)
{
   char fname[16];
   int response, ld, p, q, ai = 0;

   gmap.map_no = 0;
   gmap.zero_zone = 0;

   gmap.map_mode = 0;
   gmap.can_save = 1;
   gmap.use_sstone = 1;
   gmap.pmult = 1;
   gmap.pdiv = 1;
   gmap.tileset = 0;
   gmap.song_file[0] = 0;
   gmap.map_desc[0] = 0;
   gmap.stx = 0;
   gmap.sty = 0;
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "New map", screen);
   print_sfont (6, 18, "Width: ", screen);
   ld = get_line (48, 18, strbuf, 4);
   if (ld != 0)
      ai = atoi (strbuf);
   if (ai < 20 || ai > 640)
     {
        cmessage ("Illegal width!");
        wait_enter ();
        return;
     }
   gmap.xsize = ai;
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "New map", screen);
   print_sfont (6, 18, "Height: ", screen);
   ld = get_line (54, 18, strbuf, 4);
   if (ld != 0)
      ai = atoi (strbuf);
   if (ai < 15 || ai > 480)
     {
        cmessage ("Illegal height!");
        wait_enter ();
        return;
     }
   gmap.ysize = ai;
   bufferize ();
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "New map", screen);
   print_sfont (6, 12, "Choose a tile set. ", screen);
   print_sfont (6, 18, "TileSet#: ", screen);
   ld = get_line (66, 18, fname, 13);
   if (ld != 0)
     {
        gmap.tileset = atoi (fname);
        blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
        rectfill (screen, 0, 0, 319, 17, 0);
        rect (screen, 2, 2, 317, 15, 255);
        sprintf (strbuf, "Load %s? (y/n)", fname);
        print_sfont (6, 6, strbuf, screen);
        response = yninput ();
        if (response)
          {
             pcx_buffer = load_pcx (icon_files[gmap.tileset], pal);
             max_sets = (pcx_buffer->h / 16);
             for (p = 0; p < max_sets; p++)
                for (q = 0; q < 20; q++)
                   blit (pcx_buffer, icons[p * 20 + q], q * 16, p * 16, 0, 0,
                         16, 16);
             icon_set = 0;
             destroy_bitmap (pcx_buffer);
          }
     }
   init_entities ();
}

void init_entities (void)
{
   int p;

   for (p = 0; p < 50; p++)
     {
        gent[p].chrx = 0;
        gent[p].x = 0;
        gent[p].y = 0;
        gent[p].tilex = 0;
        gent[p].tiley = 0;
        gent[p].id = 0;
        gent[p].active = 0;
        gent[p].facing = 0;
        gent[p].moving = 0;
        gent[p].movcnt = 0;
        gent[p].framectr = 0;
        gent[p].movemode = 0;
        gent[p].obsmode = 0;
        gent[p].delay = 0;
        gent[p].delayctr = 0;
        gent[p].speed = 0;
        gent[p].scount = 0;
        gent[p].extra = 0;
        gent[p].cmd = 0;
        gent[p].sidx = 0;
        gent[p].cmdnum = 0;
        gent[p].chasing = 0;
        gent[p].atype = 0;
        strcpy (gent[p].script, "");
     }
}

void wait_enter (void)
{
   while ((readkey () >> 8) != KEY_ENTER);
}

void cmessage (char *buff)
{
   int cx;

   cx = strlen (buff) * 3;
   rectfill (screen, 154 - cx, 91, 165 + cx, 108, 0);
   rect (screen, 154 - cx, 91, 165 + cx, 108, 255);
   print_sfont (160 - cx, 97, buff, screen);
}

void paste_region (int tx, int ty)
{
   int zx, zy, bf;

   if (clipb == 0)
      return;
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             if (ty + zy <= gmap.ysize && tx + zx <= gmap.xsize)
               {
                  bf = c_map[zy * gmap.xsize + zx];
                  map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cb_map[zy * gmap.xsize + zx];
                  b_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cf_map[zy * gmap.xsize + zx];
                  f_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cz_map[zy * gmap.xsize + zx];
                  z_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = cs_map[zy * gmap.xsize + zx];
                  s_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
                  bf = co_map[zy * gmap.xsize + zx];
                  o_map[(ty + zy) * gmap.xsize + tx + zx] = bf;
               }
          }
     }
}

void copy_region (int tx, int ty)
{
   int zx, zy, bf;

   clipb = 0;
   cbh = ty - copyy;
   cbw = tx - copyx;
   if (cbh < 1 && cbw < 1)
      return;
   clipb = 1;
   for (zy = 0; zy <= cbh; zy++)
     {
        for (zx = 0; zx <= cbw; zx++)
          {
             bf = map[(copyy + zy) * gmap.xsize + copyx + zx];
             c_map[zy * gmap.xsize + zx] = bf;
             bf = b_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cb_map[zy * gmap.xsize + zx] = bf;
             bf = f_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cf_map[zy * gmap.xsize + zx] = bf;
             bf = z_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cz_map[zy * gmap.xsize + zx] = bf;
             bf = s_map[(copyy + zy) * gmap.xsize + copyx + zx];
             cs_map[zy * gmap.xsize + zx] = bf;
             bf = o_map[(copyy + zy) * gmap.xsize + copyx + zx];
             co_map[zy * gmap.xsize + zx] = bf;
          }
     }
}

void resize_map (void)
{
   int ld, nw, nh, zx, zy, bf;
   int ow, oh;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Width: ", screen);
   ld = get_line (48, 18, strbuf, 4);
   if (ld != 0)
      nw = atoi (strbuf);
   else
      return;
   if (nw < 20 || nw > 640)
     {
        cmessage ("Illegal width!");
        wait_enter ();
        return;
     }
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Resize map", screen);
   print_sfont (6, 18, "Height: ", screen);
   ld = get_line (54, 18, strbuf, 4);
   if (ld != 0)
      nh = atoi (strbuf);
   else
      return;
   if (nh < 15 || nh > 480)
     {
        cmessage ("Illegal height!");
        wait_enter ();
        return;
     }
   for (zy = 0; zy < gmap.ysize; zy++)
     {
        for (zx = 0; zx < gmap.xsize; zx++)
          {
             bf = map[zy * gmap.xsize + zx];
             c_map[zy * gmap.xsize + zx] = bf;
             bf = b_map[zy * gmap.xsize + zx];
             cb_map[zy * gmap.xsize + zx] = bf;
             bf = f_map[zy * gmap.xsize + zx];
             cf_map[zy * gmap.xsize + zx] = bf;
             bf = z_map[zy * gmap.xsize + zx];
             cz_map[zy * gmap.xsize + zx] = bf;
             bf = s_map[zy * gmap.xsize + zx];
             cs_map[zy * gmap.xsize + zx] = bf;
             bf = o_map[zy * gmap.xsize + zx];
             co_map[zy * gmap.xsize + zx] = bf;
          }
     }
   ow = gmap.xsize;
   oh = gmap.ysize;
   gmap.xsize = nw;
   gmap.ysize = nh;
   free (map);
   map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (f_map);
   f_map = (unsigned short *) malloc (gmap.xsize * gmap.ysize * 2);
   free (z_map);
   z_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (s_map);
   s_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   free (o_map);
   o_map = (unsigned char *) malloc (gmap.xsize * gmap.ysize);
   memset (map, 0, gmap.xsize * gmap.ysize * 2);
   memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (z_map, 0, gmap.xsize * gmap.ysize);
   memset (s_map, 0, gmap.xsize * gmap.ysize);
   memset (o_map, 0, gmap.xsize * gmap.ysize);
   for (zy = 0; zy < oh; zy++)
     {
        for (zx = 0; zx < ow; zx++)
          {
             if (zy < gmap.ysize && zx < gmap.xsize)
               {
                  bf = c_map[zy * ow + zx];
                  map[zy * gmap.xsize + zx] = bf;
                  bf = cb_map[zy * ow + zx];
                  b_map[zy * gmap.xsize + zx] = bf;
                  bf = cf_map[zy * ow + zx];
                  f_map[zy * gmap.xsize + zx] = bf;
                  bf = cz_map[zy * ow + zx];
                  z_map[zy * gmap.xsize + zx] = bf;
                  bf = cs_map[zy * ow + zx];
                  s_map[zy * gmap.xsize + zx] = bf;
                  bf = co_map[zy * ow + zx];
                  o_map[zy * gmap.xsize + zx] = bf;
               }
          }
     }
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
   memset (c_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cb_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cf_map, 0, gmap.xsize * gmap.ysize * 2);
   memset (cz_map, 0, gmap.xsize * gmap.ysize);
   memset (cs_map, 0, gmap.xsize * gmap.ysize);
   memset (co_map, 0, gmap.xsize * gmap.ysize);
   clipb = 0;
}

void displace_entities (void)
{
   int ld, nx, ny, a;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Displace entities", screen);
   print_sfont (6, 18, "X adjust: ", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld != 0)
      nx = atoi (strbuf);
   else
      return;
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Displace entities", screen);
   print_sfont (6, 18, "Y adjust: ", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld != 0)
      ny = atoi (strbuf);
   else
      return;
   for (a = 0; a < noe; a++)
     {
        if (gent[a].tilex + nx < gmap.xsize && gent[a].tilex + nx >= 0)
          {
             gent[a].tilex += nx;
             gent[a].x = gent[a].tilex * 16;
          }
        if (gent[a].tiley + ny < gmap.ysize && gent[a].tiley + ny >= 0)
          {
             gent[a].tiley += ny;
             gent[a].y = gent[a].tiley * 16;
          }
     }
}


void wipe_map (void)
{
   int r;

   rectfill (screen, 0, 0, 319, 17, 0);
   rect (screen, 2, 2, 317, 15, 255);
   print_sfont (6, 6, "Really clear the whole map? (y/n", screen);
   r = yninput ();
   if (r)
     {
        memset (map, 0, gmap.xsize * gmap.ysize * 2);
        memset (b_map, 0, gmap.xsize * gmap.ysize * 2);
        memset (f_map, 0, gmap.xsize * gmap.ysize * 2);
        memset (z_map, 0, gmap.xsize * gmap.ysize);
        memset (s_map, 0, gmap.xsize * gmap.ysize);
        memset (o_map, 0, gmap.xsize * gmap.ysize);
     }
}

void maptopcx (void)
{
   BITMAP *pf, *pb, *pm;
   int jx, jy;

   pb = create_bitmap (gmap.xsize, gmap.ysize);
   pm = create_bitmap (gmap.xsize, gmap.ysize);
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

void place_entity (int ex, int ey)
{
   int a;

   if (noe >= 50)
      return;
   while (mouse_b & 1)
     {
        a++;
        a--;
     }
   gent[noe].chrx = cent;
   gent[noe].tilex = ex;
   gent[noe].tiley = ey;
   gent[noe].x = ex * 16;
   gent[noe].y = ey * 16;
   gent[noe].active = 1;
   gent[noe].id = 255;
   gent[noe].movemode = 0;
   gent[noe].delay = 50;
   gent[noe].speed = 3;
   gent[noe].obsmode = 1;
   gent[noe].facing = 0;
   strcpy (gent[noe].script, "");
   noe++;
}

void erase_entity (int pex, int pey)
{
   int a, enn = -1;

   if (noe == 0)
      return;
   while (mouse_b & 2)
     {
        a++;
        a--;
     }
   for (a = 0; a < noe; a++)
      if (gent[a].tilex == pex && gent[a].tiley == pey)
         enn = a;
   if (enn == -1)
      return;
   gent[enn].active = 0;
   for (a = enn + 1; a < noe; a++)
     {
        gent[a - 1] = gent[a];
        gent[a].active = 0;
     }
   noe--;
}

void update_entities (void)
{
   int stop = 0, et = 0, c, a;
   int tgx = gx, tgy = gy, tdm = draw_mode;
   draw_mode = 7;
   if (noe == 0)
     {
        cmessage ("No entities!");
        wait_enter ();
        return;
     }
   while (!stop)
     {
        draw_entdata (et);
        c = (readkey () >> 8);
        if (c == KEY_ESC)
           stop = 1;
        if (c == KEY_UP)
          {
             et++;
             if (et == noe)
                et = 0;
          }
        if (c == KEY_DOWN)
          {
             et--;
             if (et < 0)
                et = noe - 1;
          }
        if (c == KEY_1)
          {
             gent[et].chrx++;
             if (gent[et].chrx == MAX_EPICS)
                gent[et].chrx = 0;
          }
        if (c == KEY_2)
          {
             rectfill (screen, 48, SH - 46, 71, SH - 41, 0);
             hline (screen, 48, SH - 41, 71, 255);
             a = get_line (48, SH - 46, strbuf, 4);
             if (a != 0)
               {
                  gent[et].tilex = atoi (strbuf);
                  gent[et].x = gent[et].tilex * 16;
               }
          }
        if (c == KEY_3)
          {
             rectfill (screen, 48, SH - 40, 71, SH - 35, 0);
             hline (screen, 48, SH - 35, 71, 255);
             a = get_line (48, SH - 40, strbuf, 4);
             if (a != 0)
               {
                  gent[et].tiley = atoi (strbuf);
                  gent[et].y = gent[et].tiley * 16;
               }
          }
        if (c == KEY_4)
          {
             gent[et].movemode++;
             if (gent[et].movemode > 3)
                gent[et].movemode = 0;
          }
        if (c == KEY_5)
          {
             rectfill (screen, 72, SH - 28, 95, SH - 23, 0);
             hline (screen, 72, SH - 23, 95, 255);
             a = get_line (72, SH - 28, strbuf, 4);
             if (a != 0)
                gent[et].delay = atoi (strbuf);
          }
        if (c == KEY_6)
          {
             gent[et].speed++;
             if (gent[et].speed > 7)
                gent[et].speed = 1;
          }
        if (c == KEY_7)
          {
             if (gent[et].obsmode == 0)
                gent[et].obsmode = 1;
             else
                gent[et].obsmode = 0;
          }
        if (c == KEY_8)
          {
             gent[et].facing++;
             if (gent[et].facing > 3)
                gent[et].facing = 0;
          }
        if (c == KEY_9)
          {
             gent[et].atype++;
             if (gent[et].atype > 1)
                gent[et].atype = 0;
          }
        if (c == KEY_0)
          {
             rectfill (screen, 24, SH - 10, 319, SH - 5, 0);
             hline (screen, 24, SH - 5, 319, 255);
             a = get_line (24, SH - 10, strbuf, 61);
             if (a != 0)
                strcpy (gent[et].script, strbuf);
          }
        if (c == KEY_S)
          {
             gent[et].snapback++;
             if (gent[et].snapback > 1)
                gent[et].snapback = 0;
          }
        if (c == KEY_F)
          {
             gent[et].facehero++;
             if (gent[et].facehero > 1)
                gent[et].facehero = 0;
          }
        if (c == KEY_T)
          {
             gent[et].transl++;
             if (gent[et].transl > 1)
                gent[et].transl = 0;
          }
     }
   gx = tgx;
   gy = tgy;
   draw_mode = tdm;
}

void draw_entdata (int en)
{
   int a, tdx, tdy;

   gx = gent[en].tilex - (htiles / 2);
   gy = gent[en].tiley - (vtiles / 2);
   if (gx > gmap.xsize - 1 - htiles)
      gx = gmap.xsize - 1 - htiles;
   if (gy > gmap.ysize - 1 - vtiles)
      gy = gmap.ysize - 1 - vtiles;
   if (gx < 0)
      gx = 0;
   if (gy < 0)
      gy = 0;
   clear (double_buffer);
   draw_map ();
   tdx = (gent[en].tilex - gx) * 16;
   tdy = (gent[en].tiley - gy) * 16;
   rect (double_buffer, tdx - 1, tdy - 1, tdx + 16, tdy + 16, 255);
   hline (double_buffer, 0, SH - 48, SW - 80, 255);
   vline (double_buffer, SW - 80, 0, SH - 48, 255);
   sprintf (strbuf, "Total: %d", noe);
   print_sfont (SW - 78, 0, strbuf, double_buffer);
   sprintf (strbuf, "Current: %d", en);
   print_sfont (SW - 78, 6, strbuf, double_buffer);
   print_sfont (160, SH - 46, "1 - change entity sprite", double_buffer);
   print_sfont (160, SH - 40, "8 - change entity facing", double_buffer);
   if (gent[en].obsmode == 0)
      strcpy (strbuf, "7 - Ignore obstructions");
   else
      strcpy (strbuf, "7 - Obstructive");
   print_sfont (160, SH - 34, strbuf, double_buffer);
   sprintf (strbuf, "x = %d", gent[en].tilex);
   print_sfont (24, SH - 46, strbuf, double_buffer);
   sprintf (strbuf, "y = %d", gent[en].tiley);
   print_sfont (24, SH - 40, strbuf, double_buffer);
   switch (gent[en].movemode)
     {
     case 0:
        strcpy (strbuf, "stand (0)");
        break;
     case 1:
        strcpy (strbuf, "wander (1)");
        break;
     case 2:
        strcpy (strbuf, "script (2)");
        break;
     case 3:
        strcpy (strbuf, "chase (3)");
        break;
     }
   print_sfont (24, SH - 34, strbuf, double_buffer);
   sprintf (strbuf, "delay = %d", gent[en].delay);
   print_sfont (24, SH - 28, strbuf, double_buffer);
   sprintf (strbuf, "speed = %d", gent[en].speed);
   print_sfont (24, SH - 22, strbuf, double_buffer);
   if (gent[en].atype == 0)
      sprintf (strbuf, "9 - atype = %d (normal)", gent[en].atype);
   else
      sprintf (strbuf, "9 - atype = %d (constant)", gent[en].atype);
   print_sfont (0, SH - 16, strbuf, double_buffer);
   print_sfont (24, SH - 10, gent[en].script, double_buffer);
   for (a = 1; a < 6; a++)
     {
        sprintf (strbuf, "%d -", a + 1);
        print_sfont (0, a * 6 + (SH - 52), strbuf, double_buffer);
     }
   print_sfont (0, SH - 10, "0 -", double_buffer);
   if (gent[en].snapback == 0)
      strcpy (strbuf, "S - Snap back");
   else
      strcpy (strbuf, "S - Don't snap back");
   print_sfont (160, SH - 28, strbuf, double_buffer);
   if (gent[en].facehero == 0)
      strcpy (strbuf, "F - Face hero");
   else
      strcpy (strbuf, "F - Don't face hero");
   print_sfont (160, SH - 22, strbuf, double_buffer);
   if (gent[en].transl == 0)
      strcpy (strbuf, "T - Opaque");
   else
      strcpy (strbuf, "T - Translucent");
   print_sfont (160, SH - 16, strbuf, double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
}

void describe_map (void)
{
   int ld;
   char tcd[39];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Enter map description", screen);
   sprintf (strbuf, "Current: %s", gmap.map_desc);
   print_sfont (6, 12, strbuf, screen);
   print_sfont (6, 18, "Filename: ", screen);
   ld = get_line (66, 18, tcd, 38);
   if (ld != 0)
      strcpy (gmap.map_desc, tcd);
}

void swap_layers (void)
{
   int rt, rt2, ff, tt, a, b;
   char ts[4];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Copy Layer to Layer", screen);
   print_sfont (6, 12, "From (1-3):", screen);
   rt = get_line (78, 12, ts, 2);
   if (rt != 0)
     {
        ff = atoi (ts);
        print_sfont (6, 18, "To (1-3):", screen);
        rt2 = get_line (78, 18, ts, 2);
        if (rt2 != 0)
          {
             tt = atoi (ts);
             for (a = 0; a < gmap.xsize * gmap.ysize; a++)
               {
                  if (ff == 1)
                     b = map[a];
                  else
                    {
                       if (ff == 2)
                          b = b_map[a];
                       else
                          b = f_map[a];
                    }
                  if (tt == 1)
                     map[a] = b;
                  else
                    {
                       if (tt == 2)
                          b_map[a] = b;
                       else
                          f_map[a] = b;
                    }
               }
          }
     }
}

void clear_layer (void)
{
   int rt, ml, a;
   char ts[4];

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Clear Layer", screen);
   print_sfont (6, 12, "Layer (1-3):", screen);
   rt = get_line (84, 12, ts, 2);
   if (rt != 0)
     {
        ml = atoi (ts);
        for (a = 0; a < gmap.xsize * gmap.ysize; a++)
          {
             if (ml == 1)
                map[a] = 0;
             else
               {
                  if (ml == 2)
                     b_map[a] = 0;
                  else
                     f_map[a] = 0;
               }
          }
     }
}
void print_sfont (int x, int y, char *string, BITMAP * where)
{
   int i, c;

   for (i = 0; i < (signed)strlen (string); i++)
     {
        c = string[i];
        c -= 32;
        if (c < 0)
           c = 0;
        masked_blit (font6, where, 0, c * 6, i * 6 + x, y, 6, 6);
     }
}

void getfont (void)
{
   int a, b;
   int k = 15;
   int z = 9;
   unsigned char tf[3276] = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,       /* space */
      0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, 0, k, 0, 0, k, k, k, k, k, 0, 0, k, 0, k, 0, 0, k, k, k, k, k, 0,
         0, k, 0, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, k, 0, 0, 0, 0, k, k, k, 0, 0, 0, k, 0, k, 0, 0,
         k, k, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, 0, 0, k, 0, k, k, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, k, k, 0,
         k, 0, 0, k, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, 0, 0, 0, 0, k, 0, k, 0, 0, 0, 0, k, k, 0, k, 0, k, 0, 0, k, 0, 0,
         0, k, k, 0, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, k, 0, k, 0, 0, k, k, k, 0, 0, k, k, k, k, k, 0, 0, k, k, k, 0, 0,
         k, 0, k, 0, k, 0, 0, 0, 0, 0, 0, 0,
      0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, k, k, k, k, k, 0, 0, 0, k, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, k, k, k, k, k, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0,
         k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, k, k, 0, k, 0, k, 0, k, 0, k, k, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, k, 0, 0, 0, 0, k, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, 0, 0, k, k, 0, 0, 0, k, 0, 0, 0, 0,
         k, k, k, k, k, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, k, k, 0, 0, 0, 0, 0, 0, k, 0,
         k, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, k, 0, 0, k, 0, 0, k, 0, 0, k, k, k, k, k, 0, 0, 0, 0, k, 0, 0,
         0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, k, 0, 0, 0, 0, 0, k, k, k, k, 0, 0, 0, 0, 0, 0, k, 0,
         k, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, 0, 0, k, k, k, k, 0, 0, k, 0, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, 0, k, k, k, 0, 0, k, 0, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, 0, k, k, k, k, 0, 0, 0, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, k, k, k, k, k, 0, 0, 0, 0, 0, 0, 0, k, k, k, k, k, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0,
         0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, 0, 0, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, k, k, k, k, 0,
         k, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, k, k, k, 0, 0, k, 0, 0, 0, k, 0,
         k, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, k, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0,
         0, k, k, k, k, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0,
         k, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, k, 0, 0, 0, 0, 0, k, k, k, 0, 0, 0, k, 0, 0, 0, 0, 0,
         k, k, k, k, k, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, k, 0, 0, 0, 0, 0, k, k, k, 0, 0, 0, k, 0, 0, 0, 0, 0,
         k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, k, 0, k, 0, 0, 0, 0, 0, k, 0, 0, k, k, 0, k, 0, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, k, k, k, k, 0, k, 0, 0, 0, k, 0,
         k, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0,
         k, k, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, k, 0, 0, k, 0, k, 0, 0, 0, k, k, 0, 0, 0, 0, k, 0, k, 0, 0, 0,
         k, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0,
         k, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, k, 0, k, k, 0, k, 0, k, 0, k, 0, k, 0, 0, 0, k, 0,
         k, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, k, 0, 0, k, 0, k, 0, k, 0, k, 0, k, 0, 0, k, k, 0,
         k, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, k, k, k, 0, 0,
         k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0, k, 0, 0,
         0, k, k, 0, k, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, 0, 0, k, 0, 0, 0, k, 0, k, k, k, k, 0, 0, k, 0, 0, k, 0, 0,
         k, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, k, 0, k, 0, 0, 0, 0, 0, 0, k, k, k, 0, 0, 0, 0, 0, 0, k, 0,
         k, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, k, 0, k, 0, k, 0, k, 0, k, 0, k, 0,
         0, k, 0, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, 0, k, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, k, 0, 0,
         k, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, k, 0, k, 0, 0, 0, k, 0, 0, k, 0, k, 0, 0, 0, 0, k, 0, 0, 0,
         0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      k, k, k, k, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0, k, 0, 0, 0, 0,
         k, k, k, k, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, k, 0, 0,
         0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0,
      0, k, k, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, k, 0, 0,
         0, k, k, k, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, k, 0, 0, 0, 0, k, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, k, k, k, k, k, k,
      0, 0, k, 0, 0, 0, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, z, z, z, z, 0, z, 0, 0, 0, z, 0, 0, 0, 0, 0, 0, 0,       /* lowercase a */
      z, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, z, z, z, 0, 0, z, 0, 0, 0, z, 0,
         z, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, z, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0,
         0, z, z, z, z, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0,
         z, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, z, 0, z, 0, 0, 0, 0, 0, z, z, z, 0, 0, 0, z, 0, 0, 0, 0, 0,
         z, z, z, z, z, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, z, 0, z, 0, 0, 0, 0, 0, z, z, z, 0, 0, 0, z, 0, 0, 0, 0, 0,
         z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, z, 0, z, 0, 0, 0, 0, 0, z, 0, 0, z, z, 0, z, 0, 0, 0, z, 0,
         0, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, z, z, z, z, 0, z, 0, 0, 0, z, 0,
         z, 0, 0, 0, z, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0,
         0, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, z, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0,
         z, z, z, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, z, 0, 0, z, 0, z, 0, 0, 0, z, z, 0, 0, 0, 0, z, 0, z, 0, 0, 0,
         z, 0, 0, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0,
         z, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, z, 0, z, z, 0, z, 0, z, 0, z, 0, z, 0, 0, 0, z, 0,
         z, 0, 0, 0, z, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, z, 0, 0, z, 0, z, 0, z, 0, z, 0, z, 0, 0, z, z, 0,
         z, 0, 0, 0, z, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0,
         0, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, z, z, z, 0, 0,
         z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, 0, z, 0, 0,
         0, z, z, 0, z, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, 0, 0, z, 0, 0, 0, z, 0, z, z, z, z, 0, 0, z, 0, 0, z, 0, 0,
         z, 0, 0, 0, z, 0, 0, 0, 0, 0, 0, 0,
      0, z, z, z, z, 0, z, 0, 0, 0, 0, 0, 0, z, z, z, 0, 0, 0, 0, 0, 0, z, 0,
         z, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, z, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0, 0, 0, z, 0, 0, 0,
         0, 0, z, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0,
         0, z, z, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, 0,
         0, 0, z, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, z, 0, z, 0, z, 0, z, 0, z, 0, z, 0,
         0, z, 0, z, 0, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, 0, z, 0, z, 0, 0, 0, 0, z, 0, 0, 0, 0, z, 0, z, 0, 0,
         z, 0, 0, 0, z, 0, 0, 0, 0, 0, 0, 0,
      z, 0, 0, 0, z, 0, z, 0, 0, 0, z, 0, 0, z, 0, z, 0, 0, 0, 0, z, 0, 0, 0,
         0, 0, z, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      z, z, z, z, z, 0, 0, 0, 0, z, 0, 0, 0, 0, z, 0, 0, 0, 0, z, 0, 0, 0, 0,
         z, z, z, z, z, 0, 0, 0, 0, 0, 0, 0
   };

   for (a = 0; a < 546; a++)
      for (b = 0; b < 6; b++)
         font6->line[a][b] = tf[a * 6 + b];
}
