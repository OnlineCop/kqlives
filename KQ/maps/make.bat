@if [%1] == [g++] goto SET_GPP
@set KQ_COMPILER=gcc
@goto KQ_GO
:SET_GPP
@set KQ_COMPILER=g++
:KQ_GO
%KQ_COMPILER% -c -o mapdata.o mapdata.c
%KQ_COMPILER% -c -o mapdiff.o mapdiff.c
%KQ_COMPILER% -c -o mapdump.o mapdump.c
%KQ_COMPILER% -c -o mapedit.o mapedit.c
%KQ_COMPILER% -c -o mapent.o mapent.c
%KQ_COMPILER% -c -o mapfile.o mapfile.c
%KQ_COMPILER% -c -o mapshared.o mapshared.c
%KQ_COMPILER% -c -o mapstructs.o mapstructs.c

%KQ_COMPILER% -c -o disk.o ../src/disk.c -I../include
%KQ_COMPILER% -c -o bounds.o ../src/bounds.c -I../include

%KQ_COMPILER% mapedit.o mapfile.o mapshared.o mapent.o mapdata.o mapstructs.o bounds.o disk.o -o mapdraw.exe -mwindows -lalleg
%KQ_COMPILER% mapdump.o mapshared.o disk.o -o mapdump.exe -lalleg
%KQ_COMPILER% mapdiff.o disk.o -o mapdiff.exe -lalleg

@set KQ_COMPILER=
