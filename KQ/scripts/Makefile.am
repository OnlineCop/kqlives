# Makefile to compile all .lua into .lob
# They are preprocessed first into pscripts/ and then compiled.  This
# two-step process allows luac to give better error messages, though
# the line numbers will refer to the versions in pscripts rather than
# the original scripts


LUAS:=$(wildcard $(srcdir)/*.lua)
LOBS:=$(notdir $(LUAS:.lua=.lob))
CPP=cpp -P -undef
SUFFIXES=.lua .lob

INCLUDES:=$(top_srcdir)/include/progress.h $(top_srcdir)/include/itemdefs.h

all-am: Makefile confirm-dir $(LOBS)

%.lob: pscripts/%.lua $(INCLUDES)
	luac -o $@ $<


pscripts/%.lua: $(srcdir)/%.lua
	$(CPP) $(foreach X, $(INCLUDES), -include $(X)) $< > $@

confirm-dir:
	if test ! -d pscripts; then $(mkdir_p) pscripts ; fi
