%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "param_list.h"

extern char *yytext;
extern int yylex();
extern int yyerror(char *s);

struct decl *prog;

%}

%union {
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct type *type;
	struct param_list *pals;
};

%type <decl> begi dcls decl 
%type <stmt> arii fxii smst clst opst stmt stls ostl bstl xpls oxpl bxpl
%type <expr> init brcn oasi vari atom post ngtn expo term expr comp lgan lgor asin
%type <type> type fxty fart arty paty
%type <pals> opal pals parm

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

%token	TOKEN_EOF
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

// begin the program
begi	:	dcls TOKEN_EOF												{ prog = $1; return 0; }
		;

// assignment
asin	:	lgor TOKEN_ASSIGNMENT asin								{ $$ = expr_create(EXPR_ASN, $1, $3); }
		|	lgor															{ $$ = $1; }
		;

// logical or
lgor	:	lgor TOKEN_LOG_OR	lgan									{ $$ = expr_create(EXPR_LGO, $1, $3); }
		|	lgan															{ $$ = $1; }
		;

// logical and
lgan	:	lgan TOKEN_LOG_AND comp									{ $$ = expr_create(EXPR_LGA, $1, $3); }
		|	comp															{ $$ = $1; }
		;

// comparisons
comp	:	comp TOKEN_LT expr										{ $$ = expr_create(EXPR_LT, $1, $3); }
		|	comp TOKEN_LE expr										{ $$ = expr_create(EXPR_LE, $1, $3); }
		|	comp TOKEN_GT expr										{ $$ = expr_create(EXPR_GT, $1, $3); }
		|	comp TOKEN_GE expr										{ $$ = expr_create(EXPR_GE, $1, $3); }
		|	comp TOKEN_EQ expr										{ $$ = expr_create(EXPR_EQ, $1, $3); }
		|	comp TOKEN_NE expr										{ $$ = expr_create(EXPR_NE, $1, $3); }
		|	expr															{ $$ = $1; }
		;

// addition and subtraction
expr	:	expr TOKEN_PLUS term										{ $$ = expr_create(EXPR_ADD, $1, $3); }
		|	expr TOKEN_MINUS term									{ $$ = expr_create(EXPR_SUB, $1, $3); }
		|	term															{ $$ = $1; }
		;

// multiplication, division, modulus
term	:	term TOKEN_MULTI expo									{ $$ = expr_create(EXPR_MUL, $1, $3); }
		|	term TOKEN_DIVIDE expo									{ $$ = expr_create(EXPR_DIV, $1, $3); } 
		|	term TOKEN_MODULUS expo									{ $$ = expr_create(EXPR_MOD, $1, $3); }
		|	expo															{ $$ = $1; }
		;

// exponentiation
expo	:	expo TOKEN_EXPO ngtn										{ $$ = expr_create(EXPR_EXP, $1, $3); }
		|	ngtn															{ $$ = $1; }
		;

// negation
ngtn	:	TOKEN_MINUS post											{ $$ = expr_create(EXPR_NEG, $2, 0); }
		|	TOKEN_PLUS post											{ $$ = expr_create(EXPR_PLS, $2, 0); }
		|	TOKEN_NOT post												{ $$ = expr_create(EXPR_NOT, $2, 0); }
		|	post															{ $$ =  $1; }
		;

// post increment/decrement
post	:	atom TOKEN_POST_INC										{ $$ = expr_create(EXPR_INC, $1, 0); }
		|	atom TOKEN_POST_DEC										{ $$ = expr_create(EXPR_DEC, $1, 0); }
		|	atom															{ $$ = $1; }
		;

// atomic thingies													// #TODO check these work and what do bout fx/arr
atom	:	TOKEN_FLOAT_LITERAL										{ $$ = expr_create_float_literal(atof(yytext)); }
		|	TOKEN_INTEGER_LITERAL									{ $$ = expr_create_integer_literal(atoi(yytext)); }
		|	TOKEN_CHAR_LITERAL										{ $$ = expr_create_char_literal(yytext[1]); }
		|	TOKEN_STRING_LITERAL										{ $$ = expr_create_string_literal(yytext); }
		|	TOKEN_TRUE_LITERAL										{ $$ = expr_create_boolean_literal(1); }
		|	TOKEN_FALSE_LITERAL										{ $$ = expr_create_boolean_literal(0); }
		|	TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE			{ $$ = expr_create(EXPR_GRP, $2, 0); }
		|	vari															{ $$ = $1; }
		|	vari TOKEN_PAREN_OPEN oxpl TOKEN_PAREN_CLOSE				{ $$ = expr_create_function_call($1->name, $3); }
		|	vari TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE brcn		{ $$ = expr_create_array_access($1->name, $3, $5); }
		;

// variables
vari	:	TOKEN_ID														{ $$ = expr_create_name(yytext); }

// optional parameter list
opal	:	pals															{ $$ = $1; }
		|	/* epsilon */												{ $$ =  0; }
		;

// parameter list
pals	:	parm															{ $$ = $1; }			
		|	parm TOKEN_COMMA pals									{ $1->next = $3; $$ = $1; }			
		;

// parameter															// # TODO how to isolate the ID
parm	:	vari TOKEN_COLON paty									{ $$ = param_list_create((char *)$1->name, $3, 0); }
		;

// parameter types
paty	:	type																	{ $$ = $1; }	
		|	TOKEN_ARRAY TOKEN_BRACK_OPEN TOKEN_BRACK_CLOSE paty	{ $$ = type_create(TYPE_ARRAY, $4, 0, 0); }
		;

// braced expression list
bxpl	:	TOKEN_BRACE_OPEN oxpl TOKEN_BRACE_CLOSE			{ $$ = $2; }
		;

