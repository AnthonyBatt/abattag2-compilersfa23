#include <stdio.h>

#include "encoder.h"

extern FILE *yyin;
extern int yylex();
extern char *yytext;

int scan(FILE *fp)
{
	yyin = fp;

	char token_arr[48][255] =
	{
		"TOKEN_ARRAY",
		"TOKEN_VOID",
		"TOKEN_INTEGER",
		"TOKEN_FLOAT",
		"TOKEN_CHAR",
		"TOKEN_STRING",
		"TOKEN_BOOLEAN",
		"TOKEN_TRUE_LITERAL",
		"TOKEN_FALSE_LITERAL",
		"TOKEN_IF",
		"TOKEN_ELSE",
		"TOKEN_FOR",
		"TOKEN_WHILE",
		"TOKEN_PRINT",
		"TOKEN_FUNCTION",
		"TOKEN_RETURN",
		"TOKEN_AUTO",

		"TOKEN_SEMICOLON",
		"TOKEN_COLON",
		"TOKEN_PAREN_OPEN",
		"TOKEN_PAREN_CLOSE",
		"TOKEN_BRACK_OPEN",
		"TOKEN_BRACK_CLOSE",
		"TOKEN_BRACE_OPEN",
		"TOKEN_BRACE_CLOSE",
		"TOKEN_EXPO",
		"TOKEN_MULTI",
		"TOKEN_DIVIDE",
		"TOKEN_MODULUS",
		"TOKEN_LE",
		"TOKEN_LT",
		"TOKEN_GE",
		"TOKEN_GT",
		"TOKEN_EQ",
		"TOKEN_NE",
		"TOKEN_NOT",
		"TOKEN_ASSIGNMENT",
		"TOKEN_LOG_OR",
		"TOKEN_LOG_AND",
		"TOKEN_POST_INC",
		"TOKEN_POST_DEC",
		"TOKEN_PLUS",
		"TOKEN_MINUS",

		"TOKEN_ID",
		"TOKEN_FLOAT_LITERAL",
		"TOKEN_INTEGER_LITERAL",
		"TOKEN_CHAR_LITERAL",
		"TOKEN_STRING_LITERAL"
	};

	int t;
	while(1)
	{
		t = yylex();
		if (t < 0) break;
		// if it is a character check for backslash codes
		else if (t == 46)
		{
			char c = yytext[1];
			// escape characters sub if chain
			if (c == 92)
			{
				unsigned char c2 = yytext[2];
				// if chain for c2
				// \a
				if (c2 == 97)
				{
					c = 7;
				}
				// \b
				else if (c2 == 98)
				{
					c = 8;
				}
				// \e
				else if (c2 == 101)
				{
					c = 27;
				}
				// \f
				else if (c2 == 102)
				{
					c = 12;
				}
				// \n
				else if (c2 == 110)
				{
					c = 10;
				}
				// \r
				else if (c2 == 114)
				{
					c = 13;
				}
				// \t
				else if (c2 == 116)
				{
					c = 9;
				}
				// \v
				else if (c2 == 118)
				{
					c = 11;
				}
				// /* \\ */
				else if (c2 == 92)
				{
					c = 92;
				}
				// \'
				else if (c2 == 39)
				{
					c = 39;
				}
				// \"
				else if (c2 == 34)
				{
					c = 34;
				}
				// \0
				else if (c2 == 48)
				{
					// this could be hex digit stuff
					// \0x
					if (yytext[3] != 120)
					{
						fprintf(stderr, "invalid attempt at using hexadecimal replacement, please format as such:\n\t\\0xHH where HH is a valid hexdigit combo (0-9a-fA-F)\n");
						return 1;
					}
					// valid hexdigits are 48-57, 65-70, and 97-102
					unsigned char digit1 = yytext[4];
					unsigned char digit2 = yytext[5];
					if (	 ((digit1 >= 48 && digit1 <= 57) || (digit1 >= 65 && digit1 <= 70) || (digit1 >= 97 && digit1 <= 102))
						 && ((digit2 >= 48 && digit2 <= 57) || (digit2 >= 65 && digit2 <= 70) || (digit2 >= 97 && digit2 <= 102))	 )
					{
						// convert to decimal, these can be outside the printable range
						unsigned char hex[3] = {yytext[4], yytext[5], 0};
						int dec = strtol((char *)hex, NULL, 16) & 0x000000ff;
						c = dec;
					}
					else
					{
						fprintf(stderr, "invalid hexdigits given, valid hexdigits are 0-9, a-f, and A-F, given digits were: %c and %c\n", digit1, digit2);
						return 0;
					}
				}
				// invalid escape char 
				else
				{
					fprintf(stderr, "invalid escape character: %c, if you meant to just print \\ type \\\\\n", c2);
					return 1;
				}
			}
			printf("%-25s %c\n", token_arr[t], c);
		}
		// it is a string literal
		else if (t == 47)
		{
			unsigned char ds[256];
			// decode and if it fails print an appropiate error message
			if (!(string_decode((const unsigned char *)yytext, ds)))
			{
				fprintf(stderr, "decode(%s): the string was unable to be decoded\n", yytext);
				return 1;
			}
			else
			{
				printf("%-25s %s\n", token_arr[t], ds);
			}
		}
		else printf("%-25s %s\n", token_arr[t], yytext);
	}

	if (t == -2)
	{
		printf("There was a scan error on %s\n", yytext);
		return 1;
	}

	return 0;
}
