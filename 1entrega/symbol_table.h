#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stddef.h>
#include <stdbool.h>

#define SYM_NAME_SZ 35
#define SYM_VAL_SZ  50

typedef enum {
	TABLE_INT,
	TABLE_REAL,
	TABLE_STRING
} DataType;

typedef struct {
	char name[SYM_NAME_SZ + 1];
	DataType type;
	char value[SYM_VAL_SZ + 1];
	size_t len;
} Symbol;

typedef struct _node_s {
	Symbol sym;
	struct _node_s *next;
} Node;

typedef Node* List;

typedef struct {
	Node* current_node;
} ListIterator;

List NewList(void);
void AddSymbol(List *lst, const Symbol * const sym);

ListIterator Iterator(const List * const lst);

bool HasNext(ListIterator *it);
Symbol Next(ListIterator *it);

#endif
