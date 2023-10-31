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
