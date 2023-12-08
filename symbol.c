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

void symbol_print(struct symbol *sym)
{
	if (sym->kind == SYMBOL_GLOBAL)
	{
		fprintf(stdout, "%s resolves to global %s\n", sym->name, sym->name);
	}
	else if (sym->kind == SYMBOL_PARAM)
	{
		fprintf(stdout, "%s resolves to param %d\n", sym->name, sym->which);
	}
	else if (sym->kind == SYMBOL_LOCAL)
	{
		fprintf(stdout, "%s resolves to local %d\n", sym->name, sym->which);
	}
}

const char *symbol_codegen(struct symbol *s)
{
	// as part of the function type, may need to add tracking of function parameters to the function type
	// either that or just always store at certain positions even if you are storing nothing


	return NULL;
}
