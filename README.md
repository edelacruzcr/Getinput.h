# 🛡️ get-input.h - Biblioteca de Entrada Segura y Robusta para C

[![Lenguaje](https://img.shields.io/badge/Lenguaje-C99%20%2F%20C11%20%2F%20C17%20%2F%20C23-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Licencia](https://img.shields.io/badge/Licencia-MIT-green.svg)](LICENSE)
[![Versión](https://img.shields.io/badge/Versión-2.0.0-orange.svg)](get-input.h)
[![Tipo](https://img.shields.io/badge/Tipo-Header--Only-purple.svg)](get-input.h)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](Makefile)

**`get-input.h`** es una biblioteca *Header-Only* liviana, moderna y segura para lenguaje C. Soluciona de manera definitiva los problemas tradicionales al solicitar datos por consola (`stdin`), eliminando desbordamientos de memoria (*Buffer Overflow*), bucles infinitos por entradas inválidas y caracteres residuales en la consola.

---

## 📋 Tabla de Contenidos
1. [Introducción y Comparativa](#1-introducción-y-comparativa)
2. [Instalación e Inclusión](#2-instalación-e-inclusión)
3. [Compilación](#3-compilación)
4. [Tipos de Datos y Estructuras de Configuración](#4-tipos-de-datos-y-estructuras-de-configuración)
5. [Análisis de Elementos Opcionales vs. Obligatorios](#5-análisis-de-elementos-opcionales-vs-obligatorios)
6. [Funcionamiento Interno e Higiene de Buffer](#6-funcionamiento-interno-e-higiene-de-buffer)
7. [Catálogo Completo de Funciones](#7-catálogo-completo-de-funciones)
   - [Funciones de Caracteres](#71-funciones-de-caracteres)
   - [Funciones de Enteros](#72-funciones-de-enteros)
   - [Funciones de Flotantes](#73-funciones-de-flotantes)
   - [Funciones de Cadenas de Texto](#74-funciones-de-cadenas-de-texto)
   - [Funciones de Confirmación y Dominio (Email/Teléfono/Menú)](#75-funciones-de-confirmación-y-dominio-emailteléfonomenú)
   - [Funciones de Validación Manual](#76-funciones-de-validación-manual)
8. [Macros Útiles](#8-macros-útiles)
9. [Ejemplos Prácticos Completos](#9-ejemplos-prácticos-completos)
10. [Tabla de Referencia Rápida](#10-tabla-de-referencia-rápida)
11. [Licencia](#11-licencia)

---

## 1. Introducción y Comparativa

### ¿Por qué usar `get-input.h`?

En C tradicional, leer datos por teclado suele requerir código repetitivo para vaciar `stdin`, validar tipos e intentar de nuevo cuando el usuario ingresa texto inválido.

```c
// ❌ C TRADICIONAL - Código verbose, propenso a desbordamientos y bucles infinitos
int edad;
do {
    printf("Ingresa tu edad (0-120): ");
    if (scanf("%d", &edad) != 1) {
        while (getchar() != '\n'); // Limpiar buffer manualmente
        printf(" Error: Entrada no numérica.\n");
        continue;
    }
} while (edad < 0 || edad > 120);

// ✅ CON get-input.h - Legible, seguro, robusto y validado en una sola línea
int edad = obtener_entero_rango("Ingresa tu edad (0-120): ", 0, 120);
```

---

## 2. Instalación e Inclusión

Dado que `get-input.h` es una biblioteca **Header-Only** (consta de un único archivo de cabecera C), **no requiere instalación previa ni enlace de bibliotecas dinámicas**.

### Paso 1: Clonar o Descargar

```bash
git clone https://github.com/edelacruzcr/Getinput.h.git
```

O simplemente copia el archivo [`get-input.h`](get-input.h) en el directorio de tu proyecto.

### Paso 2: Incluir en tu archivo C

```c
#include <stdio.h>
#include "get-input.h" // Incluir la biblioteca

int main(void) {
    int edad = obtener_entero("Ingresa tu edad: ");
    printf("Tienes %d años.\n", edad);
    return 0;
}
```

---

## 3. Compilación

Puedes compilar tus programas con cualquier compilador estándar de C (**C99, C11, C17 o C23**) como GCC, Clang o MSVC:

### Uso con GCC / Clang
```bash
gcc -Wall -Wextra -std=c99 main.c -o programa
./programa
```

### Compilar los Ejemplos del Repositorio
El repositorio incluye un `Makefile` para compilar automáticamente todos los ejemplos incluidos en `examples/`:

```bash
make
./bin/01_basico
./bin/02_validaciones
./bin/03_email_telefono
./bin/04_menu_interactivo
```

---

## 4. Tipos de Datos y Estructuras de Configuración

Para configuraciones avanzadas (límites de reintentos, visibilidad de errores o intervalos específicos), la biblioteca expone 4 estructuras:

### 4.1 `ConfigEntero`
```c
typedef struct {
    int min;           // Valor entero mínimo permitido (inclusive)
    int max;           // Valor entero máximo permitido (inclusive)
    int reintentos;    // Número máximo de reintentos (-1 = infinito)
    int mostrar_error; // Activar mensajes de error en stderr (1 = sí, 0 = silencioso)
} ConfigEntero;
```

### 4.2 `ConfigFlotante`
```c
typedef struct {
    double min;        // Valor mínimo permitido
    double max;        // Valor máximo permitido
    int reintentos;    // Número máximo de reintentos (-1 = infinito)
    int mostrar_error; // Activar mensajes de error (1/0)
} ConfigFlotante;
```

### 4.3 `ConfigString`
```c
typedef struct {
    int min_longitud;  // Longitud mínima requerida
    int max_longitud;  // Longitud máxima permitida
    int permitir_vacio;// Permitir cadena vacía "" (1 = sí, 0 = no)
    int reintentos;    // Número máximo de reintentos (-1 = infinito)
    int mostrar_error; // Activar mensajes de error (1/0)
} ConfigString;
```

### 4.4 `ConfigCaracter`
```c
typedef struct {
    char opciones[256]; // Cadena con caracteres permitidos (ej. "ABC")
    int reintentos;     // Número máximo de reintentos (-1 = infinito)
    int mostrar_error;  // Activar mensajes de error (1/0)
} ConfigCaracter;
```

---

## 5. Análisis de Elementos Opcionales vs. Obligatorios

| Parámetro / Elemento | Obligatorio u Opcional | Razón y Explicación Técnica |
|----------------------|-------------------------|-----------------------------|
| **`tamanio` en Cadenas** | 🛑 **OBLIGATORIO** | En lenguaje C, indicar la capacidad máxima de memoria asignada al arreglo (`sizeof(buffer)`) es indispensable para que `fgets()` detenga la lectura y evite desbordamientos de memoria (*Buffer Overflow*). |
| **Estructuras `Config...`** | 💡 **OPCIONAL** | No es necesario declararlas manualmente en lecturas sencillas. Funciones como `obtener_entero()`, `obtener_entero_rango()`, `obtener_cadena()`, etc., instancian configuraciones por defecto (`CONFIG_ENTERO_DEFAULT`, etc.). |
| **Límite de `reintentos`** | 💡 **OPCIONAL** | Por defecto vale `-1` (reintentos ilimitados). Esto es ideal para consolas interactivas donde se desea solicitar el dato hasta que sea correcto. Solo se cambia a un valor entero (ej. `3`) para casos de seguridad (ej. ingresar un PIN). |
| **Flag `mostrar_error`** | 💡 **OPCIONAL** | Por defecto es `1` (imprime alertas con `❌`). Si se asigna `0`, la lectura es silenciosa sin imprimir mensajes adicionales. |
| **Macros (`INPUT_INT`, etc.)** | 💡 **OPCIONAL** | Azúcar sintáctica diseñada para simplificar el código. |

---

## 6. Funcionamiento Interno e Higiene de Buffer

### 6.1 `_limpiar_buffer(void)`
```c
static inline void _limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
```
* **Propósito**: Vacía los caracteres sobrantes de `stdin`.
* **Explicación**: Cuando un usuario ingresa una cadena o número largo y presiona Enter (`\n`), los caracteres no procesados permanecen atrapados en el flujo de entrada. `_limpiar_buffer()` garantiza que las lecturas siguientes no lean basura previa.

### 6.2 `_mostrar_error(const char *mensaje, int mostrar)`
```c
static inline void _mostrar_error(const char *mensaje, int mostrar) {
    if (mostrar) {
        fprintf(stderr, "  ❌ %s\n", mensaje);
    }
}
```
* **Propósito**: Imprime alertas formateadas en el canal de errores estándar `stderr`.

---

## 7. Catálogo Completo de Funciones

### 7.1 Funciones de Caracteres

#### `obtener_caracter(const char *mensaje)`
Lee un único carácter del usuario.
```c
char c = obtener_caracter("Presiona una tecla: ");
```

#### `obtener_caracter_opciones(const char *mensaje, const char *opciones)`
Lee un carácter restringido a las alternativas incluidas en `opciones`.
```c
char opcion = obtener_caracter_opciones("Selecciona (A/B/C): ", "ABC");
```

#### `obtener_caracter_config(const char *mensaje, ConfigCaracter config)`
Versión avanzada con estructura de configuración.

---

### 7.2 Funciones de Enteros

#### `obtener_entero(const char *mensaje)`
Lee cualquier número entero válido dentro del rango soportado por `int`.
```c
int numero = obtener_entero("Ingresa un número: ");
```

#### `obtener_entero_rango(const char *mensaje, int min, int max)`
Lee un entero obligatoriamente comprendido entre `min` y `max`.
```c
int nota = obtener_entero_rango("Ingresa la nota (0-10): ", 0, 10);
```

#### `obtener_entero_config(const char *mensaje, ConfigEntero config)`
Versión avanzada con estructura de configuración.

---

### 7.3 Funciones de Flotantes

#### `obtener_flotante(const char *mensaje)`
Lee un número decimal (`float`).
```c
float precio = obtener_flotante("Precio: ");
```

#### `obtener_flotante_rango(const char *mensaje, float min, float max)`
Lee un decimal delimitado entre un rango mínimo y máximo.
```c
float temp = obtener_flotante_rango("Temperatura (-10.0 a 45.0): ", -10.0f, 45.0f);
```

#### `obtener_flotante_config(const char *mensaje, ConfigFlotante config)`
Versión avanzada con configuración personalizada.

---

### 7.4 Funciones de Cadenas de Texto

#### `obtener_cadena(const char *mensaje, char *buffer, int tamanio)`
Lee una línea de texto completa (incluyendo espacios) de manera segura.
```c
char nombre[50];
obtener_cadena("Nombre completo: ", nombre, sizeof(nombre));
```

#### `obtener_cadena_min(const char *mensaje, char *buffer, int tamanio, int min_longitud)`
Exige una longitud mínima de caracteres antes de aceptar la entrada.
```c
char clave[64];
obtener_cadena_min("Crea una contraseña (mín 8 caracteres): ", clave, sizeof(clave), 8);
```

#### `obtener_cadena_config(const char *mensaje, char *buffer, int tamanio, ConfigString config)`
Versión avanzada con configuración completa.

---

### 7.5 Funciones de Confirmación y Dominio (Email/Teléfono/Menú)

#### `obtener_si_no(const char *mensaje)`
Muestra la sugerencia `(s/n)` y retorna `1` para respuestas afirmativas (`'s'`, `'y'`) o `0` para negativas (`'n'`).
```c
if (obtener_si_no("¿Deseas guardar las modificaciones?")) {
    printf("Guardado exitoso.\n");
}
```

#### `obtener_email(const char *mensaje, char *buffer, int tamanio)`
Solicita y valida la estructura básica de un correo (presencia de `@` y `.`).
```c
char correo[100];
obtener_email("Ingresa tu correo: ", correo, sizeof(correo));
```

#### `obtener_telefono(const char *mensaje, char *buffer, int tamanio)`
Solicita y valida un número de teléfono de 7 a 15 caracteres numéricos.
```c
char telefono[20];
obtener_telefono("Ingresa tu número telefónico: ", telefono, sizeof(telefono));
```

#### `obtener_opcion_menu(const char *mensaje, int min, int max)`
Obtiene la opción seleccionada de un menú interactivo.
```c
int opcion = obtener_opcion_menu("Selecciona una opción (1-4): ", 1, 4);
```

---

### 7.6 Funciones de Validación Manual

Permiten validar variables o cadenas en memoria sin realizar lecturas directas por consola:

```c
int ok1 = validar_no_vacio(cadena);       // Retorna 1 si la cadena no es NULL ni vacía
int ok2 = validar_rango(valor, min, max); // Retorna 1 si el valor está en el rango
int ok3 = validar_email(correo);          // Retorna 1 si la cadena tiene formato de email
```

---

## 8. Macros Útiles

```c
#define INPUT_INT(msg) obtener_entero(msg)
#define INPUT_INT_RANGE(msg, min, max) obtener_entero_rango(msg, min, max)
#define INPUT_FLOAT(msg) obtener_flotante(msg)
#define INPUT_STR(msg, buf, size) obtener_cadena(msg, buf, size)
#define INPUT_YES_NO(msg) obtener_si_no(msg)
#define INPUT_CHAR(msg) obtener_caracter(msg)
```

---

## 9. Ejemplos Prácticos Completos

### Ejemplo 1: Formulario de Registro de Usuario
```c
#include <stdio.h>
#include "get-input.h"

int main(void) {
    printf("=== REGISTRO DE USUARIO ===\n\n");

    char nombre[50];
    obtener_cadena_min("Nombre completo: ", nombre, sizeof(nombre), 2);

    int edad = obtener_entero_rango("Edad (18-99): ", 18, 99);

    char correo[100];
    obtener_email("Correo electrónico: ", correo, sizeof(correo));

    char clave[64];
    obtener_cadena_min("Contraseña (mínimo 8 caracteres): ", clave, sizeof(clave), 8);

    if (obtener_si_no("¿Confirmas el registro?")) {
        printf("\n✅ Usuario %s registrado correctamente.\n", nombre);
    } else {
        printf("\n❌ Operación cancelada.\n");
    }

    return 0;
}
```

### Ejemplo 2: Menú Interactivo de Consola
```c
#include <stdio.h>
#include "get-input.h"

int main(void) {
    int continuar = 1;

    while (continuar) {
        printf("\n=== MENÚ PRINCIPAL ===\n");
        printf("1. Ver perfil\n");
        printf("2. Cambiar contraseña\n");
        printf("3. Salir\n");

        int opcion = obtener_opcion_menu("Elige una opción (1-3): ", 1, 3);

        switch (opcion) {
            case 1: printf("Mostrando perfil...\n"); break;
            case 2: printf("Modificando contraseña...\n"); break;
            case 3: 
                if (obtener_si_no("¿Seguro que deseas salir?")) {
                    continuar = 0;
                }
                break;
        }
    }
    return 0;
}
```

---

## 10. Tabla de Referencia Rápida

| Función | Firma / Parámetros | Retorno | Descripción Breve |
|---------|--------------------|---------|-------------------|
| `obtener_entero` | `(const char *mensaje)` | `int` | Lee un entero seguro. |
| `obtener_entero_rango` | `(const char *mensaje, int min, int max)` | `int` | Lee un entero acotado por un rango. |
| `obtener_flotante` | `(const char *mensaje)` | `float` | Lee un número flotante. |
| `obtener_flotante_rango` | `(const char *mensaje, float min, float max)` | `float` | Lee un flotante en un rango. |
| `obtener_cadena` | `(const char *mensaje, char *buffer, int tamanio)` | `void` | Lee una cadena sin desbordamiento. |
| `obtener_cadena_min` | `(const char *msg, char *buf, int size, int min)` | `int` | Lee cadena asegurando longitud mínima. |
| `obtener_caracter` | `(const char *mensaje)` | `char` | Lee un único carácter de teclado. |
| `obtener_caracter_opciones` | `(const char *mensaje, const char *opciones)` | `char` | Lee un carácter restringido a opciones. |
| `obtener_si_no` | `(const char *mensaje)` | `int` | Retorna `1` (sí) o `0` (no). |
| `obtener_email` | `(const char *mensaje, char *buffer, int tamanio)` | `int` | Lee y valida correo electrónico. |
| `obtener_telefono` | `(const char *mensaje, char *buffer, int tamanio)` | `int` | Lee y valida número telefónico. |
| `obtener_opcion_menu` | `(const char *mensaje, int min, int max)` | `int` | Lee una opción entera de menú. |

---

## 11. Licencia

Este proyecto está distribuido bajo la [Licencia MIT](LICENSE). Libre para uso comercial, académico y personal.
