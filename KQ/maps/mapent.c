/***************************************************************************\
 * This map editor is for the KQ game, supported by the kqlives community. *
 * Comments and suggestions about the editor are welcome.  Please join our *
 * mailing list: kqlives-main@lists.sourceforge.net                        *
 *                                                                         *
 * Visit our website: http://kqlives.sourceforge.net/index.php             *
 *                                                                         *
 * Mapent.c will contain all routines regarding entity movement, placement *
 * or direction.  This also includes scripted movement processing, etc.    *
\***************************************************************************/


#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include "mapdraw.h"


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
   for (a = 0; a < number_of_ents; a++) {
      /* Confirm that the input x-coords are within the map */
      if (gent[a].tilex + nx < gmap.xsize && gent[a].tilex + nx >= 0) {
         gent[a].tilex += nx;
         gent[a].x = gent[a].tilex * 16;
      }
      /* Confirm that the input x-coords are within the map */
      if (gent[a].tiley + ny < gmap.ysize && gent[a].tiley + ny >= 0) {
         gent[a].tiley += ny;
         gent[a].y = gent[a].tiley * 16;
      }
   }
}                               /* displace_entities () */


/* \brief Display the entity menu
 *
 * Display the stats for the current entity in the menu at the bottom of the
 * screen.
 *
 * \param   ent_index Index of current entity
 */
