%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern char *yytext;
extern int yylex();
extern int yyerror(char *s);

%}

%token	TOKEN_ARRAY
%token	TOKEN_VOID
%token	TOKEN_INTEGER
%token	TOKEN_FLOAT
%token	TOKEN_CHAR
%token	TOKEN_STRING
%token	TOKEN_BOOLEAN
%token	TOKEN_TRUE_LITERAL
%token	TOKEN_FALSE_LITERAL
%token	TOKEN_IF
%token	TOKEN_ELSE
%token	TOKEN_FOR
%token	TOKEN_WHILE
%token	TOKEN_PRINT
%token	TOKEN_FUNCTION
%token	TOKEN_RETURN
%token	TOKEN_AUTO

%token	TOKEN_COMMA
%token	TOKEN_SEMICOLON
%token	TOKEN_COLON
%token	TOKEN_PAREN_OPEN
%token	TOKEN_PAREN_CLOSE
%token	TOKEN_BRACK_OPEN
%token	TOKEN_BRACK_CLOSE
%token	TOKEN_BRACE_OPEN
%token	TOKEN_BRACE_CLOSE
%token	TOKEN_EXPO
%token	TOKEN_MULTI
%token	TOKEN_DIVIDE
%token	TOKEN_MODULUS
%token	TOKEN_LE
%token	TOKEN_LT
%token	TOKEN_GE
%token	TOKEN_GT
%token	TOKEN_EQ
%token	TOKEN_NE
%token	TOKEN_NOT
%token	TOKEN_ASSIGNMENT
%token	TOKEN_LOG_OR
%token	TOKEN_LOG_AND
%token	TOKEN_POST_INC
%token	TOKEN_POST_DEC
%token	TOKEN_PLUS
%token	TOKEN_MINUS

%token	TOKEN_ID
%token	TOKEN_FLOAT_LITERAL
%token	TOKEN_INTEGER_LITERAL
%token	TOKEN_CHAR_LITERAL
%token	TOKEN_STRING_LITERAL

%%

//grammar rules

prog  : 	TOKEN_ID TOKEN_COLON decl TOKEN_SEMICOLON												{ printf("prog\n"); }
		|	TOKEN_COMMA																									{ printf("test\n"); }
		;

decl	:	type																								{ printf("decl\n"); }
		;

type	: 	inte																								{ printf("inte\n"); }
		|	floa																								{ printf("floa\n"); }
		|	char																								{ printf("char\n"); }
		|	stri																								{ printf("stri\n"); }
		|	bool																								{ printf("bool\n"); }
		|																										{ printf("no inst\n"); }
		;

inte	:	TOKEN_INTEGER TOKEN_ASSIGNMENT TOKEN_INTEGER_LITERAL								{ printf("int literal\n"); }
		|	TOKEN_INTEGER																					{ printf("int\n"); }
		;

floa	: 	TOKEN_FLOAT TOKEN_ASSIGNMENT TOKEN_FLOAT_LITERAL									{ printf("float literal\n"); }
		|	TOKEN_FLOAT																						{ printf("float\n"); }
		;

char	:	TOKEN_CHAR TOKEN_ASSIGNMENT TOKEN_CHAR_LITERAL										{ printf("char literal\n"); }
		|	TOKEN_CHAR																						{ printf("char;\n"); }
		;

stri	:	TOKEN_STRING TOKEN_ASSIGNMENT TOKEN_STRING_LITERAL									{ printf("str literal\n"); }
		|	TOKEN_STRING																					{ printf("str\n"); }
		;

bool	:	TOKEN_BOOLEAN TOKEN_ASSIGNMENT torf														{ printf("bool choice\n"); }
		| 	TOKEN_BOOLEAN																					{ printf("boolean\n"); }
		;

torf	:	TOKEN_TRUE_LITERAL																			{ printf("true\n"); }
		|	TOKEN_FALSE_LITERAL																			{ printf("false\n"); }
		;

%%

int yyerror(char *s)
{
	printf("parse error: %s\n", s);
	return 1;
}
