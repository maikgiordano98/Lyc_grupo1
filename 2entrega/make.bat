C:\GnuWin32\bin\flex.exe Lexico.l
C:\GnuWin32\bin\bison.exe -dyv Sintactico.y
C:\MinGW\bin\gcc.exe -o parser.exe -w lex.yy.c y.tab.c symbol_table.c

