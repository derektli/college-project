hw2:y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -o hw2 -lfl
y.tab.c: hw2.y
	bison -y -d -g --verbose hw2.y
lex.yy.c: hw2.l
	lex hw2.l
clean:
	rm -f lex.yy.c y.tab.c

