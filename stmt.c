#include "stmt.h"

extern int terror;

struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next, struct stmt *next_elem, int braces)
{
	struct stmt *s = malloc(sizeof(struct stmt));

	s->kind = kind;
	s->decl = decl;
	s->init_expr = init_expr;
	s->expr = expr;
	s->next_expr = next_expr;
	s->body = body;
	s->else_body = else_body;
	s->next = next;
	s->next_elem = next_elem;
	s->braces = braces;

	return s;
}

void stmt_print(struct stmt *s, int indent)
{
	if (!s) return;
	// set the indent
	char tab[BUFSIZ];
	int i;
	for (i = 0; i < indent; i ++)		tab[i] = 32;
	tab[i] = 0;
	
	//printf("%d", s->expr->kind);

	if (s->kind == STMT_DECL)
	{
		decl_print(s->decl, indent);
	}
	else if (s->kind == STMT_EXPR)
	{
		fprintf(stdout, "%s", tab);
		expr_print(s->expr);
		fprintf(stdout, ";\n");
	}
	// used for an array initialization or a print statement or a list of arguments to a function call
	else if (s->kind == STMT_EXPR_LS)
	{
	//	printf("\nstart: %d\n", s->expr->literal_value);
		if (s->braces)
			printf("{");
		expr_print(s->expr);
		if (s->next_elem)
		{
			//printf("\nstart: %d\n", s->next->kind);
			fprintf(stdout, ",");
			stmt_print(s->next_elem, indent);
			if (s->braces == 1)
				printf("}");
			else if (s->braces == 2)
				printf("},");
		//	fprintf(stdout, ",");
		}
		/*
		if (s->braces && s->next_elem && s->next_elem->braces)
			printf("},");
		else if (s->braces && s->next_elem)
			printf("}");
		else if (s->braces)
			printf("},");
		*/
	//	printf("\nend: %d\n", s->expr->literal_value);
		//expr_print(s->expr);
	}
	else if (s->kind == STMT_IF_ELSE)
	{
		fprintf(stdout, "%sif(", tab);
		expr_print(s->expr);
		fprintf(stdout, ")\n%s{\n", tab);
		stmt_print(s->body, indent + 3);
		fprintf(stdout, "%s}\n", tab);
		
		if (s->else_body)
		{
			fprintf(stdout, "%selse\n%s{\n", tab, tab);
			stmt_print(s->else_body, indent + 3);
			fprintf(stdout, "%s}\n", tab);
		}
	}
	else if (s->kind == STMT_FOR)
	{
		fprintf(stdout, "%sfor(", tab);
		expr_print(s->init_expr);
		fprintf(stdout, ";");
		expr_print(s->expr);
		fprintf(stdout, ";");
		expr_print(s->next_expr);
		fprintf(stdout, ")\n%s{\n", tab);
		stmt_print(s->body, indent + 3);
		fprintf(stdout, "%s}\n", tab);
	}
	else if (s->kind == STMT_PRINT)
	{
		fprintf(stdout, "%sprint ", tab);
		stmt_print(s->body, indent);
		fprintf(stdout, ";\n");
	}
	else if (s->kind == STMT_RETURN)
	{
		fprintf(stdout, "%sreturn ", tab);
		expr_print(s->expr);
		fprintf(stdout, ";\n");
	}
	/* TODO never actually used
	else if (s->kind == STMT_BLOCK)
	{
	}*/

	if (s->next)
	{
		stmt_print(s->next, indent);
	}
}

void stmt_resolve(struct stmt *s)
{
	if (!s) return;

	if (s->kind == STMT_DECL)
	{
		decl_resolve(s->decl);
	}
	else if (s->kind == STMT_EXPR)
	{
		expr_resolve(s->expr);
	}
	else if (s->kind == STMT_EXPR_LS)
	{
		expr_resolve(s->expr);
		if (s->next_elem)
		{
			stmt_resolve(s->next_elem);
		}
	}
	else if (s->kind == STMT_IF_ELSE)
	{
		expr_resolve(s->expr);

		scope_enter();
		stmt_resolve(s->body);
		scope_exit();
		
		if (s->else_body)
		{
			scope_enter();
			stmt_resolve(s->else_body);
			scope_exit();
		}
	}
	else if (s->kind == STMT_FOR)
	{
		expr_resolve(s->init_expr);
		expr_resolve(s->expr);
		expr_resolve(s->next_expr);

		scope_enter();
		stmt_resolve(s->body);
		scope_exit();
	}
	else if (s->kind == STMT_PRINT)
	{
		stmt_resolve(s->body);
	}
	else if (s->kind == STMT_RETURN)
	{
		expr_resolve(s->expr);
	}

	if (s->next)
	{
		stmt_resolve(s->next);
	}
}

