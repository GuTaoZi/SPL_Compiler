INC_DIR = ../../include/frontend

SRCS = syntax.tab.c treeNode.c ortho.c type.c type_op.c IRgen.c IRortho.c front_utility.c
OBJ_DIR = ../../bin

all: splc clean

# build with tcc
splc: clean 
	bison -d syntax.y --defines=$(INC_DIR)/syntax.tab.h
	flex lex.l
	gcc -I$(INC_DIR) $(SRCS) -lfl -o $(OBJ_DIR)/splc
	rm -f lex.yy.c syntax.tab.c $(INC_DIR)/syntax.tab.h *.out

# build with gcc
splct: clean
	bison -d syntax.y --defines=$(INC_DIR)/syntax.tab.h
	flex lex.l
	tcc -I$(INC_DIR) $(SRCS) -lfl -o $(OBJ_DIR)/splc
	rm -f lex.yy.c syntax.tab.c $(INC_DIR)/syntax.tab.h *.out

# build and save intermediate code by flex and bison
splcd: clean 
	bison -d syntax.y --defines=$(INC_DIR)/syntax.tab.h
	flex lex.l
	gcc -I$(INC_DIR) $(SRCS) -lfl -o $(OBJ_DIR)/splc

clean:
	rm -f lex.yy.c syntax.tab.c $(INC_DIR)/syntax.tab.h *.out

.PHONY: splc splct splcd clean all
