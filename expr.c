#include "expr.h"
#include <string.h>

extern int rerror;
extern int terror;

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
			e->type = type_create(TYPE_ERROR, 0, 0, 0);
			rerror++;
			return;
		}
		else
		{
			//scope_bind((char *)e->name, e->symbol);
			symbol_print(e->symbol);
			e->type = e->symbol->type;
		}
	}
	else
	{
		expr_resolve(e->left);
		expr_resolve(e->right);
	}
}

struct type *expr_typecheck(struct expr *e)
{
	if (!e) return 0;

	struct type *lt = expr_typecheck(e->left);
	struct type *rt = expr_typecheck(e->right);

	struct type *ret;

	if (e->kind == EXPR_NAME)
	{
		ret = e->type;
	}
	else if (e->kind == EXPR_INTEGER_LITERAL)
	{
		ret = type_create(TYPE_INTEGER, 0, 0, 0);
	}
	else if (e->kind == EXPR_CHAR_LITERAL)
	{
		ret = type_create(TYPE_CHARACTER, 0, 0, 0);
	}
	else if (e->kind == EXPR_BOOLEAN_LITERAL)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
	}
	else if (e->kind == EXPR_FLOAT_LITERAL)
	{
		ret = type_create(TYPE_FLOAT, 0, 0, 0);
	}
	else if (e->kind == EXPR_STRING_LITERAL)
	{
		ret = type_create(TYPE_STRING, 0, 0, 0);
	}
	// TODO implement function and array
	else if (e->kind == EXPR_FXN)
	{
		printf("\n\n I misjudged my time and could not implement function calls due to an uncaught error in a previous part\n\n\n");
/*
		if (!e->type)
		{
			printf("functions will fuck you :)\n");
			return 0;
		}
		// when we resolve or print or something maybe pass in the function expr to assign it a type
		// functions do not have a left or right
		ret = e->type->subtype;
		// subtype cannot be function or array
*/
		ret = type_create(TYPE_ERROR, 0, 0, 0);
	}
	else if (e->kind == EXPR_ARR)
	{
		if (lt->kind != TYPE_INTEGER)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Array indexes must be integers, you gave");
			type_print_err(lt);
			fprintf(stderr, "\n");
			terror++;
		}
		printf("\n\n I misjudged my time and could not fully implement array accesses due to an uncaught error in a previous part\n\n\n");
		//type_print(lt);
		//type_print(rt);
	/*
		if (!e->type)
		{
			printf("arrays will fuck you :)\n");
			return 0;
		}

		struct type *curr = e->type;
		while (curr->subtype)
		{
			curr = curr->subtype;
		}

		ret = curr;
	*/
		ret = lt;
	}
	else if (e->kind == EXPR_ASN)
	{
		ret = lt;	
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (=): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform assignment (=) on type void\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform assignment (=) on type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_LGO)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform logical OR (||) on non boolean types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_LGA)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform logical AND (&&) on non boolean types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_LT)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (<): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than (<) on type void\n");
			terror++;
		}
		if (lt->kind == TYPE_BOOLEAN || rt->kind == TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than (<) on type boolean\n");
			terror++;
		}
		if (lt->kind == TYPE_STRING || rt->kind == TYPE_STRING)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than (<) on type string\n");
			terror++;
		}
		if (lt->kind == TYPE_ARRAY || rt->kind == TYPE_ARRAY)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than (<) on type array\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than (<) on type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_LE)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (<=): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than or equal to (<=) on type void\n");
			terror++;
		}
		if (lt->kind == TYPE_BOOLEAN || rt->kind == TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than or equal to (<=) on type boolean\n");
			terror++;
		}
		if (lt->kind == TYPE_STRING || rt->kind == TYPE_STRING)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than or equal to (<=) on type string\n");
			terror++;
		}
		if (lt->kind == TYPE_ARRAY || rt->kind == TYPE_ARRAY)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than or equal to (<=) on type array\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform less than or equal to (<=) on type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_GT)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (>): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than (>) on type void\n");
			terror++;
		}
		if (lt->kind == TYPE_BOOLEAN || rt->kind == TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than (>) on type boolean\n");
			terror++;
		}
		if (lt->kind == TYPE_STRING || rt->kind == TYPE_STRING)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than (>) on type string\n");
			terror++;
		}
		if (lt->kind == TYPE_ARRAY || rt->kind == TYPE_ARRAY)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than (>) on type array\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than (>) on type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_GE)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (>=): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than or equal to (>=) on type void\n");
			terror++;
		}
		if (lt->kind == TYPE_BOOLEAN || rt->kind == TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than or equal to (>=) on type boolean\n");
			terror++;
		}
		if (lt->kind == TYPE_STRING || rt->kind == TYPE_STRING)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than or equal to (>=) on type string\n");
			terror++;
		}
		if (lt->kind == TYPE_ARRAY || rt->kind == TYPE_ARRAY)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than or equal to (>=) on type array\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform greater than or equal to (>=) on type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_EQ)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (==): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot check equality (==) for type void\n");
			terror++;
		}
		if (lt->kind == TYPE_ARRAY || rt->kind == TYPE_ARRAY)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot check equality (==) for type array\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot check equality (==) for type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_NE)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (!=): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if (lt->kind == TYPE_VOID || rt->kind == TYPE_VOID)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot check inequality (!=) for type void\n");
			terror++;
		}
		if (lt->kind == TYPE_ARRAY || rt->kind == TYPE_ARRAY)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot check inequality (!=) for type array\n");
			terror++;
		}
		if (lt->kind == TYPE_FUNCTION || rt->kind == TYPE_FUNCTION)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot check inequality (!=) for type function\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_ADD)
	{
		ret = lt;
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (+): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if ((lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT) ||
			 (rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Addition (+) can only be performed on either integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_SUB)
	{
		ret = lt;	
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (-): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if ((lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT) ||
			 (rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Subtracttion (-) can only be performed on either integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_MUL)
	{
		ret = lt;	
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (*): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if ((lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT) ||
			 (rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Multiplication (*) can only be performed on either integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_DIV)
	{
		ret = lt;	
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (/): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if ((lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT) ||
			 (rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Division (/) can only be performed on either integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_MOD)
	{
		ret = type_create(TYPE_INTEGER, 0, 0, 0);
		if (lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform modulus (%%) on non integer types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_EXP)
	{
		ret = lt;
		if (!type_equals(lt, rt))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error (^): type");
			type_print_err(lt);
			fprintf(stderr, " doesn't match type");
			type_print_err(rt);
			fprintf(stderr, "\n");
			terror++;
		}
		if ((lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT) ||
			 (rt->kind != TYPE_INTEGER && rt->kind != TYPE_FLOAT))
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Exponentiation (^) can only be performed on either integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_NEG)
	{
		ret = lt;	
		if (lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Negation (-) can only be performed on integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_PLS)
	{
		ret = lt;	
		if (lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Posation (+) can only be performed on integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_NOT)
	{
		ret = type_create(TYPE_BOOLEAN, 0, 0, 0);
		if (lt->kind != TYPE_BOOLEAN)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform logical NOT (!) on non boolean types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_INC)
	{
		ret = lt;
		if (lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform postfix increment (++) on non integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_DEC)
	{
		ret = lt;
		if (lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT)
		{
			ret = type_create(TYPE_ERROR, 0, 0, 0);
			fprintf(stderr, "type error: Cannot perform postfix decrement (--) on non integer or float types\n");
			terror++;
		}
	}
	else if (e->kind == EXPR_GRP)
	{
		ret = lt;	
	}

	return ret;
}

void expr_codegen(struct expr *e)
{
	if (!e) return;

 	// because I am only implementing global variables, I dont need symbol_codegen
	if (e->kind == EXPR_NAME)
	{
		e->reg = scratch_alloc();
		printf("\tMOVQ\t%s, %s\n", e->name, scratch_name(e->reg));
	}
	else if (e->kind == EXPR_INTEGER_LITERAL)
	{
		e->reg = scratch_alloc();
		printf("\tMOVQ\t$%d, %s\n", e->literal_value, scratch_name(e->reg));
	}
	else if (e->kind == EXPR_CHAR_LITERAL)
	{
		e->reg = scratch_alloc();
		printf("\tMOVQ\t$%d, %s\n", e->literal_value, scratch_name(e->reg));
	}
	else if (e->kind == EXPR_BOOLEAN_LITERAL)
	{
		e->reg = scratch_alloc();
		printf("\tMOVQ\t$%d, %s\n", e->literal_value, scratch_name(e->reg));
	}
	else if (e->kind == EXPR_FLOAT_LITERAL)
	{
		fprintf(stderr, "Floating point not supported\n");
	}
	else if (e->kind == EXPR_STRING_LITERAL)
	{
		fprintf(stderr, "String literals used locally are not implemented\n");
	}
	else if (e->kind == EXPR_FXN)
	{
		fprintf(stderr, "Function calls are not implemented\n");
	}
	else if (e->kind == EXPR_ARR)
	{
		fprintf(stderr, "Array accesses are not implemented\n");
	}
	else if (e->kind == EXPR_ASN)
	{
		expr_codegen(e->right);
	
		printf("\tMOVQ\t%s, %s\n", scratch_name(e->right->reg), e->left->name);
		e->reg = e->right->reg;
	}
	else if (e->kind == EXPR_LGO)
	{
		expr_codegen(e->left);
		expr_codegen(e->right);

		printf("\tORQ\t%s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
		e->reg = e->right->reg;
		scratch_free(e->left->reg);
	}
/*
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
*/
	return;
}
