CC=gcc
FLEX=flex
BISON=bison
SRC_DIR = ./src

SRCS = syntax.tab.c treeNode.c ortho.c type.c type_op.c

.PHONY: splc splcb splcc splcd clean

splc:
	mkdir -p bin
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS) -lfl -ly -o ../bin/splc
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcb:
	mkdir -p bin
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS) -lfl -o ../bin/splc
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcc:
	mkdir -p bin
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS) -lfl -o ../bin/splc
splcd:
	mkdir -p bin
	cd $(SRC_DIR) && $(BISON) -d -Wcounterexamples syntax.y 
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS) -lfl -o ../bin/splc
clean:
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	rm -f ./bin/splc