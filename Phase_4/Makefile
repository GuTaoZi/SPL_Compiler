CC=gcc
TCC=tcc
SRC_DIR = ./src
INC_DIR = ../include
BIN_DIR=../bin
SRC=main.c tac.c mips32.c

tcg: clean
	@mkdir -p bin
	cd $(SRC_DIR) && $(CC) -I$(INC_DIR) $(SRC) -o $(BIN_DIR)/tcg

tcgt: clean
	@mkdir -p bin
	cd $(SRC_DIR) && $(TCC) -I$(INC_DIR) $(SRC) -o $(BIN_DIR)/tcg

clean:
	@rm -rf $(BIN)/tcg
.PHONY: tcg clean
