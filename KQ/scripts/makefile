# Makefile to compile all .lua into .lob
# perl is preferred but
# define NO_PERL if you don't have a working
# perl and it will use cpp instead,
# though there will be some warnings!
# I only tested it with GNU cpp, no idea if
# it will work for msvc. Default guess is NO
# (as always for msvc :P )

NO_PERL:=1

LUAS:=$(wildcard *.lua)
LOBS:=$(LUAS:.lua=.lob)
CPP=cpp -P -undef
INCLUDES:=../include/progress.h ../include/itemdefs.h

all: $(LOBS)

ifndef NO_PERL
%.lob: %.lua $(INCLUDES)
	perl prep.pl < $< | luac -o $@ -
else
%.lob: %.lua $(INCLUDES)
	$(CPP) $(foreach X, $(INCLUDES), -imacros $(X)) $< | luac -o $@ -
endif
