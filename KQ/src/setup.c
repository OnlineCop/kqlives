/*
   KQ is Copyright (C) 2002 - Josh Bolduc

   This file is part of KQ... a freeware RPG.

   KQ is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   KQ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KQ; see the file COPYING.  If not, write to
   the Free Software Foundation,
       675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <string.h>
#include "setup.h"
#include "kq.h"
#include "combat.h"
#include "draw.h"
#include "res.h"

/*
   globals
*/

JGMOD *gsong;
char debugging = 0;
char keynames[115][12] = {
   "",
   "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
   "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
   "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3",
   "4", "5", "6", "7", "8", "9", "0_PAD", "1_PAD", "2_PAD", "3_PAD",
   "4_PAD", "5_PAD", "6_PAD", "7_PAD", "8_PAD", "9_PAD", "F1", "F2", "F3",
   "F4",
   "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "ESC", "TILDE",
   "MINUS", "EQUALS", "BACKSPACE", "TAB", "OPENBRACE", "CLOSEBRACE", "ENTER",
   "COLON", "QUOTE", "BACKSLASH",
   "BACKSLASH2", "COMMA", "STOP", "SLASH", "SPACE", "INSERT", "DEL", "HOME",
   "END", "PGUP",
   "PGDN", "LEFT", "RIGHT", "UP", "DOWN", "SLASH_PAD", "ASTERISK",
   "MINUS_PAD", "PLUS_PAD", "DEL_PAD",
   "ENTER_PAD", "PRTSCR", "PAUSE", "ABNT_C1", "YEN", "KANA", "CONVERT",
   "NOCONVERT", "AT", "CIRCUMFLEX",
   "COLON2", "KANJI", "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "ALT",
   "ALTGR", "LWIN", "RWIN",
   "MENU", "SCRLOCK", "NUMLOCK", "CAPSLOCK"
};



/*  internal variables  */
static DATAFILE *sfx[MAX_SAMPLES];



/*  internal functions  */
static void load_samples (void);
static int getavalue (char *, int, int, int, int);
static int getakey (void);



/*
   Parse the setup.cfg file for key configurations.
   This file would also contain sound options, but that
   isn't necessary right now.
*/
void parse_setup (void)
{
   FILE *s;
   int dab = 0;

   kup = KEY_UP;
   kdown = KEY_DOWN;
   kright = KEY_RIGHT;
   kleft = KEY_LEFT;
   kalt = KEY_ALT;
   kctrl = KEY_LCONTROL;
   kenter = KEY_ENTER;
   kesc = KEY_ESC;
   jbalt = 0;
   jbctrl = 1;
   jbenter = 2;
   jbesc = 3;
   /* PH Why in the world doesn't he use Allegro cfg functions here? */
   if (!(s = fopen ("setup.cfg", "r")))
     {
        klog ("Could not open setup.cfg - Using defaults.");
        return;
     }
   fscanf (s, "%s", strbuf);
   while (!feof (s))
     {
        if (strbuf[0] == '#')
           fgets (strbuf, 254, s);
        if (!strcmp (strbuf, "cheat"))
          {
             fscanf (s, "%d", &dab);
             cheat = dab;
          }
        if (!strcmp (strbuf, "debug"))
          {
             fscanf (s, "%d", &dab);
             debugging = dab;
          }
        if (!strcmp (strbuf, "intro"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "no"))
                skip_intro = 1;
          }
        if (!strcmp (strbuf, "windowed"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "yes"))
                windowed = 1;
          }
        if (!strcmp (strbuf, "stretch"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "yes"))
                stretch_view = 1;
          }
        if (!strcmp (strbuf, "framerate"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "on"))
                show_frate = 1;
          }
        if (!strcmp (strbuf, "sound"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "off"))
                is_sound = 0;
          }
        if (!strcmp (strbuf, "joystick"))
          {
             fscanf (s, "%s", strbuf);
             if (!strcmp (strbuf, "no"))
                use_joy = 0;
          }
        if (!strcmp (strbuf, "rightkey"))
          {
             fscanf (s, "%s", strbuf);
             kright = atoi (strbuf);
          }
        if (!strcmp (strbuf, "leftkey"))
          {
             fscanf (s, "%s", strbuf);
             kleft = atoi (strbuf);
          }
        if (!strcmp (strbuf, "upkey"))
          {
             fscanf (s, "%s", strbuf);
             kup = atoi (strbuf);
          }
        if (!strcmp (strbuf, "downkey"))
          {
             fscanf (s, "%s", strbuf);
             kup = atoi (strbuf);
          }
        if (!strcmp (strbuf, "sysmenukey"))
          {
             fscanf (s, "%s", strbuf);
             kesc = atoi (strbuf);
          }
        if (!strcmp (strbuf, "cancelkey"))
          {
             fscanf (s, "%s", strbuf);
             kctrl = atoi (strbuf);
          }
        if (!strcmp (strbuf, "confirmkey"))
          {
             fscanf (s, "%s", strbuf);
             kalt = atoi (strbuf);
          }
        if (!strcmp (strbuf, "chrmenukey"))
          {
             fscanf (s, "%s", strbuf);
             kenter = atoi (strbuf);
          }
        fscanf (s, "%s", strbuf);
     }
   fclose (s);
}

