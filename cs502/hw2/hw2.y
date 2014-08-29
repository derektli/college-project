/*
* Name: Tingjun Li
* Email: li400@purdue.edu
*/

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    extern char *yytext;
    extern int yyleng;
    int yylex(void);
    void yyerror(char *);
    
    int data[25];    
    
%}

%union {
    int iValue;                 /* integer value */      
};

%token <iValue> INTEGER_LITERAL CHAR_LITERAL
%token OR AND NOT LINE OP_EQ OP_GE OP_LE OP_NE SYNERROR
%left '|'
%left '(' ')'
%left '*'
//%type <iValue> E bfactor bterm bexpr S A B 
%type <iValue> A B
%%
/*
Goal:
        bexpr				{if ($1) printf("YES\n"); else printf("NO\n");} 
        ;

bexpr:
	bexpr OR bterm			{$$ = $1 || $3;}
	| bterm				{$$ = $1;}
	;
	
bterm:
	bterm AND bfactor		{$$ = $1 && $3;}
	| bfactor			{$$ = $1;}
	;
	
bfactor:
	NOT bfactor			{if ($2) $$ = 0; else $$ = 1;}
	| '(' bexpr ')' 		{$$ = $2;}
	| E '>' E			{if ($1 > $3) $$ = 1; else $$ = 0;}
	| E '<' E			{if ($1 < $3) $$ = 1; else $$ = 0;}
	| E OP_EQ E			{if ($1 == $3) $$ = 1; else $$ = 0;}
	| E OP_GE E			{if ($1 >= $3) $$ = 1; else $$ = 0;}
	| E OP_LE E			{if ($1 <= $3) $$ = 1; else $$ = 0;}
	| E OP_NE E			{if ($1 != $3) $$ = 1; else $$ = 0;}
	;
	
E:
	INTEGER_LITERAL 		{ $$ = $1;}
	| LINE INTEGER_LITERAL		{ $$ = data[$2];}
	| '(' E ')' 			{ $$ = $2; }
	| E '+' E			{ $$ = $1 + $3; }
	| E '-' E			{ $$ = $1 - $3; }
	| E '*' E			{ $$ = $1 * $3; }
	| E '/' E			{ $$ = $1 / $3; }
	;
	
*/

Goal:
	A				{}
	;

A: 	
	'(' A ')'			{$$ = $2;}
	| A '|' A 			{$$ = $1||$3;}
	| A '*' 			{$$ = 1;}
	| B				{$$ = $1;}
	;
	
B:	CHAR_LITERAL B			{$$ = 0;}
	| 				{$$ = 1;}
	;
	

	
%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(int argc, char *argv[]) {
    if (argc==1)
    {
	perror("How to use: ./hw2 database.txt\n");
	return 1;
    }
    
    FILE* pFile = fopen (argv[1] , "r");
   
    if (pFile == NULL && argc<=1) perror ("Error opening file");
    else
    {
	int i = 0;
	while(fscanf(pFile, "%d", &data[i])!=EOF) i++;	
    }
	    
    yyparse();
}
