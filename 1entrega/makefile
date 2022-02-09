parser: Lexico.l Sintactico.y symbol.o
	bison -dy Sintactico.y
	flex Lexico.l
	gcc -g symbol.o lex.yy.c y.tab.c -lfl -o parser

symbol.o: symbol_table.c symbol_table.h
	gcc -g -c -o symbol.o symbol_table.c

.PHONY: clean

clean:
	rm lex.yy.c parser y.tab.c y.tab.h symbol.o lex.out test

.PHONY: test_sym

test_sym: symbol.o
	gcc -g -o test symbol_table_test.c symbol.o
	./test
