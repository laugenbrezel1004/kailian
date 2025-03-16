#ifndef LOADENV_H
#define LOADENV_H


#include <stddef.h> // Für size_t

/**
 * @brief Struktur für ein Schlüssel-Wert-Paar aus der Konfigurationsdatei.
 */
typedef struct {
    char *key;   /**< Zeiger auf den Schlüssel (z. B. "name"). */
    char *value; /**< Zeiger auf den Wert (z. B. "mistral"). */
} env;

/**
 * @brief Liest die Konfigurationsdatei und gibt ein dynamisch allokiertes Array von env-Struct zurück.
 * 
 * @param out_size Pointer auf eine Variable, die die Anzahl der gelesenen Einträge zurückgibt.
 * @return env* Pointer auf das dynamisch allokierte Array von env-Struct, oder NULL bei Fehlern.
 */
env *readEnv(const char *config_pathsize_t, size_t *out_size);

/**
 * @brief Gibt den Speicher eines env-Arrays frei.
 * 
 * @param envs Pointer auf das zu befreiende env-Array.
 * @param size Anzahl der Einträge im Array, die freigegeben werden sollen.
 */
void freeEnv(env *envs, size_t size);

#endif /* CONFIG_H */
