
#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	char *name;
	int which; // starts counting at 0
};

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name );
void symbol_print(struct symbol *sym);
const char *symbol_codegen(struct symbol *s);

#endif
