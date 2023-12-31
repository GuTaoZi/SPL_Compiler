INC_DIR = ../../include

SRC_OPT = optimizer.c back_utility.c
SRC_MIPS = main.c tac.c mips32.c
OBJ_DIR = ../../bin

all: opt tcg clean

opt: 
	rm -rf $(OBJ_DIR)/opt
	gcc -I$(INC_DIR)/backend $(SRC_OPT) -o $(OBJ_DIR)/opt

optt: 
	rm -rf $(OBJ_DIR)/opt
	tcc -I$(INC_DIR)/backend $(SRC_OPT) -o $(OBJ_DIR)/opt

tcg: 
	rm -rf $(OBJ_DIR)/tcg
	gcc -I$(INC_DIR)/backend $(SRC_MIPS) -o $(OBJ_DIR)/tcg

tcgt: 
	rm -rf $(OBJ_DIR)/tcg
	tcc -I$(INC_DIR)/backend $(SRC_MIPS) -o $(OBJ_DIR)/tcg

clean:

.PHONY: opt optt tcg tcgt clean all