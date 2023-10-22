CC=gcc
FLEX=flex
BISON=bison
splc:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c -lfl -ly -o ./bin/splc
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
clean:
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	@rm -f ./bin/splc