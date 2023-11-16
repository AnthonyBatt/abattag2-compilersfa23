#include "type.h"

struct type *type_create(type_t kind, struct type *subtype, struct param_list *params, struct expr* length)
{
	struct type *t = malloc(sizeof(struct type));

	t->kind = kind;
	t->subtype = subtype;
	t->params = params;
	t->length = length;
	
	return t;
}

void type_print(struct type *t)
{
	if (t->kind == TYPE_VOID)
	{
		fprintf(stdout, " void");
	}
	else if(t->kind == TYPE_BOOLEAN)
	{
		fprintf(stdout, " boolean");
	}
	else if(t->kind == TYPE_CHARACTER)
	{
		fprintf(stdout, " char");
	}
	else if(t->kind == TYPE_FLOAT)
	{
		fprintf(stdout, " float");
	}
	else if(t->kind == TYPE_INTEGER)
	{
		fprintf(stdout, " integer");
	}
	else if(t->kind == TYPE_STRING)
	{
		fprintf(stdout, " string");
	}
	else if(t->kind == TYPE_ARRAY)
	{
		fprintf(stdout, " array ");
		fprintf(stdout, "[");
		expr_print(t->length);
		fprintf(stdout, "]");
		type_print(t->subtype);
	}
	else if(t->kind == TYPE_FUNCTION)
	{
		fprintf(stdout, " function");
		type_print(t->subtype);
		fprintf(stdout, " (");
		param_list_print(t->params);
		fprintf(stdout, ")");
	}
}

// TODO maybe come up with a number mapping to refer to for which type error occurred
int type_equals(struct type *a, struct type *b)
{
	if (a->kind == b->kind)
	{
		if (a->kind == TYPE_ARRAY)
		{
			// then check if the subtypes match
			// TODO if the subtypes are void should error
			return type_equals(a->subtype, b->subtype);
		}
		else if (a->kind == TYPE_FUNCTION)
		{
			// check the return types are equal, cannot be recursive no fx rets a fx
			if (type_equals(a->subtype, b->subtype))
			{
				// check the parameter types are all equal
				struct param_list *als = a->params;
				struct param_list *bls = b->params;
				printf("%d %d\n", !als, !bls);
				while ((als && bls) && (als->type || bls->type))
				{
			//	printf("got here :(\n");
					// param lists of diff sizes
					if (!als->type || !bls->type)					return 0;
					// params of diff types
					if (!type_equals(als->type, bls->type))	return 0;

					als = als->next;
					bls = bls->next;
				}

				return 1;
			}
			// return types do not match
			else 
			{
				return 0;
			}
		}
		// atomic types
		else
		{
			return 1;
		}
	}
	// types dont match at all
	else
	{
		// TODO figure out if error message should be printed here or in the function that calls this
		return 0;
	}
}

/*
// ==================================== START ARRAY LENGTH CHECKING
			int len_flag = 0;
			// TODO most of this functionality should maybe be somewhere else
			// make sure length and kind have been assigned so we dont segfault
			if (a->length && b->length && a->length->kind && b->length->kind)
			{
				// check the size of the array if its length expression is of kind integer literal
				if (a->length->kind == EXPR_INTEGER_LITERAL && b->length->kind == EXPR_INTEGER_LITERAL)
				{
					len_flag = a->length->literal_value == b->length->literal_value;
				}
				// make sure symbol and type have been assigned so we dont segfault
				if (a->length->symbol && b->length->symbol && a->length->symbol->type && b->length->symbol->type)
				{
					// check that the variable is an int
					if (a->length->kind == EXPR_INTEGER_LITERAL && b->length->kind == EXPR_NAME)
					{
						return b->length->symbol->type == TYPE_INTEGER;
					}
					// check that the variable is an int
					else if (a->length->kind == EXPR_NAME && b->length->kind == EXPR_INTEGER_LITERAL)
					{
						return a->length->symbol->type == TYPE_INTEGER;
					}
					// check that both variables are an int
					else if (a->length->kind == EXPR_NAME && b->length->kind == EXPR_NAME)
					{
						return (a->length->symbol->type == TYPE_INTEGER) && (b->length->symbol->type == TYPE_INTEGER);
					}
					// one of the two arrays was declared without an integer as its size
					else
					{
						// TODO error message for declaring an array with a non int size
					}
				}
				// TODO delete this later
				else
				{
					printf("array type doesn't have a symbol or its symbol doesnt have a type\n");
				}
			// TODO delete this later
			else
			{
				printf("array type doesn't have a length or its length doesnt have a kind\n");
			}
// ==================================== END ARRAY LENGTH CHECKING
*/
