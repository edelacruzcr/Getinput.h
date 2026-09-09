// ============================================================
// util.h - BIBLIOTECA HEADER-ONLY UNIFICADA PARA C
// ============================================================
// VERSIÓN: 2.0.0
// LICENCIA: MIT
// TIPO: Header-Only
// ============================================================
// COMPONENTES:
//   1. Entrada de Datos (get-input.h): Lectura y validación robusta desde consola.
//   2. Arreglo Flexible (arreglo.h): Arreglo dinámico y Tabla Hash O(1) multitipo.
// ============================================================

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

// ============================================================
// CONSTANTES Y VERSIONES
// ============================================================

#define UTIL_VERSION "2.0.0"
#define GET_INPUT_VERSION "2.0.0"
#define ARREGLO_VERSION "2.0.0"

#define MAX_BUFFER_SIZE 4096
#define MAX_STRING_LENGTH 1024

#define ARREGLO_CAPACIDAD_INICIAL 16
#define ARREGLO_FACTOR_CARGA 0.75f

// ============================================================
// SECCIÓN 1: ENTRADA DE DATOS DESDE CONSOLA (get-input)
// ============================================================

typedef struct {
    int min;           // Valor mínimo permitido
    int max;           // Valor máximo permitido
    int reintentos;    // Número de reintentos (-1 = infinito)
    int mostrar_error; // Mostrar mensajes de error (1/0)
} ConfigEntero;

typedef struct {
    double min;        // Valor mínimo permitido
    double max;        // Valor máximo permitido
    int reintentos;    // Número de reintentos (-1 = infinito)
    int mostrar_error; // Mostrar mensajes de error (1/0)
} ConfigFlotante;

typedef struct {
    int min_longitud;  // Longitud mínima
    int max_longitud;  // Longitud máxima
    int permitir_vacio;// Permitir string vacío (1/0)
    int reintentos;    // Número de reintentos (-1 = infinito)
    int mostrar_error; // Mostrar mensajes de error (1/0)
} ConfigString;

typedef struct {
    char opciones[256]; // Caracteres válidos
    int reintentos;     // Número de reintentos (-1 = infinito)
    int mostrar_error;  // Mostrar mensajes de error (1/0)
} ConfigCaracter;

// Configuraciones por defecto
static const ConfigEntero CONFIG_ENTERO_DEFAULT = {
    .min = INT_MIN,
    .max = INT_MAX,
    .reintentos = -1,
    .mostrar_error = 1
};

static const ConfigFlotante CONFIG_FLOTANTE_DEFAULT = {
    .min = -1e308,
    .max = 1e308,
    .reintentos = -1,
    .mostrar_error = 1
};

static const ConfigString CONFIG_STRING_DEFAULT = {
    .min_longitud = 0,
    .max_longitud = MAX_STRING_LENGTH,
    .permitir_vacio = 1,
    .reintentos = -1,
    .mostrar_error = 1
};

static const ConfigCaracter CONFIG_CARACTER_DEFAULT = {
    .opciones = "",
    .reintentos = -1,
    .mostrar_error = 1
};

// Funciones internas auxiliares para entrada de datos
static inline void _limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static inline void _mostrar_error(const char *mensaje, int mostrar) {
    if (mostrar) {
        fprintf(stderr, "  Error: %s\n", mensaje);
    }
}

// --- Caracteres ---

static inline char obtener_caracter_config(const char *mensaje, ConfigCaracter config) {
    char c;
    int intentos = 0;
    int tiene_opciones = (strlen(config.opciones) > 0);
    
    while (config.reintentos == -1 || intentos < config.reintentos) {
        printf("%s", mensaje);
        
        if (scanf(" %c", &c) == 1) {
            _limpiar_buffer();
            
            if (tiene_opciones) {
                if (strchr(config.opciones, c) != NULL) {
                    return c;
                }
                char msg[320];
                snprintf(msg, sizeof(msg), "Carácter no válido. Opciones: %s", config.opciones);
                _mostrar_error(msg, config.mostrar_error);
            } else {
                return c;
            }
        } else {
            _limpiar_buffer();
            _mostrar_error("Error al leer el carácter", config.mostrar_error);
        }
        
        intentos++;
    }
    
    _mostrar_error("Demasiados intentos fallidos", 1);
    return '\0';
}

