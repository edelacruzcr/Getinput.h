# GUÍA COMPLETA Y MANUAL DE REFERENCIA: `util.h`

[![Versión](https://img.shields.io/badge/Versión-2.0.0-orange.svg)](util.h)
[![Tamaño](https://img.shields.io/badge/Peso-28.9%20KB%20(28%2C907%20bytes)-blue.svg)](util.h)
[![Líneas](https://img.shields.io/badge/Líneas-686-green.svg)](util.h)
[![Tipo](https://img.shields.io/badge/Tipo-Header--Only-purple.svg)](util.h)

---

## Índice

0. [Instalación y Desinstalación de Versiones Anteriores](#0-instalación-y-desinstalación-de-versiones-anteriores)
1. [Especificaciones y Métricas Reales](#1-especificaciones-y-métricas-reales)
2. [Arquitectura y Principios de Diseño](#2-arquitectura-y-principios-de-diseño)
3. [Sección 1: Entrada de Datos y Validación por Consola](#3-sección-1-entrada-de-datos-y-validación-por-consola)
   - [Estructuras de Configuración](#estructuras-de-configuración)
   - [Funciones de Carácter](#funciones-de-carácter)
   - [Funciones de Enteros](#funciones-de-enteros)
   - [Funciones de Flotantes](#funciones-de-flotantes)
   - [Funciones de Cadenas de Texto](#funciones-de-cadenas-de-texto)
   - [Funciones de Confirmación y Dominio](#funciones-de-confirmación-y-dominio)
   - [Validaciones Directas y Macros](#validaciones-directas-y-macros)
4. [Sección 2: Arreglos Flexibles y Tablas Hash O(1)](#4-sección-2-arreglos-flexibles-y-tablas-hash-o1)
   - [Estructuras de Datos](#estructuras-de-datos)
   - [Creación y Destrucción](#creación-y-destrucción)
   - [Inserción y Modificación](#inserción-y-modificación)
   - [Búsqueda y Consulta](#búsqueda-y-consulta)
   - [Eliminación e Inspección](#eliminación-e-inspección)
   - [Operaciones Avanzadas (Copia, Mezcla, Orden)](#operaciones-avanzadas)
   - [Macro de Iteración (`arreglo_recorrer`)](#macro-de-iteración)
5. [Ejemplo Completo de Uso Integrado](#5-ejemplo-completo-de-uso-integrado)

---

## 0. Instalación y Desinstalación de Versiones Anteriores

### Instalación Automática
Para instalar o actualizar `util.h` en tu sistema (`/usr/local/include`):

```bash
curl -fsSL https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/install.sh | bash
```

### Descarga Directa
Para descargar únicamente `util.h` en la carpeta de tu proyecto:

```bash
curl -fsSL https://raw.githubusercontent.com/edelacruzcr/Getinput.h/main/util.h -o util.h
```

### Código para Borrar Versión Anterior (`get-input.h` y `arreglo.h`)

Si habías descargado o instalado la versión anterior de 2 archivos, puedes eliminarlos ejecutando:

- **Globalmente en el sistema (`/usr/local/include`)**:
  ```bash
  sudo rm -f /usr/local/include/get-input.h /usr/local/include/arreglo.h
  ```
- **Localmente en tu carpeta de proyecto**:
  ```bash
  rm -f get-input.h arreglo.h
  ```

*(Nota: El script `install.sh` elimina automáticamente `get-input.h` y `arreglo.h` al ejecutarse).*

---

## 1. Especificaciones y Métricas Reales

| Métrica | Valor Real |
| :--- | :--- |
| **Nombre del archivo** | `util.h` |
| **Peso exacto en disco** | **28,907 bytes** (~28.9 KB) |
| **Total de líneas de código** | **686 líneas** |
| **Estándar C compatible** | C99, C11, C17, C23 |
| **Dependencias externas** | Ninguna (solo bibliotecas estándar de C: `stdio`, `stdlib`, `string`, `stdbool`, `stdint`, `ctype`, `limits`, `errno`) |
| **Modo de inclusión** | Header-Only (`static inline` para todas las funciones) |
| **Total de funciones** | 40 funciones (19 de entrada + 21 de arreglo flexible) |

---

## 2. Arquitectura y Principios de Diseño

`util.h` utiliza el patrón **Header-Only** mediante cualificadores `static inline`.

### Beneficios Técnicos:
1. **Inclusión sin conflictos**: Se puede incluir `#include "util.h"` en múltiples archivos `.c` de un mismo proyecto sin provocar errores de duplicación de símbolos en el enlazador (*linker duplicate symbols*).
2. **Inlining del Compilador**: Al ser `static inline`, los compiladores modernos (`gcc`, `clang`, `msvc`) pueden insertar el código binario directamente en la llamada, eliminando la sobrecarga de salto de función.
3. **Limpieza Automática de Buffer (`stdin`)**: Resuelve el problema común de caracteres residuales en `stdin` consumiendo el salto de línea `\n` sobrante.

---

## 3. Sección 1: Entrada de Datos y Validación por Consola

### Estructuras de Configuración

Permiten personalizar el comportamiento de lectura, límites de valores, cantidad de reintentos y visualización de errores.

#### `ConfigEntero`
- **Campos**:
  - `int min`: Límite mínimo permitido.
  - `int max`: Límite máximo permitido.
  - `int reintentos`: Número máximo de reintentos (-1 para infinito).
  - `int mostrar_error`: `1` para imprimir mensaje de error en `stderr`, `0` para modo silencioso.

#### `ConfigFlotante`
- **Campos**:
  - `double min`: Límite mínimo permitido.
  - `double max`: Límite máximo permitido.
  - `int reintentos`: Número de reintentos (-1 para infinito).
  - `int mostrar_error`: `1` activa errores, `0` los oculta.

#### `ConfigString`
- **Campos**:
  - `int min_longitud`: Longitud mínima en caracteres.
  - `int max_longitud`: Longitud máxima en caracteres.
  - `int permitir_vacio`: `1` permite enter vacío `""`, `0` lo exige.
  - `int reintentos`: Intentos máximos (-1 para infinito).
  - `int mostrar_error`: `1` activa errores, `0` los oculta.

#### `ConfigCaracter`
- **Campos**:
  - `char opciones[256]`: Cadena con caracteres aceptados (ej. `"abc"`). Si está vacía, acepta cualquier carácter.
  - `int reintentos`: Intentos máximos (-1 para infinito).
  - `int mostrar_error`: `1` activa errores, `0` los oculta.

---

### Funciones de Carácter

#### 1. `obtener_caracter_config`
- **¿Qué hace?**: Solicita un carácter con reglas de la estructura `ConfigCaracter`.
- **¿Qué recibe?**:
  - `const char *mensaje`: Prompt a mostrar al usuario.
  - `ConfigCaracter config`: Estructura con restricciones.
- **¿Qué devuelve?**: `char` leído o `'\0'` si se superan los reintentos.
- **Ejemplo**:
  ```c
  ConfigCaracter cfg = {.opciones = "sSnN", .reintentos = 3, .mostrar_error = 1};
  char opcion = obtener_caracter_config("¿Desea continuar? (s/n): ", cfg);
  ```

#### 2. `obtener_caracter`
- **¿Qué hace?**: Solicita cualquier carácter sin restricciones.
- **¿Qué recibe?**: `const char *mensaje`
- **¿Qué devuelve?**: `char`
- **Ejemplo**:
  ```c
  char tecla = obtener_caracter("Presione una tecla: ");
  ```

#### 3. `obtener_caracter_opciones`
- **¿Qué hace?**: Solicita un carácter restringido a una lista de opciones.
- **¿Qué recibe?**:
  - `const char *mensaje`: Prompt al usuario.
  - `const char *opciones`: Cadena con caracteres permitidos (ej. `"ABC"`).
- **¿Qué devuelve?**: `char` válido.
- **Ejemplo**:
  ```c
  char menu = obtener_caracter_opciones("Seleccione [A, B, C]: ", "ABCabc");
  ```

---

### Funciones de Enteros

#### 4. `obtener_entero_config`
- **¿Qué hace?**: Lee un número entero de consola aplicando las reglas de `ConfigEntero`.
- **¿Qué recibe?**:
  - `const char *mensaje`: Prompt al usuario.
  - `ConfigEntero config`: Configuración con rangos y reintentos.
- **¿Qué devuelve?**: `int` validado.
- **Ejemplo**:
  ```c
  ConfigEntero cfg = {.min = 1, .max = 10, .reintentos = 5, .mostrar_error = 1};
  int num = obtener_entero_config("Nota (1-10): ", cfg);
  ```

#### 5. `obtener_entero`
- **¿Qué hace?**: Lee un entero cualquiera (entre `INT_MIN` e `INT_MAX`).
- **¿Qué recibe?**: `const char *mensaje`
- **¿Qué devuelve?**: `int`
- **Ejemplo**:
  ```c
  int cantidad = obtener_entero("Ingrese la cantidad: ");
  ```

#### 6. `obtener_entero_rango`
- **¿Qué hace?**: Lee un entero obligatoriamente entre `min` y `max`.
- **¿Qué recibe?**:
  - `const char *mensaje`: Prompt al usuario.
  - `int min`: Límite inferior.
  - `int max`: Límite superior.
- **¿Qué devuelve?**: `int` dentro del rango especificado.
- **Ejemplo**:
  ```c
  int mes = obtener_entero_rango("Mes de nacimiento (1-12): ", 1, 12);
  ```

---

### Funciones de Flotantes

#### 7. `obtener_flotante_config`
- **¿Qué hace?**: Lee un decimal (`double`) según la configuración `ConfigFlotante`.
- **¿Qué recibe?**: `const char *mensaje`, `ConfigFlotante config`
- **¿Qué devuelve?**: `double` validado.
- **Ejemplo**:
  ```c
  ConfigFlotante cfg = {.min = 0.0, .max = 100.0, .reintentos = -1, .mostrar_error = 1};
  double precio = obtener_flotante_config("Precio ($): ", cfg);
  ```

#### 8. `obtener_flotante`
- **¿Qué hace?**: Lee un número decimal de consola sin límites específicos.
- **¿Qué recibe?**: `const char *mensaje`
- **¿Qué devuelve?**: `float`
- **Ejemplo**:
  ```c
  float altura = obtener_flotante("Ingrese su altura en metros (ej. 1.75): ");
  ```

#### 9. `obtener_flotante_rango`
- **¿Qué hace?**: Lee un número decimal dentro del rango `[min, max]`.
- **¿Qué recibe?**: `const char *mensaje`, `float min`, `float max`
- **¿Qué devuelve?**: `float`
- **Ejemplo**:
  ```c
  float promedio = obtener_flotante_rango("Promedio escolar (0.0 - 10.0): ", 0.0f, 10.0f);
  ```

---

### Funciones de Cadenas de Texto

#### 10. `obtener_cadena_config`
- **¿Qué hace?**: Lee una línea de texto de consola aplicando `ConfigString`. Elimina el `\n` al final y protege contra desbordamientos.
- **¿Qué recibe?**:
  - `const char *mensaje`: Prompt.
  - `char *buffer`: Puntero al buffer de destino.
  - `int tamanio`: Capacidad total del buffer.
  - `ConfigString config`: Reglas de validación.
- **¿Qué devuelve?**: `1` si fue exitoso, `0` en caso de falla.
- **Ejemplo**:
  ```c
  char clave[32];
  ConfigString cfg = {.min_longitud = 8, .max_longitud = 30, .permitir_vacio = 0, .reintentos = 3, .mostrar_error = 1};
  obtener_cadena_config("Cree su contraseña: ", clave, sizeof(clave), cfg);
  ```

#### 11. `obtener_cadena`
- **¿Qué hace?**: Lee una cadena de texto sin restricciones especiales.
- **¿Qué recibe?**: `const char *mensaje`, `char *buffer`, `int tamanio`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  char nombre[50];
  obtener_cadena("Ingrese su nombre: ", nombre, sizeof(nombre));
  ```

#### 12. `obtener_cadena_min`
- **¿Qué hace?**: Lee una cadena de texto exigiendo una longitud mínima determinada.
- **¿Qué recibe?**: `const char *mensaje`, `char *buffer`, `int tamanio`, `int min_longitud`
- **¿Qué devuelve?**: `1` si fue válida, `0` si falló.
- **Ejemplo**:
  ```c
  char usuario[30];
  obtener_cadena_min("Nombre de usuario (mínimo 4 letras): ", usuario, sizeof(usuario), 4);
  ```

---

### Funciones de Confirmación y Dominio

#### 13. `obtener_si_no`
- **¿Qué hace?**: Solicita una respuesta de confirmación. Acepta `'s'`, `'S'`, `'n'`, `'N'`, `'y'`, `'Y'`.
- **¿Qué recibe?**: `const char *mensaje`
- **¿Qué devuelve?**: `1` para Sí / Yes, `0` para No.
- **Ejemplo**:
  ```c
  if (obtener_si_no("¿Desea guardar los cambios?")) {
      printf("Guardado exitoso.\n");
  }
  ```

#### 14. `obtener_email`
- **¿Qué hace?**: Lee y valida sintácticamente un correo electrónico (requiere `'@'` y `'.'` posterior).
- **¿Qué recibe?**: `const char *mensaje`, `char *buffer`, `int tamanio`
- **¿Qué devuelve?**: `1` si el email es válido, `0` si es inválido.
- **Ejemplo**:
  ```c
  char email[100];
  obtener_email("Correo electrónico: ", email, sizeof(email));
  ```

#### 15. `obtener_telefono`
- **¿Qué hace?**: Lee y valida un número de teléfono (exclusivamente caracteres numéricos de 7 a 15 dígitos).
- **¿Qué recibe?**: `const char *mensaje`, `char *buffer`, `int tamanio`
- **¿Qué devuelve?**: `1` si es válido, `0` en caso contrario.
- **Ejemplo**:
  ```c
  char tel[20];
  obtener_telefono("Teléfono de contacto: ", tel, sizeof(tel));
  ```

#### 16. `obtener_opcion_menu`
- **¿Qué hace?**: Lee una opción de menú entre `min` y `max`.
- **¿Qué recibe?**: `const char *mensaje`, `int min`, `int max`
- **¿Qué devuelve?**: `int` seleccionado.
- **Ejemplo**:
  ```c
  int op = obtener_opcion_menu("Seleccione opción [1-4]: ", 1, 4);
  ```

---

### Validaciones Directas y Macros

#### Validaciones Manuales de Cadenas y Datos
- **`validar_no_vacio(const char *texto)`**: Devuelve `1` si no es `NULL` ni `""`, `0` si es nulo/vacío.
- **`validar_rango(int valor, int min, int max)`**: Devuelve `1` si `min <= valor <= max`.
- **`validar_email(const char *email)`**: Devuelve `1` si contiene estructura válida de email.

#### Macros de Atajo Directo
```c
#define INPUT_INT(msg)                     obtener_entero(msg)
#define INPUT_INT_RANGE(msg, min, max)     obtener_entero_rango(msg, min, max)
#define INPUT_FLOAT(msg)                   obtener_flotante(msg)
#define INPUT_STR(msg, buf, size)          obtener_cadena(msg, buf, size)
#define INPUT_YES_NO(msg)                  obtener_si_no(msg)
#define INPUT_CHAR(msg)                    obtener_caracter(msg)
```

---

## 4. Sección 2: Arreglos Flexibles y Tablas Hash O(1)

### Estructuras de Datos

#### `ArregloTipo` (Enumeración)
- `ARREGLO_TEXTO`: Almacena `char*`.
- `ARREGLO_ENTERO`: Almacena `int`.
- `ARREGLO_DECIMAL`: Almacena `double`.
- `ARREGLO_BOOLEANO`: Almacena `bool`.

#### `Arreglo` (Estructura Principal)
- `ElementoArreglo *elementos`: Vector de casillas asociativas.
- `size_t capacidad`: Capacidad asignada actual (por defecto 16).
- `size_t tamaño`: Cantidad de claves registradas en uso.

---

### Creación y Destrucción

#### 1. `arreglo_nuevo`
- **¿Qué hace?**: Reserva e inicializa un nuevo Arreglo Flexible con capacidad por defecto de 16 casillas.
- **¿Qué recibe?**: `void`
- **¿Qué devuelve?**: Puntero `Arreglo*` inicializado o `NULL` si falla memoria.
- **Ejemplo**:
  ```c
  Arreglo *mi_arreglo = arreglo_nuevo();
  ```

#### 2. `arreglo_nuevo_cap`
- **¿Qué hace?**: Crea un Arreglo Flexible especificando una capacidad inicial personalizada.
- **¿Qué recibe?**: `size_t capacidad_inicial`
- **¿Qué devuelve?**: Puntero `Arreglo*`.
- **Ejemplo**:
  ```c
  Arreglo *tabla_grande = arreglo_nuevo_cap(1000);
  ```

#### 3. `arreglo_vaciar`
- **¿Qué hace?**: Libera la memoria de todas las claves y valores almacenados, pero mantiene la estructura y su capacidad intactas.
- **¿Qué recibe?**: `Arreglo *a`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_vaciar(mi_arreglo);
  ```

#### 4. `arreglo_liberar`
- **¿Qué hace?**: Libera de forma recursiva toda la memoria dinámica ocupada por el arreglo, sus elementos y la estructura misma.
- **¿Qué recibe?**: `Arreglo *a`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_liberar(mi_arreglo);
  ```

---

### Inserción y Modificación

#### 5. `arreglo_guardar`
- **¿Qué hace?**: Guarda o reemplaza una cadena de texto asociada a una clave alfanumérica. Si la tabla supera el factor de carga (75%), duplica la capacidad automáticamente.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `const char *valor`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_guardar(mi_arreglo, "ciudad", "San José");
  ```

#### 6. `arreglo_guardar_int`
- **¿Qué hace?**: Guarda un entero asociado a una clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `int valor`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_guardar_int(mi_arreglo, "edad", 28);
  ```

#### 7. `arreglo_guardar_float`
- **¿Qué hace?**: Guarda un número decimal (`double`) asociado a una clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `double valor`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_guardar_float(mi_arreglo, "pi", 3.14159);
  ```

#### 8. `arreglo_guardar_bool`
- **¿Qué hace?**: Guarda un valor booleano (`true` / `false`) asociado a una clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool valor`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_guardar_bool(mi_arreglo, "activo", true);
  ```

---

### Búsqueda y Consulta

#### 9. `arreglo_buscar`
- **¿Qué hace?**: Busca y devuelve la cadena de texto asociada a una clave en tiempo constante $O(1)$ promedio gracias al algoritmo hash FNV-1a.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`
- **¿Qué devuelve?**: `char*` apuntando al texto almacenado o `NULL` si la clave no existe.
- **Ejemplo**:
  ```c
  char *val = arreglo_buscar(mi_arreglo, "ciudad");
  ```

#### 10. `arreglo_buscar_int`
- **¿Qué hace?**: Busca y obtiene un entero por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool *encontrado` (puntero opcional).
- **¿Qué devuelve?**: `int` encontrado o `0` si no existe.
- **Ejemplo**:
  ```c
  bool ok;
  int edad = arreglo_buscar_int(mi_arreglo, "edad", &ok);
  ```

#### 11. `arreglo_buscar_float`
- **¿Qué hace?**: Busca y obtiene un decimal por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool *encontrado` (puntero opcional).
- **¿Qué devuelve?**: `double` encontrado o `0.0`.
- **Ejemplo**:
  ```c
  double pi = arreglo_buscar_float(mi_arreglo, "pi", NULL);
  ```

#### 12. `arreglo_buscar_bool`
- **¿Qué hace?**: Busca y obtiene un booleano por su clave.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`, `bool *encontrado` (puntero opcional).
- **¿Qué devuelve?**: `bool` (`true`/`false`).
- **Ejemplo**:
  ```c
  bool activo = arreglo_buscar_bool(mi_arreglo, "activo", NULL);
  ```

---

### Eliminación e Inspección

#### 13. `arreglo_tiene`
- **¿Qué hace?**: Verifica si una clave existe registrada en el arreglo.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`
- **¿Qué devuelve?**: `bool` (`true` si existe, `false` si no).
- **Ejemplo**:
  ```c
  if (arreglo_tiene(mi_arreglo, "edad")) { ... }
  ```

#### 14. `arreglo_borrar`
- **¿Qué hace?**: Elimina un elemento por su clave y reorganiza las colisiones internas mediante desplazamiento lineal.
- **¿Qué recibe?**: `Arreglo *a`, `const char *clave`
- **¿Qué devuelve?**: `bool` (`true` si fue eliminado, `false` si no existía).
- **Ejemplo**:
  ```c
  arreglo_borrar(mi_arreglo, "ciudad");
  ```

#### 15. `arreglo_cuantos`
- **¿Qué hace?**: Retorna la cantidad total de elementos activos en el arreglo.
- **¿Qué recibe?**: `Arreglo *a`
- **¿Qué devuelve?**: `size_t`
- **Ejemplo**:
  ```c
  size_t total = arreglo_cuantos(mi_arreglo);
  ```

#### 16. `arreglo_claves`
- **¿Qué hace?**: Devuelve un arreglo dinámico de cadenas de texto (`char**`) con todas las claves presentes.
- **¿Qué recibe?**: `Arreglo *a`, `size_t *total` (puntero donde escribe el total).
- **¿Qué devuelve?**: `char**` (debe liberarse la memoria de este vector con `free()`).
- **Ejemplo**:
  ```c
  size_t n;
  char **lista_claves = arreglo_claves(mi_arreglo, &n);
  free(lista_claves);
  ```

#### 17. `arreglo_valores`
- **¿Qué hace?**: Devuelve un arreglo dinámico de cadenas (`char**`) representando los valores convertidos a texto.
- **¿Qué recibe?**: `Arreglo *a`, `size_t *total`.
- **¿Qué devuelve?**: `char**`.

---

### Operaciones Avanzadas

#### 18. `arreglo_copiar`
- **¿Qué hace?**: Crea una copia profunda completa de un arreglo.
- **¿Qué recibe?**: `Arreglo *a`
- **¿Qué devuelve?**: Puntero a la nueva estructura `Arreglo*`.
- **Ejemplo**:
  ```c
  Arreglo *copia = arreglo_copiar(mi_arreglo);
  ```

#### 19. `arreglo_mezclar`
- **¿Qué hace?**: Combina dos arreglos en uno nuevo. Si hay claves duplicadas, prevalece el valor del segundo arreglo (`b`).
- **¿Qué recibe?**: `Arreglo *a`, `Arreglo *b`
- **¿Qué devuelve?**: Nuevo `Arreglo*` resultante.
- **Ejemplo**:
  ```c
  Arreglo *unido = arreglo_mezclar(a, b);
  ```

#### 20. `arreglo_ordenar_claves`
- **¿Qué hace?**: Reordena internamente la tabla hash en orden alfabético según sus claves.
- **¿Qué recibe?**: `Arreglo *a`
- **¿Qué devuelve?**: `void`
- **Ejemplo**:
  ```c
  arreglo_ordenar_claves(mi_arreglo);
  ```

---

### Macro de Iteración

#### `arreglo_recorrer(a, clave_var, valor_var)`
- **¿Qué hace?**: Macro para iterar sobre todas las parejas clave-valor de forma limpia y legible.
- **Ejemplo**:
  ```c
  arreglo_recorrer(mi_arreglo, clave, valor) {
      printf("Clave: %s -> Valor: %s\n", clave, valor);
  }
  ```

---

## 5. Ejemplo Completo de Uso Integrado

```c
#include <stdio.h>
#include "util.h"

int main(void) {
    printf("=== REGISTRO DE USUARIO CON UTIL.H ===\n\n");

    // 1. Crear estructura de almacenamiento
    Arreglo *usuario = arreglo_nuevo();

    // 2. Obtener datos validados desde consola
    char nombre[100];
    obtener_cadena("Ingresa tu nombre: ", nombre, sizeof(nombre));
    int edad = obtener_entero_rango("Ingresa tu edad (18-99): ", 18, 99);
    char email[100];
    obtener_email("Ingresa tu email: ", email, sizeof(email));
    bool es_premium = obtener_si_no("¿Desea suscripción Premium?");

    // 3. Guardar en el Arreglo Flexible
    arreglo_guardar(usuario, "nombre", nombre);
    arreglo_guardar_int(usuario, "edad", edad);
    arreglo_guardar(usuario, "email", email);
    arreglo_guardar_bool(usuario, "premium", es_premium);

    // 4. Mostrar datos consultando el Arreglo
    printf("\n--- FICHA DE USUARIO GUARDADA ---\n");
    printf("Nombre:  %s\n", arreglo_buscar(usuario, "nombre"));
    printf("Edad:    %d años\n", arreglo_buscar_int(usuario, "edad", NULL));
    printf("Email:   %s\n", arreglo_buscar(usuario, "email"));
    printf("Premium: %s\n", arreglo_buscar_bool(usuario, "premium", NULL) ? "Sí" : "No");

    // 5. Liberar memoria
    arreglo_liberar(usuario);
    return 0;
}
```
