CC=gcc
FLEX=flex
BISON=bison
splc:
	cd ./src
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -ly -o ../bin/splc
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcb:
	cd ./src
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -o ../bin/splc
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcc:
	cd ./src
	$(BISON) -d syntax.y
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -o ../bin/splc
splcd:
	cd ./src
	$(BISON) -d -Wcounterexamples syntax.y 
	$(FLEX) lex.l
	$(CC) syntax.tab.c treeNode.c -lfl -o ../bin/splc
clean:
	cd ./src
	@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	@rm -f ../bin/splc