# make the map editor
# makefile by PH
mapdraw$(EXE_SUFFIX): mapedit.o mapfile.o mapent.o mapdata.o
	$(LINK.o)  $^ -o $@ $(LDLIBS) $(LFLAGS)

mapedit.o: mapedit.c mapdraw.h
mapfile.o: mapfile.c mapdraw.h
mapdata.o: mapdata.c mapdraw.h
mapent.o:  mapent.c  mapdraw.h
