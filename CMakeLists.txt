cmake_minimum_required(VERSION 3.10)
project(get-input VERSION 2.0.0 LANGUAGES C)

# Instalación de la biblioteca Header-Only
install(FILES get-input.h DESTINATION /usr/local/include)

# Mensaje informativo
message(STATUS "✅ get-input.h preparado para instalación en /usr/local/include")
message(STATUS "📝 Usa: #include <get-input.h>")