static inline char obtener_caracter(const char *mensaje) {
    return obtener_caracter_config(mensaje, CONFIG_CARACTER_DEFAULT);
}

static inline char obtener_caracter_opciones(const char *mensaje, const char *opciones) {
    ConfigCaracter config = CONFIG_CARACTER_DEFAULT;
    strncpy(config.opciones, opciones, sizeof(config.opciones) - 1);
    config.opciones[sizeof(config.opciones) - 1] = '\0';
    return obtener_caracter_config(mensaje, config);
}

// --- Enteros ---

static inline int obtener_entero_config(const char *mensaje, ConfigEntero config) {
    char buffer[64];
    int intentos = 0;
    long valor;
    char *endptr;
    
    while (config.reintentos == -1 || intentos < config.reintentos) {
        printf("%s", mensaje);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            _mostrar_error("Error al leer la entrada", config.mostrar_error);
            intentos++;
            continue;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (buffer[0] == '\0') {
            _mostrar_error("Entrada vacía", config.mostrar_error);
            intentos++;
            continue;
        }
        
        int valido = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (i == 0 && (buffer[i] == '-' || buffer[i] == '+')) continue;
            if (!isdigit((unsigned char)buffer[i])) {
                valido = 0;
                break;
            }
        }
        
        if (!valido) {
            _mostrar_error("Entrada no numérica", config.mostrar_error);
            intentos++;
            continue;
        }
        
        errno = 0;
        valor = strtol(buffer, &endptr, 10);
        
        if (errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            _mostrar_error("Número fuera de rango de entero", config.mostrar_error);
            intentos++;
            continue;
        }
        
        if (valor < config.min || valor > config.max) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Valor fuera de rango [%d, %d]", config.min, config.max);
            _mostrar_error(msg, config.mostrar_error);
            intentos++;
            continue;
        }
        
        return (int)valor;
    }
    
    _mostrar_error("Demasiados intentos fallidos", 1);
    return config.min;
}

static inline int obtener_entero(const char *mensaje) {
    return obtener_entero_config(mensaje, CONFIG_ENTERO_DEFAULT);
}

static inline int obtener_entero_rango(const char *mensaje, int min, int max) {
    ConfigEntero config = CONFIG_ENTERO_DEFAULT;
    config.min = min;
    config.max = max;
    return obtener_entero_config(mensaje, config);
}

// --- Flotantes ---

static inline double obtener_flotante_config(const char *mensaje, ConfigFlotante config) {
    char buffer[128];
    int intentos = 0;
    double valor;
    char *endptr;
    
    while (config.reintentos == -1 || intentos < config.reintentos) {
        printf("%s", mensaje);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            _mostrar_error("Error al leer la entrada", config.mostrar_error);
            intentos++;
            continue;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (buffer[0] == '\0') {
            _mostrar_error("Entrada vacía", config.mostrar_error);
            intentos++;
            continue;
        }
        
        errno = 0;
        valor = strtod(buffer, &endptr);
        
        if (errno == ERANGE || *endptr != '\0') {
            _mostrar_error("Número flotante no válido", config.mostrar_error);
            intentos++;
            continue;
        }
        
        if (valor < config.min || valor > config.max) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Valor fuera de rango [%.2f, %.2f]", config.min, config.max);
            _mostrar_error(msg, config.mostrar_error);
            intentos++;
            continue;
        }
        
        return valor;
    }
    
    _mostrar_error("Demasiados intentos fallidos", 1);
    return config.min;
}

static inline float obtener_flotante(const char *mensaje) {
    return (float)obtener_flotante_config(mensaje, CONFIG_FLOTANTE_DEFAULT);
}

static inline float obtener_flotante_rango(const char *mensaje, float min, float max) {
    ConfigFlotante config = CONFIG_FLOTANTE_DEFAULT;
    config.min = min;
    config.max = max;
    return (float)obtener_flotante_config(mensaje, config);
}

// --- Cadenas ---

