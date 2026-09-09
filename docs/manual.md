# MANUAL TÉCNICO Y ARQUITECTURA DE `util.h`

[![Versión](https://img.shields.io/badge/Versión-2.0.0-orange.svg)](util.h)
[![Tamaño](https://img.shields.io/badge/Peso-28.9%20KB%20(28%2C907%20bytes)-blue.svg)](util.h)
[![Líneas](https://img.shields.io/badge/Líneas-686-green.svg)](util.h)

---

## 1. Detalles Técnicos y Peso del Archivo

- **Ruta del archivo**: `util.h`
- **Tamaño exacto en disco**: **28,907 bytes** (28.9 KB)
- **Número de líneas de código**: **686 líneas**
- **Estándar de C**: C99 / C11 / C17 / C23
- **Sin dependencias externas**: Requiere únicamente cabeceras estándar de C (`stdio.h`, `stdlib.h`, `string.h`, `stdbool.h`, `stdint.h`, `ctype.h`, `limits.h`, `errno.h`).

---

## 2. Arquitectura Interna

### 2.1 Patrón Header-Only (`static inline`)
Todas las funciones públicas e internas están cualificadas con `static inline`:
1. Evita errores de símbolos duplicados (*linker duplicate symbols*) al incluir `util.h` en múltiples archivos de compilación (`.c`).
2. Permite al compilador realizar optimizaciones agresivas (*inlining*) eliminando la latencia del salto de pila de función.

### 2.2 Higiene de Buffer y Gestión de `stdin`
- **`_limpiar_buffer(void)`**: Vacía el flujo de entrada `stdin` hasta encontrar un salto de línea `\n` o `EOF`.
- **Prevención de desbordamientos**: Uso estricto de `fgets()`, `strtod()`, `strtol()` y macros seguras en lugar de `scanf("%s")` inseguro.

### 2.3 Estructura del Arreglo Flexible (Tabla Hash)
- **Algoritmo de Dispersión**: FNV-1a de 64 bits para distribución uniforme de claves.
- **Resolución de Colisiones**: Redireccionamiento abierto con sondeo lineal (*linear probing*).
- **Factor de Carga**: 0.75 (redimensiona automáticamente al doble de capacidad al superar el 75% de ocupación).
- **Eliminación Segura**: Implementa el algoritmo de eliminación con desplazamiento lineal retroactivo para no romper las cadenas de sondeo existente.

---

## 3. Tabla Resumen de Funciones

### Entrada de Datos (`stdin`)

| Función | ¿Qué hace? | Parámetros | Retorno |
| :--- | :--- | :--- | :--- |
| `obtener_entero` | Lee un entero | `mensaje` | `int` |
| `obtener_entero_rango` | Lee entero entre min y max | `mensaje`, `min`, `max` | `int` |
| `obtener_flotante` | Lee un flotante | `mensaje` | `float` |
| `obtener_flotante_rango` | Lee flotante entre min y max | `mensaje`, `min`, `max` | `float` |
| `obtener_cadena` | Lee una cadena de texto | `mensaje`, `buffer`, `tamanio` | `void` |
| `obtener_cadena_min` | Lee cadena con min de caracteres | `mensaje`, `buffer`, `tamanio`, `min` | `int` (1/0) |
| `obtener_caracter` | Lee un carácter | `mensaje` | `char` |
| `obtener_caracter_opciones` | Lee carácter restringido | `mensaje`, `opciones` | `char` |
| `obtener_si_no` | Pregunta confirmación (s/n) | `mensaje` | `int` (1/0) |
| `obtener_email` | Lee y valida correo electrónico | `mensaje`, `buffer`, `tamanio` | `int` (1/0) |
| `obtener_telefono` | Lee y valida teléfono (dígitos) | `mensaje`, `buffer`, `tamanio` | `int` (1/0) |
| `obtener_opcion_menu` | Lee opción de menú | `mensaje`, `min`, `max` | `int` |

### Arreglo Flexible (Tabla Hash O(1))

| Función | ¿Qué hace? | Parámetros | Retorno |
| :--- | :--- | :--- | :--- |
| `arreglo_nuevo` | Crea un arreglo flexible nuevo | Ninguno | `Arreglo*` |
| `arreglo_nuevo_cap` | Crea arreglo con capacidad inicial | `capacidad_inicial` | `Arreglo*` |
| `arreglo_guardar` | Guarda un texto por clave | `a`, `clave`, `valor` | `void` |
| `arreglo_guardar_int` | Guarda un entero por clave | `a`, `clave`, `valor` | `void` |
| `arreglo_guardar_float` | Guarda un decimal por clave | `a`, `clave`, `valor` | `void` |
| `arreglo_guardar_bool` | Guarda un booleano por clave | `a`, `clave`, `valor` | `void` |
| `arreglo_buscar` | Busca texto por clave | `a`, `clave` | `char*` |
| `arreglo_buscar_int` | Busca entero por clave | `a`, `clave`, `&encontrado` | `int` |
| `arreglo_buscar_float` | Busca decimal por clave | `a`, `clave`, `&encontrado` | `double` |
| `arreglo_buscar_bool` | Busca booleano por clave | `a`, `clave`, `&encontrado` | `bool` |
| `arreglo_tiene` | Verifica si existe una clave | `a`, `clave` | `bool` |
| `arreglo_borrar` | Elimina elemento por clave | `a`, `clave` | `bool` |
| `arreglo_cuantos` | Número total de elementos | `a` | `size_t` |
| `arreglo_claves` | Lista de todas las claves | `a`, `&total` | `char**` |
| `arreglo_valores` | Lista de todos los valores | `a`, `&total` | `char**` |
| `arreglo_vaciar` | Elimina elementos (mantiene capacidad) | `a` | `void` |
| `arreglo_liberar` | Libera toda la memoria ocupada | `a` | `void` |
| `arreglo_copiar` | Crea una copia profunda del arreglo | `a` | `Arreglo*` |
| `arreglo_mezclar` | Une dos arreglos en uno nuevo | `a`, `b` | `Arreglo*` |
| `arreglo_ordenar_claves` | Ordena alfabéticamente por clave | `a` | `void` |
