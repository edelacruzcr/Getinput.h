CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
BIN_DIR = bin
EXAMPLES_DIR = examples
PREFIX ?= /usr/local

HEADERS = util.h
SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
TARGETS = $(patsubst $(EXAMPLES_DIR)/%.c, $(BIN_DIR)/%, $(SOURCES))

.PHONY: all clean install uninstall

all: $(BIN_DIR) $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

install:
	@echo "Instalando util.h en $(PREFIX)/include..."
	@sudo mkdir -p $(PREFIX)/include
	@sudo cp util.h $(PREFIX)/include/
	@echo "Instalado en $(PREFIX)/include/"
	@echo "Ahora puedes usar: #include <util.h>"

uninstall:
	@echo "Desinstalando util.h y eliminando versiones anteriores de $(PREFIX)/include..."
	@sudo rm -f $(PREFIX)/include/util.h $(PREFIX)/include/get-input.h $(PREFIX)/include/arreglo.h
	@echo "Biblioteca desinstalada con éxito"

clean:
	rm -rf $(BIN_DIR)