static inline int obtener_cadena_config(const char *mensaje, char *buffer, int tamanio, ConfigString config) {
    if (buffer == NULL || tamanio <= 0) {
        return 0;
    }
    
    int intentos = 0;
    
    while (config.reintentos == -1 || intentos < config.reintentos) {
        printf("%s", mensaje);
        
        if (fgets(buffer, tamanio, stdin) == NULL) {
            _mostrar_error("Error al leer la entrada", config.mostrar_error);
            intentos++;
            continue;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        } else {
            _limpiar_buffer();
            if ((int)len == tamanio - 1) {
                _mostrar_error("Entrada demasiado larga", config.mostrar_error);
                intentos++;
                continue;
            }
        }
        
        if (!config.permitir_vacio && len == 0) {
            _mostrar_error("La entrada no puede estar vacía", config.mostrar_error);
            intentos++;
            continue;
        }
        
        if ((int)len < config.min_longitud) {
            char msg[128];
            snprintf(msg, sizeof(msg), "La entrada debe tener al menos %d caracteres", config.min_longitud);
            _mostrar_error(msg, config.mostrar_error);
            intentos++;
            continue;
        }
        
        if ((int)len > config.max_longitud) {
            char msg[128];
            snprintf(msg, sizeof(msg), "La entrada no puede tener más de %d caracteres", config.max_longitud);
            _mostrar_error(msg, config.mostrar_error);
            intentos++;
            continue;
        }
        
        return 1;
    }
    
    _mostrar_error("Demasiados intentos fallidos", 1);
    buffer[0] = '\0';
    return 0;
}

static inline void obtener_cadena(const char *mensaje, char *buffer, int tamanio) {
    obtener_cadena_config(mensaje, buffer, tamanio, CONFIG_STRING_DEFAULT);
}

static inline int obtener_cadena_min(const char *mensaje, char *buffer, int tamanio, int min_longitud) {
    ConfigString config = CONFIG_STRING_DEFAULT;
    config.min_longitud = min_longitud;
    config.permitir_vacio = 0;
    return obtener_cadena_config(mensaje, buffer, tamanio, config);
}

// --- Confirmación y Validaciones Especiales ---

static inline int obtener_si_no(const char *mensaje) {
    char respuesta;
    
    while (1) {
        printf("%s (s/n): ", mensaje);
        
        if (scanf(" %c", &respuesta) == 1) {
            _limpiar_buffer();
            respuesta = (char)tolower((unsigned char)respuesta);
            
            if (respuesta == 's' || respuesta == 'y') {
                return 1;
            }
            if (respuesta == 'n') {
                return 0;
            }
        } else {
            _limpiar_buffer();
        }
        
        _mostrar_error("Respuesta inválida. Introduce 's' o 'n'", 1);
    }
}

static inline int obtener_email(const char *mensaje, char *buffer, int tamanio) {
    ConfigString config = CONFIG_STRING_DEFAULT;
    config.min_longitud = 5;
    config.max_longitud = 100;
    config.permitir_vacio = 0;
    
    int resultado = obtener_cadena_config(mensaje, buffer, tamanio, config);
    if (!resultado) return 0;
    
    char *arroba = strchr(buffer, '@');
    char *punto = strrchr(buffer, '.');
    
    if (arroba == NULL || punto == NULL || punto < arroba) {
        _mostrar_error("Email no válido (debe contener @ y . después de la @)", 1);
        return 0;
    }
    
    return 1;
}

static inline int obtener_telefono(const char *mensaje, char *buffer, int tamanio) {
    ConfigString config = CONFIG_STRING_DEFAULT;
    config.min_longitud = 7;
    config.max_longitud = 15;
    config.permitir_vacio = 0;
    
    int resultado = obtener_cadena_config(mensaje, buffer, tamanio, config);
    if (!resultado) return 0;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buffer[i])) {
            _mostrar_error("Teléfono inválido (solo se permiten dígitos)", 1);
            return 0;
        }
    }
    
    return 1;
}

static inline int obtener_opcion_menu(const char *mensaje, int min, int max) {
    ConfigEntero config = CONFIG_ENTERO_DEFAULT;
    config.min = min;
    config.max = max;
    config.mostrar_error = 1;
    return obtener_entero_config(mensaje, config);
}

// Macros de atajo
#define INPUT_INT(msg) obtener_entero(msg)
#define INPUT_INT_RANGE(msg, min, max) obtener_entero_rango(msg, min, max)
#define INPUT_FLOAT(msg) obtener_flotante(msg)
#define INPUT_STR(msg, buf, size) obtener_cadena(msg, buf, size)
#define INPUT_YES_NO(msg) obtener_si_no(msg)
#define INPUT_CHAR(msg) obtener_caracter(msg)

