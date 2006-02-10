SUBDIRS=scripts
bin_PROGRAMS=kq mapdraw mapdiff mapdump

kq_SOURCES = src/enemyc.c src/intrface.c src/sgame.c\
	src/combat.c src/entity.c src/movement.c src/shopmenu.c\
	src/credits.c src/eqpmenu.c src/itemmenu.c src/music.c src/timing.c\
	src/disk.c src/eskill.c src/kq.c  src/res.c \
	src/fade.c src/magic.c src/scrnshot.c 	\
	src/draw.c src/heroc.c src/masmenu.c src/selector.c\
	src/effects.c src/hskill.c src/menu.c  src/setup.c \
	include/combat.h include/fade.h include/masmenu.h include/setup.h\
	include/credits.h include/heroc.h include/menu.h include/sgame.h\
	include/disk.h include/hskill.h include/movement.h include/shopmenu.h\
	include/draw.h include/intrface.h include/mpcx.h include/skills.h\
	include/effects.h include/itemdefs.h include/music.h include/ssprites.h\
	include/enemyc.h include/itemmenu.h include/progress.h include/structs.h\
	include/entity.h include/kq.h include/res.h include/timing.h\
	include/eqpmenu.h include/kqsnd.h include/scrnshot.h\
	include/eskill.h include/magic.h include/selector.h 
EXTRA_kq_SOURCES = src/unix.c src/osx.m src/beos.c src/win.c src/dos.c
kq_LDADD = $(kq_extra_files)
kq_DEPENDENCIES = $(kq_extra_files)

mapdraw_SOURCES = maps/mapdata.c maps/mapdraw.h maps/mapedit.c\
maps/mapfile.c maps/mapent.c maps/mapshared.c src/disk.c \
include/disk.h include/structs.h

mapdiff_SOURCES = maps/mapdraw.h maps/mapdiff.c src/disk.c \
include/disk.h include/structs.h

mapdump_SOURCES = maps/mapdump.c maps/mapshared.c maps/mapdata.c\
src/disk.c include/disk.h include/structs.h

APPR=KQ.app/Contents/Resources
KQ.app: kq
	install -d $(APPR)/data $(APPR)/music $(APPR)/scripts $(APPR)/maps 
	install -d KQ.app/Contents/MacOS
	install -C Mac/Info.plist KQ.app/Contents
	install -C bin/kq KQ.app/Contents/MacOS
	install -C $(dist_gfx_DATA) $(APPR)/data
	install -C $(dist_map_DATA) $(APPR)/maps
	install -C $(dist_music_DATA) $(APPR)/music
	install -C $(cscripts_DATA) $(APPR)/scripts
	install Mac/KQ-icon.icns $(APPR)/KQ.icns

dist_map_DATA=  maps/starting.map maps/bridge2.map\
maps/estate.map maps/temple1.map maps/bridge.map \
maps/temple2.map maps/camp.map maps/fort.map maps/tower.map\
maps/cave1.map maps/goblin.map maps/town1.map maps/cave2.map\
maps/grotto2.map maps/town2.map maps/cave3a.map maps/grotto.map\
maps/town3.map maps/cave3b.map maps/guild.map maps/town4.map\
maps/cave4.map maps/main.map maps/town5.map maps/manor.map maps/town6.map\
maps/cave5.map maps/town7.map maps/cave6a.map maps/town8.map\
maps/cave6b.map  maps/coliseum.map maps/pass.map\
maps/cult.map maps/dville.map 

mapdir=$(pkgdatadir)/maps

cscripts_DATA=scripts/bridge2.lob scripts/dville.lob\
scripts/starting.lob scripts/bridge.lob scripts/estate.lob\
scripts/temple1.lob scripts/camp.lob scripts/esteria.lob\
scripts/temple2.lob scripts/cave1.lob scripts/fort.lob\
scripts/tower.lob scripts/cave2.lob scripts/global.lob\
scripts/town1.lob scripts/cave3a.lob scripts/goblin.lob\
scripts/town2.lob scripts/cave3b.lob scripts/grotto2.lob\
scripts/town3.lob scripts/cave4.lob scripts/grotto.lob\
scripts/town4.lob scripts/cave5.lob scripts/guild.lob\
scripts/town5.lob scripts/cave6a.lob scripts/main.lob\
scripts/town6.lob scripts/cave6b.lob scripts/manor.lob\
scripts/town7.lob scripts/town8.lob scripts/coliseum.lob\
scripts/pass.lob scripts/tunnel.lob scripts/cult.lob\
scripts/shrine.lob
MOSTLYCLEANFILES = $(cscripts_DATA)
cscriptsdir=$(pkgdatadir)/scripts

EXTRA_DIST=scripts/bridge2.lua scripts/dville.lua\
scripts/starting.lua scripts/bridge.lua scripts/estate.lua\
scripts/temple1.lua scripts/camp.lua scripts/esteria.lua\
scripts/temple2.lua scripts/cave1.lua scripts/fort.lua\
scripts/tower.lua scripts/cave2.lua scripts/global.lua\
scripts/town1.lua scripts/cave3a.lua scripts/goblin.lua\
scripts/town2.lua scripts/cave3b.lua scripts/grotto2.lua\
scripts/town3.lua scripts/cave4.lua scripts/grotto.lua\
scripts/town4.lua scripts/cave5.lua scripts/guild.lua\
scripts/town5.lua scripts/cave6a.lua scripts/main.lua\
scripts/town6.lua scripts/cave6b.lua scripts/manor.lua\
scripts/town7.lua scripts/town8.lua scripts/coliseum.lua\
scripts/pass.lua scripts/tunnel.lua scripts/cult.lua\
scripts/shrine.lua Mac/Info.plist Mac/KQ-icon.icns

dist_gfx_DATA=data/kqsnd.dat data/mpcx.dat data/ssprites.dat\
data/hero.kq data/tileset.kq data/allstat.mon data/resabil.mon

gfxdir=$(pkgdatadir)/data

dist_music_DATA=music/aa_arofl.xm music/exp.s3m music/oxford.s3m\
music/walk.s3m music/comeback.mod music/hm.s3m music/rain.s3m\
music/waterw.xm music/dag_4.xm music/infanita.mod music/rend5.s3m\
music/enfero.xm music/into61.s3m music/toroia.s3m music/eranasp.mod\
music/land.mod music/town.mod

musicdir=$(pkgdatadir)/music

dist_man_MANS=debian/kq.6

AM_CFLAGS=-I$(top_srcdir)/include -DKQ_BIN=\"$(bindir)/KQ$(EXEEXT)\" -DKQ_DATA=\"$(pkgdatadir)\"

