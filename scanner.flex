%{
#include "scanner.h"
%}


%%
[\ \t\n\r]																						{ /* do nothing */ }
\/\*([^*]|\*+[^/])*\*\/ 																	{ /* do nothing */ }
\/\/[^\n]*\n																					{ /* do nothing */ }

"array"																							{ return TOKEN_ARRAY; }
"auto"																							{ return TOKEN_AUTO; }  
"boolean"																						{ return TOKEN_BOOLEAN; }
"char" 																							{ return TOKEN_CHAR; }  
"else" 																							{ return TOKEN_ELSE; }   
"false"																							{ return TOKEN_FALSE_LITERAL; }
"float"																							{ return TOKEN_FLOAT; }  
"for"  																							{ return TOKEN_FOR; }   
"function"																						{ return TOKEN_FUNCTION; }
"if"   																							{ return TOKEN_IF; }     
"integer" 																						{ return TOKEN_INTEGER; }
"print"																							{ return TOKEN_PRINT; }  
"return"																							{ return TOKEN_RETURN; } 
"string"																							{ return TOKEN_STRING; } 
"true"																							{ return TOKEN_TRUE_LITERAL; }   
"void"																							{ return TOKEN_VOID; }   
"while"																							{ return TOKEN_WHILE; }  

;																									{ return TOKEN_SEMICOLON; }
:																									{ return TOKEN_COLON; }  
\(																									{ return TOKEN_PAREN_OPEN; }
\)																									{ return TOKEN_PAREN_CLOSE; }
\[																									{ return TOKEN_BRACK_OPEN; }
\]																									{ return TOKEN_BRACK_CLOSE; }
\{																									{ return TOKEN_BRACE_OPEN; }
\}																									{ return TOKEN_BRACE_CLOSE; }
\^																									{ return TOKEN_EXPO; }   
\*																									{ return TOKEN_MULTI; }  
\/																									{ return TOKEN_DIVIDE; } 
\%																									{ return TOKEN_MODULUS; }
\<=																								{ return TOKEN_LE; }     
\<																									{ return TOKEN_LT; }     
\>=																								{ return TOKEN_GE; }     
\>																									{ return TOKEN_GT; }     
==																									{ return TOKEN_EQ; }     
!=																									{ return TOKEN_NE; }     
!																									{ return TOKEN_NOT; }    
=																									{ return TOKEN_ASSIGNMENT; }
\|\|																								{ return TOKEN_LOG_OR; } 
\&\&																								{ return TOKEN_LOG_AND; }

[a-zA-Z_][a-zA-Z0-9_]*																		{ return TOKEN_ID; }     
[\+\-]?[0-9]*\.[0-9]+([eE][\+\-]?[0-9]+)?												{ return TOKEN_FLOAT_LITERAL; }
[\+\-]?[0-9]+ 																					{ return TOKEN_INTEGER_LITERAL; }
\+\+																								{ return TOKEN_POST_INC; }
\+																									{ return TOKEN_PLUS; }    
\-\-																								{ return TOKEN_POST_DEC; }
\-																									{ return TOKEN_MINUS; }
\'([ -~]|\\[abefnrtv\\\'\"]|\\0x[A-Fa-f0-9]{2})\'									{ return TOKEN_CHAR_LITERAL; }
\"[ -~]*\"																						{ return TOKEN_STRING_LITERAL; }

.																									{ return -2; }
%%

int yywrap() 	{ return -1; }
