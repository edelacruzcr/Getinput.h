# GUÍA COMPLETA Y MANUAL DE FUNCIONES - `get-input.h`

## Índice
1. [Introducción y Arquitectura](#1-introducción-y-arquitectura)
2. [Funcionamiento Interno e Higiene de Buffer](#2-funcionamiento-interno-e-higiene-de-buffer)
3. [Tipos de Datos y Estructuras](#3-tipos-de-datos-y-estructuras)
4. [Análisis de Elementos Opcionales vs Obligatorios](#4-análisis-de-elementos-opcionales-vs-obligatorios)
5. [Examen de Funciones de Carácter](#5-examen-de-funciones-de-carácter)
6. [Examen de Funciones de Enteros](#6-examen-de-funciones-de-enteros)
7. [Examen de Funciones de Flotantes](#7-examen-de-funciones-de-flotantes)
8. [Examen de Funciones de Cadenas](#8-examen-de-funciones-de-cadenas)
9. [Examen de Funciones de Confirmación y Dominio](#9-examen-de-funciones-de-confirmación-y-dominio)
10. [Funciones de Validación Manual](#10-funciones-de-validación-manual)
11. [Macros Útiles](#11-macros-útiles)
12. [Ejemplos Prácticos Completos](#12-ejemplos-prácticos-completos)
13. [Resumen Rápido de Referencia](#13-resumen-rápido-de-referencia)

---

## 1. Introducción y Arquitectura

### ¿Qué es `get-input.h`?

Es una biblioteca **header-only** en lenguaje C que **simplifica y asegura la entrada de datos por consola (`stdin`)**, solucionando automáticamente:
- Validación automática de tipos de datos.
- Limpieza transparente del buffer de entrada.
- Reintentos automáticos configurables en caso de error.
- Protección contra desbordamiento de buffer (*Buffer Overflow*).
- Mensajes de error claros y personalizables.

### Comparativa: Código Tradicional vs. `get-input.h`

```c
// SIN get-input.h - Propenso a errores, desbordamiento y bucles infinitos
int edad;
do {
    printf("Edad: ");
    if (scanf("%d", &edad) != 1) {
        while (getchar() != '\n'); // Limpiar buffer manualmente
        printf("Error: entrada no válida.\n");
        continue;
    }
} while (edad < 0 || edad > 120);

// CON get-input.h - Una sola línea limpia, segura y validada
int edad = obtener_entero_rango("Edad: ", 0, 120);
```

---

## 2. Funcionamiento Interno e Higiene de Buffer

### 2.1 Limpieza del Buffer (`_limpiar_buffer`)
```c
static inline void _limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
```
* **Cómo funciona**: Consume caracteres de `stdin` uno a uno con `getchar()` hasta hallar un salto de línea (`\n`) o fin de flujo (`EOF`).
* **Por qué es vital**: Al leer datos en C, los caracteres sobrantes o el `\n` presionado por el usuario permanecen en el buffer del teclado. Si no se limpian, las siguientes llamadas a `scanf()` o `fgets()` leerán basura inmediatamente o fallarán en silencio.

### 2.2 Control de Errores (`_mostrar_error`)
```c
static inline void _mostrar_error(const char *mensaje, int mostrar) {
    if (mostrar) {
        fprintf(stderr, "  Error: %s\n", mensaje);
    }
}
```
* **Cómo funciona**: Imprime advertencias en el canal de errores estándar `stderr`.
* **Modo Silencioso**: Si `mostrar == 0`, la biblioteca opera en modo silencioso sin imprimir texto extra en la terminal.

---

## 3. Tipos de Datos y Estructuras

Para lecturas con reglas avanzadas, la biblioteca expone 4 estructuras de configuración:

### 3.1 `ConfigEntero`
```c
typedef struct {
    int min;           // Valor entero mínimo permitido (inclusive)
    int max;           // Valor entero máximo permitido (inclusive)
    int reintentos;    // Número de reintentos (-1 = infinito)
    int mostrar_error; // Mostrar mensajes de error (1/0)
} ConfigEntero;
```

### 3.2 `ConfigFlotante`
```c
typedef struct {
    double min;        // Valor mínimo permitido
    double max;        // Valor máximo permitido
    int reintentos;    // Número de reintentos (-1 = infinito)
    int mostrar_error; // Mostrar mensajes de error (1/0)
} ConfigFlotante;
```

### 3.3 `ConfigString`
```c
typedef struct {
    int min_longitud;  // Longitud mínima requerida
    int max_longitud;  // Longitud máxima permitida
    int permitir_vacio;// Permitir string vacío (1 = sí, 0 = no)
    int reintentos;    // Número de reintentos (-1 = infinito)
    int mostrar_error; // Mostrar mensajes de error (1/0)
} ConfigString;
```

### 3.4 `ConfigCaracter`
```c
typedef struct {
    char opciones[256]; // Cadena con caracteres permitidos (ej. "ABC")
    int reintentos;     // Número de reintentos (-1 = infinito)
    int mostrar_error;  // Mostrar mensajes de error (1/0)
} ConfigCaracter;
```

---

## 4. Análisis de Elementos Opcionales vs Obligatorios

| Elemento | Obligatorio u Opcional | Razón y Explicación Técnica |
|----------|-------------------------|-----------------------------|
| **`tamanio` en Cadenas** | **OBLIGATORIO** | Al manipular cadenas en C, pasar el tamaño real del arreglo (`sizeof(buffer)`) es indispensable para evitar que `fgets()` escriba más allá de la memoria asignada (*Buffer Overflow*). |
| **Estructuras `Config...`** | **OPCIONAL** | No estás obligado a crear estas estructuras manualmente. Funciones como `obtener_entero()`, `obtener_entero_rango()`, `obtener_cadena()`, etc., las generan automáticamente usando valores por defecto. |
| **Límite de `reintentos`** | **OPCIONAL** | Por defecto vale `-1` (reintentos infinitos). Esto es ideal para consolas interactivas donde se desea insistir hasta obtener un valor válido. Solo se especifica un número (ej. `3`) en casos de seguridad (ej. PINs). |
| **Flag `mostrar_error`** | **OPCIONAL** | Por defecto es `1` (activo). Puede cambiarse a `0` si deseas validar datos de forma silenciosa o construir tu propia interfaz gráfica/TUI. |
| **Macros (`INPUT_INT`, etc.)** | **OPCIONAL** | Son azúcar sintáctica para programadores que buscan escribir código más compacto. |

---

## 5. Examen de Funciones de Carácter

### 5.1 `obtener_caracter_config(mensaje, config)`
* **Funcionamiento interno**: Usa `scanf(" %c", &c)` para saltar espacios iniciales y capturar un carácter. Llama inmediatamente a `_limpiar_buffer()`. Si `config.opciones` contiene caracteres, busca la coincidencia con `strchr()`.
* **Uso**:
```c
ConfigCaracter config = { .opciones = "SIsi", .reintentos = 3, .mostrar_error = 1 };
char r = obtener_caracter_config("¿Aceptas? (s/n): ", config);
```

### 5.2 `obtener_caracter(mensaje)`
* **Funcionamiento**: Lee un único carácter sin restringir sus opciones.

### 5.3 `obtener_caracter_opciones(mensaje, opciones)`
* **Funcionamiento**: Permite especificar la lista de caracteres válidos en una cadena directa sin declarar la estructura.
* **Ejemplo**:
```c
char opcion = obtener_caracter_opciones("Selecciona (A/B/C): ", "ABC");
```

---

## 6. Examen de Funciones de Enteros

### 6.1 `obtener_entero_config(mensaje, config)`
* **Funcionamiento interno**:
  1. Lee la línea de consola con `fgets()`.
  2. Verifica que solo contenga dígitos numéricos (y opcionalmente signo `+` o `-` inicial).
  3. Convierte el texto con `strtol()` y valida desbordamientos mediante `errno == ERANGE`.
  4. Comprueba que el resultado esté en el intervalo `[config.min, config.max]`.

### 6.2 `obtener_entero(mensaje)`
* **Funcionamiento**: Lee cualquier entero dentro del rango soportado por `int`.

### 6.3 `obtener_entero_rango(mensaje, min, max)`
* **Funcionamiento**: Limita la lectura entre `min` y `max`.
* **Ejemplo**:
```c
int nota = obtener_entero_rango("Calificación (0 a 10): ", 0, 10);
```

---

## 7. Examen de Funciones de Flotantes

### 7.1 `obtener_flotante_config(mensaje, config)`
* **Funcionamiento interno**: Lee mediante `fgets()`, convierte con `strtod()`, valida punteros de fin de conversión (`*endptr == '\0'`) y verifica límites con `config.min` y `config.max`.

### 7.2 `obtener_flotante(mensaje)`
* **Funcionamiento**: Lee un número decimal tipo `float`.

### 7.3 `obtener_flotante_rango(mensaje, min, max)`
* **Funcionamiento**: Lee un número decimal dentro de un rango determinado.
* **Ejemplo**:
```c
float precio = obtener_flotante_rango("Precio (0.50 a 99.99): ", 0.50f, 99.99f);
```

---

## 8. Examen de Funciones de Cadenas

### 8.1 `obtener_cadena_config(mensaje, buffer, tamanio, config)`
* **Funcionamiento interno**:
  1. Lee mediante `fgets(buffer, tamanio, stdin)`.
  2. Si la entrada excede `tamanio - 1`, vacía el sobrante con `_limpiar_buffer()`.
  3. Valida la longitud contra `config.min_longitud`, `config.max_longitud` y `config.permitir_vacio`.

### 8.2 `obtener_cadena(mensaje, buffer, tamanio)`
* **Funcionamiento**: Lee una cadena de texto respetando el límite del buffer.

### 8.3 `obtener_cadena_min(mensaje, buffer, tamanio, min_longitud)`
* **Funcionamiento**: Requiere una longitud mínima antes de aceptar la entrada.
* **Ejemplo**:
```c
char pass[64];
obtener_cadena_min("Contraseña (mínimo 8 caracteres): ", pass, sizeof(pass), 8);
```

---

## 9. Examen de Funciones de Confirmación y Dominio

### 9.1 `obtener_si_no(mensaje)`
* **Funcionamiento**: Presenta el prompt `(s/n)`, lee el carácter, lo convierte a minúscula con `tolower()` y retorna `1` para `'s'`/`'y'` o `0` para `'n'`. Reintenta automáticamente en caso de error.

### 9.2 `obtener_email(mensaje, buffer, tamanio)`
* **Funcionamiento**: Obtiene una cadena y confirma mediante `strchr()` y `strrchr()` la existencia de una `@` y un punto `.` posterior.

### 9.3 `obtener_telefono(mensaje, buffer, tamanio)`
* **Funcionamiento**: Obtiene una cadena de 7 a 15 caracteres comprobando con `isdigit()` que contenga únicamente dígitos.

### 9.4 `obtener_opcion_menu(mensaje, min, max)`
* **Funcionamiento**: Solicita una opción numérica dentro del intervalo `[min, max]` de un menú.

---

## 10. Funciones de Validación Manual

Sirven para validar variables o cadenas de datos en memoria sin solicitar entrada por consola:

* `validar_no_vacio(texto)`: Retorna `1` si la cadena no es nula ni vacía.
* `validar_rango(valor, min, max)`: Retorna `1` si el entero está en el rango.
* `validar_email(email)`: Retorna `1` si la estructura del email es válida.

---

## 11. Macros Útiles

```c
#define INPUT_INT(msg) obtener_entero(msg)
#define INPUT_INT_RANGE(msg, min, max) obtener_entero_rango(msg, min, max)
#define INPUT_FLOAT(msg) obtener_flotante(msg)
#define INPUT_STR(msg, buf, size) obtener_cadena(msg, buf, size)
#define INPUT_YES_NO(msg) obtener_si_no(msg)
#define INPUT_CHAR(msg) obtener_caracter(msg)
```

---

## 12. Ejemplos Prácticos Completos

### Ejemplo 1: Formulario Completo de Registro
```c
#include <stdio.h>
#include "get-input.h"

int main(void) {
    printf("=== FORMULARIO DE REGISTRO ===\n\n");

    char nombre[50];
    obtener_cadena_min("Nombre completo (mín 2 caracteres): ", nombre, sizeof(nombre), 2);

    int edad = obtener_entero_rango("Edad (0 - 120): ", 0, 120);

    char correo[100];
    obtener_email("Correo electrónico: ", correo, sizeof(correo));

    char telefono[20];
    obtener_telefono("Teléfono (dígitos): ", telefono, sizeof(telefono));

    if (obtener_si_no("¿Aceptas los términos y condiciones?")) {
        printf("\n[OK] Registro finalizado con éxito para %s.\n", nombre);
    } else {
        printf("\n[CANCELADO] Registro cancelado.\n");
    }

    return 0;
}
```

---

## 13. Resumen Rápido de Referencia

| Función | Parámetros Principales | Retorna | Descripción Breve |
|---------|-----------------------|---------|-------------------|
| `obtener_entero` | `mensaje` | `int` | Lee un entero seguro. |
| `obtener_entero_rango` | `mensaje, min, max` | `int` | Lee un entero en un rango. |
| `obtener_flotante` | `mensaje` | `float` | Lee un número decimal. |
| `obtener_flotante_rango` | `mensaje, min, max` | `float` | Lee un decimal en un rango. |
| `obtener_cadena` | `mensaje, buffer, tamanio` | `void` | Lee texto de forma segura. |
| `obtener_cadena_min` | `mensaje, buffer, tamanio, min` | `int` | Lee texto con longitud mínima. |
| `obtener_caracter` | `mensaje` | `char` | Lee un único carácter. |
| `obtener_caracter_opciones` | `mensaje, opciones` | `char` | Lee carácter de lista permitida. |
| `obtener_si_no` | `mensaje` | `int` | Retorna `1` (sí) o `0` (no). |
| `obtener_email` | `mensaje, buffer, tamanio` | `int` | Lee y valida correo electrónico. |
| `obtener_telefono` | `mensaje, buffer, tamanio` | `int` | Lee y valida número de teléfono. |
| `obtener_opcion_menu` | `mensaje, min, max` | `int` | Lee opción numérica de menú. |
