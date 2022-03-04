C:\GnuWin32\bin\flex Lexico.l
c:\GnuWin32\bin\bison -dyv Sintactico.y
c:\MinGW\bin\gcc.exe -w lex.yy.c y.tab.c -o Tercera.exe
Tercera.exe prueba.txt
pause
del lex.yy.c
del Tercera.exe
del y.tab.c
del y.tab.h
del auxiliar.txt
del data.txt
del intermedia.txt
del ts.txt
del y.output
pause