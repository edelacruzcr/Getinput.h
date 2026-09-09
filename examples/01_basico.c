// ============================================================
// EJEMPLO 1: USO BÁSICO DE GET-INPUT.H
// ============================================================

#include <stdio.h>
#include "util.h"

int main(void) {
    printf("=== EJEMPLO DE ENTRADA BÁSICA ===\n\n");

    // Solicitar un entero simple
    int edad = obtener_entero("Ingresa tu edad: ");

    // Solicitar una cadena de texto
    char nombre[50];
    obtener_cadena("Ingresa tu nombre completo: ", nombre, sizeof(nombre));

    // Solicitar un número flotante
    float estatura = obtener_flotante("Ingresa tu estatura en metros (ej. 1.75): ");

    printf("\n--- DATOS REGISTRADOS ---\n");
    printf("Nombre:   %s\n", nombre);
    printf("Edad:     %d años\n", edad);
    printf("Estatura: %.2f m\n", estatura);

    return 0;
}
