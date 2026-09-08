#!/bin/bash
# ============================================================
# install.sh - Instalador automático universal para get-input.h
# ============================================================

set -e

echo "Instalando get-input.h..."

# Detectar sistema operativo
OS=$(uname -s 2>/dev/null || echo "Unknown")
echo "📡 Sistema operativo detectado: $OS"

# Definir directorio de instalación
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

# Verificar si el archivo get-input.h existe localmente o descargarlo de GitHub
if [ ! -f "get-input.h" ]; then
    echo "📥 Descargando get-input.h desde el repositorio oficial de GitHub..."
    if command -v curl >/dev/null 2>&1; then
        curl -fsSL https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/get-input.h -o get-input.h
    elif command -v wget >/dev/null 2>&1; then
        wget -q https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/get-input.h -O get-input.h
    else
        echo "❌ Error: Se requiere 'curl' o 'wget' para descargar get-input.h"
        exit 1
    fi
fi

# Copiar el archivo al directorio de destino
echo "📂 Copiando get-input.h a $INSTALL_DIR..."
if [ "$INSTALL_DIR" = "/usr/local/include" ] || [ "$INSTALL_DIR" = "/usr/include" ]; then
    if [ "$(id -u)" -ne 0 ]; then
        sudo mkdir -p "$INSTALL_DIR"
        sudo cp get-input.h "$INSTALL_DIR/"
    else
        mkdir -p "$INSTALL_DIR"
        cp get-input.h "$INSTALL_DIR/"
    fi
else
    mkdir -p "$INSTALL_DIR"
    cp get-input.h "$INSTALL_DIR/"
fi

# Verificación de la instalación
if [ -f "$INSTALL_DIR/get-input.h" ]; then
    echo ""
    echo "✅ ¡get-input.h se ha instalado correctamente en: $INSTALL_DIR/get-input.h!"
    echo "📝 Ahora puedes incluirlo en tus programas en C usando:"
    echo "   #include <get-input.h>"
else
    echo "❌ Error al instalar get-input.h en $INSTALL_DIR"
    exit 1
fi
