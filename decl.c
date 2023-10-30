#include "decl.h"

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
	fprintf(stdout, "%s:", d->name);
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
		else if 	(d->type->kind == TYPE_FUNCTION) fprintf(stdout, " =\n{\n");
		stmt_print(d->code, indent);
		if 		(d->type->kind == TYPE_ARRAY)		fprintf(stdout, "};\n");
		else if 	(d->type->kind == TYPE_FUNCTION) fprintf(stdout, "}\n");
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
