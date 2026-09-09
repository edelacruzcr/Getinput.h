#include <stdio.h>
#include "arreglo.h"

int main(void) {
    printf("=== DEMO DE ARREGLO.H v2.0 ===\n\n");
    
    // 1. Crear arreglo
    Arreglo *a = arreglo_nuevo();
    
    // 2. Guardar diferentes tipos
    arreglo_guardar(a, "nombre", "Juan");
    arreglo_guardar_int(a, "edad", 25);
    arreglo_guardar_float(a, "altura", 1.75);
    arreglo_guardar_bool(a, "activo", true);
    
    // 3. Buscar valores
    printf("1. Valores guardados:\n");
    printf("   nombre: %s\n", arreglo_buscar(a, "nombre"));
    
    bool encontrado = false;
    int edad = arreglo_buscar_int(a, "edad", &encontrado);
    if (encontrado) printf("   edad: %d\n", edad);
    
    double altura = arreglo_buscar_float(a, "altura", &encontrado);
    if (encontrado) printf("   altura: %.2f\n", altura);
    
    // 4. Verificar existencia
    printf("\n2. Verificacion:\n");
    printf("   Tiene 'nombre'? %s\n", arreglo_tiene(a, "nombre") ? "si" : "no");
    printf("   Tiene 'email'? %s\n", arreglo_tiene(a, "email") ? "si" : "no");
    
    // 5. Contar elementos
    printf("\n3. Total: %zu elementos\n", arreglo_cuantos(a));
    
    // 6. Recorrer
    printf("\n4. Recorrido:\n");
    arreglo_recorrer(a, clave, valor) {
        printf("   %s -> %s\n", clave, valor);
    }
    
    // 7. Copiar
    Arreglo *copia = arreglo_copiar(a);
    printf("\n5. Copia creada: %zu elementos\n", arreglo_cuantos(copia));
    arreglo_liberar(copia);
    
    // 8. Mezclar
    Arreglo *b = arreglo_nuevo();
    arreglo_guardar(b, "email", "juan@email.com");
    Arreglo *mezcla = arreglo_mezclar(a, b);
    printf("6. Mezcla: %zu elementos\n", arreglo_cuantos(mezcla));
    arreglo_liberar(mezcla);
    arreglo_liberar(b);
    
    // 9. Liberar memoria
    arreglo_liberar(a);
    
    return 0;
}