void stmt_typecheck(struct stmt *s, struct type *ret_t, struct type *arr_t)
{
	if (!s) return;

	struct type *t;

	if (s->kind == STMT_DECL)
	{
		decl_typecheck(s->decl);
	}
	else if (s->kind == STMT_EXPR)
	{
		t = expr_typecheck(s->expr);
	}
	// TODO GOTTA DO FUNCTION AND ARRAY SHIT HERE
	else if (s->kind == STMT_EXPR_LS)
	{
		t = expr_typecheck(s->expr);
		if (arr_t && !type_equals(t, arr_t))
		{
			fprintf(stderr, "type error: array initializations must match their array's type\n");
			terror++;
		}
		if (s->next_elem)
		{
			stmt_typecheck(s->next_elem, ret_t, arr_t);
		}
	}
	else if (s->kind == STMT_IF_ELSE)
	{
		t = expr_typecheck(s->expr);
		if (t->kind != TYPE_BOOLEAN)
		{
			fprintf(stderr, "type error: cannot evaluate if for non boolean types\n");
			terror++;
		}

		stmt_typecheck(s->body, ret_t, arr_t);
		
		if (s->else_body)
		{
			stmt_typecheck(s->else_body, ret_t, arr_t);
		}
	}
	else if (s->kind == STMT_FOR)
	{
		t = expr_typecheck(s->init_expr);
		if (t->kind != TYPE_INTEGER)
		{
			fprintf(stderr, "type error: cannot evaluate init expression in for loops for non integer types\n");
			terror++;
		}
		t = expr_typecheck(s->expr);
		if (t->kind != TYPE_BOOLEAN)
		{
			fprintf(stderr, "type error: cannot evaluate stopping condition in for loops for non boolean types\n");
			terror++;
		}
		t = expr_typecheck(s->next_expr);
		if (t->kind != TYPE_BOOLEAN)
		{
			fprintf(stderr, "type error: cannot evaluate next expression in for loops for non integer types\n");
			terror++;
		}

		stmt_typecheck(s->body, ret_t, arr_t);
	}
	// TODO what do here?
	else if (s->kind == STMT_PRINT)
	{
		stmt_typecheck(s->body, ret_t, arr_t);
	}
	else if (s->kind == STMT_RETURN)
	{
		t = expr_typecheck(s->expr);
		if (!type_equals(t, ret_t))
		{
			fprintf(stderr, "type error: the type of the value returned,");
			type_print_err(t);
			fprintf(stderr, ", doesn't equal the function's return type,");
			type_print_err(ret_t);
			fprintf(stderr, "\n");
			terror++;
		}
	}

	if (s->next)
	{
		stmt_typecheck(s->next, ret_t, arr_t);
	}
}

void stmt_codegen(struct stmt *s)
{
	if (!s) return;

	if (s->kind == STMT_DECL)
	{
		//decl_codegen(s->decl);
		//since only global variables exist, there is no need for this
		return;
	}
	else if (s->kind == STMT_EXPR)
	{
		expr_codegen(s->expr);
		scratch_free(s->expr->reg);
	}
	else if (s->kind == STMT_EXPR_LS)
	{
		expr_codegen(s->expr);
		scratch_free(s->expr->reg);
		if (s->next_elem)
		{
			stmt_codegen(s->next_elem);
		}
	}
	else if (s->kind == STMT_IF_ELSE)
	{
		return;
	}
	else if (s->kind == STMT_FOR)
	{
		return;
	}
	else if (s->kind == STMT_PRINT)
	{
		return;
	}
	else if (s->kind == STMT_RETURN)
	{
		expr_codegen(s->expr);
		scratch_free(s->expr->reg);

		printf("\tMOVQ\t%s, %%rax\n", scratch_name(s->expr->reg)); 
		printf("\tRET\n");
	}

	if (s->next)
	{
		stmt_codegen(s->next);
	}
}