// Validaciones sueltas
static inline int validar_no_vacio(const char *texto) {
    return texto != NULL && strlen(texto) > 0;
}

static inline int validar_rango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}

static inline int validar_email(const char *email) {
    if (email == NULL) return 0;
    const char *arroba = strchr(email, '@');
    const char *punto = strrchr(email, '.');
    return arroba != NULL && punto != NULL && punto > arroba;
}


// ============================================================
// SECCIÓN 2: ARREGLOS FLEXIBLES Y HASH TABLES (arreglo)
// ============================================================

typedef enum {
    ARREGLO_TEXTO,      // Texto (char*)
    ARREGLO_ENTERO,     // Número entero (int)
    ARREGLO_DECIMAL,    // Número decimal (double)
    ARREGLO_BOOLEANO    // Booleano (bool)
} ArregloTipo;

typedef struct {
    char *clave;           // Clave alfanumérica
    ArregloTipo tipo;      // Tipo de dato almacenado
    
    union {
        char *texto;       // Valor textual
        int entero;        // Valor entero
        double decimal;    // Valor decimal
        bool booleano;     // Valor booleano
    } valor;
    
    bool ocupado;          // Indica si la casilla está en uso
} ElementoArreglo;

typedef struct {
    ElementoArreglo *elementos;  // Vector de casillas
    size_t capacidad;            // Capacidad total actual
    size_t tamaño;               // Cantidad de elementos guardados
} Arreglo;

// Funciones auxiliares internas de Arreglo
static inline char* _arreglo_strdup(const char *cadena) {
    if (!cadena) return NULL;
    size_t longitud = strlen(cadena);
    char *copia = (char*)malloc(longitud + 1);
    if (copia) {
        memcpy(copia, cadena, longitud + 1);
    }
    return copia;
}

static inline uint64_t _arreglo_hash(const char *cadena) {
    uint64_t hash = 1469598103934665603ULL;
    while (cadena && *cadena) {
        hash ^= (unsigned char)*cadena++;
        hash *= 1099511628211ULL;
    }
    return hash;
}

static inline void _arreglo_redimensionar(Arreglo *a, size_t nueva_capacidad) {
    if (!a || nueva_capacidad == 0) return;
    
    ElementoArreglo *nuevos = (ElementoArreglo*)calloc(nueva_capacidad, sizeof(ElementoArreglo));
    if (!nuevos) return;
    
    for (size_t i = 0; i < a->capacidad; i++) {
        if (a->elementos[i].ocupado) {
            uint64_t idx = _arreglo_hash(a->elementos[i].clave) % nueva_capacidad;
            while (nuevos[idx].ocupado) {
                idx = (idx + 1) % nueva_capacidad;
            }
            nuevos[idx] = a->elementos[i];
        }
    }
    
    free(a->elementos);
    a->elementos = nuevos;
    a->capacidad = nueva_capacidad;
}

static inline size_t _arreglo_buscar_posicion(Arreglo *a, const char *clave) {
    if (!a || !clave || a->capacidad == 0) return SIZE_MAX;
    
    uint64_t idx = _arreglo_hash(clave) % a->capacidad;
    size_t intentos = 0;
    
    while (a->elementos[idx].ocupado && intentos < a->capacidad) {
        if (a->elementos[idx].clave && strcmp(a->elementos[idx].clave, clave) == 0) {
            return idx;
        }
        idx = (idx + 1) % a->capacidad;
        intentos++;
    }
    return SIZE_MAX;
}

static inline Arreglo* arreglo_nuevo_cap(size_t capacidad_inicial);

static inline Arreglo* arreglo_nuevo(void) {
    return arreglo_nuevo_cap(ARREGLO_CAPACIDAD_INICIAL);
}

static inline Arreglo* arreglo_nuevo_cap(size_t capacidad_inicial) {
    Arreglo *a = (Arreglo*)malloc(sizeof(Arreglo));
    if (!a) return NULL;
    
    a->capacidad = capacidad_inicial > 0 ? capacidad_inicial : ARREGLO_CAPACIDAD_INICIAL;
    a->tamaño = 0;
    a->elementos = (ElementoArreglo*)calloc(a->capacidad, sizeof(ElementoArreglo));
    
    if (!a->elementos) {
        free(a);
        return NULL;
    }
    return a;
}

