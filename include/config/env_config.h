#ifndef ENV_CONFIG_H
#define ENV_CONFIG_H

#include <stddef.h>

typedef struct {
    char *key;
    char *value;
} ConfigEntry;

typedef struct {
    ConfigEntry *entries;
    size_t count;
    size_t capacity;
} Config;

Config *config_load(const char *path);
void config_free(Config *config);
const char *config_get(const Config *config, const char *key);
int config_set(Config *config, const char *key, const char *value);

#endif // ENV_CONFIG_H 