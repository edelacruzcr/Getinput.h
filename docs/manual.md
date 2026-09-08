# 📖 Manual Técnico de Referencia - `get-input.h`

Este documento ofrece la especificación técnica completa sobre la arquitectura, estructuras de datos, funciones internas y análisis de elementos opcionales de la biblioteca **`get-input.h`**.

---

## 🏗️ 1. Arquitectura Header-Only

`get-input.h` implementa el patrón **Header-Only** en lenguaje C. Todas sus funciones públicas e internas están definidas con el cualificador `static inline`.

### Ventajas de `static inline`:
1. **Sin duplicación de símbolos**: Se puede incluir `#include "get-input.h"` en múltiples archivos `.c` dentro del mismo proyecto sin generar errores de enlace (*linker duplicate symbol errors*).
2. **Optimización en tiempo de compilación**: El compilador puede insertar el código de la función directamente en el sitio de llamada (*inlining*), eliminando el costo de salto de función.

---

## 🧼 2. Mecanismos Internos de Limpieza e Higiene de Buffer

### 2.1 `_limpiar_buffer(void)`
```c
static inline void _limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
```
- **Propósito**: Elimina cualquier carácter pendiente en el flujo estándar de entrada (`stdin`).
- **Detalle de Ejecución**: Cuando el usuario escribe datos e ingresa Enter (`\n`), o cuando ingresa más texto del que cabe en el buffer asignado, los caracteres no consumidos quedan atrapados en `stdin`. `_limpiar_buffer()` vacía el flujo hasta alcanzar el salto de línea `\n` o el fin de archivo `EOF`.

### 2.2 `_mostrar_error(const char *mensaje, int mostrar)`
```c
static inline void _mostrar_error(const char *mensaje, int mostrar) {
    if (mostrar) {
        fprintf(stderr, "  ❌ %s\n", mensaje);
    }
}
```
- **Propósito**: Escribe los mensajes de error en el canal de salida de errores estándar `stderr`. Si el parámetro `mostrar` vale `0`, desactiva las salidas de error en consola (modo silencioso).

---

## ⚙️ 3. Estructuras de Configuración Avanzada

### 3.1 `ConfigEntero`
```c
typedef struct {
    int min;           // Valor entero mínimo permitido (inclusive)
    int max;           // Valor entero máximo permitido (inclusive)
    int reintentos;    // Número máximo de reintentos (-1 = infinito)
    int mostrar_error; // Activar mensajes de error en stderr (1/0)
} ConfigEntero;
```

### 3.2 `ConfigFlotante`
```c
typedef struct {
    double min;        // Valor flotante mínimo permitido
    double max;        // Valor flotante máximo permitido
    int reintentos;    // Número máximo de reintentos
    int mostrar_error; // Activar mensajes de error
} ConfigFlotante;
```

### 3.3 `ConfigString`
```c
typedef struct {
    int min_longitud;  // Longitud mínima requerida
    int max_longitud;  // Longitud máxima permitida
    int permitir_vacio;// Permitir cadena vacía (1/0)
    int reintentos;    // Número máximo de reintentos
    int mostrar_error; // Activar mensajes de error
} ConfigString;
```

### 3.4 `ConfigCaracter`
```c
typedef struct {
    char opciones[256]; // Cadena con los caracteres permitidos
    int reintentos;     // Número máximo de reintentos
    int mostrar_error;  // Activar mensajes de error
} ConfigCaracter;
```

---

## 💡 4. Análisis de Elementos Opcionales vs. Obligatorios

### Obligatorio:
- **`tamanio` en lectura de cadenas**: Es **estrictamente obligatorio** pasar el tamaño real en bytes de la memoria asignada al buffer al llamar a `obtener_cadena` o `obtener_cadena_config`. Esto permite que la función configure internamente el parámetro de tamaño en `fgets()`, previniendo desbordamientos de buffer.

### Opcionales:
- **Uso directo de estructuras de configuración (`Config...`)**: **Opcional**. Si solo requieres leer un entero o una cadena básica, puedes usar directamente `obtener_entero()` u `obtener_cadena()`. La biblioteca instanciará las configuraciones por defecto (`CONFIG_ENTERO_DEFAULT`, etc.).
- **Parámetro `reintentos`**: **Opcional**. Su valor por defecto es `-1` (reintentos ilimitados). Solo se modifica cuando deseas limitar las oportunidades del usuario (ej. 3 intentos para un PIN de seguridad).
- **Parámetro `mostrar_error`**: **Opcional**. Su valor por defecto es `1`. Puede desactivarse (`0`) si deseas manejar la presentación de errores por tu cuenta.
- **Macros auxiliares (`INPUT_INT`, etc.)**: **Opcionales**. Son atajos sintácticos opcionales.

---

## 📊 5. Catálogo Completo de Funciones

### Funciones de Carácter
- `obtener_caracter(mensaje)`: Lee un carácter.
- `obtener_caracter_opciones(mensaje, opciones)`: Lee un carácter restringido a las opciones.
- `obtener_caracter_config(mensaje, config)`: Lee un carácter con configuración personalizada.

### Funciones de Enteros
- `obtener_entero(mensaje)`: Lee un entero.
- `obtener_entero_rango(mensaje, min, max)`: Lee un entero dentro del rango `[min, max]`.
- `obtener_entero_config(mensaje, config)`: Lee un entero con configuración personalizada.

### Funciones de Flotantes
- `obtener_flotante(mensaje)`: Lee un número decimal.
- `obtener_flotante_rango(mensaje, min, max)`: Lee un decimal dentro del rango.
- `obtener_flotante_config(mensaje, config)`: Lee un decimal con configuración personalizada.

### Funciones de Cadenas
- `obtener_cadena(mensaje, buffer, tamanio)`: Lee una línea de texto de forma segura.
- `obtener_cadena_min(mensaje, buffer, tamanio, min)`: Lee una cadena con longitud mínima.
- `obtener_cadena_config(mensaje, buffer, tamanio, config)`: Lee una cadena con configuración personalizada.

### Funciones Especiales y Validación
- `obtener_si_no(mensaje)`: Lee confirmaciones afirmativas o negativas. Retorna `1` o `0`.
- `obtener_email(mensaje, buffer, tamanio)`: Lee y valida estructura de correo electrónico.
- `obtener_telefono(mensaje, buffer, tamanio)`: Lee y valida teléfonos numéricos.
- `obtener_opcion_menu(mensaje, min, max)`: Lee la selección de un menú numérico.
- `validar_no_vacio(texto)`: Comprueba si un string no está vacío.
- `validar_rango(valor, min, max)`: Comprueba si un entero está en rango.
- `validar_email(email)`: Comprueba formato de email.