static inline void arreglo_guardar(Arreglo *a, const char *clave, const char *valor) {
    if (!a || !clave || !valor) return;
    
    if ((float)(a->tamaño + 1) / (float)a->capacidad > ARREGLO_FACTOR_CARGA) {
        _arreglo_redimensionar(a, a->capacidad * 2);
    }
    
    uint64_t idx = _arreglo_hash(clave) % a->capacidad;
    while (a->elementos[idx].ocupado) {
        if (a->elementos[idx].clave && strcmp(a->elementos[idx].clave, clave) == 0) {
            if (a->elementos[idx].tipo == ARREGLO_TEXTO && a->elementos[idx].valor.texto) {
                free(a->elementos[idx].valor.texto);
            }
            a->elementos[idx].tipo = ARREGLO_TEXTO;
            a->elementos[idx].valor.texto = _arreglo_strdup(valor);
            return;
        }
        idx = (idx + 1) % a->capacidad;
    }
    
    a->elementos[idx].clave = _arreglo_strdup(clave);
    a->elementos[idx].tipo = ARREGLO_TEXTO;
    a->elementos[idx].valor.texto = _arreglo_strdup(valor);
    a->elementos[idx].ocupado = true;
    a->tamaño++;
}

static inline void arreglo_guardar_int(Arreglo *a, const char *clave, int valor) {
    char buffer_texto[32];
    snprintf(buffer_texto, sizeof(buffer_texto), "%d", valor);
    arreglo_guardar(a, clave, buffer_texto);
}

static inline void arreglo_guardar_float(Arreglo *a, const char *clave, double valor) {
    char buffer_texto[64];
    snprintf(buffer_texto, sizeof(buffer_texto), "%f", valor);
    arreglo_guardar(a, clave, buffer_texto);
}

static inline void arreglo_guardar_bool(Arreglo *a, const char *clave, bool valor) {
    arreglo_guardar(a, clave, valor ? "true" : "false");
}

static inline char* arreglo_buscar(Arreglo *a, const char *clave) {
    if (!a || !clave) return NULL;
    
    size_t idx = _arreglo_buscar_posicion(a, clave);
    if (idx == SIZE_MAX || a->elementos[idx].tipo != ARREGLO_TEXTO) {
        return NULL;
    }
    return a->elementos[idx].valor.texto;
}

static inline int arreglo_buscar_int(Arreglo *a, const char *clave, bool *encontrado) {
    if (encontrado) *encontrado = false;
    if (!a || !clave) return 0;
    
    size_t idx = _arreglo_buscar_posicion(a, clave);
    if (idx == SIZE_MAX) return 0;
    
    if (encontrado) *encontrado = true;
    if (a->elementos[idx].tipo == ARREGLO_ENTERO) {
        return a->elementos[idx].valor.entero;
    }
    if (a->elementos[idx].tipo == ARREGLO_TEXTO && a->elementos[idx].valor.texto) {
        return atoi(a->elementos[idx].valor.texto);
    }
    return 0;
}

static inline double arreglo_buscar_float(Arreglo *a, const char *clave, bool *encontrado) {
    if (encontrado) *encontrado = false;
    if (!a || !clave) return 0.0;
    
    size_t idx = _arreglo_buscar_posicion(a, clave);
    if (idx == SIZE_MAX) return 0.0;
    
    if (encontrado) *encontrado = true;
    if (a->elementos[idx].tipo == ARREGLO_DECIMAL) {
        return a->elementos[idx].valor.decimal;
    }
    if (a->elementos[idx].tipo == ARREGLO_TEXTO && a->elementos[idx].valor.texto) {
        return atof(a->elementos[idx].valor.texto);
    }
    return 0.0;
}

static inline bool arreglo_buscar_bool(Arreglo *a, const char *clave, bool *encontrado) {
    if (encontrado) *encontrado = false;
    if (!a || !clave) return false;
    
    size_t idx = _arreglo_buscar_posicion(a, clave);
    if (idx == SIZE_MAX) return false;
    
    if (encontrado) *encontrado = true;
    if (a->elementos[idx].tipo == ARREGLO_BOOLEANO) {
        return a->elementos[idx].valor.booleano;
    }
    if (a->elementos[idx].tipo == ARREGLO_TEXTO && a->elementos[idx].valor.texto) {
        return (strcmp(a->elementos[idx].valor.texto, "true") == 0 ||
                strcmp(a->elementos[idx].valor.texto, "1") == 0 ||
                strcmp(a->elementos[idx].valor.texto, "verdadero") == 0);
    }
    return false;
}

