CC=gcc
TCC=tcc
FLEX=flex
BISON=bison
SRC_DIR = ./src
INC_DIR = ../include

SRCS1 = syntax.tab.c treeNode.c ortho.c type.c type_op.c IRgen.c IRortho.c GAS_utility.c
SRCS2 = optimizer.c GAS_utility.c

.PHONY: splc splcb splcc splcd clean

splc:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS1) -lfl -ly -o ../bin/splc 
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS2) -o ../bin/optimizer 
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcb:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS2) -o ../bin/optimizer
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splct:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(TCC) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(TCC) $(SRCS2) -o ../bin/optimizer
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
splcc:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS2) -o ../bin/optimizer
splcd:
	cd $(SRC_DIR) && $(BISON) -d -Wcounterexamples syntax.y 
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRCS2) -o ../bin/optimizer
splct:
	cd $(SRC_DIR) && $(BISON) -d syntax.y
	cd $(SRC_DIR) && $(FLEX) lex.l
	cd $(SRC_DIR) && tcc -I$(INC_DIR) $(SRCS1) -lfl -o ../bin/splc
	cd $(SRC_DIR) && tcc -I$(INC_DIR) $(SRCS2) -o ../bin/optimizer
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
clean:
	cd $(SRC_DIR) && rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
	rm -f ./bin/splc