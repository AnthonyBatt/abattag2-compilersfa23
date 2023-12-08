#ifndef EXPR_H
#define EXPR_H

#include "encoder.h"
#include "stmt.h"
#include "symbol.h"

#include <stdlib.h>
#include <string.h>

typedef enum {
	EXPR_NAME,
	EXPR_INTEGER_LITERAL,
	EXPR_FLOAT_LITERAL,
	EXPR_BOOLEAN_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_ASN,
	EXPR_LGO,
	EXPR_LGA,
	EXPR_LT,
	EXPR_LE,
	EXPR_GT,
	EXPR_GE,
	EXPR_EQ,
	EXPR_NE,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_EXP,
	EXPR_NEG,
	EXPR_PLS,
	EXPR_NOT,
	EXPR_INC,
	EXPR_DEC,
	EXPR_GRP,
	EXPR_FXN,
	EXPR_ARR
	/* may need to add more */
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	float float_literal;
	const char* string_literal;
	struct stmt *args;
	struct symbol *symbol;
	struct type *type;
	int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_float_literal( float c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( int c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_create_function_call( const char *n, struct stmt *a );
struct expr * expr_create_array_access( const char *n, struct expr *i, struct expr *j );

void expr_print( struct expr *e );
void expr_resolve(struct expr *e);
struct type *expr_typecheck(struct expr *e);
void expr_codegen(struct expr *e);

#endif