/*
   This is the config menu that is called from the system
   menu.  Here you can adjust the music or sound volume, or
   the speed that the battle guage moves at.
*/
void config_menu (void)
{
   int stop = 0, ptr = 0, rd = 1, p;
   char dc[15][40] = {
      "Display KQ in a window.",
      "Stretch to fit 640x480 resolution.",
      "Display the frame rate during play.",
      "Wait for vertical retrace.",
      "Key used to move up.",
      "Key used to move down.",
      "Key used to move left.",
      "Key used to move right.",
      "Key used to call system menu.",
      "Key used to cancel action.",
      "Key used to confirm action.",
      "Key used to call character menu.",
      "Toggle sound and music on/off.",
      "Overall sound volume (affects music).",
      "Music volume.",
   };

   unpress ();
   while (!stop)
     {
        if (rd)
          {
             drawmap ();
             menubox (double_buffer, 88 + xofs, yofs, 16, 1, BLUE);
             print_font (double_buffer, 96 + xofs, 8 + yofs,
                         "KQ Configuration", FGOLD);
             menubox (double_buffer, 32 + xofs, 24 + yofs, 30, 17, BLUE);
             print_font (double_buffer, 48 + xofs, 32 + yofs,
                         "Windowed Mode:", FNORMAL);
             print_font (double_buffer, 48 + xofs, 40 + yofs,
                         "Stretch Display:", FNORMAL);
             print_font (double_buffer, 48 + xofs, 48 + yofs,
                         "Show Frame Rate:", FNORMAL);
             print_font (double_buffer, 48 + xofs, 56 + yofs,
                         "Wait for Retrace:", FNORMAL);
             print_font (double_buffer, 48 + xofs, 72 + yofs, "Up Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 80 + yofs, "Down Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 88 + yofs, "Left Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 96 + yofs, "Right Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 104 + yofs, "Confirm Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 112 + yofs, "Cancel Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 120 + yofs, "Menu Key:",
                         FNORMAL);
             print_font (double_buffer, 48 + xofs, 128 + yofs,
                         "System Menu Key:", FNORMAL);
             print_font (double_buffer, 48 + xofs, 144 + yofs,
                         "Sound System:", FNORMAL);
             if (is_sound == 2)
               {
                  print_font (double_buffer, 48 + xofs, 152 + yofs,
                              "Sound Volume:", FNORMAL);
                  print_font (double_buffer, 48 + xofs, 160 + yofs,
                              "Music Volume:", FNORMAL);
               }
             if (is_sound == 0)
               {
                  print_font (double_buffer, 48 + xofs, 152 + yofs,
                              "Sound Volume:", FDARK);
                  print_font (double_buffer, 48 + xofs, 160 + yofs,
                              "Music Volume:", FDARK);
               }
             if (windowed == 1)
                print_font (double_buffer, 256 + xofs, 32 + yofs, "YES",
                            FNORMAL);
             else
                print_font (double_buffer, 264 + xofs, 32 + yofs, "NO",
                            FNORMAL);
             if (stretch_view == 1)
                print_font (double_buffer, 256 + xofs, 40 + yofs, "YES",
                            FNORMAL);
             else
                print_font (double_buffer, 264 + xofs, 40 + yofs, "NO",
                            FNORMAL);
             if (show_frate == 1)
                print_font (double_buffer, 256 + xofs, 48 + yofs, "YES",
                            FNORMAL);
             else
                print_font (double_buffer, 264 + xofs, 48 + yofs, "NO",
                            FNORMAL);
             if (wait_retrace == 1)
                print_font (double_buffer, 256 + xofs, 56 + yofs, "YES",
                            FNORMAL);
             else
                print_font (double_buffer, 264 + xofs, 56 + yofs, "NO",
                            FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kup]) * 8) + xofs, 72 + yofs,
                         keynames[kup], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kdown]) * 8) + xofs,
                         80 + yofs, keynames[kdown], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kleft]) * 8) + xofs,
                         88 + yofs, keynames[kleft], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kright]) * 8) + xofs,
                         96 + yofs, keynames[kright], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kalt]) * 8) + xofs,
                         104 + yofs, keynames[kalt], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kctrl]) * 8) + xofs,
                         112 + yofs, keynames[kctrl], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kenter]) * 8) + xofs,
                         120 + yofs, keynames[kenter], FNORMAL);
             print_font (double_buffer,
                         280 - (strlen (keynames[kesc]) * 8) + xofs,
                         128 + yofs, keynames[kesc], FNORMAL);
             if (is_sound == 0)
               {
                  print_font (double_buffer, 256 + xofs, 144 + yofs, "OFF",
                              FNORMAL);
                  print_font (double_buffer, 256 + xofs, 152 + yofs, "N/A",
                              FDARK);
                  print_font (double_buffer, 256 + xofs, 160 + yofs, "N/A",
                              FDARK);
               }
             else
               {
                  print_font (double_buffer, 264 + xofs, 144 + yofs, "ON",
                              FNORMAL);
                  sprintf (strbuf, "%3d%%", gsvol * 100 / 250);
                  print_font (double_buffer, 248 + xofs, 152 + yofs, strbuf,
                              FNORMAL);
                  sprintf (strbuf, "%3d%%", gmvol * 100 / 250);
                  print_font (double_buffer, 248 + xofs, 160 + yofs, strbuf,
                              FNORMAL);
               }
             p = ptr;
             if (ptr > 3)
                p++;
             if (ptr > 11)
                p++;
             draw_sprite (double_buffer, menuptr, 32 + xofs, p * 8 + 32 + yofs);
             menubox (double_buffer, xofs, 216 + yofs, 38, 1, BLUE);
             print_font (double_buffer, 8 + xofs, 224 + yofs, dc[ptr], FNORMAL);
             blit2screen (xofs, yofs);
          }
        readcontrols ();
        if (up)
          {
             unpress ();
             ptr--;
             if (ptr < 0)
                ptr = 14;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (down)
          {
             unpress ();
             ptr++;
             if (ptr > 14)
                ptr = 0;
             play_effect (SND_CLICK, 128);
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             switch (ptr)
               {
               case 0:
                  bubble_text (255, "Changing the display mode to or",
                               "from windowed view could have",
                               "serious ramifications. It is",
                               "advised that you save first.");
                  if (windowed == 0)
                     sprintf (strbuf, "Switch to windowed mode?");
                  else
                     sprintf (strbuf, "Switch to full screen?");
                  p = prompt (255, 2, B_TEXT, strbuf, "  no", "  yes", "");
                  if (p == 1)
                    {
                       if (windowed == 0)
                          windowed = 1;
                       else
                          windowed = 0;
                       set_graphics_mode ();
                    }
                  break;
               case 1:
                  bubble_text (255, "Changing the stretched view option",
                               "could have serious ramifications.",
                               "It is advised that you save your",
                               "game before trying this.");
                  if (windowed == 0)
                     sprintf (strbuf, "Try to stretch the display?");
                  else
                     sprintf (strbuf, "Switch to unstretched display?");
                  p = prompt (255, 2, B_TEXT, strbuf, "  no", "  yes", "");
                  if (p == 1)
                    {
                       if (stretch_view == 0)
                          stretch_view = 1;
                       else
                          stretch_view = 0;
                       set_graphics_mode ();
                    }
                  break;
               case 2:
                  if (show_frate == 0)
                     show_frate = 1;
                  else
                     show_frate = 0;
                  break;
               case 3:
                  if (wait_retrace == 0)
                     wait_retrace = 1;
                  else
                     wait_retrace = 0;
                  break;
               case 4:
                  kup = getakey ();
                  unpress ();
                  break;
               case 5:
                  kdown = getakey ();
                  unpress ();
                  break;
               case 6:
                  kleft = getakey ();
                  unpress ();
                  break;
               case 7:
                  kright = getakey ();
                  unpress ();
                  break;
               case 8:
                  kalt = getakey ();
                  unpress ();
                  break;
               case 9:
                  kctrl = getakey ();
                  unpress ();
                  break;
               case 10:
                  kenter = getakey ();
                  unpress ();
                  break;
               case 11:
                  kesc = getakey ();
                  unpress ();
                  break;
               case 12:
                  if (is_sound == 2)
                     sound_init ();
                  else
                    {
                       if (is_sound == 0)
                         {
                            is_sound = 1;
                            sound_init ();
                            play_song (g_map.song_file, 0);
                         }
                    }
                  break;
               case 13:
                  if (is_sound == 2)
                    {
                       p = getavalue ("Sound Volume", 0, 25, gsvol / 10, 1);
                       if (p != -1)
                         {
                            gsvol = p * 10;
                            set_volume (gsvol, 0);
                         }
                    }
                  else
                     play_effect (SND_BAD, 128);
                  break;
               case 14:
                  if (is_sound == 2)
                    {
                       p = getavalue ("Music Volume", 0, 25, gmvol / 10, 1);
                       if (p != -1)
                         {
                            gmvol = p * 10;
                            set_mod_volume (gmvol);
                         }
                    }
                  else
                     play_effect (SND_BAD, 128);
                  break;
               }
             rd = 1;
          }
        if (bctrl)
          {
             unpress ();
             stop = 1;
          }
     }
}

