CC=gcc
FLEX=flex
BISON=bison
SRC_DIR = ./src

SRCS1 = syntax.tab.c treeNode.c ortho.c type.c type_op.c IRgen.c IRortho.c
SRCS2 = optimizer.c 

.PHONY: splc splcb splcc splcd clean

splc:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS1) -lfl -ly -o ../bin/splc
	cd $(SRC_DIR) && $(CC) $(SRCS2) -o ../bin/optimizer
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcb:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(CC) $(SRCS2) -o ../bin/optimizer
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcc:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(CC) $(SRCS2) -o ../bin/optimizer
splcd:
	cd $(SRC_DIR) && $(BISON) -d -Wcounterexamples syntax.y 
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(CC) $(SRCS2) -o ../bin/optimizer
clean:
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	rm -f ./bin/splc