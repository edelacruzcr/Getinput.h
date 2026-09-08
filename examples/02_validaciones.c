// ============================================================
// EJEMPLO 2: VALIDACIONES DE RANGO, LONGITUD Y CONFIRMACIÓN
// ============================================================

#include <stdio.h>
#include "get-input.h"

int main(void) {
    printf("=== EJEMPLO DE VALIDACIONES ===\n\n");

    // Entero en un rango específico (0 a 10)
    int nota = obtener_entero_rango("Ingresa la calificación del estudiante (0 - 10): ", 0, 10);
    printf("Nota aceptada: %d\n\n", nota);

    // Cadena con longitud mínima requerida
    char clave[50];
    obtener_cadena_min("Crea una contraseña (mínimo 8 caracteres): ", clave, sizeof(clave), 8);
    printf("Contraseña guardada correctamente (longitud: %zu caracteres).\n\n", strlen(clave));

    // Carácter restringido a opciones
    char turno = obtener_caracter_opciones("Selecciona turno (M = Mañana, T = Tarde, N = Noche): ", "MTNmtn");
    printf("Turno seleccionado: %c\n\n", turno);

    // Confirmación Sí / No
    int aceptar = obtener_si_no("¿Deseas guardar los cambios realizados?");
    if (aceptar) {
        printf("[OK] Cambios guardados con éxito.\n");
    } else {
        printf("[CANCELADO] Operación cancelada por el usuario.\n");
    }

    return 0;
}
