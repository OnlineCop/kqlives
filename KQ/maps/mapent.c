#include <allegro.h>
#include <string.h>
#include <stdio.h>
#include "mapdraw.h"


/*! \brief Prepares the entity array for new entries
 *
 * This is called for each time the user wants to reset the entities;
*/
void init_entities (void)
{
   int p;

   /* Clear all entities' personalities */
   for (p = 0; p < 50; p++)
     {
        /* Entity's identity (what s/he looks like) */
        gent[p].chrx = 0;
        /* x-coord on map */
        gent[p].x = 0;
        /* y-coord on map */
        gent[p].y = 0;
        /* x-coord tile that entity is standing on */
        gent[p].tilex = 0;
        /* y-coord tile that entity is standing on */
        gent[p].tiley = 0;
        /* Entity type (fighter, enemy, normal) */
        gent[p].eid = 0;
        /* "Alive" and on-screen or not */
        gent[p].active = 0;
        /* Direction facing */
        gent[p].facing = 0;
        /* In the middle of a move */
        gent[p].moving = 0;
        /* How far along into the move s/he is */
        gent[p].movcnt = 0;
        /* Counter for determining animation frame */
        gent[p].framectr = 0;
        /* Stand, wander, script, or chasing */
        gent[p].movemode = 0;
        /* Affected by obstacles (some can walk through walls/other NPCs) */
        gent[p].obsmode = 0;
        /* Movement delay (between steps) */
        gent[p].delay = 0;
        /* Counter for movement delay */
        gent[p].delayctr = 0;
        /* How hyperactive the entity is */
        gent[p].speed = 0;
        /* Hyperactivity counter */
        gent[p].scount = 0;
        /* Move, wait, facing command */
        gent[p].cmd = 0;
        /* Script index number */
        gent[p].sidx = 0;
        /* ??? */
        gent[p].extra = 0;
        /* Entity is following another */
        gent[p].chasing = 0;
        /* Number of times we need to repeat 'cmd' */
        gent[p].cmdnum = 0;
        /* (unused) */
        gent[p].atype = 0;
        /* Movement script (pacing, etc.) */
        strcpy (gent[p].script, "");
     } /* for (p = 0; p < 50; p++) */

   /* Reset the entity count */
   noe = 0;
} /* init_entities () */