static inline bool arreglo_tiene(Arreglo *a, const char *clave) {
    if (!a || !clave) return false;
    return _arreglo_buscar_posicion(a, clave) != SIZE_MAX;
}

static inline bool arreglo_borrar(Arreglo *a, const char *clave) {
    if (!a || !clave) return false;
    
    size_t idx = _arreglo_buscar_posicion(a, clave);
    if (idx == SIZE_MAX) return false;
    
    free(a->elementos[idx].clave);
    a->elementos[idx].clave = NULL;
    if (a->elementos[idx].tipo == ARREGLO_TEXTO && a->elementos[idx].valor.texto) {
        free(a->elementos[idx].valor.texto);
        a->elementos[idx].valor.texto = NULL;
    }
    a->elementos[idx].ocupado = false;
    a->tamaño--;
    
    size_t j = idx;
    while (true) {
        j = (j + 1) % a->capacidad;
        if (!a->elementos[j].ocupado) break;
        
        uint64_t k = _arreglo_hash(a->elementos[j].clave) % a->capacidad;
        if ((j > idx) ? (k <= idx || k > j) : (k <= idx && k > j)) {
            a->elementos[idx] = a->elementos[j];
            a->elementos[j].ocupado = false;
            a->elementos[j].clave = NULL;
            a->elementos[j].valor.texto = NULL;
            idx = j;
        }
    }
    
    return true;
}

static inline size_t arreglo_cuantos(Arreglo *a) {
    return a ? a->tamaño : 0;
}

static inline char** arreglo_claves(Arreglo *a, size_t *total) {
    if (!a || !total) {
        if (total) *total = 0;
        return NULL;
    }
    
    if (a->tamaño == 0) {
        *total = 0;
        return NULL;
    }
    
    char **claves = (char**)malloc(a->tamaño * sizeof(char*));
    if (!claves) {
        *total = 0;
        return NULL;
    }
    
    size_t idx = 0;
    for (size_t i = 0; i < a->capacidad && idx < a->tamaño; i++) {
        if (a->elementos[i].ocupado) {
            claves[idx++] = a->elementos[i].clave;
        }
    }
    
    *total = idx;
    return claves;
}

static inline char** arreglo_valores(Arreglo *a, size_t *total) {
    if (!a || !total) {
        if (total) *total = 0;
        return NULL;
    }
    
    if (a->tamaño == 0) {
        *total = 0;
        return NULL;
    }
    
    char **valores = (char**)malloc(a->tamaño * sizeof(char*));
    if (!valores) {
        *total = 0;
        return NULL;
    }
    
    size_t idx = 0;
    for (size_t i = 0; i < a->capacidad && idx < a->tamaño; i++) {
        if (a->elementos[i].ocupado) {
            if (a->elementos[i].tipo == ARREGLO_TEXTO && a->elementos[i].valor.texto) {
                valores[idx] = a->elementos[i].valor.texto;
            } else {
                char *temp = (char*)malloc(64);
                if (temp) {
                    if (a->elementos[i].tipo == ARREGLO_ENTERO) {
                        snprintf(temp, 64, "%d", a->elementos[i].valor.entero);
                    } else if (a->elementos[i].tipo == ARREGLO_DECIMAL) {
                        snprintf(temp, 64, "%f", a->elementos[i].valor.decimal);
                    } else {
                        snprintf(temp, 64, "%s", a->elementos[i].valor.booleano ? "true" : "false");
                    }
                }
                valores[idx] = temp;
            }
            idx++;
        }
    }
    
    *total = idx;
    return valores;
}

static inline void arreglo_vaciar(Arreglo *a) {
    if (!a) return;
    
    for (size_t i = 0; i < a->capacidad; i++) {
        if (a->elementos[i].ocupado) {
            free(a->elementos[i].clave);
            a->elementos[i].clave = NULL;
            if (a->elementos[i].tipo == ARREGLO_TEXTO && a->elementos[i].valor.texto) {
                free(a->elementos[i].valor.texto);
                a->elementos[i].valor.texto = NULL;
            }
            a->elementos[i].ocupado = false;
        }
    }
    a->tamaño = 0;
}

