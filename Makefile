CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
BIN_DIR = bin
EXAMPLES_DIR = examples
PREFIX ?= /usr/local

SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
TARGETS = $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(SOURCES))

.PHONY: all clean install uninstall

all: $(BIN_DIR) $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c get-input.h
	$(CC) $(CFLAGS) $< -o $@

install:
	@echo "Instalando get-input.h en $(PREFIX)/include..."
	@sudo mkdir -p $(PREFIX)/include
	@sudo cp get-input.h $(PREFIX)/include/
	@echo "Instalado en $(PREFIX)/include/get-input.h"
	@echo "Ahora puedes usar: #include <get-input.h>"

uninstall:
	@echo "Desinstalando get-input.h de $(PREFIX)/include..."
	@sudo rm -f $(PREFIX)/include/get-input.h
	@echo "get-input.h desinstalado con éxito"

clean:
	rm -rf $(BIN_DIR)
