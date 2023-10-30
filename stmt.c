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
	if (s->kind == STMT_DECL)
	{
		decl_print(s->decl, indent);
	}
	// TODO may need to add a semicolon
	// TODO may need to diff between an expr and an expr list
	else if (s->kind == STMT_EXPR)
	{
		expr_print(s->expr);
		if (s->next)
		{
			fprintf(", ");
			stmt_print(s->next, indent);
		}
	}
	else if (s->kind == STMT_IF_ELSE)
	{
	}
	else if (s->kind == STMT_FOR)
	{
	}
	else if (s->kind == STMT_PRINT)
	{
	}
	else if (s->kind == STMT_RETURN)
	{
		
	}
	/* TODO never actually used
	else if (s->kind == STMT_BLOCK)
	{
	}*/
}
