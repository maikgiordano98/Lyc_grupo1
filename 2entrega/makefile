parser: Lexico.l Sintactico.y symbol.o pila.o cola.o lista.o funciones.o
	bison -dy Sintactico.y
	flex Lexico.l
	gcc -g symbol.o pila.o cola.o lista.o funciones.o lex.yy.c y.tab.c -lfl -o parser

symbol.o: symbol_table.c symbol_table.h
	gcc -g -c -o symbol.o symbol_table.c

pila.o: Pila.h
	gcc -g -c -o pila.o Pila.h

cola.o: Cola.h
	gcc -g -c -o cola.o Cola.h

lista.o: Lista.h
	gcc -g -c -o lista.o Lista.h

funciones.o: funciones.h
	gcc -g -c -o funciones.o funciones.h

.PHONY: clean

clean:
	rm lex.yy.c parser y.tab.c y.tab.h symbol.o lista.o cola.o pila.o funciones.o lex.out test

.PHONY: test_sym

test_sym: symbol.o
	gcc -g -o test symbol_table_test.c symbol.o
	./test
