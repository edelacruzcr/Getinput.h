# SECCIÓN DE ARREGLOS FLEXIBLES Y TABLAS HASH EN `util.h`

> **Nota de actualización**: La funcionalidad de arreglos flexibles previamente documentada en `arreglo.h` ahora se encuentra 100% integrada dentro de **`util.h`**.

---

## Características Principales

- **Header-Only**: Incluido directamente en `util.h`.
- **Complejidad O(1)**: Búsquedas e inserciones ultrarrápidas mediante el algoritmo Hash FNV-1a de 64 bits.
- **Crecimiento Automático**: Duplica su capacidad cuando el factor de carga supera el 75% (`0.75`).
- **Soporte Multitipo**: Texto (`char*`), Enteros (`int`), Decimales (`double`) y Booleanos (`bool`).

---

## Funciones Principales y Especificación

### 1. `arreglo_nuevo()`
- **¿Qué hace?**: Crea un nuevo arreglo dinámico con capacidad inicial de 16 elementos.
- **¿Qué recibe?**: `void`
- **¿Qué devuelve?**: `Arreglo*` (puntero a la estructura o `NULL` si falla memoria).

### 2. `arreglo_guardar(a, clave, valor)`
- **¿Qué hace?**: Asocia un valor de texto a una clave alfanumérica.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `const char *valor`
- **¿Qué devuelve?**: `void`

### 3. `arreglo_guardar_int(a, clave, valor)`
- **¿Qué hace?**: Asocia un número entero a una clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `int valor`
- **¿Qué devuelve?**: `void`

### 4. `arreglo_guardar_float(a, clave, valor)`
- **¿Qué hace?**: Asocia un decimal a una clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `double valor`
- **¿Qué devuelve?**: `void`

### 5. `arreglo_guardar_bool(a, clave, valor)`
- **¿Qué hace?**: Asocia un booleano (`true`/`false`) a una clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool valor`
- **¿Qué devuelve?**: `void`

### 6. `arreglo_buscar(a, clave)`
- **¿Qué hace?**: Recupera el valor textual por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`
- **¿Qué devuelve?**: `char*` o `NULL` si la clave no existe.

### 7. `arreglo_buscar_int(a, clave, encontrado)`
- **¿Qué hace?**: Recupera un número entero por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool *encontrado`
- **¿Qué devuelve?**: `int` (escribe `true`/`false` en `*encontrado` si se pasa el puntero).

### 8. `arreglo_buscar_float(a, clave, encontrado)`
- **¿Qué hace?**: Recupera un número decimal por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool *encontrado`
- **¿Qué devuelve?**: `double`.

### 9. `arreglo_buscar_bool(a, clave, encontrado)`
- **¿Qué hace?**: Recupera un valor booleano por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool *encontrado`
- **¿Qué devuelve?**: `bool`.

### 10. `arreglo_borrar(a, clave)`
- **¿Qué hace?**: Elimina un elemento por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`
- **¿Qué devuelve?**: `bool` (`true` si fue eliminado).

### 11. `arreglo_liberar(a)`
- **¿Qué hace?**: Libera toda la memoria asignada al arreglo.
- **¿Qué recibe?**: `Arreglo *a`
- **¿Qué devuelve?**: `void`
