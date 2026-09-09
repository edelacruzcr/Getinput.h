#include <stdio.h>
#include "util.h"

int main(void) {
    Arreglo *config = arreglo_nuevo();
    
    // Guardar configuracion del sistema
    arreglo_guardar(config, "tema", "oscuro");
    arreglo_guardar(config, "idioma", "espanol");
    arreglo_guardar_int(config, "fuente_tamano", 14);
    arreglo_guardar_bool(config, "notificaciones", true);
    
    // Leer configuracion
    char *tema = arreglo_buscar(config, "tema");
    printf("Tema actual: %s\n", tema ? tema : "predeterminado");
    
    bool encontrado = false;
    int tamano_fuente = arreglo_buscar_int(config, "fuente_tamano", &encontrado);
    if (encontrado) {
        printf("Tamano de fuente: %dpx\n", tamano_fuente);
    }
    
    // Actualizar clave existente
    arreglo_guardar(config, "tema", "claro");
    printf("Nuevo tema: %s\n", arreglo_buscar(config, "tema"));
    
    // Recorrer toda la configuracion
    printf("\nConfiguracion completa:\n");
    arreglo_recorrer(config, clave, valor) {
        printf("  %s: %s\n", clave, valor);
    }
    
    arreglo_liberar(config);
    return 0;
}