static int getakey (void)
{
   int a;

   clear_keybuf ();
   menubox (double_buffer, 108 + xofs, 108 + yofs, 11, 1, DARKBLUE);
   print_font (double_buffer, 116 + xofs, 116 + yofs, "press a key", FNORMAL);
   blit2screen (xofs, yofs);
   while (keypressed () == 0)
     {
        for (a = 0; a < KEY_MAX; a++)
           if (key[a] != 0)
              return a;
     }
   return 0;
}

static int getavalue (char *capt, int minu, int maxu, int cv, int sp)
{
   int stop = 0, a, b, rd = 1;

   if (maxu == 0)
      return -1;
   while (!stop)
     {
        if (rd == 1)
          {
             menubox (double_buffer, 148 - (maxu * 4) + xofs, 100 + yofs,
                      maxu + 1, 3, DARKBLUE);
             print_font (double_buffer, 160 - (strlen (capt) * 4) + xofs,
                         108 + yofs, capt, FGOLD);
             print_font (double_buffer, 152 - (maxu * 4) + xofs, 116 + yofs,
                         "<", FNORMAL);
             print_font (double_buffer, 160 + (maxu * 4) + xofs, 116 + yofs,
                         ">", FNORMAL);
             b = 160 - (maxu * 4) + xofs;
             for (a = 0; a < cv; a++)
               {
                  rectfill (double_buffer, a * 8 + b + 1, 117 + yofs,
                            a * 8 + b + 7, 123 + yofs, 50);
                  rectfill (double_buffer, a * 8 + b, 116 + yofs,
                            a * 8 + b + 6, 122 + yofs, 21);
               }
             if (sp == 1)
                sprintf (strbuf, "%d%%", cv * 100 / maxu);
             else
                sprintf (strbuf, "%d", cv);
             print_font (double_buffer, 160 - (strlen (strbuf) * 4) + xofs,
                         124 + yofs, strbuf, FGOLD);
             blit2screen (xofs, yofs);
          }
        readcontrols ();
        if (left)
          {
             unpress ();
             cv--;
             if (cv < minu)
                cv = minu;
             rd = 1;
          }
        if (right)
          {
             unpress ();
             cv++;
             if (cv > maxu)
                cv = maxu;
             rd = 1;
          }
        if (balt)
          {
             unpress ();
             stop = 1;
          }
        if (bctrl)
          {
             unpress ();
             return -1;
          }
     }
   return cv;
}



