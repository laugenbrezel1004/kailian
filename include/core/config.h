#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include "error.h"

typedef struct {
    const char *key;
    union {
        int64_t as_int;
        double as_float;
        bool as_bool;
        char *as_string;
    } value;
    enum {
        CONFIG_TYPE_INT,
        CONFIG_TYPE_FLOAT,
        CONFIG_TYPE_BOOL,
        CONFIG_TYPE_STRING
    } type;
} ConfigEntry;

typedef struct Config Config;

Config* config_create(void);
void config_destroy(Config **config);
ErrorCode config_load_file(Config *config, const char *filename);
ErrorCode config_get_value(const Config *config, const char *key, ConfigEntry *entry);

#endif 