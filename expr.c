#include "expr.h"
#include <string.h>

extern int rerror;

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right)
{
	struct expr *e = malloc(sizeof(struct expr));

	e->kind = kind;
	e->left = left;
	e->right = right;

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

struct expr *expr_create_char_literal(int c)
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
		int c = e->literal_value;

		// \\    //
		if (c == 92)
		{
			fprintf(stdout, "'\\\\'");
		}
		// \"
		else if (c == 34)
		{
			fprintf(stdout, "'\\\"'");
		}
		// \'
		else if (c == 39)
		{
			fprintf(stdout, "'\\\''");
		}
		// \a
		else if (c == 7)
		{
			fprintf(stdout, "'\\a'");
		}
		// \b
		else if (c == 8)
		{
			fprintf(stdout, "'\\b'");
		}
		// \e
		else if (c == 27)
		{
			fprintf(stdout, "'\\e'");
		}
		// \f
		else if (c == 12)
		{
			fprintf(stdout, "'\\f'");
		}
		// \n
		else if (c == 10)
		{
			fprintf(stdout, "'\\n'");
		}
		// \r
		else if (c == 13)
		{
			fprintf(stdout, "'\\r'");
		}
		// \t
		else if (c == 9)
		{
			fprintf(stdout, "'\\t'");
		}
		// \v
		else if (c == 11)
		{
			fprintf(stdout, "'\\v'");
		}
		// \0xHH
		else if ( /*c > 126 || */ c < 32)
		{
			// we need to make two hexdigits out of the value 
			unsigned char hex[3];
			sprintf((char *)hex, "%.2x", c);
			//printf("%s\n", hex);
			fprintf(stdout, "'\\0x%c%c'", hex[0], hex[1]);
		}
		// it is a printable non special character
		else
		{
			fprintf(stdout, "'%c'", e->literal_value);
		}
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
		//printf("%d", e->args->kind);
		fprintf(stdout, "%s(", e->name);
		stmt_print(e->args, 0);
		fprintf(stdout, ")");
	}
	// TODO make sure array accesses like arr[x][y][z] work
	else if (e->kind == EXPR_ARR)
	{
		if (e->name)
		{
			fprintf(stdout, "%s[", e->name);
			expr_print(e->left);
			fprintf(stdout, "]");
		}
		else
		{
			expr_print(e->left);
		}

		// if it is a multi dimensional array access
		if (e->right)
		{
		/*	printf("\nright: %d\n", e->right->kind);
			if (e->left) {
				printf("left: %d\n", e->left->kind);
			}*/
			if (!e->name)
				fprintf(stdout, "[");
			expr_print(e->right);
			if (!e->name)
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

void expr_resolve(struct expr *e)
{
	if (!e) return;

	if (e->kind==EXPR_NAME)
	{
		e->symbol = scope_lookup((char *)e->name);
		if (!e->symbol)
		{
			fprintf(stderr, "resolve error (expression): %s has yet to be declared in this scope\n", e->name);
			rerror++;
			return;
		}
		else
		{
			//scope_bind((char *)e->name, e->symbol);
			symbol_print(e->symbol);
		}
	}
	else
	{
		expr_resolve(e->left);
		expr_resolve(e->right);
	}
}
