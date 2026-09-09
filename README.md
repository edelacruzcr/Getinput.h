# Biblioteca Header-Only para C (`util.h`)

[![Lenguaje](https://img.shields.io/badge/Lenguaje-C99%20%2F%20C11%20%2F%20C17%20%2F%20C23-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Licencia](https://img.shields.io/badge/Licencia-MIT-green.svg)](LICENSE)
[![Versión](https://img.shields.io/badge/Versión-2.0.0-orange.svg)](util.h)
[![Tipo](https://img.shields.io/badge/Tipo-Header--Only-purple.svg)](util.h)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](Makefile)

Biblioteca **header-only** de alto rendimiento en C que reúne en un solo archivo (**`util.h`**):

1. **Entrada de Datos Consola**: Validación de números, cadenas, correos, teléfonos y menú interactivo.
2. **Arreglo Flexible / Hash Table**: Tabla hash O(1) con crecimiento automático y soporte multitipo.

---

## Guía de Instalación

### Opción 1: Instalación Automática (Recomendada)
Para instalar o actualizar `util.h` en tu sistema (`/usr/local/include`):

```bash
curl -fsSL https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/install.sh | bash
```

### Opción 2: Descarga Directa (1 solo archivo)
Si prefieres no usar el instalador y tener el archivo directamente en la carpeta de tu proyecto:

```bash
curl -fsSL https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/util.h -o util.h
```

---

## 🗑️ Eliminar Versión Anterior (`get-input.h` y `arreglo.h`)

Si tenías instalada la versión previa de dos archivos, ejecuta el siguiente comando para **borrar los archivos antiguos** del sistema:

### Si los instalaste globalmente en el sistema:
```bash
sudo rm -f /usr/local/include/get-input.h /usr/local/include/arreglo.h
```

### Si los tenías descargados localmente en tu carpeta de proyecto:
```bash
rm -f get-input.h arreglo.h
```

> **Nota**: El nuevo instalador (`install.sh`) detecta y elimina automáticamente las cabeceras antiguas `get-input.h` y `arreglo.h` si estuvieran presentes en tu sistema.

---

## Ejemplo de Uso

Una vez instalado, únicamente necesitas incluir `<util.h>` en tu código de C:

```c
#include <util.h>

int main(void) {
    Arreglo *datos = arreglo_nuevo();
    int edad = obtener_entero_rango("Ingresa tu edad (0-120): ", 0, 120);
    arreglo_guardar_int(datos, "edad", edad);
    
    printf("Edad guardada: %d anos\n", arreglo_buscar_int(datos, "edad", NULL));
    arreglo_liberar(datos);
    return 0;
}
```

---

# 1. arreglo.h - Arreglo Flexible Ultra Mejorado para C

**Versión: 2.0.0 | Licencia: MIT | Header-Only | Máximo Rendimiento**

## ¿Qué es?

Un **Arreglo Flexible** es una estructura de datos que combina arreglos dinámicos y diccionarios (tabla hash). Permite disponer de una lista de crecimiento automático y búsquedas ultrarrápidas asociadas a claves alfanuméricas.

### Características Principales

- **Crecimiento automático**: Redimensionamiento automático según el factor de carga (0.75).
- **Búsqueda ultrarrápida**: Tabla hash con algoritmo FNV-1a y complejidad O(1) promedio.
- **Múltiples tipos de datos**: Compatible con texto, enteros, decimales y booleanos.
- **Iteración sencilla**: Macro `arreglo_recorrer()` para iterar pares clave-valor.
- **Seguro**: Verificación de punteros nulos y gestión transparente de memoria.
- **Operaciones avanzadas**: Copia profunda, mezcla de arreglos y ordenamiento de claves.

---

## Tabla de Funciones de `arreglo.h`

| Función | ¿Qué hace? | ¿Qué recibe? | ¿Qué devuelve? | Ejemplo |
|---------|------------|--------------|----------------|---------|
| `arreglo_nuevo()` | Crea un arreglo vacío | Nada | `Arreglo*` | `Arreglo *a = arreglo_nuevo();` |
| `arreglo_nuevo_cap(n)` | Crea con capacidad inicial | Capacidad | `Arreglo*` | `Arreglo *a = arreglo_nuevo_cap(100);` |
| `arreglo_guardar(a, k, v)` | Guarda texto con clave | Arreglo, clave, valor | `void` | `arreglo_guardar(a, "nombre", "Juan");` |
| `arreglo_guardar_int(a, k, n)` | Guarda número entero | Arreglo, clave, entero | `void` | `arreglo_guardar_int(a, "edad", 25);` |
| `arreglo_guardar_float(a, k, n)` | Guarda decimal | Arreglo, clave, decimal | `void` | `arreglo_guardar_float(a, "pi", 3.14);` |
| `arreglo_guardar_bool(a, k, b)` | Guarda booleano | Arreglo, clave, bool | `void` | `arreglo_guardar_bool(a, "ok", true);` |
| `arreglo_buscar(a, k)` | Busca texto por clave | Arreglo, clave | `char*` o `NULL` | `char *v = arreglo_buscar(a, "nombre");` |
| `arreglo_buscar_int(a, k, &ok)` | Busca entero por clave | Arreglo, clave, puntero | `int` | `int edad = arreglo_buscar_int(a, "edad", &ok);` |
| `arreglo_buscar_float(a, k, &ok)` | Busca decimal por clave | Arreglo, clave, puntero | `double` | `double h = arreglo_buscar_float(a, "h", &ok);` |
| `arreglo_buscar_bool(a, k, &ok)` | Busca booleano | Arreglo, clave, puntero | `bool` | `bool act = arreglo_buscar_bool(a, "a", &ok);` |
| `arreglo_tiene(a, k)` | Verifica existencia | Arreglo, clave | `bool` | `if (arreglo_tiene(a, "edad"))` |
| `arreglo_borrar(a, k)` | Elimina elemento | Arreglo, clave | `bool` | `arreglo_borrar(a, "edad");` |
| `arreglo_cuantos(a)` | Número de elementos | Arreglo | `size_t` | `size_t n = arreglo_cuantos(a);` |
| `arreglo_claves(a, &n)` | Obtiene todas las claves | Arreglo, puntero | `char**` | `char **k = arreglo_claves(a, &n);` |
| `arreglo_valores(a, &n)` | Obtiene todos los valores | Arreglo, puntero | `char**` | `char **v = arreglo_valores(a, &n);` |
| `arreglo_vaciar(a)` | Elimina todo el contenido | Arreglo | `void` | `arreglo_vaciar(a);` |
| `arreglo_liberar(a)` | Libera toda la memoria | Arreglo | `void` | `arreglo_liberar(a);` |
| `arreglo_copiar(a)` | Copia el arreglo | Arreglo | `Arreglo*` | `Arreglo *b = arreglo_copiar(a);` |
| `arreglo_mezclar(a, b)` | Une dos arreglos | Arreglo, Arreglo | `Arreglo*` | `Arreglo *c = arreglo_mezclar(a, b);` |
| `arreglo_recorrer(a, k, v)` | Macro de iteración | Arreglo, vars | Loop | `arreglo_recorrer(a, k, v) { ... }` |
| `arreglo_ordenar_claves(a)` | Ordena por clave | Arreglo | `void` | `arreglo_ordenar_claves(a);` |

---

## Rendimiento de `arreglo.h`

| Operación | Complejidad | Velocidad |
|-----------|-------------|-----------|
| Insertar | O(1) promedio | Ultrarrápido |
| Buscar | O(1) promedio | Ultrarrápido |
| Eliminar | O(1) promedio | Ultrarrápido |
| Recorrer | O(n) | Rápido |
| Copiar | O(n) | Rápido |
| Mezclar | O(n+m) | Rápido |

---

## Ejemplo Completo de `arreglo.h`

```c
#include <stdio.h>
#include "arreglo.h"

int main(void) {
    // 1. Crear
    Arreglo *a = arreglo_nuevo();
    
    // 2. Guardar
    arreglo_guardar(a, "nombre", "Juan");
    arreglo_guardar_int(a, "edad", 25);
    arreglo_guardar_float(a, "altura", 1.75);
    arreglo_guardar_bool(a, "activo", true);
    
    // 3. Buscar
    printf("Nombre: %s\n", arreglo_buscar(a, "nombre"));
    
    bool ok;
    int edad = arreglo_buscar_int(a, "edad", &ok);
    if (ok) printf("Edad: %d años\n", edad);
    
    // 4. Recorrer
    printf("\nElementos guardados:\n");
    arreglo_recorrer(a, clave, valor) {
        printf("  %s -> %s\n", clave, valor);
    }
    
    // 5. Liberar
    arreglo_liberar(a);
    return 0;
}
```

---

# 2. get-input.h - Entrada Segura y Robusta para C

**Versión: 2.0.0 | Licencia: MIT | Header-Only**

## ¿Qué es?

`get-input.h` simplifica la lectura de datos por consola (`stdin`), evitando errores típicos de `scanf()`, desbordamientos de buffer y bucles infinitos por entradas inválidas.

### Solución

```c
// Código C tradicional propenso a errores:
int edad;
if (scanf("%d", &edad) != 1) { while(getchar() != '\n'); }

// Con get-input.h:
int edad = obtener_entero_rango("Edad: ", 0, 120);
```

---

## Tabla de Funciones de `get-input.h`

| Función | Parámetros | Retorna | Descripción |
|---------|------------|---------|-------------|
| `obtener_caracter` | `mensaje` | `char` | Lee un carácter |
| `obtener_caracter_opciones` | `mensaje, opciones` | `char` | Lee carácter de opciones permitidas |
| `obtener_entero` | `mensaje` | `int` | Lee un entero |
| `obtener_entero_rango` | `mensaje, min, max` | `int` | Lee entero en rango |
| `obtener_flotante` | `mensaje` | `float` | Lee un flotante |
| `obtener_flotante_rango` | `mensaje, min, max` | `float` | Lee flotante en rango |
| `obtener_cadena` | `mensaje, buffer, tamaño` | `void` | Lee cadena de texto segura |
| `obtener_cadena_min` | `mensaje, buffer, tamaño, min` | `int` | Lee cadena con longitud mínima |
| `obtener_si_no` | `mensaje` | `int` | Confirmación Sí/No (1/0) |
| `obtener_email` | `mensaje, buffer, tamaño` | `int` | Lee y valida correo electrónico |
| `obtener_telefono` | `mensaje, buffer, tamaño` | `int` | Lee y valida teléfono numérico |
| `obtener_opcion_menu` | `mensaje, min, max` | `int` | Lee opción válida de menú |

---

## Métodos de Instalación y Compilación

### Opción 1: Script de instalación local
```bash
git clone https://github.com/edelacruzcr/Getinput.h.git
cd Getinput.h
./install.sh
```

### Opción 2: Compilar ejemplos con Makefile
```bash
make
./bin/01_basico
./bin/05_arreglo_basico
./bin/06_arreglo_configuracion
./bin/07_arreglo_contador_palabras
```

### Opción 3: CMake
```bash
mkdir build && cd build
cmake ..
sudo make install
```

---

## Licencia

Este proyecto se distribuye bajo la [Licencia MIT](LICENSE). Libre para uso personal, académico y comercial.
