How to compile KQ with MinGW and MSYS
=====================================

1. MinGW
You can either use the installer (MinGW 5.0.x) or download and install manually:
- mingw runtime
- gcc-core
- binutils
- w32-api
- mingw32-make

2. MSYS
Download and install MSYS-1.0.10.exe. If you build KQ from CVS, also get msysDTK-1.0.1.exe.
Make sure that you have "mounted" /mingw in /etc/fstab

3. Lua
Download and extract lua-5.0.x.tar.gz. Edit the file "config",
change "INSTALL_ROOT= /usr/local" to "INSTALL_ROOT= /mingw"
and run "make && make install" (in MSYS).

for crosscompilation, run this:
make CC=i586-mingw32msvc-gcc RANLIB=i586-mingw32msvc-ranlib \
mingw
make INSTALL_TOP=/usr/local/i586-mingw32msvc \
'INSTALL_EXEC= $(INSTALL) -m 0755' 'INSTALL_DATA= $(INSTALL) -m 0644' \
"TO_LIB=lua51.dll" "TO_BIN=lua.exe luac.exe" \
install

4. Allegro
Download and extract all4xx.zip.
Download and extract to the MinGW root directory dx70_mgw.zip or dx80_mgw.zip.
Run "fix.sh mingw32 && MINGDIR=/mingw make && MINGDIR=/mingw make install" in the allegro directory. (in MSYS)

5. DUMB
Download and extract dumb-0.9.x.zip.
Start the regular windows shell, and run:
  >cd \path\to\dumb-0.9.x
  >set MINGDIR="c:\path\to\MinGW"
  >mingw32-make
  >mingw32-make install
If you do not want to put mingw in your PATH globally, this also works:
  >cd \path\to\dumb-0.9.x
  >PATH="c:\path\to\MinGW\bin";%PATH%
  >set MINGDIR="c:\path\to\MinGW"
  >mingw32-make CC="c:\\path\\to\\MinGW\\bin\\gcc.exe"
  >mingw32-make install

6. KQ
If you build from CVS, run "autoreconf -i".
That creates configure from configure.ac and Makefile.in from Makefile.am.
Run "./configure", then "make". "./configure --help" gives some possible options.
