#include "decl.h"

extern int rerror;

struct decl *decl_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next)
{
	struct decl *d = malloc(sizeof(struct decl));

	d->name = name;
	d->type = type;
	d->value = value;
	d->code = code;
	d->next = next;

	return d;
}

void decl_print(struct decl *d, int indent)
{
	// set the indent
	char tab[BUFSIZ];
	int i;
	for (i = 0; i < indent; i ++)		tab[i] = 32;
	tab[i] = 0;

	fprintf(stdout, "%s%s:", tab, d->name);
	type_print(d->type);
	// standard types
	if (d->value)
	{
		fprintf(stdout, " = ");
		expr_print(d->value);
		fprintf(stdout, ";\n");
	}
	// functions and arrays
	else if(d->code)
	{
		if 		(d->type->kind == TYPE_ARRAY)		fprintf(stdout, " = {");
		else if 	(d->type->kind == TYPE_FUNCTION) fprintf(stdout, " =\n%s{\n", tab);
		stmt_print(d->code, indent + 3);
		if 		(d->type->kind == TYPE_ARRAY)		fprintf(stdout, "};\n");
		else if 	(d->type->kind == TYPE_FUNCTION) fprintf(stdout, "%s}\n", tab);
	}
	// no intialization
	else
	{
		fprintf(stdout, ";\n");
	}

	// if the program continues
	if (d->next)
	{
		decl_print(d->next, indent);
	}
}

void decl_resolve(struct decl *d)
{
	symbol_t kind = scope_level() > 0 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	//printf("scope level: %d\n", scope_level());

	d->symbol = symbol_create(kind, d->type, d->name);
	struct symbol *check;
	if ((check = scope_lookup_curr(d->name)))
	{
		if (check->type->kind != TYPE_FUNCTION || d->type->kind != TYPE_FUNCTION)
		{
			fprintf(stderr, "resolve error (declaration): %s was already declared in this scope\n", d->name);
			rerror++;
			return;
		}
	}

	if (d->value)
	{
		expr_resolve(d->value);
	}

	scope_bind(d->name, d->symbol);
	symbol_print(d->symbol);

	if (d->code)
	{
		//printf("I have code\n");
		// if it is an array, because I was dumb and implemented array initializations as stmt blocks
		if (d->code->kind == STMT_EXPR_LS && !d->type->params)
		{
			stmt_resolve(d->code);
		}
		else
		{
			scope_enter();
			//printf("scope level: %d\n", scope_level());
			param_list_resolve(d->type->params);
			scope_enter();
			stmt_resolve(d->code);
			scope_exit();
			scope_exit();
		}
	}
	
	if (d->next)
	{
		decl_resolve(d->next);
	}
}
