// ============================================================
// EJEMPLO 4: MENÚ INTERACTIVO PARA CONSOLA
// ============================================================

#include <stdio.h>
#include "get-input.h"

int main(void) {
    int ejecucion = 1;

    while (ejecucion) {
        printf("\n====================================\n");
        printf("        SISTEMA DE GESTIÓN          \n");
        printf("====================================\n");
        printf("  1. Registrar usuario              \n");
        printf("  2. Consultar catálogo             \n");
        printf("  3. Ajustes de configuración       \n");
        printf("  4. Salir del programa             \n");
        printf("====================================\n");

        int opcion = obtener_opcion_menu("Selecciona una opción (1-4): ", 1, 4);

        switch (opcion) {
            case 1:
                printf("\n[PROCESO] Iniciando módulo de registro...\n");
                break;
            case 2:
                printf("\n[PROCESO] Cargando lista de catálogo...\n");
                break;
            case 3:
                printf("\n[PROCESO] Abriendo panel de ajustes...\n");
                break;
            case 4:
                printf("\nSaliendo del sistema...\n");
                ejecucion = 0;
                break;
            default:
                break;
        }
    }

    return 0;
}
