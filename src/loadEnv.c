#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char name[300];
        char endpoint_generate[300];
        char endpoint_info[300];
        char endpoint_running_model[300];
        char endpoint_ollama_version[300];
        char endpoint_chat[300];
        char endpoint_show[300];
        char system[300];
} Env;

#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

typedef struct {
        const char *name; // Configuration key name
        char *dest;       // Pointer to the destination field in Env
        size_t size;      // Size of the destination buffer
} Field;

// Trims leading and trailing spaces from src and copies to dest
static void trim_copy(char *dest, const char *src) {
    const char *start = src;
    while (*start == ' ')
        start++; // Skip leading spaces
    const char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ')
        end--; // Skip trailing spaces
    size_t len = end - start + 1;
    memcpy(dest, start, len); // Copy trimmed string
    dest[len] = '\0';         // Null-terminate
}

Env readEnv() {
    Env env;
    memset(&env, 0,
           sizeof(env)); // Initialize all fields to zero (empty strings)

    // Array mapping config keys to Env struct fields
    // redundater coder?!?!?!?!!? -> einfach argument list aus
    // arguments/argumentList.c nutzen?
    Field fields[] = {
        {"name", env.name, sizeof(env.name)},
        {"endpoint", env.endpoint_generate, sizeof(env.endpoint_generate)},
        {"endpoint_info", env.endpoint_info, sizeof(env.endpoint_info)},
        {"endpoint_running_model", env.endpoint_running_model,
         sizeof(env.endpoint_running_model)},
        {"endpoint_ollama_version", env.endpoint_ollama_version,
         sizeof(env.endpoint_ollama_version)},
        {"endpoint_chat", env.endpoint_chat, sizeof(env.endpoint_chat)},
        {"endpoint_show", env.endpoint_show, sizeof(env.endpoint_show)},
        {"system", env.system, sizeof(env.system)},
        {NULL, NULL, 0} // Sentinel to mark end of array
    };

    FILE *fptr;
    char line[256];
    char key[256];
    char value[256];

    fptr = fopen("/etc/kailian/kailian.conf", "r");
    if (fptr == NULL) {
        fprintf(
            stderr,
            "kailian: /etc/kailian/kailian.conf: No such file or directory\n");
        exit(ENOENT);
    }

    while (fgets(line, sizeof(line), fptr)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';

        // Find the first '=' to split key and value
        char *equal_sign = strchr(line, '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0';   // Split the string into key and value
            trim_copy(key, line); // Trim key
            trim_copy(value, equal_sign + 1); // Trim value

            // Assign value to the corresponding field
            for (Field *f = fields; f->name != NULL; f++) {
                if (strcmp(key, f->name) == 0) {
                    strncpy(f->dest, value, f->size - 1);
                    f->dest[f->size - 1] = '\0'; // Ensure null-termination
                    break;
                }
            }
        }
        // Lines without '=' are silently skipped
    }

    fclose(fptr);
    return env;
}
