/*! \file
* \brief Functions to load/save to disk
*
* These functions are endian independent
* \author PH
* \date 20030629
*/
#include "disk.h"

int load_s_map (s_map * sm, PACKFILE * f)
{
   sm->map_no = pack_getc (f);
   sm->zero_zone = pack_getc (f);
   sm->map_mode = pack_getc (f);
   sm->can_save = pack_getc (f);
   sm->tileset = pack_getc (f);
   sm->use_sstone = pack_getc (f);
   sm->can_warp = pack_getc (f);
   sm->extra_byte = pack_getc (f);
   sm->xsize = pack_igetl (f);
   sm->ysize = pack_igetl (f);
   sm->pmult = pack_igetl (f);
   sm->pdiv = pack_igetl (f);
   sm->stx = pack_igetl (f);
   sm->sty = pack_igetl (f);
   sm->warpx = pack_igetl (f);
   sm->warpy = pack_igetl (f);
   sm->extra_sdword1 = pack_igetl (f);
   sm->extra_sdword2 = pack_igetl (f);
   pack_fread (sm->song_file, sizeof (sm->song_file), f);
   pack_fread (sm->map_desc, sizeof (sm->map_desc), f);
   return 0;
}

int load_s_entity (s_entity * s, PACKFILE * f)
{
   s->chrx = pack_getc (f);
   pack_getc (f);               /* alignment */
   s->x = pack_igetw (f);
   s->y = pack_igetw (f);
   s->tilex = pack_igetw (f);
   s->tiley = pack_igetw (f);
   s->eid = pack_getc (f);
   s->active = pack_getc (f);
   s->facing = pack_getc (f);
   s->moving = pack_getc (f);
   s->movcnt = pack_getc (f);
   s->framectr = pack_getc (f);
   s->movemode = pack_getc (f);
   s->obsmode = pack_getc (f);
   s->delay = pack_getc (f);
   s->delayctr = pack_getc (f);
   s->speed = pack_getc (f);
   s->scount = pack_getc (f);
   s->cmd = pack_getc (f);
   s->sidx = pack_getc (f);
   s->extra = pack_getc (f);
   s->chasing = pack_getc (f);
   pack_igetw (f);              /* alignment */
   s->cmdnum = pack_igetl (f);
   s->atype = pack_getc (f);
   s->snapback = pack_getc (f);
   s->facehero = pack_getc (f);
   s->transl = pack_getc (f);
   pack_fread (s->script, sizeof (s->script), f);
   return 0;
}

int load_s_player (s_player * s, PACKFILE * f)
{
   int i;
   pack_fread (s->name, sizeof (s->name), f);
   pack_getc (f);               /* alignment */
   pack_getc (f);               /* alignment */
   pack_getc (f);               /* alignment */
   s->xp = pack_igetl (f);
   s->next = pack_igetl (f);
   s->lvl = pack_igetl (f);
   s->mrp = pack_igetl (f);
   s->hp = pack_igetl (f);
   s->mhp = pack_igetl (f);
   s->mp = pack_igetl (f);
   s->mmp = pack_igetl (f);
   for (i = 0; i < NUM_STATS; ++i)
     {
        s->stats[i] = pack_igetl (f);
     }
   for (i = 0; i < 16; ++i)
     {
        s->res[i] = pack_getc (f);
     }
   for (i = 0; i < 24; ++i)
     {
        s->sts[i] = pack_getc (f);
     }
   for (i = 0; i < 6; ++i)
     {
        s->eqp[i] = pack_getc (f);
     }
   for (i = 0; i < 60; ++i)
     {
        s->spells[i] = pack_getc (f);
     }
   pack_getc (f);               /* alignment */
   pack_getc (f);               /* alignment */
   return 0;
}

int save_s_player (s_player * s, PACKFILE * f)
{
   int i;
   pack_fwrite (s->name, sizeof (s->name), f);
   pack_putc (0, f);            /* alignment */
   pack_putc (0, f);            /* alignment */
   pack_putc (0, f);            /* alignment */
   pack_iputl (s->xp, f);
   pack_iputl (s->next, f);
   pack_iputl (s->lvl, f);
   pack_iputl (s->mrp, f);
   pack_iputl (s->hp, f);
   pack_iputl (s->mhp, f);
   pack_iputl (s->mp, f);
   pack_iputl (s->mmp, f);
   for (i = 0; i < NUM_STATS; ++i)
     {
        pack_iputl (s->stats[i], f);
     }
   for (i = 0; i < 16; ++i)
     {
        pack_putc (s->res[i], f);
     }
   for (i = 0; i < 24; ++i)
     {
        pack_putc (s->sts[i], f);
     }
   for (i = 0; i < 6; ++i)
     {
        pack_putc (s->eqp[i], f);
     }
   for (i = 0; i < 60; ++i)
     {
        pack_putc (s->spells[i], f);
     }
   pack_putc (0, f);            /* alignment */
   pack_putc (0, f);            /* alignment */
   return 0;
}