/*! \brief Offset the x/y coodinates of entities
 *
 * Adjusts the location of all entities on the map
*/
void displace_entities (void)
{
   int ld, nx, ny, a;

   /* Draw a box and ask for x-coord adjustment */
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Displace entities", screen);
   print_sfont (6, 18, "X adjust: ", screen);
   ld = get_line (66, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   /* This number CAN be negative! */
   nx = atoi (strbuf);

   /* Clear the box and ask for y-coord adjustment */
   rectfill (screen, 0, 0, 319, 29, 0);
   rect (screen, 2, 2, 317, 27, 255);
   print_sfont (6, 6, "Displace entities", screen);
   print_sfont (6, 18, "Y adjust: ", screen);
   ld = get_line (66, 18, strbuf, 4);

   /* Make sure the line isn't blank */
   if (ld == 0)
      return;

   /* This number CAN be negative! */
   ny = atoi (strbuf);

   /* This moves all the entities */
   for (a = 0; a < noe; a++)
     {
        /* Confirm that the input x-coords are within the map */
        if (gent[a].tilex + nx < gmap.xsize && gent[a].tilex + nx >= 0)
          {
             gent[a].tilex += nx;
             gent[a].x = gent[a].tilex * 16;
          }
        /* Confirm that the input x-coords are within the map */
        if (gent[a].tiley + ny < gmap.ysize && gent[a].tiley + ny >= 0)
          {
             gent[a].tiley += ny;
             gent[a].y = gent[a].tiley * 16;
          }
     }
} /* displace_entities () */


/*! \brief Place an entity on the map
 *
 * Give the little lemming a home
 *
 * \param   ex Entity's x-coord
 * \param   ey Entity's y-coord
*/
void place_entity (int ex, int ey)
{
   int a;

   /* Yea, can't have to many lemmings on the map at once */
   if (noe >= 50)
      return;

   /* Wait for the over-zealous user to let go of the mouse button */
   while (mouse_b & 1)
     {
        a++;
        a--;
     }

   /* Set its personality/attributes/stats */
   gent[noe].chrx = cent;  /* What it looks like */
   gent[noe].tilex = ex;   /* which tile it's standing on */
   gent[noe].tiley = ey;   /* ..same.. */
   gent[noe].x = ex * 16;  /* Will be the same as tilex unless moving */
   gent[noe].y = ey * 16;  /* ..same.. */
   gent[noe].active = 1;   /* Showing on map or not */
   gent[noe].eid = 255;     /* */
   gent[noe].movemode = 0; /* 0=stand, 1=wander, 2=script, 3=chase */
   gent[noe].delay = 50;   /* Length of time between commands */
   gent[noe].speed = 3;    /* Walking speed */
   gent[noe].obsmode = 1;  /* 0=not obstacle, 1=obstacle */
   gent[noe].facing = 0;   /* 0=S, 1=N, 2=W, 3=E */
   strcpy (gent[noe].script, "");  /* Pre-defined movement (pace, dance...) */
   noe++;
} /* place_entity () */


/*! \brief Remove an entity from the map
 *
 * Rub the useless guy out
 *
 * \param   pex Entity's x-coord
 * \param   pey Entity's y-coord
*/
void erase_entity (int pex, int pey)
{
   int a, enn = -1;

   /* This function does nothing if there aren't already entities to remove */
   if (noe == 0)
      return;

   /* Be careful not to delete everyone standing here */
   while (mouse_b & 2)
     {
        a++;
        a--;
     }

   /* Could you imaging a 3D game?  pex, pey and Pez!  Yum-yum. */
   for (a = 0; a < noe; a++)
      /* Get the index number of the last-drawn entity from this spot */
      if (gent[a].tilex == pex && gent[a].tiley == pey)
         enn = a;

   /* There is no entity under the mouse */
   if (enn == -1)
      return;

   /* This is how we erase instances of an entity; set active to 0 */
   gent[enn].active = 0;

   /* This re-numerates the index of entities */
   for (a = enn + 1; a < noe; a++)
     {
        gent[a - 1] = gent[a];
        gent[a].active = 0;
     }

   /* Oh yea, and do this too */
   noe--;
} /* erase_entity () */


/*! \brief Update stats for entities
 *
 * Allow the user to give each entity its own personality
*/
void update_entities (void)
{
   /* Well, it doesn't mean GO... */
   int stop = 0;
   /* Entity's index */
   int et = 0;
   /* Key press */
   int c;
   /* User input */
   int a;
   /* Temporary window coordinates */
   int tgx = gx, tgy = gy;
   /* Temporary draw_mode */
   int tdm = draw_mode;

   draw_mode = MAP_ENTITIES;

   /* Updating entities ineffective if no one is there */
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

        /* Stop updating entities */
        if (c == KEY_ESC)
           stop = 1;

        /* Select the previous entity in the queue */
        if (c == KEY_DOWN)
          {
             et--;
             if (et < 0)
                et = noe - 1;
          }

        /* Select the following entity in the queue */
        if (c == KEY_UP)
          {
             et++;
             if (et == noe)
                et = 0;
          }

        /* Change the entity's sprite (what it looks like) */
        if (c == KEY_1)
          {
             gent[et].chrx++;
             if (gent[et].chrx == MAX_EPICS)
                gent[et].chrx = 0;
          }

        /* Change the x-coord */
        if (c == KEY_2)
          {
             rectfill (screen, 48, SH - 46, 71, SH - 41, 0);
             hline (screen, 48, SH - 41, 71, 255);
             a = get_line (48, SH - 46, strbuf, 4);

             /* Make sure the line isn't blank */
             if (a == 0)
                return;

             gent[et].tilex = atoi (strbuf);
             gent[et].x = gent[et].tilex * 16;
          }

        /* Change the y-coord */
        if (c == KEY_3)
          {
             rectfill (screen, 48, SH - 40, 71, SH - 35, 0);
             hline (screen, 48, SH - 35, 71, 255);
             a = get_line (48, SH - 40, strbuf, 4);

             /* Make sure the line isn't blank */
             if (a == 0)
                return;

             gent[et].tiley = atoi (strbuf);
             gent[et].y = gent[et].tiley * 16;
          }

        /* Change the method of movement (Stand, Wander, Script, Chase) */
        if (c == KEY_4)
          {
             gent[et].movemode++;
             if (gent[et].movemode > 3)
                gent[et].movemode = 0;
          }

        /* Change the movement delay */
        if (c == KEY_5)
          {
             rectfill (screen, 72, SH - 28, 95, SH - 23, 0);
             hline (screen, 72, SH - 23, 95, 255);
             a = get_line (72, SH - 28, strbuf, 4);

             /* Make sure the line isn't blank */
             if (a == 0)
                return;

             gent[et].delay = atoi (strbuf);
          }

        /* Change the entity's speed */
        if (c == KEY_6)
          {
             gent[et].speed++;
             if (gent[et].speed > 7)
                gent[et].speed = 1;
          }

        /* Entity is walk-through-able or solid */
        if (c == KEY_7)
           gent[et].obsmode = 1 - gent[et].obsmode;

        /* Change the direction entity is facing */
        if (c == KEY_8)
          {
             gent[et].facing++;
             if (gent[et].facing > 3)
                gent[et].facing = 0;
          }

        /* Change the atype (unused) */
        if (c == KEY_9)
          {
             gent[et].atype++;
             if (gent[et].atype > 1)
                gent[et].atype = 0;
          }

        /* Change the entity's scripted movement */
        if (c == KEY_0)
          {
             rectfill (screen, 24, SH - 10, 319, SH - 5, 0);
             hline (screen, 24, SH - 5, 319, 255);
             a = get_line (24, SH - 10, strbuf, 61);

             /* Make sure the line isn't blank */
             if (a == 0)
                return;

             strcpy (gent[et].script, strbuf);
          }

        /* After you talk to this entity, it will return to the direction it
           was facing before you ever talked to him/her */
        if (c == KEY_S)
          {
             gent[et].snapback++;
             if (gent[et].snapback > 1)
                gent[et].snapback = 0;
          }

        /* Face the hero when s/he is talking to this entity */
        if (c == KEY_F)
          {
             gent[et].facehero++;
             if (gent[et].facehero > 1)
                gent[et].facehero = 0;
          }

        /* Make this entity transparent */
        if (c == KEY_T)
          {
             gent[et].transl++;
             if (gent[et].transl > 1)
                gent[et].transl = 0;
          }
     } /* while (!stop) */
   gx = tgx;
   gy = tgy;
   draw_mode = tdm;
} /* update_entities () */


