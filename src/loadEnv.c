// loadEnv.c
#include "../include/loadEnv.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CONFIG_PATH "/etc/kailian/kailian.conf"
#define ENV_CONFIG_VAR "KAILIAN_CONFIG"

/**
 * @brief Schneidet Leerzeichen aus einem String und kopiert ihn.
 * @param dest Zielbuffer.
 * @param src Quellstring.
 * @param dest_size Maximale Größe des Zielbuffers.
 */
static void trim_copy(char *dest, const char *src, size_t dest_size) {
    const char *start = src;
    while (*start == ' ')
        start++;
    const char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ')
        end--;
    size_t len =
        (end - start + 1) < dest_size ? (end - start + 1) : dest_size - 1;
    memcpy(dest, start, len);
    dest[len] = '\0';
}

/**
 * @brief Liest eine Konfigurationsdatei und gibt ein env-Array zurück.
 * @param config_path Pfad zur Konfigurationsdatei (optional, sonst Standard).
 * @param out_size Pointer auf die Anzahl der gelesenen Einträge.
 * @return env* Array von Schlüssel-Wert-Paaren oder NULL bei Fehler.
 */
env *readEnv(const char *config_path, size_t *out_size) {
    const char *path = config_path ? config_path : getenv(ENV_CONFIG_VAR);
    if (!path)
        path = DEFAULT_CONFIG_PATH;

    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        fprintf(stderr, "kailian: %s: No such file or directory\n", path);
        return NULL;
    }

    env *envs = malloc(8 * sizeof(env));
    if (!envs) {
        perror("malloc failed for envs");
        fclose(fptr);
        return NULL;
    }
    size_t capacity = 8, index = 0;
    for (size_t i = 0; i < capacity; i++) {
        envs[i].key = envs[i].value = NULL;
    }

    char line[256], key[256], value[256];
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0';
        char *equal_sign = strchr(line, '=');
        if (!equal_sign)
            continue;

        *equal_sign = '\0';
        trim_copy(key, line, sizeof(key));
        trim_copy(value, equal_sign + 1, sizeof(value));

        if (index >= capacity) {
            capacity *= 2;
            env *temp = realloc(envs, capacity * sizeof(env));
            if (!temp) {
                perror("realloc failed");
                freeEnv(envs, index);
                fclose(fptr);
                return NULL;
            }
            envs = temp;
            for (size_t i = index; i < capacity; i++) {
                envs[i].key = envs[i].value = NULL;
            }
        }

        // loadEnv.c (Auszug)
        envs[index].key = malloc(strlen(key) + 1); // +1 für Null-Terminierung
        envs[index].value =
            malloc(strlen(value) + 1); // +1 für Null-Terminierung
        if (!envs[index].key || !envs[index].value) {
            perror("malloc failed");
            freeEnv(envs, index);
            fclose(fptr);
            return NULL;
        }
        strcpy(envs[index].key, key);
        strcpy(envs[index].value, value);
        index++;
    }

    fclose(fptr);
    *out_size = index;
    return envs;
}

/**
 * @brief Gibt den Speicher eines env-Arrays frei.
 * @param envs Zu befreiendes Array.
 * @param size Anzahl der Einträge.
 */
void freeEnv(env *envs, size_t size) {
    for (size_t i = 0; i < size; i++) {
        free(envs[i].key);
        free(envs[i].value);
    }
    free(envs);
}
