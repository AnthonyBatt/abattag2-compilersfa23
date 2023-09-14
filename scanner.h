int scan(FILE *fp);

enum tokens
{
	TOKEN_ARRAY,
	TOKEN_VOID,
	TOKEN_INTEGER,
	TOKEN_FLOAT,
	TOKEN_CHAR,
	TOKEN_STRING,
	TOKEN_BOOLEAN,
	TOKEN_TRUE_LITERAL,
	TOKEN_FALSE_LITERAL,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_FOR,
	TOKEN_WHILE,
	TOKEN_PRINT,
	TOKEN_FUNCTION,
	TOKEN_RETURN,
	TOKEN_AUTO,

	TOKEN_SEMICOLON,
	TOKEN_COLON,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_BRACK_OPEN,
	TOKEN_BRACK_CLOSE,
	TOKEN_BRACE_OPEN,
	TOKEN_BRACE_CLOSE,
	TOKEN_EXPO,
	TOKEN_MULTI,
	TOKEN_DIVIDE,
	TOKEN_MODULUS,
	TOKEN_LE,
	TOKEN_LT,
	TOKEN_GE,
	TOKEN_GT,
	TOKEN_EQ,
	TOKEN_NE,
	TOKEN_NOT,
	TOKEN_ASSIGNMENT,
	TOKEN_LOG_OR,
	TOKEN_LOG_AND,
	TOKEN_POST_INC,
	TOKEN_POST_DEC,
	TOKEN_PLUS,
	TOKEN_MINUS,

	TOKEN_ID,
	TOKEN_FLOAT_LITERAL,
	TOKEN_INTEGER_LITERAL,
	TOKEN_CHAR_LITERAL,
	TOKEN_STRING_LITERAL
};
