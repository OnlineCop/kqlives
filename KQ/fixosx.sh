#!/bin/sh 
# Fix for Unix (MacOSX) build
echo Configuring the package for MacOSX
cat <<EOF > makefile
# generated by fixosx.sh
include mkfiles/makefile.cfg
include mkfiles/makefile.osx
include mkfiles/makefile.all
-include \$(OBJDIR)/makefile.dep
EOF

echo Done!