#include "decl.h"

extern int rerror;
extern int terror;

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
		// both are functions
		else
		{
			if (!type_equals(check->type, d->type))
			{
				fprintf(stderr, "type error: functions and their prototypes must have matching return types and parameters\n");
				fprintf(stderr, "Prototype,");
				type_print_err(check->type);
				fprintf(stderr, ", doesn't match function,");
				type_print_err(d->type);
				fprintf(stderr, "\n");
				terror++;
			};
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
		// if it is an array, because I was dumb and implemented array initializations as stmt blocks
		if (d->code->kind == STMT_EXPR_LS && !d->type->params)
		{
			stmt_resolve(d->code);
		}
		else
		{
			scope_enter();
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

// check global declaration restrictions and stuff here
void decl_typecheck(struct decl *d)
{
	struct type *t = 0;
	struct type *arr_t = 0;

	if (d->value)
	{
		t = expr_typecheck(d->value);
		if (!type_equals(t, d->symbol->type))
		{
			fprintf(stderr, "type error (%s): Declarations must match their initialization\n", d->name);
			fprintf(stderr, "Initializor type");
			type_print_err(t);
			fprintf(stderr, " doesn't match declaration type");
			type_print_err(d->symbol->type);
			fprintf(stderr, "\n");
			terror++;
		}
		if (d->symbol->kind == SYMBOL_GLOBAL)
		{
			// if it is not a constant value
			if (d->value->kind < EXPR_INTEGER_LITERAL || d->value->kind > EXPR_STRING_LITERAL)
			{
				fprintf(stderr, "type error (%s): globals must be initialized with a constant\n", d->name);
				terror++;
			}
		}
	}

	// TODO move this to resolve so that I can lookup variables and check their types
	if (d->type->kind == TYPE_ARRAY)
	{
		//printf("checking array %s\n", d->name);
		if (d->symbol->kind == SYMBOL_LOCAL && d->code)
		{
			fprintf(stderr, "type error (%s): local arrays cannot be initialized\n", d->name);
			terror++;
		}
		if (d->symbol->kind == SYMBOL_GLOBAL && d->type->length->kind != EXPR_INTEGER_LITERAL)
		{
			fprintf(stderr, "type error (%s): global arrays must have constant integer size\n", d->name);
			terror++;
		}
		struct type *arr_len_t = expr_typecheck(d->type->length);
		if (arr_len_t && arr_len_t->kind != TYPE_INTEGER)
		{
			fprintf(stderr, "type error (%s): all arrays must have integer size\n", d->name);
			terror++;
		}
		struct type *curr = d->type;
		while (curr->subtype)
		{
			curr = curr->subtype;
		}
		arr_t = curr;
		if (curr->kind == TYPE_FUNCTION || curr->kind == TYPE_VOID)
		{
			fprintf(stderr, "type error (%s): arrays cannot hold functions or void\n", d->name);
			terror++;
		}
	}

	if (d->code)
	{
		if (d->type->kind == TYPE_FUNCTION)
		{
			t = d->type->subtype;
			if (t->kind == TYPE_ARRAY || t->kind == TYPE_FUNCTION)
			{
				fprintf(stderr, "type error (%s): Functions that return arrays or other functions are not supported\n", d->name);
				terror++;
			}
		}
		stmt_typecheck(d->code, t, arr_t);
	}

	if (d->next)
	{
		decl_typecheck(d->next);
	}
}

void decl_codegen(struct decl *d, FILE *fp)
{
	if (d->symbol->kind != SYMBOL_GLOBAL)
	{
		fprintf(stderr, "Non global declarations are not implemented\n");
	}

	if (d->value)
	{
		if (d->value->kind == EXPR_INTEGER_LITERAL || 
			 d->value->kind == EXPR_BOOLEAN_LITERAL || 
			 d->value->kind == EXPR_CHAR_LITERAL
			)
		{
			fprintf(fp, "%s:\t.quad %d\n", d->name, d->value->literal_value); 
		}
		else if (d->value->kind == EXPR_STRING_LITERAL)
		{
			fprintf(fp, "%s:\t.string %s\n", d->name, d->value->string_literal); 
		}
	}

	if (d->code)
	{
		// only will accept the creation of the main function
		if (strcmp(d->name, "main") || d->type->kind != TYPE_FUNCTION)
		{
			fprintf(stderr, "Only the main function is supported at this time (also no arrays)\n");
			return;
		}
		// no local variables or parameters will be allowed
		fprintf(fp, ".text\n.global main\n");
		fprintf(fp, "%s:\n", d->name);

		stmt_codegen(d->code, fp);
	}
	
	if (d->next)
	{
		decl_codegen(d->next, fp);
	}
}
