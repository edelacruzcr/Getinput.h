// ============================================================
// arreglo.h - ARREGLO FLEXIBLE ULTRA MEJORADO PARA C
// ============================================================
// VERSIÓN: 2.0.0
// LICENCIA: MIT
// TIPO: Header-Only
// ============================================================
// CARACTERÍSTICAS:
//   - Crecimiento automático de capacidad
//   - Búsqueda ultrarrápida (tabla hash con FNV-1a)
//   - Soporte para múltiples tipos de datos (texto, entero, decimal, booleano)
//   - Iteración conveniente mediante macro
//   - Manejo seguro de memoria y punteros nulos
//   - Funciones para copiar, mezclar y ordenar claves
// ============================================================

#ifndef ARREGLO_H
#define ARREGLO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// ============================================================
// CONFIGURACIÓN Y CONSTANTES
// ============================================================
#define ARREGLO_VERSION "2.0.0"
#define ARREGLO_CAPACIDAD_INICIAL 16
#define ARREGLO_FACTOR_CARGA 0.75f

// ============================================================
// ESTRUCTURAS DE DATOS
// ============================================================

// Tipos de datos soportados en el arreglo
typedef enum {
    ARREGLO_TEXTO,      // Texto (char*)
    ARREGLO_ENTERO,     // Número entero (int)
    ARREGLO_DECIMAL,    // Número decimal (double)
    ARREGLO_BOOLEANO    // Booleano (bool)
} ArregloTipo;

// Elemento individual almacenado en la tabla hash
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

// Estructura principal del Arreglo Flexible
typedef struct {
    ElementoArreglo *elementos;  // Vector de casillas
    size_t capacidad;            // Capacidad total actual
    size_t tamaño;               // Cantidad de elementos guardados
} Arreglo;

// ============================================================
// FUNCIONES AUXILIARES INTERNAS
// ============================================================

// Duplicado de cadena portable (estándar C99)
static inline char* _arreglo_strdup(const char *cadena) {
    if (!cadena) return NULL;
    size_t longitud = strlen(cadena);
    char *copia = (char*)malloc(longitud + 1);
    if (copia) {
        memcpy(copia, cadena, longitud + 1);
    }
    return copia;
}

// Algoritmo de dispersión Hash FNV-1a de 64 bits
static inline uint64_t _arreglo_hash(const char *cadena) {
    uint64_t hash = 1469598103934665603ULL;
    while (cadena && *cadena) {
        hash ^= (unsigned char)*cadena++;
        hash *= 1099511628211ULL;
    }
    return hash;
}

// Redimensionar la tabla hash interna
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

// Buscar el índice de casilla asignado a una clave
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

// Declaraciones previas
static inline Arreglo* arreglo_nuevo_cap(size_t capacidad_inicial);

// ============================================================
// FUNCIONES PÚBLICAS
// ============================================================

// 1. CREAR - Crea un arreglo vacío con capacidad predeterminada
static inline Arreglo* arreglo_nuevo(void) {
    return arreglo_nuevo_cap(ARREGLO_CAPACIDAD_INICIAL);
}

// 2. CREAR CON CAPACIDAD - Crea un arreglo con capacidad inicial personalizada
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

// 3. GUARDAR TEXTO - Guarda o actualiza un texto asociado a una clave
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

// 4. GUARDAR ENTERO - Guarda un número entero asociado a una clave
static inline void arreglo_guardar_int(Arreglo *a, const char *clave, int valor) {
    char buffer_texto[32];
    snprintf(buffer_texto, sizeof(buffer_texto), "%d", valor);
    arreglo_guardar(a, clave, buffer_texto);
}

// 5. GUARDAR DECIMAL - Guarda un número decimal asociado a una clave
static inline void arreglo_guardar_float(Arreglo *a, const char *clave, double valor) {
    char buffer_texto[64];
    snprintf(buffer_texto, sizeof(buffer_texto), "%f", valor);
    arreglo_guardar(a, clave, buffer_texto);
}