// optional expression list
oxpl	:	xpls															{ $$ = $1; }
		|	/* epsilon */												{ $$ =  0; }
		;

// expression list													
xpls	:	asin															{ $$ = stmt_create(STMT_EXPR_LS, 0, 0, $1, 0, 0, 0, 0); }
		|	asin TOKEN_COMMA xpls									{ $$ = stmt_create(STMT_EXPR_LS, 0, 0, $1, 0, 0, 0, $3); }
		|	bxpl															{ $$ = $1; }
		|	bxpl TOKEN_COMMA xpls									{ $1->next = $3; $$ = $1; }
		;

// braced statement list
bstl	:	TOKEN_BRACE_OPEN ostl TOKEN_BRACE_CLOSE			{ $$ = $2; }
		;

// optional statement list
ostl	:	stls															{ $$ = $1; }
		|	/* epsilon */												{ $$ =  0; }
		;

// statement list
stls	:	stmt															{ $$ = $1; }					
		|	stmt stls													{ $1->next = $2; $$ = $1; }					
		;

// statement
stmt	:	opst															{ $$ = $1; }
		|	clst															{ $$ = $1; }
		;

// open statement
opst	:	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst	 					  { $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5,  0, 0); }
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE opst  					  { $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5,  0, 0); }
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst TOKEN_ELSE opst  { $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
		|	TOKEN_FOR TOKEN_PAREN_OPEN oasi TOKEN_SEMICOLON oasi TOKEN_SEMICOLON oasi TOKEN_PAREN_CLOSE opst 	{ $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); }
		;

// closed statement
clst	:	smst 																																{ $$ = $1; }
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst TOKEN_ELSE clst  										{ $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
		|	TOKEN_FOR TOKEN_PAREN_OPEN oasi TOKEN_SEMICOLON oasi TOKEN_SEMICOLON oasi TOKEN_PAREN_CLOSE clst 	{ $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0); } 
		;

// simple statement
smst	:	bstl															{ $$ = $1; }
		|	decl 															{ $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
		|	TOKEN_RETURN oasi TOKEN_SEMICOLON 					{ $$ = stmt_create(STMT_RETURN, 0, 0, $2, 0, 0, 0, 0); }
		|	TOKEN_PRINT oxpl TOKEN_SEMICOLON 					{ $$ = stmt_create(STMT_PRINT, 0, 0, 0, 0, $2, 0, 0);	}	
		|	asin TOKEN_SEMICOLON										{ $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0, 0); } 
		;

// optional assignment
oasi	:	asin															{ $$ = $1; }
		|	/* epsilon */												{ $$ =  0; }
		;

// declaration list
dcls	:	decl dcls													{ $1->next = $2; $$ = $1; } 
		|	/* epsilon */												{ $$ = 0; }
		;

// declaration													// #TODO how to isolate ID
decl  : 	vari TOKEN_COLON type init TOKEN_SEMICOLON		{ $$ = decl_create((char *)$1->name, $3, $4, 0, 0); }	
		|	vari TOKEN_COLON arty arii TOKEN_SEMICOLON		{ $$ = decl_create((char *)$1->name, $3, 0, $4, 0); }
		|	vari TOKEN_COLON fxty fxii								{ $$ = decl_create((char *)$1->name, $3, 0, $4, 0); }
		;

// function initialization
fxii	:	TOKEN_ASSIGNMENT bstl									{ $$ = $2; }
		|	TOKEN_SEMICOLON											{ $$ =  0; }
		;

// allows for multi dimensional arrays					// #TODO make sure this works
brcn	:	brcn TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE		{ $$ = expr_create(EXPR_ARR, $1, $3); }
		|	/* epsilon */												{ $$ = 0; }
		;

// TODO: double check that arrays can be decalred with a [], feels wrong but may be right

// fart, lol												
arty	:	TOKEN_ARRAY TOKEN_BRACK_OPEN oasi TOKEN_BRACK_CLOSE fart		{ $$ = type_create(TYPE_ARRAY, $5, 0, $3); }
		;

// array type												
fart	:	type																			{ $$ = $1; }
		|	TOKEN_ARRAY TOKEN_BRACK_OPEN oasi TOKEN_BRACK_CLOSE fart		{ $$ = type_create(TYPE_ARRAY, $5, 0, $3); }
		;

// function type
fxty	:	TOKEN_FUNCTION type TOKEN_PAREN_OPEN opal TOKEN_PAREN_CLOSE	{ $$ = type_create(TYPE_FUNCTION, $2, $4, 0); }
		;

// array initialization
arii	:	TOKEN_ASSIGNMENT TOKEN_BRACE_OPEN xpls TOKEN_BRACE_CLOSE 	{ $$ = $3; }
		|	/* epsilon */																{ $$ =  0; }
		;

// basic initialization
init	:	TOKEN_ASSIGNMENT asin													{ $$ = $2; }
		|	/* epsilon */																{ $$ =  0; }
		;

// types
type	:	TOKEN_INTEGER																{ $$ = type_create(TYPE_INTEGER, 0, 0, 0); } 
		|	TOKEN_FLOAT																	{ $$ = type_create(TYPE_FLOAT, 0, 0, 0); }
		|	TOKEN_CHAR																	{ $$ = type_create(TYPE_CHARACTER, 0, 0, 0); }
		|	TOKEN_STRING																{ $$ = type_create(TYPE_STRING, 0, 0, 0); }
		|	TOKEN_BOOLEAN																{ $$ = type_create(TYPE_BOOLEAN, 0, 0, 0); }
		|	TOKEN_VOID																	{ $$ = type_create(TYPE_VOID, 0, 0, 0); }
		;

%%

int yyerror(char *s)
{
	printf("parse error: %s\n", s);
	return 1;
}
