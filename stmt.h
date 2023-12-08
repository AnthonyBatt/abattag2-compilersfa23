
#ifndef STMT_H
#define STMT_H

#include "decl.h"

#include <stdlib.h>
#include <string.h>

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_EXPR_LS,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
	struct stmt *next_elem;
	int braces;
};

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next, struct stmt *elem, int braces );
void stmt_print( struct stmt *s, int indent );
void stmt_resolve(struct stmt *s);
void stmt_typecheck(struct stmt *s, struct type *ret_t, struct type *arr_t);
void stmt_codegen(struct stmt *s, FILE *fp);

#endif
