#ifndef GET_INPUT_H
#define GET_INPUT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

// ============================================================
// CONFIGURACIÓN Y CONSTANTES
// ============================================================

#define GET_INPUT_VERSION "2.0.0"
#define MAX_BUFFER_SIZE 4096
#define MAX_STRING_LENGTH 1024

// ============================================================
// ESTRUCTURAS DE CONFIGURACIÓN
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

// ============================================================
// CONFIGURACIONES POR DEFECTO
// ============================================================

static const ConfigEntero CONFIG_ENTERO_DEFAULT = {
    .min = INT_MIN,
    .max = INT_MAX,
    .reintentos = -1,  // Infinito
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

// ============================================================
// FUNCIONES INTERNAS (PRIVADAS)
// ============================================================

static inline void _limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static inline void _mostrar_error(const char *mensaje, int mostrar) {
    if (mostrar) {
        fprintf(stderr, "  ❌ %s\n", mensaje);
    }
}

// ============================================================
// FUNCIONES: CARÁCTER
// ============================================================

/**
 * Obtiene un carácter del usuario con validación avanzada.
 */
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

/**
 * Obtiene un carácter del usuario (versión simple).
 */
static inline char obtener_caracter(const char *mensaje) {
    return obtener_caracter_config(mensaje, CONFIG_CARACTER_DEFAULT);
}

/**
 * Obtiene un carácter restringido a opciones específicas.
 */
static inline char obtener_caracter_opciones(const char *mensaje, const char *opciones) {
    ConfigCaracter config = CONFIG_CARACTER_DEFAULT;
    strncpy(config.opciones, opciones, sizeof(config.opciones) - 1);
    config.opciones[sizeof(config.opciones) - 1] = '\0';
    return obtener_caracter_config(mensaje, config);
}

// ============================================================
// FUNCIONES: ENTEROS
// ============================================================

/**
 * Obtiene un número entero del usuario con configuración personalizada.
 */
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

/**
 * Obtiene un entero del usuario (versión simple).
 */
static inline int obtener_entero(const char *mensaje) {
    return obtener_entero_config(mensaje, CONFIG_ENTERO_DEFAULT);
}

/**
 * Obtiene un entero restringido a un rango específico.
 */
static inline int obtener_entero_rango(const char *mensaje, int min, int max) {
    ConfigEntero config = CONFIG_ENTERO_DEFAULT;
    config.min = min;
    config.max = max;
    return obtener_entero_config(mensaje, config);
}

// ============================================================
// FUNCIONES: FLOTANTES
// ============================================================

/**
 * Obtiene un número flotante del usuario con configuración personalizada.
 */
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

/**
 * Obtiene un flotante del usuario (versión simple).
 */
static inline float obtener_flotante(const char *mensaje) {
    return (float)obtener_flotante_config(mensaje, CONFIG_FLOTANTE_DEFAULT);
}

/**
 * Obtiene un flotante en un rango específico.
 */
static inline float obtener_flotante_rango(const char *mensaje, float min, float max) {
    ConfigFlotante config = CONFIG_FLOTANTE_DEFAULT;
    config.min = min;
    config.max = max;
    return (float)obtener_flotante_config(mensaje, config);
}

// ============================================================
// FUNCIONES: CADENAS DE TEXTO
// ============================================================

/**
 * Obtiene una cadena de texto del usuario con configuración personalizada.
 */
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

/**
 * Obtiene una cadena de texto del usuario (versión simple).
 */
static inline void obtener_cadena(const char *mensaje, char *buffer, int tamanio) {
    obtener_cadena_config(mensaje, buffer, tamanio, CONFIG_STRING_DEFAULT);
}

/**
 * Obtiene una cadena con longitud mínima requerida.
 */
static inline int obtener_cadena_min(const char *mensaje, char *buffer, int tamanio, int min_longitud) {
    ConfigString config = CONFIG_STRING_DEFAULT;
    config.min_longitud = min_longitud;
    config.permitir_vacio = 0;
    return obtener_cadena_config(mensaje, buffer, tamanio, config);
}

// ============================================================
// FUNCIONES: CONFIRMACIÓN (SÍ / NO)
// ============================================================

/**
 * Obtiene una confirmación de sí/no por consola.
 */
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

// ============================================================
// FUNCIONES DE VALIDACIÓN ESPECÍFICAS
// ============================================================

/**
 * Obtiene y valida una dirección de correo electrónico.
 */
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

/**
 * Obtiene y valida un número de teléfono (solo dígitos).
 */
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

/**
 * Obtiene una opción numérica de menú entre min y max.
 */
static inline int obtener_opcion_menu(const char *mensaje, int min, int max) {
    ConfigEntero config = CONFIG_ENTERO_DEFAULT;
    config.min = min;
    config.max = max;
    config.mostrar_error = 1;
    return obtener_entero_config(mensaje, config);
}

// ============================================================
// MACROS ÚTILES
// ============================================================

#define INPUT_INT(msg) obtener_entero(msg)
#define INPUT_INT_RANGE(msg, min, max) obtener_entero_rango(msg, min, max)
#define INPUT_FLOAT(msg) obtener_flotante(msg)
#define INPUT_STR(msg, buf, size) obtener_cadena(msg, buf, size)
#define INPUT_YES_NO(msg) obtener_si_no(msg)
#define INPUT_CHAR(msg) obtener_caracter(msg)

// ============================================================
// FUNCIONES DE VALIDACIÓN MANUAL
// ============================================================

/**
 * Valida que una cadena no sea nula ni vacía.
 */
static inline int validar_no_vacio(const char *texto) {
    return texto != NULL && strlen(texto) > 0;
}

/**
 * Valida que un valor entero esté comprendido entre un mínimo y un máximo.
 */
static inline int validar_rango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}

/**
 * Valida que una cadena contenga una estructura básica de email.
 */
static inline int validar_email(const char *email) {
    if (email == NULL) return 0;
    const char *arroba = strchr(email, '@');
    const char *punto = strrchr(email, '.');
    return arroba != NULL && punto != NULL && punto > arroba;
}

#endif // GET_INPUT_H