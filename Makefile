CC=gcc
FLEX=flex
BISON=bison
splc:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -ly -o ./bin/splc
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcb:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -o ./bin/splc
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcc:
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -o ./bin/splc
splcd:
	$(BISON) -d -Wcounterexamples syntax.y 
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -o ./bin/splc
clean:
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	@rm -f ./bin/splc