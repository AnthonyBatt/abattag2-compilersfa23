struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right)
{
	struct expr e* = malloc(sizeof(struct expr));

	e->kind = kind;
	e->left = left;
	e->right = right;

/*					TODO: ask matt or evan or someone if we need to track values as we go
	if (...)
	{
	
	}
	else if (...)
	{

	}
	EXPR_NAME,
	EXPR_INTEGER_LITERAL,
	EXPR_FLOAT_LITERAL,
	EXPR_BOOLEAN_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_ASN,
	EXPR_LGO,
	EXPR_LGA,
	EXPR_LT,
	EXPR_LE,
	EXPR_GT,
	EXPR_GE,
	EXPR_EQ,
	EXPR_NE,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_EXP,
	EXPR_NEG,
	EXPR_PLS,
	EXPR_NOT,
	EXPR_PIC,
	EXPR_PDC,
	EXPR_GRP,
	EXPR_FXN,
	EXPR_ARR
*/

	return e;
}

struct expr *expr_create_name(const char *n)
{
	struct expr *name = expr_create(EXPR_NAME, 0, 0);
	name->name = strdup(n);
	
	return name;
}

struct expr *expr_create_integer_literal(int c)
{
	struct expr *integer = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
	integer->literal_value = c;

	return integer;
}

struct expr *expr_create_float_literal(float f)
{
	struct expr *float_ = expr_create(EXPR_FLOAT_LITERAL, 0, 0);
	float_->float_literal = f;

	return float_;
}

struct expr *expr_create_boolean_literal(int c)
{
	struct expr *boolean = expr_create(EXPR_BOOLEAN_LITERAL, 0, 0);
	boolean->literal_value = c;

	return boolean;
}

struct expr *expr_create_char_literal(char c)
{
	struct expr *character = expr_create(EXPR_CHAR_LITERAL, 0, 0);
	character->literal_value = c;

	return character;
}

struct expr *expr_create_string_literal(const char *str)
{
	struct expr* string = expr_create(EXPR_STRING_LITERAL, 0, 0);
	string->string_literal = strdup(str);

	return string;
}
