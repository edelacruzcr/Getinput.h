# 🛡️ get-input.h - Biblioteca de Entrada Segura y Robusta para C

[![Lenguaje](https://img.shields.io/badge/Lenguaje-C99%20%2F%20C11%20%2F%20C17%20%2F%20C23-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Licencia](https://img.shields.io/badge/Licencia-MIT-green.svg)](LICENSE)
[![Versión](https://img.shields.io/badge/Versión-2.0.0-orange.svg)](get-input.h)
[![Tipo](https://img.shields.io/badge/Tipo-Header--Only-purple.svg)](get-input.h)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](Makefile)

**`get-input.h`** es una biblioteca **header-only** para lenguaje C que simplifica la entrada de datos del usuario por consola (`stdin`), haciendo que sea **segura, robusta y fácil de usar**.

---

## 🎯 ¿Qué problema resuelve?

### El problema en C tradicional:

```c
// ❌ Código complicado y peligroso (propenso a desbordamiento y bucles infinitos)
int edad;
printf("Edad: ");
if (scanf("%d", &edad) != 1) {
    while(getchar() != '\n'); // Limpiar buffer
    printf("Error\n");
}
```

### La solución con `get-input.h`:

```c
// ✅ Simple, seguro y validado en una sola línea
int edad = obtener_entero_rango("Edad: ", 0, 120);
```

---

## 🔧 Características Principales

| Característica | Descripción |
|----------------|-------------|
| **Header-Only** | Solo un archivo `.h`, no necesita compilación separada |
| **Seguro** | Protege contra desbordamientos de buffer (*Buffer Overflow*) |
| **Robusto** | Maneja entradas inválidas automáticamente |
| **Configurable** | Límites, reintentos, mensajes de error |
| **Multiplataforma** | Funciona en Linux, macOS, Windows |
| **Sin dependencias** | Solo usa la biblioteca estándar de C |

---

## 📦 Instalación e Inclusión

Dado que `get-input.h` es una biblioteca **Header-Only**, no requiere compilación previa ni enlazado de bibliotecas adicionales.

### 1. Instalación Global (Disponible en todo el sistema)

Copia el archivo a la ruta de inclusión del sistema:

```bash
sudo cp get-input.h /usr/local/include/
```

Y en tu código de C inclúyelo con corchetes angular:
```c
#include <get-input.h>
```

---

### 2. Instalación Local (Por Proyecto)

Copia el archivo `get-input.h` a tu proyecto o carpeta de cabeceras:

```bash
mkdir -p include
cp get-input.h include/
```

Y en tu código inclúyelo con comillas:
```c
#include "get-input.h"       // Si está en el mismo directorio
// o bien:
#include "include/get-input.h" // Si está en la carpeta include/
```

---

## ⚙️ Compilación

Puedes compilar tus archivos C usando cualquier compilador estándar (GCC, Clang, MSVC) sin librerías extra:

```bash
gcc -Wall -Wextra -std=c99 main.c -o programa
./programa
```

### Compilar los Ejemplos del Repositorio
El repositorio incluye un `Makefile` para compilar los ejemplos en `examples/`:

```bash
make
./bin/01_basico
./bin/02_validaciones
./bin/03_email_telefono
./bin/04_menu_interactivo
```

---

## 📐 Estructuras de Configuración

```c
// Configuración para enteros
typedef struct {
    int min;           // Valor mínimo permitido
    int max;           // Valor máximo permitido
    int reintentos;    // Intentos permitidos (-1 = infinito)
    int mostrar_error; // Mostrar errores (1 = sí, 0 = silencioso)
} ConfigEntero;

// Configuración para flotantes
typedef struct {
    double min;        // Valor mínimo permitido
    double max;        // Valor máximo permitido
    int reintentos;    // Intentos permitidos
    int mostrar_error; // Mostrar errores
} ConfigFlotante;

// Configuración para cadenas de texto
typedef struct {
    int min_longitud;  // Longitud mínima
    int max_longitud;  // Longitud máxima
    int permitir_vacio;// Permitir cadena vacía (1/0)
    int reintentos;    // Intentos permitidos
    int mostrar_error; // Mostrar errores
} ConfigString;

// Configuración para caracteres
typedef struct {
    char opciones[256]; // Caracteres permitidos (ej. "ABC")
    int reintentos;     // Intentos permitidos
    int mostrar_error;  // Mostrar errores
} ConfigCaracter;
```

---

## 💡 Elementos Opcionales vs. Obligatorios

- 🛑 **Buffer y Tamaño (`tamanio`)**: **Obligatorio** en lectura de cadenas (ej. `obtener_cadena(msg, buf, sizeof(buf))`). Esto le indica a `fgets()` la capacidad máxima de memoria para evitar desbordamientos.
- 💡 **Estructuras `Config...`**: **Opcional**. Si usas funciones simples como `obtener_entero()` o `obtener_cadena()`, la biblioteca aplicará valores por defecto automáticamente.
- 💡 **Reintentos (`reintentos`)**: **Opcional**. Por defecto vale `-1` (reintentos infinitos). Se modifica solo en casos de seguridad (ej. 3 intentos para un PIN).
- 💡 **Mensajes de error (`mostrar_error`)**: **Opcional**. Por defecto es `1`. Puedes cambiarlo a `0` para operaciones silenciosas.

---

## 📚 Guía de Funciones

### 1. Funciones de Caracteres
```c
// Leer cualquier carácter
char c = obtener_caracter("Escribe algo: ");

// Leer restringido a opciones
char c = obtener_caracter_opciones("Elige (A/B/C): ", "ABC");

// Leer con configuración personalizada
ConfigCaracter config = { .opciones = "SI", .reintentos = 3, .mostrar_error = 1 };
char c = obtener_caracter_config("¿Continuar? (S/N): ", config);
```

### 2. Funciones de Enteros
```c
// Leer cualquier entero
int num = obtener_entero("Número: ");

// Leer en un rango determinado
int edad = obtener_entero_rango("Edad (0-120): ", 0, 120);

// Leer con configuración avanzada
ConfigEntero config = { .min = 18, .max = 65, .reintentos = 3, .mostrar_error = 1 };
int edad = obtener_entero_config("Edad (18-65): ", config);
```

### 3. Funciones de Flotantes
```c
// Leer cualquier flotante
float precio = obtener_flotante("Precio: ");

// Leer con rango
float temp = obtener_flotante_rango("Temp (-10 a 40): ", -10.0f, 40.0f);

// Leer con configuración
ConfigFlotante config = { .min = 0.0, .max = 10.0, .reintentos = 5, .mostrar_error = 0 };
float nota = obtener_flotante_config("Nota: ", config);
```

### 4. Funciones de Cadenas
```c
// Leer texto con espacios (seguro contra desbordamientos)
char nombre[50];
obtener_cadena("Nombre: ", nombre, sizeof(nombre));

// Leer con longitud mínima requerida
char pass[50];
obtener_cadena_min("Contraseña (mín 8): ", pass, sizeof(pass), 8);

// Leer con configuración
ConfigString config = { .min_longitud = 3, .max_longitud = 20, .permitir_vacio = 0, .reintentos = 3, .mostrar_error = 1 };
char usuario[50];
obtener_cadena_config("Usuario: ", usuario, sizeof(usuario), config);
```

### 5. Funciones Especiales (Email, Teléfono, Menú, Confirmación)
```c
// Confirmación Sí / No
if (obtener_si_no("¿Deseas continuar?")) {
    printf("Continuando...\n");
}

// Email (valida @ y .)
char email[100];
obtener_email("Email: ", email, sizeof(email));

// Teléfono (solo dígitos)
char telefono[20];
obtener_telefono("Teléfono: ", telefono, sizeof(telefono));

// Opción de menú numérico
int opcion = obtener_opcion_menu("Selecciona (1-4): ", 1, 4);
```

### 6. Funciones de Validación Manual
```c
if (validar_no_vacio(nombre)) { printf("Nombre no vacío\n"); }
if (validar_rango(edad, 0, 120)) { printf("Edad en rango\n"); }
if (validar_email(email)) { printf("Formato de email correcto\n"); }
```

---

## ⚡ Macros Útiles

```c
#define INPUT_INT(msg) obtener_entero(msg)
#define INPUT_INT_RANGE(msg, min, max) obtener_entero_rango(msg, min, max)
#define INPUT_FLOAT(msg) obtener_flotante(msg)
#define INPUT_STR(msg, buf, size) obtener_cadena(msg, buf, size)
#define INPUT_YES_NO(msg) obtener_si_no(msg)
#define INPUT_CHAR(msg) obtener_caracter(msg)
```

**Ejemplo:**
```c
int edad = INPUT_INT("Edad: ");
char nombre[50];
INPUT_STR("Nombre: ", nombre, sizeof(nombre));
if (INPUT_YES_NO("¿Aceptas?")) {
    printf("Bienvenido\n");
}
```

---

## 💡 Ejemplo Completo de Formulario

```c
#include <stdio.h>
#include "get-input.h" // o <get-input.h> si lo instalaste globalmente

int main(void) {
    printf("=== REGISTRO DE USUARIO ===\n\n");
    
    // 1. Nombre completo (mínimo 2 caracteres)
    char nombre[50];
    obtener_cadena_min("Nombre completo: ", nombre, sizeof(nombre), 2);
    
    // 2. Edad (0-120)
    int edad = obtener_entero_rango("Edad (0-120): ", 0, 120);
    
    // 3. Email (validado)
    char email[100];
    obtener_email("Correo electrónico: ", email, sizeof(email));
    
    // 4. Teléfono (solo dígitos)
    char telefono[20];
    obtener_telefono("Teléfono: ", telefono, sizeof(telefono));
    
    // 5. Contraseña (mínimo 8 caracteres)
    char pass[50];
    obtener_cadena_min("Contraseña (mín 8): ", pass, sizeof(pass), 8);
    
    // 6. Confirmación
    if (obtener_si_no("¿Confirmas el registro?")) {
        printf("\n✅ Usuario %s registrado correctamente.\n", nombre);
        printf("📧 Email: %s\n", email);
        printf("📱 Teléfono: %s\n", telefono);
    } else {
        printf("\n❌ Registro cancelado.\n");
    }
    
    return 0;
}
```

---

## 📊 Tabla de Funciones

| Función | Parámetros | Retorna | Descripción |
|---------|------------|---------|-------------|
| `obtener_caracter` | `mensaje` | `char` | Lee un carácter |
| `obtener_caracter_opciones` | `mensaje, opciones` | `char` | Lee carácter con opciones |
| `obtener_entero` | `mensaje` | `int` | Lee entero |
| `obtener_entero_rango` | `mensaje, min, max` | `int` | Lee entero en rango |
| `obtener_flotante` | `mensaje` | `float` | Lee flotante |
| `obtener_flotante_rango` | `mensaje, min, max` | `float` | Lee flotante en rango |
| `obtener_cadena` | `mensaje, buffer, tamaño` | `void` | Lee cadena |
| `obtener_cadena_min` | `mensaje, buffer, tamaño, min` | `int` | Lee cadena con mínimo |
| `obtener_si_no` | `mensaje` | `int` | Lee sí/no |
| `obtener_email` | `mensaje, buffer, tamaño` | `int` | Lee y valida email |
| `obtener_telefono` | `mensaje, buffer, tamaño` | `int` | Lee y valida teléfono |
| `obtener_opcion_menu` | `mensaje, min, max` | `int` | Lee opción de menú |
| `validar_no_vacio` | `texto` | `int` | Valida no vacío |
| `validar_rango` | `valor, min, max` | `int` | Valida rango |
| `validar_email` | `email` | `int` | Valida email |

---

## 🎯 ¿Cuándo Usar Cada Función?

| Situación | Función Sugerida |
|-----------|------------------|
| **Una letra** | `obtener_caracter()` |
| **Una letra con opciones** | `obtener_caracter_opciones()` |
| **Un número cualquiera** | `obtener_entero()` |
| **Un número en rango** | `obtener_entero_rango()` |
| **Un número decimal** | `obtener_flotante()` |
| **Un número decimal en rango** | `obtener_flotante_rango()` |
| **Un texto con espacios** | `obtener_cadena()` |
| **Un texto con longitud mínima** | `obtener_cadena_min()` |
| **Confirmación sí/no** | `obtener_si_no()` |
| **Email validado** | `obtener_email()` |
| **Teléfono validado** | `obtener_telefono()` |
| **Opción de menú** | `obtener_opcion_menu()` |

---

## ⭐ Ventajas

1. 🛡️ **Seguro**: Protege contra desbordamientos de buffer.
2. 🔄 **Robusto**: Maneja entradas inválidas y limpia `stdin`.
3. ⚡ **Simple**: Una sola línea para cada operación.
4. ⚙️ **Configurable**: Límites, reintentos y control de errores.
5. 🌐 **Portable**: Funciona en Linux, macOS y Windows.
6. 📦 **Header-only**: Un solo archivo de cabecera.
7. 📖 **Documentado**: Funciones explicadas y con ejemplos claros.

---

## 🎉 ¡Listo para Usar!

```c
#include "get-input.h"   // ¡Así de simple!

int main(void) {
    int edad = obtener_entero_rango("Edad (0-120): ", 0, 120);
    printf("Edad: %d años\n", edad);
    return 0;
}
```

---

## 📄 Licencia

Este proyecto se distribuye bajo la [Licencia MIT](LICENSE). Libre para uso comercial, académico y personal.
