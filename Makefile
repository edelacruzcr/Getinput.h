CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
BIN_DIR = bin
EXAMPLES_DIR = examples

SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
TARGETS = $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(SOURCES))

.PHONY: all clean run

all: $(BIN_DIR) $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c get-input.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

