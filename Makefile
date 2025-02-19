CC = gcc
CFLAGS = -Wall -Wextra -O2 -lm
SRC_DIR = src
TEST_DIR = tests

.PHONY: all clean test

all: simulator

simulator:
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/parse.c $(SRC_DIR)/mna.c $(SRC_DIR)/solver.c -o $@

test: simulator
	cd $(TEST_DIR) && ./test_runner.sh

clean:
	rm -f simulator
	rm -f $(TEST_DIR)/test_cases/*.output