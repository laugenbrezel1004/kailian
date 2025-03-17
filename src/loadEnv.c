#define _POSIX_C_SOURCE 200809L
#include "../include/loadEnv.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CONFIG_PATH "/etc/kailian/kailian.conf"
#define ENV_CONFIG_VAR "KAILIAN_CONFIG"
#define INITIAL_CAPACITY 8

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

env *readEnv(const char *config_path, size_t *out_size) {
    const char *path = config_path ? config_path : getenv(ENV_CONFIG_VAR);
    path = path ? path : DEFAULT_CONFIG_PATH;

    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "kailian: %s: %s\n", path, strerror(errno));
        return NULL;
    }

    env *envs = malloc(INITIAL_CAPACITY * sizeof(*envs));
    if (!envs) {
        perror("kailian: malloc failed");
        fclose(fp);
        return NULL;
    }
    size_t capacity = INITIAL_CAPACITY;
    size_t count = 0;
    for (size_t i = 0; i < capacity; i++) {
        envs[i].key = envs[i].value = NULL;
    }

    char line[256], key[256], value[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *eq = strchr(line, '=');
        if (!eq)
            continue;

        *eq = '\0';
        trim_copy(key, line, sizeof(key));
        trim_copy(value, eq + 1, sizeof(value));

        if (count >= capacity) {
            capacity *= 2;
            env *temp = realloc(envs, capacity * sizeof(*envs));
            if (!temp) {
                perror("kailian: realloc failed");
                freeEnv(envs, count);
                fclose(fp);
                return NULL;
            }
            envs = temp;
            for (size_t i = count; i < capacity; i++) {
                envs[i].key = envs[i].value = NULL;
            }
        }

        envs[count].key = strdup(key);
        envs[count].value = strdup(value);
        if (!envs[count].key || !envs[count].value) {
            perror("kailian: strdup failed");
            freeEnv(envs, count);
            fclose(fp);
            return NULL;
        }
        count++;
    }

    fclose(fp);
    *out_size = count;
    return envs;
}

void freeEnv(env *envs, size_t size) {
    if (!envs)
        return;
    for (size_t i = 0; i < size; i++) {
        free(envs[i].key);
        free(envs[i].value);
    }
    free(envs);
}
