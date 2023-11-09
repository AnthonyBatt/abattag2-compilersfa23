#include "symbol.h"

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name)
{
	struct symbol *sym = malloc(sizeof(struct symbol));

	sym->kind = kind;
	sym->type = type;
	sym->name = name;
	sym->which = -1; // make it negative one to show it has been set yet

	return sym;
}
