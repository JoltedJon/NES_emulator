ASM=ca65
LNK=ld65
SRC_DIR=tests/6502_tests/source
BIN_DIR=tests/6502_tests/roms
CFG=$(SRC_DIR)/nes.cfg

all:
	@echo "Use make <test_case_name> to assemble and link a specific test case"

%:
	@if [ -f "$(SRC_DIR)/$@.s" ]; then \
		mkdir -p $(BIN_DIR);\
		$(ASM) $(SRC_DIR)/$@.s -o $(BIN_DIR)/$@.o; \
		$(LNK) -C $(CFG) $(BIN_DIR)/$@.o -o $(BIN_DIR)/$@.nes; \
		echo "Generated $(BIN_DIR)/$@.nes"; \
	else \
		echo "Source file $(SRC_DIR)/$@.s doesn't exist."; \
	fi

clean:
	rm $(BIN_DIR)/*.o $(BIN_DIR)/*.nes
