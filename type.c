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
	else if(t->kind == TYPE_STRING)
	{
		fprintf(stdout, " error");
	}
}

void type_print_err(struct type *t)
{
	if (t->kind == TYPE_VOID)
	{
		fprintf(stderr, " void");
	}
	else if(t->kind == TYPE_BOOLEAN)
	{
		fprintf(stderr, " boolean");
	}
	else if(t->kind == TYPE_CHARACTER)
	{
		fprintf(stderr, " char");
	}
	else if(t->kind == TYPE_FLOAT)
	{
		fprintf(stderr, " float");
	}
	else if(t->kind == TYPE_INTEGER)
	{
		fprintf(stderr, " integer");
	}
	else if(t->kind == TYPE_STRING)
	{
		fprintf(stderr, " string");
	}
	else if(t->kind == TYPE_ARRAY)
	{
		fprintf(stderr, " array ");
		fprintf(stderr, "[");
		//expr_print(t->length);         NOT NEEDED FOR ERROR MESSAGE
		fprintf(stderr, "]");
		type_print_err(t->subtype);
	}
	else if(t->kind == TYPE_FUNCTION)
	{
		fprintf(stderr, " function");
		type_print_err(t->subtype);
		fprintf(stderr, " (");
		param_list_print_err(t->params);
		fprintf(stderr, ")");
	}
	else if(t->kind == TYPE_STRING)
	{
		fprintf(stderr, " error");
	}
}

// TODO maybe come up with a number mapping to refer to for which type error occurred
int type_equals(struct type *a, struct type *b)
{
	if (a->kind == TYPE_ERROR || b->kind == TYPE_ERROR) return 0;
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
				//printf("%d %d\n", !als, !bls);
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
