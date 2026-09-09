cmake_minimum_required(VERSION 3.10)
project(getinput_arreglo VERSION 2.0.0 LANGUAGES C)

# Instalacion de las bibliotecas Header-Only
install(FILES get-input.h arreglo.h DESTINATION /usr/local/include)

# Mensaje informativo
message(STATUS "get-input.h y arreglo.h preparados para instalacion en /usr/local/include")
message(STATUS "Usa: #include <get-input.h> y #include <arreglo.h>")
