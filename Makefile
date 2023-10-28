CC=gcc
FLEX=flex
BISON=bison
SRC_DIR = ./src

.PHONY: splc splcb splcc splcd clean

splc:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) syntax.tab.c treeNode.c -lfl -ly -o ../bin/splc
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcb:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) syntax.tab.c treeNode.c -lfl -o ../bin/splc
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcc:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) syntax.tab.c treeNode.c -lfl -o ../bin/splc
splcd:
	cd $(SRC_DIR) && $(BISON) -d -Wcounterexamples syntax.y 
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) syntax.tab.c treeNode.c -lfl -o ../bin/splc
clean:
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	rm -f ./bin/splc