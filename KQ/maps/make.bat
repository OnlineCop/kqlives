@echo off
gcc -Wall -W -c -o mapedit.o mapedit.c
gcc -Wall -W -c -o mapfile.o mapfile.c
gcc -Wall -W -c -o mapdata.o mapdata.c
gcc -Wall -W -c -o mapent.o mapent.c
gcc -Wall -W -c -o mapshared.o mapshared.c
gcc -Wall -W -c -o disk.o ../src/disk.c -I../include
gcc -Wall -W -c -o mapdump.o mapdump.c
gcc -Wall -W -c -o mapdiff.o mapdiff.c
gcc mapedit.o mapfile.o mapshared.o mapent.o mapdata.o disk.o -o mapdraw.exe -mwindows -lalleg
gcc mapdump.o mapshared.o disk.o -o mapdump.exe -lalleg
gcc mapdiff.o disk.o -o mapdiff.exe -lalleg
