@echo off
gcc -Wall -c -o mapedit.o mapedit.c
gcc -Wall -c -o mapfile.o mapfile.c
gcc -Wall -c -o mapdata.o mapdata.c
gcc -Wall -c -o mapent.o mapent.c
gcc -Wall -c -o disk.o ../src/disk.c -I../include
gcc -Wall -c -o mapdump.o mapdump.c
gcc mapedit.o mapfile.o mapdata.o mapent.o disk.o -o mapdraw.exe -lalleg
gcc mapdump.o disk.o -o mapdump.exe -lalleg