// 6. GUARDAR BOOLEANO - Guarda un valor booleano asociado a una clave
static inline void arreglo_guardar_bool(Arreglo *a, const char *clave, bool valor) {
    arreglo_guardar(a, clave, valor ? "true" : "false");
}

// 7. BUSCAR - Obtiene la cadena de texto asociada a una clave
static inline char* arreglo_buscar(Arreglo *a, const char *clave) {
    if (!a || !clave) return NULL;
    
    size_t idx = _arreglo_buscar_posicion(a, clave);
    if (idx == SIZE_MAX || a->elementos[idx].tipo != ARREGLO_TEXTO) {
        return NULL;
    }
    return a->elementos[idx].valor.texto;
}

// 8. BUSCAR ENTERO - Obtiene un valor entero por su clave
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

// 9. BUSCAR DECIMAL - Obtiene un valor decimal por su clave
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

// 10. BUSCAR BOOLEANO - Obtiene un valor booleano por su clave
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

// 11. TIENE - Comprueba si existe una clave en el arreglo
static inline bool arreglo_tiene(Arreglo *a, const char *clave) {
    if (!a || !clave) return false;
    return _arreglo_buscar_posicion(a, clave) != SIZE_MAX;
}

// 12. BORRAR - Elimina un elemento por su clave
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
    
    // Algoritmo de eliminación segura por deslazamiento lineal
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

// 13. CUANTOS - Devuelve el número total de elementos almacenados
static inline size_t arreglo_cuantos(Arreglo *a) {
    return a ? a->tamaño : 0;
}

// 14. CLAVES - Obtiene un array dinámico con todas las claves del arreglo
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

// 15. VALORES - Obtiene un array dinámico con todos los valores convertidos a texto
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

// 16. VACIAR - Borra todos los elementos manteniendo la capacidad actual
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

// 17. LIBERAR - Libera toda la memoria consumida por el arreglo
static inline void arreglo_liberar(Arreglo *a) {
    if (!a) return;
    arreglo_vaciar(a);
    free(a->elementos);
    free(a);
}

// 18. COPIAR - Crea una copia completa (copia profunda) del arreglo
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

// 19. MEZCLAR - Combina dos arreglos creando uno nuevo
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

// 20. RECORRER - Macro para iterar sobre las parejas clave-valor
#define arreglo_recorrer(a, clave_var, valor_var) \
    for (size_t _arreglo_idx = 0; (a) && _arreglo_idx < (a)->capacidad; _arreglo_idx++) \
        if ((a)->elementos[_arreglo_idx].ocupado) \
            for (char *clave_var = (a)->elementos[_arreglo_idx].clave, \
                     *valor_var = ((a)->elementos[_arreglo_idx].tipo == ARREGLO_TEXTO) ? \
                                  (a)->elementos[_arreglo_idx].valor.texto : ""; \
                 clave_var; \
                 clave_var = NULL, valor_var = NULL)

// 21. ORDENAR CLAVES - Ordena internamente la estructura por orden alfabético de clave
static inline void arreglo_ordenar_claves(Arreglo *a) {
    if (!a || a->tamaño <= 1) return;
    
    size_t total = 0;
    char **claves = arreglo_claves(a, &total);
    if (!claves || total <= 1) {
        if (claves) free(claves);
        return;
    }
    
    // Ordenamiento por burbuja simple para el vector de claves
    for (size_t i = 0; i < total - 1; i++) {
        for (size_t j = i + 1; j < total; j++) {
            if (strcmp(claves[i], claves[j]) > 0) {
                char *temp = claves[i];
                claves[i] = claves[j];
                claves[j] = temp;
            }
        }
    }
    
    // Crear nuevo arreglo temporal en orden
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
    
    // Intercambiar estructuras internas manteniendo validez
    arreglo_vaciar(a);
    free(a->elementos);
    a->elementos = ordenado->elementos;
    a->capacidad = ordenado->capacidad;
    a->tamaño = ordenado->tamaño;
    free(ordenado);
    free(claves);
}

#endif // ARREGLO_H
