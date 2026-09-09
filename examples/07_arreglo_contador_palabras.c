#include <stdio.h>
#include <string.h>
#include "arreglo.h"

int main(void) {
    char texto[] = "el sol brilla y el cielo es azul";
    Arreglo *frecuencias = arreglo_nuevo();
    
    // Contar ocurrencias de cada palabra
    char *palabra = strtok(texto, " ");
    while (palabra) {
        bool encontrado = false;
        int contador = arreglo_buscar_int(frecuencias, palabra, &encontrado);
        
        if (encontrado) {
            arreglo_guardar_int(frecuencias, palabra, contador + 1);
        } else {
            arreglo_guardar_int(frecuencias, palabra, 1);
        }
        
        palabra = strtok(NULL, " ");
    }
    
    // Mostrar frecuencia de palabras
    printf("Frecuencia de palabras:\n");
    arreglo_recorrer(frecuencias, p, veces) {
        printf("  %s -> %s veces\n", p, veces);
    }
    
    // Obtener claves ordenadas
    arreglo_ordenar_claves(frecuencias);
    printf("\nPalabras en orden alfabetico:\n");
    arreglo_recorrer(frecuencias, p_ord, veces_ord) {
        printf("  %s -> %s veces\n", p_ord, veces_ord);
    }
    
    arreglo_liberar(frecuencias);
    return 0;
}
