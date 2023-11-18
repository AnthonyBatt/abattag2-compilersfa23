#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_FLOAT,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_ERROR
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr* length;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr *length );
void          type_print( struct type *t );
void			  type_print_err(struct type *t);
int           type_equals(struct type *a, struct type *b);	

#endif
