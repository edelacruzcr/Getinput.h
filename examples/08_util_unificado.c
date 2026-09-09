#include <stdio.h>
#include "util.h"

int main(void) {
    printf("=== DEMOSTRACIÓN DE LA BIBLIOTECA UNIFICADA UTIL.H ===\n\n");

    // Crear un arreglo flexible
    Arreglo *perfil = arreglo_nuevo();

    // 1. Obtención de datos validados usando la sección get-input
    char nombre[100];
    obtener_cadena("Ingresa tu nombre completo: ", nombre, sizeof(nombre));
    int edad = obtener_entero_rango("Ingresa tu edad (0 a 120): ", 0, 120);
    double salario = obtener_flotante_config("Ingresa tu pretensión salarial ($): ", CONFIG_FLOTANTE_DEFAULT);

    // 2. Almacenar datos en la estructura Arreglo (sección arreglo)
    arreglo_guardar(perfil, "nombre", nombre);
    arreglo_guardar_int(perfil, "edad", edad);
    arreglo_guardar_float(perfil, "salario", salario);

    // 3. Mostrar reporte desde el Arreglo Flexible
    printf("\n--- REPORTE GUARDADO EN ARREGLO FLEXIBLE ---\n");
    printf("Nombre:  %s\n", arreglo_buscar(perfil, "nombre"));
    printf("Edad:    %d años\n", arreglo_buscar_int(perfil, "edad", NULL));
    printf("Salario: $%.2f\n", arreglo_buscar_float(perfil, "salario", NULL));

    // Iteración completa
    printf("\n--- RECORRIENDO CLAVES GUARDADAS ---\n");
    arreglo_recorrer(perfil, clave, valor) {
        printf("  [%s] -> %s\n", clave, valor);
    }

    // Liberación de memoria
    arreglo_liberar(perfil);
    printf("\n¡Prueba completada con éxito!\n");
    return 0;
}
