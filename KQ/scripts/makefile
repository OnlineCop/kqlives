LUAS:=$(wildcard *.lua)
LOBS:=$(LUAS:.lua=.lob)
all: $(LOBS)

%.lob: %.lua ../progress.h ../itemdefs.h
	perl prep.pl < $< | luac -o $@ -