void set_graphics_mode (void)
{
   if (stretch_view == 1)
     {
        if (windowed == 1)
           set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
        else
           set_gfx_mode (GFX_AUTODETECT, 640, 480, 0, 0);
     }
   else
     {
        if (windowed == 1)
           set_gfx_mode (GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);
        else
           set_gfx_mode (GFX_AUTODETECT, 320, 240, 0, 0);
     }

   set_palette (pal);
}



/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated : 20020914 - 05:28 (RB)                                          */
/*  Purpose :                                                                */
/*  Returns : Nothing.                                                       */
/*                                                                           */
void sound_init (void)
{
   if (is_sound == 1)
     {
        reserve_voices (32, -1);
        set_volume_per_voice (2);

        if (install_sound (DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == -1)
          {
             reserve_voices (-1, -1);
             is_sound = 0;
          }
        else
          {
             install_mod (28);
             load_samples ();
             is_sound = 2;
          }
     }
   else if (is_sound == 2)
     {
        stop_music ();
        remove_sound ();
        reserve_voices (-1, -1);
        remove_mod ();
        free_samples ();
        is_sound = 0;
     }
}



/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated : 20020914 - 05:20 (RB)                                          */
/*  Purpose : Load the list of samples from the data file.                   */
/*  Returns : Nothing.                                                       */
/*                                                                           */
static void load_samples (void)
{
   /*  RB FIXME: This must be generated from the kqsnd.h header,  */
   /*            not hardcoded, to make it easier to maintain.    */
   /*            (a perl script?).                                */
   AL_CONST char *sndfiles[MAX_SAMPLES] = {
      "WHOOSH_WAV", "MENUMOVE_WAV", "BAD_WAV", "ITEM_WAV",
      "EQUIP_WAV", "DEEQUIP_WAV", "BUYSELL_WAV", "TWINKLE_WAV",
      "SCORCH_WAV", "POISON_WAV", "CHOP_WAV", "SLASH_WAV",
      "STAB_WAV", "HIT_WAV", "ICE_WAV", "WIND_WAV",
      "QUAKE_WAV", "BLACK_WAV", "WHITE_WAV", "BOLT1_WAV",
      "FLOOD_WAV", "HURT_WAV", "BMAGIC_WAV", "SHIELD_WAV",
      "KILL_WAV", "DOOROPEN_WAV", "DOOR2_WAV", "STAIRS_WAV",
      "TELEPORT_WAV", "CURE_WAV", "RECOVER_WAV", "ARROW_WAV",
      "BOLT2_WAV", "BOLT3_WAV", "FLAME_WAV", "BLIND_WAV",
      "INN_WAV", "CONFUSE_WAV", "DISPEL_WAV", "DOOM_WAV",
      "DRAIN_WAV", "GAS_WAV"
   };
   int index;

   if (is_sound == 0)
      return;

   for (index = 0; index < MAX_SAMPLES; index++)
     {
        sfx[index] = load_datafile_object (SOUND_DATAFILE, sndfiles[index]);
        if (sfx[index] == NULL)
          {
             sprintf (strbuf, "Error loading .WAV file: %s.\n",
                      sndfiles[index]);
             klog (strbuf);
             /*  RB FIXME: why call sound_init() again?  */
             /*  sound_init();  */
          }
     }
}



/*                                                                           */
/*  Author  : Josh Bolduc                                                    */
/*  Created : ???????? - ??:??                                               */
/*  Updated : Never.                                                         */
/*  Purpose : Duh.                                                           */
/*  Returns : Nothing.                                                       */
/*                                                                           */
void free_samples (void)
{
   int index;

   if (is_sound == 0)
      return;

   for (index = 0; index < MAX_SAMPLES; index++)
      unload_datafile_object (sfx[index]);
}



/*
   Play some music... if possible/necessary.
*/
void play_song (char *sngnme, int start_track)
{
   if (is_sound == 0)
      return;
   if (is_mod_playing ())
      stop_mod ();
   if (gsong)
      destroy_mod (gsong);
/*  RB: Modifed from "%s\%s" to "%s/%s"  */
   sprintf (strbuf, "%s/%s", MUSIC_DIR, sngnme);
   if (!(gsong = load_mod (strbuf)))
     {
        sprintf (strbuf, "Could not load %s!", sngnme);
        klog (strbuf);
     }
   else
     {
        if (start_track > 0)
           goto_mod_track (start_track);
        play_mod (gsong, 1);
     }
}

/*
   I have no idea what this function does :p
*/
void stop_music (void)
{
   if (is_sound == 0)
      return;
   if (is_mod_playing ())
      stop_mod ();
}

/*
   Play an effect... if possible/necessary.  If the effect to
   be played is the 'bad-move' effect, than do something visually
   so that even if sound is off you know you did something bad :)
*/
void play_effect (int efc, int panning)
{
   int a, xo = 0, yo = 0;
   int bx[8] = { -1, 0, 1, 0, -1, 0, 1, 0 };
   int by[8] = { -1, 0, 1, 0, 1, 0, -1, 0 };

   if (is_sound != 0)
      play_sample ((SAMPLE *) sfx[efc]->dat, gsvol, panning, 1000, 0);
   if (efc == SND_BAD)
     {
        blit (double_buffer, back, 0, 0, 0, 0, 352, 280);
        blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
        clear_bitmap (double_buffer);
        blit (back, double_buffer, xofs, yofs, xofs, yofs, 320, 240);
        if (in_combat == 0)
          {
             xo = xofs;
             yo = yofs;
          }
        for (a = 0; a < 8; a++)
          {
             blit2screen (xo + bx[a], yo + by[a]);
             rest (10);
          }
        blit (back, double_buffer, 0, 0, 0, 0, 352, 280);
     }
}