static inline void arreglo_liberar(Arreglo *a) {
    if (!a) return;
    arreglo_vaciar(a);
    free(a->elementos);
    free(a);
}

static inline Arreglo* arreglo_copiar(Arreglo *a) {
    if (!a) return NULL;
    
    Arreglo *copia = arreglo_nuevo_cap(a->capacidad);
    if (!copia) return NULL;
    
    for (size_t i = 0; i < a->capacidad; i++) {
        if (a->elementos[i].ocupado) {
            switch (a->elementos[i].tipo) {
                case ARREGLO_TEXTO:
                    arreglo_guardar(copia, a->elementos[i].clave, a->elementos[i].valor.texto);
                    break;
                case ARREGLO_ENTERO:
                    arreglo_guardar_int(copia, a->elementos[i].clave, a->elementos[i].valor.entero);
                    break;
                case ARREGLO_DECIMAL:
                    arreglo_guardar_float(copia, a->elementos[i].clave, a->elementos[i].valor.decimal);
                    break;
                case ARREGLO_BOOLEANO:
                    arreglo_guardar_bool(copia, a->elementos[i].clave, a->elementos[i].valor.booleano);
                    break;
            }
        }
    }
    
    return copia;
}

static inline Arreglo* arreglo_mezclar(Arreglo *a, Arreglo *b) {
    if (!a && !b) return NULL;
    if (!a) return arreglo_copiar(b);
    if (!b) return arreglo_copiar(a);
    
    Arreglo *mezcla = arreglo_copiar(a);
    if (!mezcla) return NULL;
    
    for (size_t i = 0; i < b->capacidad; i++) {
        if (b->elementos[i].ocupado) {
            switch (b->elementos[i].tipo) {
                case ARREGLO_TEXTO:
                    arreglo_guardar(mezcla, b->elementos[i].clave, b->elementos[i].valor.texto);
                    break;
                case ARREGLO_ENTERO:
                    arreglo_guardar_int(mezcla, b->elementos[i].clave, b->elementos[i].valor.entero);
                    break;
                case ARREGLO_DECIMAL:
                    arreglo_guardar_float(mezcla, b->elementos[i].clave, b->elementos[i].valor.decimal);
                    break;
                case ARREGLO_BOOLEANO:
                    arreglo_guardar_bool(mezcla, b->elementos[i].clave, b->elementos[i].valor.booleano);
                    break;
            }
        }
    }
    
    return mezcla;
}

#define arreglo_recorrer(a, clave_var, valor_var) \
    for (size_t _arreglo_idx = 0; (a) && _arreglo_idx < (a)->capacidad; _arreglo_idx++) \
        if ((a)->elementos[_arreglo_idx].ocupado) \
            for (char *clave_var = (a)->elementos[_arreglo_idx].clave, \
                      *valor_var = ((a)->elementos[_arreglo_idx].tipo == ARREGLO_TEXTO) ? \
                                   (a)->elementos[_arreglo_idx].valor.texto : ""; \
                  clave_var; \
                  clave_var = NULL, valor_var = NULL)

static inline void arreglo_ordenar_claves(Arreglo *a) {
    if (!a || a->tamaño <= 1) return;
    
    size_t total = 0;
    char **claves = arreglo_claves(a, &total);
    if (!claves || total <= 1) {
        if (claves) free(claves);
        return;
    }
    
    for (size_t i = 0; i < total - 1; i++) {
        for (size_t j = i + 1; j < total; j++) {
            if (strcmp(claves[i], claves[j]) > 0) {
                char *temp = claves[i];
                claves[i] = claves[j];
                claves[j] = temp;
            }
        }
    }
    
    Arreglo *ordenado = arreglo_nuevo_cap(a->capacidad);
    if (!ordenado) {
        free(claves);
        return;
    }
    
    for (size_t i = 0; i < total; i++) {
        char *val = arreglo_buscar(a, claves[i]);
        if (val) {
            arreglo_guardar(ordenado, claves[i], val);
        }
    }
    
    arreglo_vaciar(a);
    free(a->elementos);
    a->elementos = ordenado->elementos;
    a->capacidad = ordenado->capacidad;
    a->tamaño = ordenado->tamaño;
    free(ordenado);
    free(claves);
}

#endif // UTIL_H
