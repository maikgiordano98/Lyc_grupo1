#include "symbol_table.h"
#include <stdio.h>

int main(void)
{
	Symbol symbols[2] = {
		{"uno", TABLE_INT, "10", 0},
		{"dos", TABLE_STRING, "dos", 3}
	};

	List lst = NewList();
	for ( int i = 0 ; i < 2 ; ++i ) {
		AddSymbol(&lst, &symbols[i]);
	}

	ListIterator it = Iterator(&lst);
	while ( HasNext(&it) ) {
		Symbol sym = Next(&it);
		printf("%s\n", sym.name);
	}

	return 0;
}
