#include "stmt.h"

struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next)
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

	return s;
}

void stmt_print(struct stmt *s, int indent)
{
	// TODO make a white space string depending on size of indent
	// char *tab = TODO
	
	if (s->kind == STMT_DECL)
	{
		decl_print(s->decl, indent);
	}
	else if (s->kind == STMT_EXPR)
	{
		expr_print(s->expr);
		fprintf(stdout, ";\n");
	}
	// used for an array initialization or a print statement or a list of arguments to a function call
	else if (s->kind == STMT_EXPR_LS)
	{
		expr_print(s->expr);
		if (s->next)
		{
			fprintf(stdout, ", ");
			stmt_print(s->next, indent);
		}
	}
	else if (s->kind == STMT_IF_ELSE)
	{
		fprintf(stdout, "if (");
		expr_print(s->expr);
		fprintf(stdout, ")\n{\n");
		stmt_print(s->body, indent);
		fprintf(stdout, "}\n");
		
		if (s->else_body)
		{
			fprintf(stdout, "else\n{\n");
			stmt_print(s->else_body, indent);
			fprintf(stdout, "}\n");
		}
	}
	else if (s->kind == STMT_FOR)
	{
		fprintf(stdout, "for (");
		expr_print(s->init_expr);
		fprintf(stdout, "; ");
		expr_print(s->expr);
		fprintf(stdout, "; ");
		expr_print(s->next_expr);
		fprintf(stdout, ")\n{\n");
		stmt_print(s->body, indent);
		fprintf(stdout, "}\n");
	}
	else if (s->kind == STMT_PRINT)
	{
		fprintf(stdout, "print ");
		stmt_print(s->body, indent);
		fprintf(stdout, ";\n");
	}
	else if (s->kind == STMT_RETURN)
	{
		fprintf(stdout, "return ");
		expr_print(s->expr);
		fprintf(stdout, ";\n");
	}
	/* TODO never actually used
	else if (s->kind == STMT_BLOCK)
	{
	}*/
}
