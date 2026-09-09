CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
BIN_DIR = bin
EXAMPLES_DIR = examples
PREFIX ?= /usr/local

HEADERS = get-input.h arreglo.h
SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
TARGETS = $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(SOURCES))

.PHONY: all clean install uninstall

all: $(BIN_DIR) $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

install:
	@echo "Instalando get-input.h y arreglo.h en $(PREFIX)/include..."
	@sudo mkdir -p $(PREFIX)/include
	@sudo cp get-input.h arreglo.h $(PREFIX)/include/
	@echo "Instalados en $(PREFIX)/include/"
	@echo "Ahora puedes usar: #include <get-input.h> y #include <arreglo.h>"

uninstall:
	@echo "Desinstalando get-input.h y arreglo.h de $(PREFIX)/include..."
	@sudo rm -f $(PREFIX)/include/get-input.h $(PREFIX)/include/arreglo.h
	@echo "Bibliotecas desinstaladas con exito"

clean:
	rm -rf $(BIN_DIR)
