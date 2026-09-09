# Biblioteca Header-Only para C (`util.h`)

[![Lenguaje](https://img.shields.io/badge/Lenguaje-C99%20%2F%20C11%20%2F%20C17%20%2F%20C23-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Licencia](https://img.shields.io/badge/Licencia-MIT-green.svg)](LICENSE)
[![Versión](https://img.shields.io/badge/Versión-2.0.0-orange.svg)](util.h)
[![Tipo](https://img.shields.io/badge/Tipo-Header--Only-purple.svg)](util.h)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](Makefile)

Biblioteca **header-only** de alto rendimiento en C que reúne en un solo archivo (**`util.h`**):

1. **Entrada de Datos Consola**: Validación de números, cadenas, correos, teléfonos y menús interactivos.
2. **Arreglo Flexible / Hash Table**: Tabla hash O(1) con crecimiento automático y soporte multitipo.

---

## ⚡ Guía de Instalación

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

## 🗑️ Migración desde Versiones Anteriores (`get-input.h` y `arreglo.h`)

Si tenías instalada la versión previa de dos archivos, ejecuta el siguiente comando para **borrar los archivos antiguos**:

- **En el sistema (`/usr/local/include`)**:
  ```bash
  sudo rm -f /usr/local/include/get-input.h /usr/local/include/arreglo.h
  ```
- **En la carpeta de tu proyecto**:
  ```bash
  rm -f get-input.h arreglo.h
  ```

> **Nota**: El instalador (`install.sh`) detecta y elimina automáticamente las cabeceras antiguas `get-input.h` y `arreglo.h`.

---

## 🚀 Ejemplos de Uso

Todos los ejemplos se encuentran disponibles en la carpeta [`examples/`](examples) del repositorio y están listos para compilar con `make`.

### Índice de Ejemplos y Archivos Fuente

| N° | Ejemplo | Descripción | Archivo Fuente |
|---|---|---|---|
| **01** | **Entrada Básica** | Lectura limpia de enteros, flotantes y cadenas sin buffer residual | [`01_basico.c`](examples/01_basico.c) |
| **02** | **Validaciones Avanzadas** | Rangos de números, longitud mínima de clave y confirmaciones | [`02_validaciones.c`](examples/02_validaciones.c) |
| **03** | **Email y Teléfono** | Validación automática de formato de correo y número telefónico | [`03_email_telefono.c`](examples/03_email_telefono.c) |
| **04** | **Menú Interactivo** | Creación rápida de menús de consola seguros frente a entradas inválidas | [`04_menu_interactivo.c`](examples/04_menu_interactivo.c) |
| **05** | **Arreglo / Hash Table** | Creación, inserción multitipo, consulta, recorrido y clonación | [`05_arreglo_basico.c`](examples/05_arreglo_basico.c) |
| **06** | **Configuración Sistema** | Almacenamiento y actualización de ajustes tipo clave-valor | [`06_arreglo_configuracion.c`](examples/06_arreglo_configuracion.c) |
| **07** | **Contador & Ordenamiento** | Conteo de frecuencia de palabras y ordenamiento alfabético de claves | [`07_arreglo_contador_palabras.c`](examples/07_arreglo_contador_palabras.c) |
| **08** | **Uso Unificado Integrado** | Captura de datos de consola y guardado en estructura hash O(1) | [`08_util_unificado.c`](examples/08_util_unificado.c) |

---

### Fragmentos de Código de los Casos Más Frecuentes

#### 1. Entrada de Datos por Consola (`obtener_*`)

```c
#include <stdio.h>
#include "util.h"

int main(void) {
    // Lectura simple
    int edad = obtener_entero("Ingresa tu edad: ");
    char nombre[50];
    obtener_cadena("Ingresa tu nombre completo: ", nombre, sizeof(nombre));
    float estatura = obtener_flotante("Ingresa tu estatura (m): ");

    // Validaciones avanzadas
    int nota = obtener_entero_rango("Calificación (0-10): ", 0, 10);
    char correo[100];
    if (obtener_email("Correo electrónico: ", correo, sizeof(correo))) {
        printf("Email guardado: %s\n", correo);
    }
    
    int aceptar = obtener_si_no("¿Deseas guardar los cambios?");
    return 0;
}
```

#### 2. Arreglo Flexible / Tabla Hash O(1) (`arreglo_*`)

```c
#include <stdio.h>
#include "util.h"

int main(void) {
    Arreglo *a = arreglo_nuevo();
    
    // Guardar múltiples tipos por clave
    arreglo_guardar(a, "nombre", "Juan");
    arreglo_guardar_int(a, "edad", 25);
    arreglo_guardar_float(a, "altura", 1.75);
    arreglo_guardar_bool(a, "activo", true);
    
    // Búsqueda
    bool ok = false;
    int edad = arreglo_buscar_int(a, "edad", &ok);
    if (ok) printf("Edad: %d años\n", edad);
    
    // Recorrido clave-valor
    arreglo_recorrer(a, clave, valor) {
        printf("  %s -> %s\n", clave, valor);
    }
    
    arreglo_liberar(a);
    return 0;
}
```

#### 3. Menú Interactivo Seguro

```c
#include <stdio.h>
#include "util.h"

int main(void) {
    while (1) {
        printf("\n--- MENU DE OPCIONES ---\n1. Continuar\n2. Salir\n");
        int opcion = obtener_opcion_menu("Selecciona una opción (1-2): ", 1, 2);
        if (opcion == 2) break;
        printf("Ejecutando acción...\n");
    }
    return 0;
}
```

#### 4. Ejemplo Integrado (Entrada + Tabla Hash)

```c
#include <stdio.h>
#include "util.h"

int main(void) {
    Arreglo *perfil = arreglo_nuevo();

    // Lectura de datos validados
    char nombre[100];
    obtener_cadena("Nombre completo: ", nombre, sizeof(nombre));
    int edad = obtener_entero_rango("Edad (0-120): ", 0, 120);

    // Almacenamiento en la Tabla Hash
    arreglo_guardar(perfil, "nombre", nombre);
    arreglo_guardar_int(perfil, "edad", edad);

    // Mostrar datos almacenados
    printf("Nombre: %s, Edad: %d\n", 
            arreglo_buscar(perfil, "nombre"), 
            arreglo_buscar_int(perfil, "edad", NULL));

    arreglo_liberar(perfil);
    return 0;
}
```

---

## 🛠️ Compilación y Ejecución de Ejemplos

Puedes compilar y ejecutar todos los ejemplos fácilmente mediante el `Makefile` incluido:

```bash
# Compilar todos los ejemplos
make clean && make

# Ejecutar un ejemplo individual
./bin/01_basico
./bin/04_menu_interactivo
./bin/05_arreglo_basico
./bin/08_util_unificado
```

---

## 📘 Referencia de la API (`util.h`)

### 1. Módulo de Arreglos Flexibles y Tablas Hash O(1)

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
| `arreglo_valores(a, &n)` | Obtiene todos los valores | Arreglo, puntero | `char**` | `char **v = arreglo_valores(a, &v);` |
| `arreglo_vaciar(a)` | Elimina todo el contenido | Arreglo | `void` | `arreglo_vaciar(a);` |
| `arreglo_liberar(a)` | Libera toda la memoria | Arreglo | `void` | `arreglo_liberar(a);` |
| `arreglo_copiar(a)` | Copia el arreglo | Arreglo | `Arreglo*` | `Arreglo *b = arreglo_copiar(a);` |
| `arreglo_mezclar(a, b)` | Une dos arreglos | Arreglo, Arreglo | `Arreglo*` | `Arreglo *c = arreglo_mezclar(a, b);` |
| `arreglo_recorrer(a, k, v)` | Macro de iteración | Arreglo, vars | Loop | `arreglo_recorrer(a, k, v) { ... }` |
| `arreglo_ordenar_claves(a)` | Ordena por clave | Arreglo | `void` | `arreglo_ordenar_claves(a);` |

---

### 2. Módulo de Entrada de Datos y Validación (`stdin`)

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

## 📄 Documentación Adicional

Para más detalles sobre la arquitectura interna y la guía de uso paso a paso:
- 📖 [Guía de Uso Completa](docs/guia_de_uso.md)
- 🛠️ [Manual Técnico y Arquitectura](docs/manual.md)

---

## Licencia

Este proyecto se distribuye bajo la [Licencia MIT](LICENSE). Libre para uso personal, académico y comercial.

