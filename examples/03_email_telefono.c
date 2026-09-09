// ============================================================
// EJEMPLO 3: VALIDACIÓN DE CORREO Y TELÉFONO
// ============================================================

#include <stdio.h>
#include "util.h"

int main(void) {
    printf("=== REGISTRO DE CONTACTO ===\n\n");

    char correo[100];
    if (obtener_email("Ingresa tu correo electrónico: ", correo, sizeof(correo))) {
        printf("Correo válido aceptado: %s\n\n", correo);
    } else {
        printf("No se pudo obtener un correo válido.\n\n");
    }

    char telefono[20];
    if (obtener_telefono("Ingresa tu número telefónico (solo dígitos): ", telefono, sizeof(telefono))) {
        printf("Número telefónico aceptado: %s\n\n", telefono);
    } else {
        printf("No se pudo obtener un teléfono válido.\n\n");
    }

    return 0;
}
