#include "expr.h"
#include <string.h>

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right)
{
	struct expr *e = malloc(sizeof(struct expr));

	e->kind = kind;
	e->left = left;
	e->right = right;

/*					TODO: ask matt or evan or someone if we need to track values as we go
	if (...)
	{
	
	}
	else if (...)
	{

	}
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
	EXPR_PIC,
	EXPR_PDC,
	EXPR_GRP,
	EXPR_FXN,
	EXPR_ARR
*/

	return e;
}

struct expr *expr_create_name(const char *n)
{
	struct expr *name = expr_create(EXPR_NAME, 0, 0);
	name->name = strdup(n);
	
	return name;
}

struct expr *expr_create_integer_literal(int c)
{
	struct expr *integer = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
	integer->literal_value = c;

	return integer;
}

struct expr *expr_create_float_literal(float f)
{
	struct expr *float_ = expr_create(EXPR_FLOAT_LITERAL, 0, 0);
	float_->float_literal = f;

	return float_;
}

struct expr *expr_create_boolean_literal(int c)
{
	struct expr *boolean = expr_create(EXPR_BOOLEAN_LITERAL, 0, 0);
	boolean->literal_value = c;

	return boolean;
}

struct expr *expr_create_char_literal(char c)
{
	struct expr *character = expr_create(EXPR_CHAR_LITERAL, 0, 0);
	character->literal_value = c;

	return character;
}

struct expr *expr_create_string_literal(const char *str)
{
	struct expr* string = expr_create(EXPR_STRING_LITERAL, 0, 0);
	string->string_literal = strdup(str);

	return string;
}

struct expr *expr_create_function_call(const char *n, struct stmt *a)
{
	struct expr* fxn = expr_create(EXPR_FXN, 0, 0);
	fxn->name = strdup(n);
	fxn->args = a;

	return fxn;
}

struct expr *expr_create_array_access(const char *n, struct expr *i, struct expr *j)
{
	struct expr* arr = expr_create(EXPR_ARR, i, j);
	arr->name = strdup(n);

	return arr;
}

void expr_print(struct expr *e)
{
	// some things depend on this I think
	if (!e) return;


	if (e->kind == EXPR_NAME)
	{
		fprintf(stdout, "%s", e->name);
	}
	else if (e->kind == EXPR_INTEGER_LITERAL)
	{
		fprintf(stdout, "%d", e->literal_value);
	}
	else if (e->kind == EXPR_CHAR_LITERAL)
	{
		fprintf(stdout, "'%c'", e->literal_value);
	}
	else if (e->kind == EXPR_BOOLEAN_LITERAL)
	{
		if (e->literal_value) 	fprintf(stdout, "true");
		else							fprintf(stdout, "false");
	}
	else if (e->kind == EXPR_FLOAT_LITERAL)
	{
		fprintf(stdout, "%g", e->float_literal);
	}
	else if (e->kind == EXPR_STRING_LITERAL)
	{
		fprintf(stdout, "%s", e->string_literal);
	}
	else if (e->kind == EXPR_FXN)
	{
		fprintf(stdout, "%s(", e->name);
		stmt_print(e->args, 0);
		fprintf(stdout, ")");
	}
	// TODO make sure array accesses like arr[x][y][z] work
	else if (e->kind == EXPR_ARR)
	{
		fprintf(stdout, "%s[", e->name);
		expr_print(e->left);
		fprintf(stdout, "]");
		// if it is a multi dimensional array access
		if (e->right)
		{
			fprintf(stdout, "[");
			expr_print(e->right);
			fprintf(stdout, "]");
		}
	}
	else if (e->kind == EXPR_ASN)
	{
		expr_print(e->left);
		fprintf(stdout, "=");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_LGO)
	{
		expr_print(e->left);
		fprintf(stdout, "||");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_LGA)
	{
		expr_print(e->left);
		fprintf(stdout, "&&");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_LT)
	{
		expr_print(e->left);
		fprintf(stdout, "<");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_LE)
	{
		expr_print(e->left);
		fprintf(stdout, "<=");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_GT)
	{
		expr_print(e->left);
		fprintf(stdout, ">");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_GE)
	{
		expr_print(e->left);
		fprintf(stdout, ">=");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_EQ)
	{
		expr_print(e->left);
		fprintf(stdout, "==");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_NE)
	{
		expr_print(e->left);
		fprintf(stdout, "!=");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_ADD)
	{
		expr_print(e->left);
		fprintf(stdout, "+");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_SUB)
	{
		expr_print(e->left);
		fprintf(stdout, "-");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_MUL)
	{
		expr_print(e->left);
		fprintf(stdout, "*");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_DIV)
	{
		expr_print(e->left);
		fprintf(stdout, "/");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_MOD)
	{
		expr_print(e->left);
		fprintf(stdout, "%%");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_EXP)
	{
		expr_print(e->left);
		fprintf(stdout, "^");
		expr_print(e->right);
	}
	else if (e->kind == EXPR_NEG)
	{
		fprintf(stdout, "-");
		expr_print(e->left);
	}
	else if (e->kind == EXPR_PLS)
	{
		fprintf(stdout, "+");
		expr_print(e->left);
	}
	else if (e->kind == EXPR_NOT)
	{
		fprintf(stdout, "!");
		expr_print(e->left);
	}
	else if (e->kind == EXPR_INC)
	{
		expr_print(e->left);
		fprintf(stdout, "++");
	}
	else if (e->kind == EXPR_DEC)
	{
		expr_print(e->left);
		fprintf(stdout, "--");
	}
	else if (e->kind == EXPR_GRP)
	{
		fprintf(stdout, "(");
		expr_print(e->left);
		fprintf(stdout, ")");
	}
}
