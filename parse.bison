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

begi	:	stmt																								{ printf("result = %d\n", $1); return 0; }
		|	/* epsilon */
		;

asin	:	lgor TOKEN_ASSIGNMENT asin																	{ $$ = $3; }
		|	lgor																								{ $$ = $1; }
		;

lgor	:	lgor TOKEN_LOG_OR	lgan																		{ $$ = $1 || $3; }
		|	lgan																								{ $$ = $1; }
		;

lgan	:	lgan TOKEN_LOG_AND comp																		{ $$ = $1 && $3; }
		|	comp																								{ $$ = $1; }
		;

comp	:	comp TOKEN_LT expr																			{ $$ = $1 <  $3; }
		|	comp TOKEN_LE expr																			{ $$ = $1 <= $3; }
		|	comp TOKEN_GT expr																			{ $$ = $1 >  $3; }
		|	comp TOKEN_GE expr																			{ $$ = $1 >= $3; }
		|	comp TOKEN_EQ expr																			{ $$ = $1 == $3; }
		|	comp TOKEN_NE expr																			{ $$ = $1 != $3; }
		|	expr																								{ $$ = $1; }
		;

expr	:	expr TOKEN_PLUS term																			{ $$ = $1 + $3; }
		|	expr TOKEN_MINUS term																		{ $$ = $1 - $3; }
		|	term																								{ $$ = $1; }
		;

term	:	term TOKEN_MULTI expo																		{ $$ = $1 * $3; }
		|	term TOKEN_DIVIDE expo																		{ if ($3) {$$ = $1 / $3;} else {printf("cannot divide by zero\n"); return 1;} }
		|	term TOKEN_MODULUS expo																		{ if ($3) {$$ = $1 % $3;} else {printf("cannot divide by zero\n"); return 1;} }
		|	expo																								{ $$ = $1; }
		;

expo	:	expo TOKEN_EXPO ngtn																			{ $$ = pow($1, $3); }
		|	ngtn																								{ $$ = $1; }
		;

ngtn	:	TOKEN_MINUS post																				{ $$ = -$2; }
		|	TOKEN_PLUS post																				{ $$ =  $2; }
		|	TOKEN_NOT post																					{ $$ = !$2; }
		|	post																								{ $$ =  $1; }
		;

post	:	atom TOKEN_POST_INC																			{ $$ = $1++; }
		|	atom TOKEN_POST_DEC																			{ $$ = $1--; }
		|	atom																								{ $$ = $1;   }
		;

atom	:	TOKEN_FLOAT_LITERAL
		|	TOKEN_INTEGER_LITERAL																		{ $$ = atoi(yytext); }
		|	TOKEN_CHAR_LITERAL																			{ $$ = 4; }
		|	TOKEN_STRING_LITERAL																			{ $$ = 5; }
		|	TOKEN_TRUE_LITERAL																			{ $$ = 1; }
		|	TOKEN_FALSE_LITERAL																			{ $$ = 0; }
		|	TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE												{ $$ = $2; }
		|	TOKEN_ID																							{ $$ = 42; }
		|	TOKEN_ID TOKEN_PAREN_OPEN oxpl TOKEN_PAREN_CLOSE									{ $$ = 0; }
		|	TOKEN_ID TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE brcn								{ $$ = 3; }
		;

opal	:	pals
		|	/* epsilon */
		;

pals	:	parm																							
		|	parm TOKEN_COMMA pals																
		;

parm	:	TOKEN_ID TOKEN_COLON type
		;

bxpl	:	TOKEN_BRACE_OPEN oxpl TOKEN_BRACE_CLOSE
		;

oxpl	:	xpls
		|	/* epsilon */
		;

xpls	:	asin																								
		|	asin TOKEN_COMMA xpls																		
		|	bxpl
		|	bxpl TOKEN_COMMA xpls
		;

bstl	:	TOKEN_BRACE_OPEN ostl TOKEN_BRACE_CLOSE
		;

ostl	:	stls
		|	/* epsilon */
		;

stls	:	stmt																				
		|	stmt stls																		
		;

stmt	:	opst
		|	clst
		;

opst	:	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst	{printf("opst: if clst\n");}
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE opst {printf("opst: if opst\n");}
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst TOKEN_ELSE opst {printf("opst: if clst else opst\n");}
		|	TOKEN_FOR TOKEN_PAREN_OPEN oasi TOKEN_SEMICOLON oasi TOKEN_SEMICOLON oasi TOKEN_PAREN_CLOSE opst {printf("opst: for opst\n");}
		;

clst	:	smst {printf("clst: smst\n");}
		|	TOKEN_IF TOKEN_PAREN_OPEN asin TOKEN_PAREN_CLOSE clst TOKEN_ELSE clst {printf("clst: if clst else clst\n");}
		|	TOKEN_FOR TOKEN_PAREN_OPEN oasi TOKEN_SEMICOLON oasi TOKEN_SEMICOLON oasi TOKEN_PAREN_CLOSE clst {printf("clst: for clst\n");}
		;


smst	:	bstl {printf("smst: bstl\n");}
		|	decl {printf("smst: decl\n");}
		|	TOKEN_RETURN oasi TOKEN_SEMICOLON {printf("smst: return\n");}
		|	TOKEN_PRINT oxpl TOKEN_SEMICOLON {printf("smst: print\n");}
		|	asin TOKEN_SEMICOLON {printf("smst:\n");}
		;

oasi	:	asin
		|	/* epsilon */
		;

decl  : 	TOKEN_ID TOKEN_COLON type init TOKEN_SEMICOLON
		|	TOKEN_ID TOKEN_COLON arty arii TOKEN_SEMICOLON
		|	TOKEN_ID TOKEN_COLON fxty fxii
		;

fxii	:	TOKEN_ASSIGNMENT bstl
		|	/* epsilon */
		;

brcn	:	brcn TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE
		|	/* epsilon */
		;

arty	:	TOKEN_ARRAY TOKEN_BRACK_OPEN asin TOKEN_BRACK_CLOSE type							{ if (!$3) {printf("array length cant be 0\n"); return 1;} }
		;

fxty	:	TOKEN_FUNCTION type TOKEN_PAREN_OPEN opal TOKEN_PAREN_CLOSE
		;

arii	:	TOKEN_ASSIGNMENT bxpl 
		|	/* epsilon */
		;

init	:	TOKEN_ASSIGNMENT asin
		|	/* epsilon */
		;

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
