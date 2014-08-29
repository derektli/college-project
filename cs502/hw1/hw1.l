/*
* Name: Tingjun Li
* Email: li400@purdue.edu
*/

%option noyywrap

ID [a-zA-Z][a-zA-Z0-9_]*
INTEGER [0-9]+
COMMENT "/*"([^*]|[\n]|(\*+([^*/]|[\n])))*\*+"/"
OPERATOR "&&"|"||"|"=="|"!="|"<"|">"|"<="|">="|"+"|"-"|"*"|"/"

%%

{COMMENT} ;

{ID} 	{
		printf("%s\n",yytext);
	}

{INTEGER} 	{	
			printf("%s\n",yytext);
		}
		
{OPERATOR} 	{
			printf("%s\n",yytext);
		}
.|\n ;

%%

int main()
{
	yylex();
	return 0;
}
