cmake_minimum_required(VERSION 3.10)
project(util_h VERSION 2.0.0 LANGUAGES C)

# Instalación de la biblioteca Header-Only
install(FILES util.h DESTINATION /usr/local/include)

# Mensaje informativo
message(STATUS "util.h preparado para instalación en /usr/local/include")
message(STATUS "Usa: #include <util.h>")
