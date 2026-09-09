# arreglo.h - Guia Completa y Manual de Uso

## Introduccion

`arreglo.h` es una biblioteca header-only para C que implementa un **Arreglo Flexible / Tabla Hash** dinamica. Permite almacenar datos asociados a claves alfanumericas con acceso promedio O(1), redimensionamiento automatico y soporte para multiples tipos de datos (texto, enteros, decimales y booleanos).

---

## Caracteristicas Destacadas

- **Header-Only**: Un unico archivo `.h` listo para incluir en tu proyecto.
- **Sin Dependencias Externas**: Utiliza unicamente la biblioteca estandar de C (C99 o superior).
- **Crecimiento Automatico**: Duplica la capacidad interna cuando el factor de carga supera el 75%.
- **Dispersion Optimizada**: Implementa el algoritmo Hash FNV-1a de 64 bits para una distribucion uniforme de claves.
- **Multiples Tipos de Datos**: Soporte nativo para cadenas de texto, enteros (`int`), decimales (`double`) y booleanos (`bool`).
- **Operaciones Avanzadas**: Copia profunda de arreglos, fusion/mezcla de estructuras y ordenamiento por clave.

---

## Referencia de Funciones

### Creacion y Destruccion

#### `arreglo_nuevo()`
Crea un arreglo flexible con la capacidad inicial predeterminada (16 elementos).
- **Devuelve**: `Arreglo*` (Puntero a la nueva estructura o `NULL` en caso de falla de memoria).

#### `arreglo_nuevo_cap(size_t capacidad_inicial)`
Crea un arreglo con la capacidad inicial especificada.
- **Parametros**: `capacidad_inicial` - Numero inicial de casillas.
- **Devuelve**: `Arreglo*`

#### `arreglo_vaciar(Arreglo *a)`
Elimina todos los elementos del arreglo liberando su memoria interna, manteniendo la capacidad asignada.

#### `arreglo_liberar(Arreglo *a)`
Libera completamente la memoria del arreglo y todos sus elementos.

---

### Insercion y Modificacion

#### `arreglo_guardar(Arreglo *a, const char *clave, const char *valor)`
Guarda un texto asociado a la clave especificada. Si la clave ya existe, sustituye su valor.

#### `arreglo_guardar_int(Arreglo *a, const char *clave, int valor)`
Guarda un valor entero asociado a la clave.

#### `arreglo_guardar_float(Arreglo *a, const char *clave, double valor)`
Guarda un valor decimal (`double`) asociado a la clave.

#### `arreglo_guardar_bool(Arreglo *a, const char *clave, bool valor)`
Guarda un valor booleano (`true` o `false`) asociado a la clave.

---

### Consulta y Busqueda

#### `arreglo_buscar(Arreglo *a, const char *clave)`
Obtiene la cadena de texto asociada a la clave.
- **Devuelve**: `char*` con el valor o `NULL` si no existe.

#### `arreglo_buscar_int(Arreglo *a, const char *clave, bool *encontrado)`
Busca un valor entero por clave.
- **Parametros**: `encontrado` - Puntero a booleano que se establece en `true` si la clave existe.
- **Devuelve**: `int` con el valor o `0` si no se encuentra.

#### `arreglo_buscar_float(Arreglo *a, const char *clave, bool *encontrado)`
Busca un valor decimal por clave.
- **Devuelve**: `double` con el valor o `0.0`.

#### `arreglo_buscar_bool(Arreglo *a, const char *clave, bool *encontrado)`
Busca un valor booleano por clave.
- **Devuelve**: `bool` (`true` / `false`).

#### `arreglo_tiene(Arreglo *a, const char *clave)`
Comprueba si una clave existe en el arreglo.
- **Devuelve**: `bool` (`true` si la clave esta presente, `false` en caso contrario).

#### `arreglo_cuantos(Arreglo *a)`
Devuelve el numero de elementos actualmente guardados.
- **Devuelve**: `size_t`

---

### Borrado y Utilidades

#### `arreglo_borrar(Arreglo *a, const char *clave)`
Elimina la clave y su valor asociado del arreglo.
- **Devuelve**: `bool` (`true` si fue eliminado, `false` si no se encontro).

#### `arreglo_copiar(Arreglo *a)`
Crea una copia profunda e independiente del arreglo especificado.
- **Devuelve**: `Arreglo*`

#### `arreglo_mezclar(Arreglo *a, Arreglo *b)`
Combina dos arreglos en uno nuevo. Si existen claves duplicadas, prevalecen los valores de `b`.
- **Devuelve**: `Arreglo*`

#### `arreglo_claves(Arreglo *a, size_t *total)`
Devuelve un vector dinamico con todas las claves presentes en el arreglo.
- **Devuelve**: `char**` (Debe ser liberado con `free()` por el usuario).

#### `arreglo_valores(Arreglo *a, size_t *total)`
Devuelve un vector dinamico con todos los valores en formato texto.
- **Devuelve**: `char**` (Debe ser liberado con `free()` por el usuario).

#### `arreglo_ordenar_claves(Arreglo *a)`
Reordena internamente las claves del arreglo en orden alfabetico.

---

## Macro de Recorrido

### `arreglo_recorrer(a, clave_var, valor_var)`
Macro para iterar de manera sencilla sobre todos los pares clave-valor guardados.

```c
arreglo_recorrer(mi_arreglo, k, v) {
    printf("Clave: %s -> Valor: %s\n", k, v);
}
```

---

## Ejemplo Completo de Uso

```c
#include <stdio.h>
#include "arreglo.h"

int main(void) {
    Arreglo *persona = arreglo_nuevo();
    
    arreglo_guardar(persona, "nombre", "Carlos");
    arreglo_guardar_int(persona, "edad", 30);
    arreglo_guardar_bool(persona, "empleado", true);
    
    printf("Nombre: %s\n", arreglo_buscar(persona, "nombre"));
    
    bool ok;
    int edad = arreglo_buscar_int(persona, "edad", &ok);
    if (ok) printf("Edad: %d años\n", edad);
    
    arreglo_liberar(persona);
    return 0;
}
```
