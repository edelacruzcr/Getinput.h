#!/bin/bash
# ============================================================
# install.sh - Instalador automático universal
# Biblioteca: util.h
# ============================================================

set -e

echo "Instalando biblioteca C (util.h)..."

# Detectar sistema operativo
OS=$(uname -s 2>/dev/null || echo "Unknown")
echo "Sistema operativo detectado: $OS"

# Definir directorio de instalacion
case "$OS" in
    Linux|Darwin)
        INSTALL_DIR="/usr/local/include"
        ;;
    MINGW*|CYGWIN*|MSYS*)
        if [ -d "C:/MinGW/include" ]; then
            INSTALL_DIR="C:/MinGW/include"
        elif [ -d "C:/msys64/usr/include" ]; then
            INSTALL_DIR="C:/msys64/usr/include"
        else
            INSTALL_DIR="./include"
            mkdir -p "$INSTALL_DIR"
        fi
        ;;
    *)
        INSTALL_DIR="./include"
        mkdir -p "$INSTALL_DIR"
        ;;
esac

# Limpiar versiones anteriores obsoletas si existen
OLD_FILES=("$INSTALL_DIR/get-input.h" "$INSTALL_DIR/arreglo.h")
for OLD_FILE in "${OLD_FILES[@]}"; do
    if [ -f "$OLD_FILE" ]; then
        echo "Eliminando cabecera antigua obsoleta: $OLD_FILE..."
        if [ "$INSTALL_DIR" = "/usr/local/include" ] || [ "$INSTALL_DIR" = "/usr/include" ]; then
            if [ "$(id -u)" -ne 0 ]; then
                sudo rm -f "$OLD_FILE"
            else
                rm -f "$OLD_FILE"
            fi
        else
            rm -f "$OLD_FILE"
        fi
    fi
done

ARCHIVOS=("util.h")

for ARCHIVO in "${ARCHIVOS[@]}"; do
    # Verificar si el archivo existe localmente o descargarlo de GitHub
    if [ ! -f "$ARCHIVO" ]; then
        echo "Descargando $ARCHIVO desde el repositorio oficial de GitHub..."
        if command -v curl >/dev/null 2>&1; then
            curl -fsSL "https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/$ARCHIVO" -o "$ARCHIVO"
        elif command -v wget >/dev/null 2>&1; then
            wget -q "https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/$ARCHIVO" -O "$ARCHIVO"
        else
            echo "Error: Se requiere 'curl' o 'wget' para descargar $ARCHIVO"
            exit 1
        fi
    fi

    # Copiar el archivo al directorio de destino
    echo "Copiando $ARCHIVO a $INSTALL_DIR..."
    if [ "$INSTALL_DIR" = "/usr/local/include" ] || [ "$INSTALL_DIR" = "/usr/include" ]; then
        if [ "$(id -u)" -ne 0 ]; then
            sudo mkdir -p "$INSTALL_DIR"
            sudo cp "$ARCHIVO" "$INSTALL_DIR/"
        else
            mkdir -p "$INSTALL_DIR"
            cp "$ARCHIVO" "$INSTALL_DIR/"
        fi
    else
        mkdir -p "$INSTALL_DIR"
        cp "$ARCHIVO" "$INSTALL_DIR/"
    fi

    # Verificacion de la instalacion
    if [ -f "$INSTALL_DIR/$ARCHIVO" ]; then
        echo "  - $ARCHIVO instalado correctamente en: $INSTALL_DIR/$ARCHIVO"
    else
        echo "Error al instalar $ARCHIVO en $INSTALL_DIR"
        exit 1
    fi
done

echo ""
echo "Instalacion completada con exito."
echo "Ahora puedes incluir la biblioteca en tus programas de C:"
echo "   #include <util.h>"