void draw_entdata (const int ent_index)
{
   int a, ent_x, ent_y;

   /* Move the window so the user can see the selected entity and make sure
      the view isn't out of the boundaries */
   window_x = gent[ent_index].tilex - (htiles / 2);
   window_y = gent[ent_index].tiley - (vtiles / 2);

   normalize_view ();
   clear (double_buffer);
   draw_map ();

   ent_x = (gent[ent_index].tilex - window_x) * 16;
   ent_y = (gent[ent_index].tiley - window_y) * 16;
   rect (double_buffer, ent_x - 1, ent_y - 1, ent_x + 16, ent_y + 16, 255);
   hline (double_buffer, 0, SH - 48, SW - 80, 255);
   vline (double_buffer, SW - 80, 0, SH - 48, 255);

   /* Print the number of entities and currently-selected one in right menu */
   sprintf (strbuf, "Total: %d", number_of_ents);
   print_sfont (SW - 78, 0, strbuf, double_buffer);
   sprintf (strbuf, "Current: %d", ent_index);
   print_sfont (SW - 78, 6, strbuf, double_buffer);

   /* Print all the options in the bottom menu */
   sprintf (strbuf, "1 - change entity sprite (%d)", gent[ent_index].chrx);
   print_sfont (160, SH - 46, strbuf, double_buffer);

   switch (gent[ent_index].facing) {
   case 0:
      sprintf (strbuf, "S");
      break;
   case 1:
      sprintf (strbuf, "N");
      break;
   case 2:
      sprintf (strbuf, "W");
      break;
   case 3:
      sprintf (strbuf, "E");
      break;
   }
   sprintf (strbuf, "8 - direction facing (%s)", strbuf);
   print_sfont (160, SH - 40, strbuf, double_buffer);

#if 0
   /* Display the direction facing: makes more sense that just "1" or "3" */
   if (gent[ent_index].facing == 0)
      print_sfont (160, SH - 40, "8 - direction facing (S)", double_buffer);
   if (gent[ent_index].facing == 1)
      print_sfont (160, SH - 40, "8 - direction facing (N)", double_buffer);
   if (gent[ent_index].facing == 2)
      print_sfont (160, SH - 40, "8 - direction facing (W)", double_buffer);
   if (gent[ent_index].facing == 3)
      print_sfont (160, SH - 40, "8 - direction facing (E)", double_buffer);
#endif

   if (gent[ent_index].obsmode == 0)
      strcpy (strbuf, "7 - ignore obstructions");
   else
      strcpy (strbuf, "7 - obstructive");
   print_sfont (160, SH - 34, strbuf, double_buffer);
   sprintf (strbuf, "x = %d", gent[ent_index].tilex);
   print_sfont (24, SH - 46, strbuf, double_buffer);
   sprintf (strbuf, "y = %d", gent[ent_index].tiley);
   print_sfont (24, SH - 40, strbuf, double_buffer);
   switch (gent[ent_index].movemode) {
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
   sprintf (strbuf, "delay = %d", gent[ent_index].delay);
   print_sfont (24, SH - 28, strbuf, double_buffer);
   sprintf (strbuf, "speed = %d", gent[ent_index].speed);
   print_sfont (24, SH - 22, strbuf, double_buffer);
   if (gent[ent_index].atype == 0)
      sprintf (strbuf, "9 - atype = %d (normal)", gent[ent_index].atype);
   else
      sprintf (strbuf, "9 - atype = %d (constant)", gent[ent_index].atype);
   print_sfont (0, SH - 16, strbuf, double_buffer);
   print_sfont (24, SH - 10, gent[ent_index].script, double_buffer);
   for (a = 1; a < 6; a++) {
      sprintf (strbuf, "%d -", a + 1);
      print_sfont (0, a * 6 + (SH - 52), strbuf, double_buffer);
   }
   print_sfont (0, SH - 10, "0 -", double_buffer);
   if (gent[ent_index].snapback == 0)
      strcpy (strbuf, "S - snap back");
   else
      strcpy (strbuf, "S - don't snap back");
   print_sfont (160, SH - 28, strbuf, double_buffer);
   if (gent[ent_index].facehero == 0)
      strcpy (strbuf, "F - face hero");
   else
      strcpy (strbuf, "F - don't face hero");
   print_sfont (160, SH - 22, strbuf, double_buffer);
   if (gent[ent_index].transl == 0)
      strcpy (strbuf, "T - opaque");
   else
      strcpy (strbuf, "T - translucent");
   print_sfont (160, SH - 16, strbuf, double_buffer);
   blit (double_buffer, screen, 0, 0, 0, 0, SW, SH);
}                               /* draw_entdata () */


/*! \brief Draw the entities
 *
 * Draws the entities onto the screen and takes into consideration any layer
 * effects that need to take place (see map_mode_text[] for details).
 * \author PH
 * \date 20031205
 */
void draw_ents (void)
{
   int d, x0, y0;
   BITMAP *ent;
   x0 = window_x * 16;
   y0 = window_y * 16;

   for (d = 0; d < number_of_ents; d++) {
      /* Draw only the entities within the view-screen */
      if ((gent[d].tilex >= window_x) && (gent[d].tilex < window_x + htiles)
          && (gent[d].tiley >= window_y)
          && (gent[d].tiley < window_y + vtiles)) {
         ent = eframes[gent[d].chrx][gent[d].facing * 3];
         /* Draw either a normal sprite or a translucent one */
         if (gent[d].transl == 0)
            draw_sprite (double_buffer, ent, gent[d].tilex * 16 - x0,
                         gent[d].tiley * 16 - y0);
         else
            draw_trans_sprite (double_buffer, ent, gent[d].tilex * 16 - x0,
                               gent[d].tiley * 16 - y0);
      }
   }
}                               /* draw_ents () */


/*! \brief Remove an entity from the map
 *
 * Rub the useless guy out
 *
 * \param   ent_x Entity's x-coord
 * \param   ent_y Entity's y-coord
 */
void erase_entity (const int ent_x, const int ent_y)
{
   int a, ent_index = -1;

   /* This function does nothing if there aren't already entities to remove */
   if (number_of_ents == 0)
      return;

   /* Be careful not to delete everyone standing here */
   while (mouse_b & 2) {
      a++;
      a--;
   }

   for (a = 0; a < number_of_ents; a++)
      /* Get the index number of the last-drawn entity from this spot */
      if (gent[a].tilex == ent_x && gent[a].tiley == ent_y)
         ent_index = a;

   /* There is no entity under the mouse */
   if (ent_index == -1)
      return;

   /* This is how we erase instances of an entity; set active to 0 */
   gent[ent_index].active = 0;

   /* This re-numerates the index of entities */
   for (a = ent_index + 1; a < number_of_ents; a++) {
      gent[a - 1] = gent[a];
      gent[a].active = 0;
   }

   /* Oh yea, and do this too */
   number_of_ents--;
}                               /* erase_entity () */


/*! \brief Prepares the entity array for new entries
 *
 * This is called for each time the user wants to reset the entities;
 */
void init_entities (void)
{
   int p;

   /* Clear all entities' personalities */
   for (p = 0; p < 50; p++) {
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
   }                            /* for (p = 0; p < 50; p++) */

   /* Reset the entity count */
   number_of_ents = 0;
}                               /* init_entities () */


/*! \brief Place an entity on the map
 *
 * Give the little lemming a home
 *
 * \param   ent_x Entity's x-coord
 * \param   ent_y Entity's y-coord
 */
void place_entity (int ent_x, int ent_y)
{
   int a;

   /* Yea, can't have to many lemmings on the map at once */
   if (number_of_ents >= 50)
      return;

   /* Wait for the over-zealous user to let go of the mouse button */
   while (mouse_b & 1) {
      a++;
      a--;
   }

   /* Set its personality/attributes/stats */
   gent[number_of_ents].chrx = current_ent;     /* What it looks like */
   gent[number_of_ents].tilex = ent_x;     /* which tile it's standing on */
   gent[number_of_ents].tiley = ent_y;     /* ..same.. */
   gent[number_of_ents].x = ent_x * 16;    /* Will be the same as tilex unless moving */
   gent[number_of_ents].y = ent_y * 16;    /* ..same.. */
   gent[number_of_ents].active = 1;     /* Showing on map or not */
   gent[number_of_ents].eid = 255;      /* */
   gent[number_of_ents].movemode = 0;   /* 0=stand, 1=wander, 2=script, 3=chase */
   gent[number_of_ents].delay = 50;     /* Length of time between commands */
   gent[number_of_ents].speed = 3;      /* Walking speed */
   gent[number_of_ents].obsmode = 1;    /* 0=not obstacle, 1=obstacle */
   gent[number_of_ents].facing = 0;     /* 0=S, 1=N, 2=W, 3=E */
   strcpy (gent[number_of_ents].script, "");    /* Pre-defined movement (pace, dance...) */
   number_of_ents++;
}                               /* place_entity () */


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
   int t_window_x = window_x, t_window_y = window_y;

   /* Updating entities ineffective if no one is there */
   if (number_of_ents == 0) {
      cmessage ("No entities!");
      wait_enter ();
      return;
   }

   while (!stop) {
      draw_entdata (et);
      c = (readkey () >> 8);

      /* Stop updating entities */
      if (c == KEY_ESC)
         stop = 1;

      /* Select the previous entity in the queue */
      if (c == KEY_DOWN) {
         et--;
         if (et < 0)
            et = number_of_ents - 1;
      }

      /* Select the following entity in the queue */
      if (c == KEY_UP) {
         et++;
         if (et == number_of_ents)
            et = 0;
      }

      /* Change the entity's sprite (what it looks like) */
      if (c == KEY_1) {
         gent[et].chrx++;
         if (gent[et].chrx == MAX_EPICS)
            gent[et].chrx = 0;
      }

      /* Change the x-coord */
      if (c == KEY_2) {
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
      if (c == KEY_3) {
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
      if (c == KEY_4) {
         gent[et].movemode++;
         if (gent[et].movemode > 3)
            gent[et].movemode = 0;
      }

      /* Change the movement delay */
      if (c == KEY_5) {
         rectfill (screen, 72, SH - 28, 95, SH - 23, 0);
         hline (screen, 72, SH - 23, 95, 255);
         a = get_line (72, SH - 28, strbuf, 4);

         /* Make sure the line isn't blank */
         if (a == 0)
            return;

         gent[et].delay = atoi (strbuf);
      }

      /* Change the entity's speed */
      if (c == KEY_6) {
         gent[et].speed++;
         if (gent[et].speed > 7)
            gent[et].speed = 1;
      }

      /* Entity is walk-through-able or solid */
      if (c == KEY_7)
         gent[et].obsmode = 1 - gent[et].obsmode;

      /* Change the direction entity is facing */
      if (c == KEY_8) {
         gent[et].facing++;
         if (gent[et].facing > 3)
            gent[et].facing = 0;
      }

      /* Change the atype (unused) */
      if (c == KEY_9) {
         gent[et].atype++;
         if (gent[et].atype > 1)
            gent[et].atype = 0;
      }

      /* Change the entity's scripted movement */
      if (c == KEY_0) {
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
      if (c == KEY_S) {
         gent[et].snapback++;
         if (gent[et].snapback > 1)
            gent[et].snapback = 0;
      }

      /* Face the hero when s/he is talking to this entity */
      if (c == KEY_F) {
         gent[et].facehero++;
         if (gent[et].facehero > 1)
            gent[et].facehero = 0;
      }

      /* Make this entity transparent */
      if (c == KEY_T) {
         gent[et].transl++;
         if (gent[et].transl > 1)
            gent[et].transl = 0;
      }
   }                            /* while (!stop) */
   window_x = t_window_x;
   window_y = t_window_y;
}                               /* update_entities () */