/* Display the entity menu
 *
 * Display the stats for the current entity in the menu at the bottom of the
 * screen.
 *
 * \param   en Index of current entity
*/
void draw_entdata (int en)
{
   int a, tdx, tdy;

   /* Move the window so the user can see the selected entity and make sure
      the view isn't out of the boundaries */
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

   /* Print the number of entities and currently-selected one in right menu */
   sprintf (strbuf, "Total: %d", noe);
   print_sfont (SW - 78, 0, strbuf, double_buffer);
   sprintf (strbuf, "Current: %d", en);
   print_sfont (SW - 78, 6, strbuf, double_buffer);

   /* Print all the options in the bottom menu */
   print_sfont (160, SH - 46, "1 - change entity sprite", double_buffer);

   /* TT: hack */
   if (gent[en].facing == 0)
      print_sfont (160, SH - 40, "8 - direction facing (S)", double_buffer);
   if (gent[en].facing == 1)
      print_sfont (160, SH - 40, "8 - direction facing (N)", double_buffer);
   if (gent[en].facing == 2)
      print_sfont (160, SH - 40, "8 - direction facing (W)", double_buffer);
   if (gent[en].facing == 3)
      print_sfont (160, SH - 40, "8 - direction facing (E)", double_buffer);

   /* TT: end hack */
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
} /* draw_entdata () */
