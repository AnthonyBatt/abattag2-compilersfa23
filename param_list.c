#include "param_list.h"

extern int rerror;

struct param_list *param_list_create(char *name, struct type *type, struct param_list *next)
{
	struct param_list *p = malloc(sizeof(struct param_list));

	p->name = name;
	p->type = type;
	p->next = next;

	return p;
}

void param_list_print(struct param_list *a)
{
	if (!a) return;
	fprintf(stdout, "%s:", a->name);
	type_print(a->type);
	
	if (a->next)
	{
		fprintf(stdout, ", ");
		param_list_print(a->next);
	}
}

void param_list_print_err(struct param_list *a)
{
	if (!a) return;
	fprintf(stderr, "%s:", a->name);
	type_print_err(a->type);
	
	if (a->next)
	{
		fprintf(stderr, ", ");
		param_list_print_err(a->next);
	}
}

void param_list_resolve(struct param_list *a)
{
	if (!a) return;

	a->symbol = symbol_create(SYMBOL_PARAM, a->type, a->name);
	if (scope_lookup_curr(a->name))
	{
		fprintf(stderr, "resolve error (parameter list): %s was already used as a parameter\n", a->name);
		rerror++;
	}
	else
	{
		scope_bind(a->name, a->symbol);
		symbol_print(a->symbol);
	}

	if (a->next)
	{
		param_list_resolve(a->next);
	}
}
