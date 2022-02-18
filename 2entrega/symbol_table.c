#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

List NewList(void)
{
	const List lst = NULL;

	return lst;
}


void AddSymbol(List *lst, const Symbol * const sym)
{
	Node *new = malloc(sizeof(Node));
	assert( new != NULL );

	int cmp = -1;
	while ( *lst != NULL
			&& ( cmp = strcmp((*lst)->sym.name, sym->name) ) < 0 ) {
		lst = &((*lst)->next);
	}

	/* ya esta en la lista */
	if ( cmp == 0 )
		return;

	/* Agrego el simbolo en orden */
	new->sym = *sym;
	new->next = *lst;
	*lst = new;
}



ListIterator Iterator(const List * const lst)
{
	assert(lst != NULL);

	ListIterator it = {
		.current_node = *lst
	};

	return it;
}



bool HasNext(ListIterator *it)
{
	assert(it != NULL);
	return it->current_node != NULL;
}


Symbol Next(ListIterator *it)
{
	assert(it != NULL);
	assert(it->current_node != NULL);

	/* guardo el simbolo a retornar y avanzo */
	const Symbol sym = it->current_node->sym;
	it->current_node = it->current_node->next;

	return sym;
}


