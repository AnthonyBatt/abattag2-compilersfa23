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
begi	:	dcls TOKEN_EOF												{ printf("result = %d\n", $1); return 0; }
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
ngtn	:	TOKEN_MINUS post											{ $$ = expr_create(EXPR_NEG, $1, $3); }
		|	TOKEN_PLUS post											{ $$ = expr_create(EXPR_PLS, $1, $3); }
		|	TOKEN_NOT post												{ $$ = expr_create(EXPR_NOT, $1, $3); }
		|	post															{ $$ =  $1; }
		;

// post increment/decrement
post	:	atom TOKEN_POST_INC										{ $$ = expr_create(EXPR_INC, $1, $3); }
		|	atom TOKEN_POST_DEC										{ $$ = expr_create(EXPR_DEC, $1, $3); }
		|	atom															{ $$ = $1; }
		;

// atomic thingies													// #TODO check these work and what do bout fx/arr
atom	:	TOKEN_FLOAT_LITERAL										{ $$ = expr_create_float_literal(atof(yytext)); }
		|	TOKEN_INTEGER_LITERAL									{ $$ = expr_create_integer_literal(atoi(yytext)); }
		|	TOKEN_CHAR_LITERAL										{ $$ = expr_create_char_literal(yytext[0]); }
		|	TOKEN_STRING_LITERAL										{ $$ = expr_create_string_literal(yytext); }
		|	TOKEN_TRUE_LITERAL										{ $$ = expr_create_boolean_literal(1); }
		|	TOKEN_FALSE_LITERAL										{ $$ = expr_create_boolean_literal(0); }
		|	TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE			{ $$ = $2; }
		|	TOKEN_ID														{ $$ = expr_create_name(yytext); }
		|	TOKEN_ID TOKEN_PAREN_OPEN oxpl TOKEN_PAREN_CLOSE			{ $$ = 3; }
		|	TOKEN_ID TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE brcn		{ $$ = 3; }
		;

// optional parameter list
opal	:	pals															{ $$ = $1; }
		|	/* epsilon */												{ $$ =  0; }
		;

// parameter list
pals	:	parm															{ $$ = $1; }			
		|	parm TOKEN_COMMA pals									{ $1->next = $3; $$ = $1; }			
		;

// parameter															// # TODO how to capture the ID
parm	:	TOKEN_ID TOKEN_COLON paty								{ $$ = param_list_create($1, $3, 0); }
		;

// parameter types
paty	:	type																	{ $$ = $1; }	
		|	TOKEN_ARRAY TOKEN_BRACK_OPEN TOKEN_BRACK_CLOSE paty	{ $$ = type_create(TYPE_ARRAY, $4, 0); }
		;

// braced expression list
bxpl	:	TOKEN_BRACE_OPEN oxpl TOKEN_BRACE_CLOSE			{ $$ = $2; }
		;

// optional expression list
oxpl	:	xpls															{ $$ = $1; }
		|	/* epsilon */												{ $$ =  0; }
		;

// expression list													
xpls	:	asin															{ $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0, 0);	
		|	asin TOKEN_COMMA xpls									{ $1->next = $3; $$ = $1; }									
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
opst	:	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst	 					  { stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5,  0, 0); }
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE opst  					  { stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5,  0, 0); }
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst TOKEN_ELSE opst  { stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0); }
		|	TOKEN_FOR TOKEN_PAREN_OPEN oasi TOKEN_SEMICOLON oasi TOKEN_SEMICOLON oasi TOKEN_PAREN_CLOSE opst 	{ stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0, 0);
		;

// closed statement
clst	:	smst {printf("clst: smst\n");}
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst TOKEN_ELSE clst {printf("clst: if clst else clst\n");}
		|	TOKEN_FOR TOKEN_PAREN_OPEN oasi TOKEN_SEMICOLON oasi TOKEN_SEMICOLON oasi TOKEN_PAREN_CLOSE clst 
		;

// simple statement
smst	:	bstl
		|	decl 
		|	TOKEN_RETURN oasi TOKEN_SEMICOLON 
		|	TOKEN_PRINT oxpl TOKEN_SEMICOLON 
		|	asin TOKEN_SEMICOLON 
		;

// optional assignment
oasi	:	asin
		|	/* epsilon */
		;

// declaration list
dcls	:	decl dcls
		|	/* epsilon */	
		;

// declaration
decl  : 	TOKEN_ID TOKEN_COLON type init TOKEN_SEMICOLON	
		|	TOKEN_ID TOKEN_COLON arty arii TOKEN_SEMICOLON
		|	TOKEN_ID TOKEN_COLON fxty fxii						
		;

// function initialization
fxii	:	TOKEN_ASSIGNMENT bstl
		|	TOKEN_SEMICOLON
		;

// allows for multi dimensional arrays
brcn	:	brcn TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE
		|	/* epsilon */
		;

// TODO: double check that arrays can be decalred with a [], feels wrong but may be right

arty	:	TOKEN_ARRAY TOKEN_BRACK_OPEN oasi TOKEN_BRACK_CLOSE fart							//{ if (!$3) {printf("array length cant be 0\n"); return 1;} }
		;

// array type
fart	:	type
		|	TOKEN_ARRAY TOKEN_BRACK_OPEN oasi TOKEN_BRACK_CLOSE fart						//{ if (!$3) {printf("array length cant be 0\n"); return 1;} }
		;

// function type
fxty	:	TOKEN_FUNCTION type TOKEN_PAREN_OPEN opal TOKEN_PAREN_CLOSE
		;

// array initialization
arii	:	TOKEN_ASSIGNMENT TOKEN_BRACE_OPEN xpls TOKEN_BRACE_CLOSE 
		|	/* epsilon */
		;

// basic initialization
init	:	TOKEN_ASSIGNMENT asin
		|	/* epsilon */
		;

// types
type	:	TOKEN_INTEGER
		|	TOKEN_FLOAT
		|	TOKEN_CHAR
		|	TOKEN_STRING
		|	TOKEN_BOOLEAN
		|	TOKEN_VOID
		;

%%

int yyerror(char *s)
{
	printf("parse error: %s\n", s);
	return 1;
}
