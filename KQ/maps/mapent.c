#include <allegro.h>
#include <string.h>
#include <stdio.h>
#include "mapdraw.h"


// Give each of the entities a purpose
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
   noe=0;
}


// Move the entities
void displace_entities (void)
{
   int ld, nx, ny, a;

   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Displace entities", screen);
   print_sfont (6, 18, "X adjust: ", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld == 0)
      return;
   nx = atoi (strbuf);
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Displace entities", screen);
   print_sfont (6, 18, "Y adjust: ", screen);
   ld = get_line (66, 18, strbuf, 4);
   if (ld == 0)
      return;
   ny = atoi (strbuf);

   for (a = 0; a < noe; a++)
     {
        // Confirm that the input x-coords are within the map
        if (gent[a].tilex + nx < gmap.xsize && gent[a].tilex + nx >= 0)
          {
             gent[a].tilex += nx;
             gent[a].x = gent[a].tilex * 16;
          }
        // Confirm that the input x-coords are within the map
        if (gent[a].tiley + ny < gmap.ysize && gent[a].tiley + ny >= 0)
          {
             gent[a].tiley += ny;
             gent[a].y = gent[a].tiley * 16;
          }
     }
}


// Give the little lemming a home
void place_entity (int ex, int ey)
{
   int a;

   // Yea, can't have to many lemmings on the map at once
   if (noe >= 50)
      return;
   // Wait for the over-zealous user to let go of the mouse button
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


// Rub the useless guy out
void erase_entity (int pex, int pey)
{
   int a, enn = -1;

   if (noe == 0)
      return;
   // Be careful not to delete everyone standing here
   while (mouse_b & 2)
     {
        a++;
        a--;
     }
   // Could you imaging a 3D game?  pex, pey and Pez!  Yum-yum.
   for (a = 0; a < noe; a++)
      // Get the index number of the last-drawn entity from this spot
      if (gent[a].tilex == pex && gent[a].tiley == pey)
         enn = a;
   if (enn == -1)
      return;
   gent[enn].active = 0;
   // This shifts all entity indexes 'above' the selected one, down one
   for (a = enn + 1; a < noe; a++)
     {
        gent[a - 1] = gent[a];
        gent[a].active = 0;
     }
   // Oh yea, and do this too
   noe--;
}


// Allow the user to give each entity its own personality
void update_entities (void)
{
   int stop = 0;  // Well, it doesn't mean GO...
   int et = 0; // Index of entity
   int c;   // Key press
   int a;   // User input
   int tgx = gx, tgy = gy; // Temporary window coordinates
   int tdm = draw_mode; // Temporary draw_mode

   draw_mode = A_ENTITIES;

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
        if (c == KEY_DOWN)
          {
             et--;
             if (et < 0)
                et = noe - 1;
          }
        if (c == KEY_UP)
          {
             et++;
             if (et == noe)
                et = 0;
          }
        if (c == KEY_1) // Entity's sprite (what it looks like)
          {
             gent[et].chrx++;
             if (gent[et].chrx == MAX_EPICS)
                gent[et].chrx = 0;
          }
        if (c == KEY_2) // Change the x-coordinate
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
        if (c == KEY_3) // Change the y-coordinate
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
        if (c == KEY_4) // Stand-Wander-Script-Chase mode
          {
             gent[et].movemode++;
             if (gent[et].movemode > 3)
                gent[et].movemode = 0;
          }
        if (c == KEY_5) // Delay
          {
             rectfill (screen, 72, SH - 28, 95, SH - 23, 0);
             hline (screen, 72, SH - 23, 95, 255);
             a = get_line (72, SH - 28, strbuf, 4);
             if (a != 0)
                gent[et].delay = atoi (strbuf);
          }
        if (c == KEY_6) // Speed
          {
             gent[et].speed++;
             if (gent[et].speed > 7)
                gent[et].speed = 1;
          }
        if (c == KEY_7) // Obstructive
           gent[et].obsmode = 1 - gent[et].obsmode;
        if (c == KEY_8) // Change direction
          {
             gent[et].facing++;
             if (gent[et].facing > 3)
                gent[et].facing = 0;
          }
        if (c == KEY_9)
          {
             gent[et].atype = 1 - gent[et].atype;

             // This does the same as:
             /*
             gent[et].atype++;
             if (gent[et].atype > 1)
                gent[et].atype = 0;
             */
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
             gent[et].snapback = 1 - gent[et].snapback;

             // This does the same as:
             /*
             gent[et].snapback++;
             if (gent[et].snapback > 1)
                gent[et].snapback = 0;
             */
          }
        if (c == KEY_F)
          {
             gent[et].facehero = 1 - gent[et].facehero;

             // This does the same as:
             /*
             gent[et].facehero++;
             if (gent[et].facehero > 1)
                gent[et].facehero = 0;
             */
          }
        if (c == KEY_T)
          {
             gent[et].transl = 1 - gent[et].transl;

             // This does the same as:
             /*
             gent[et].transl++;
             if (gent[et].transl > 1)
                gent[et].transl = 0;
             */
          }
     }
   gx = tgx;
   gy = tgy;
   draw_mode = tdm;
}


// Fill in the options at the bottom of the screen
void draw_entdata (int en)
{
   int a, tdx, tdy;

   // Make sure we're not out of the boundaries
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
