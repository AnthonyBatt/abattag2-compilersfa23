#include <stdio.h>

extern int yyparse();

int main()
{
	if (yyparse())		printf("Parse Failed\n");
	else					printf("Parse Successful\n");
}
