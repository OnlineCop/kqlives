rem Indent all KQ files as we like them. Requires Gnu Indent
rem See http://gnuwin32.sourceforge.net/

set INDENT_PROFILE=..\other\indent.pro
"c:\program files\gnuwin32\bin\indent" ..\src\*.c ..\include\*.h
