#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    const char *log_level;
    const char *config_path;
    bool debug_mode;
    size_t max_handlers;
} ApplicationConfig;

ErrorCode load_config(const char *config_path, ApplicationConfig *config);
void free_config(ApplicationConfig *config);

#endif 