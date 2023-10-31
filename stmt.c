#include "stmt.h"

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
