# -*- makefile -*-
# make the map editor
# makefile by PH
DISK_H=../include/structs.h ../include/disk.h

CFLAGS+=-I../include

all: mapdraw$(EXE_SUFFIX) mapdump$(EXE_SUFFIX)

mapdraw$(EXE_SUFFIX): mapedit.o mapfile.o mapent.o mapdata.o disk.o
	$(LINK.o)  $^ -o $@  $(LDFLAGS)

disk.o: ../src/disk.c $(DISK_H)
	$(COMPILE.c)  $< -o $@

mapedit.o: mapedit.c mapdraw.h $(DISK_H)
mapfile.o: mapfile.c mapdraw.h $(DISK_H)
mapdata.o: mapdata.c mapdraw.h $(DISK_H)
mapent.o:  mapent.c  mapdraw.h $(DISK_H)

LDFLAGS  := $(addprefix -l,$(LIBRARY))

mapdump$(EXE_SUFFIX): mapdump.o disk.o
	$(LINK.o)$^ -o $@ $(LDFLAGS)
