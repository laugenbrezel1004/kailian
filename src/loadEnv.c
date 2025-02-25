#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char name[100];
        char endpoint[100];
        char info_endpoint[100];
        char running_model_endpoint[100];
        char ollama_version_endpoint[100];
} Env;

#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

// prototype
Env readEnv();
static void removeSpaces(char *str);

Env readEnv() {
    Env env = {"", "", "", "", ""}; // Initialize with empty strings
    FILE *fptr;
    char line[256];
    const char delim[] = "=";
    char *token;

    fptr = fopen(".env", "r");
    if (fptr == NULL) {
        fprintf(stderr,
                "kailian: no such file or directory: .env \nThere should be an "
                ".env file in the root of the kailian directory.");

        exit(1);
    }

    while (fgets(line, sizeof(line), fptr)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;
        removeSpaces(line);

        token = strtok(line, delim);
        if (token != NULL) {

            if (strcmp(token, "name") == 0) {
                token = strtok(NULL, delim);
                if (token != NULL) {
                    strncpy(env.name, token, sizeof(env.name) - 1);
                    env.name[sizeof(env.name) - 1] =
                        '\0'; // Ensure null-termination
                }
            } else if (strcmp(token, "endpoint") == 0) {
                token = strtok(NULL, delim);
                if (token != NULL) {
                    strncpy(env.endpoint, token, sizeof(env.endpoint) - 1);
                    env.endpoint[sizeof(env.endpoint) - 1] =
                        '\0'; // Ensure null-termination
                }
            } else if (strcmp(token, "info_endpoint") == 0) {
                token = strtok(NULL, delim);
                if (token != NULL) {
                    strncpy(env.info_endpoint, token,
                            sizeof(env.info_endpoint) - 1);
                    env.info_endpoint[sizeof(env.info_endpoint) - 1] =
                        '\0'; // Ensure null-termination
                }
            } else if (strcmp(token, "running_model_endpoint") == 0) {
                token = strtok(NULL, delim);
                if (token != NULL) {
                    strncpy(env.running_model_endpoint, token,
                            sizeof(env.info_endpoint) - 1);
                    env.info_endpoint[sizeof(env.running_model_endpoint) - 1] =
                        '\0'; // Ensure null-termination
                }
            }
        }
    }

    fclose(fptr);
    return env;
}

void removeSpaces(char *str) {
    int count = 0; // Zähler für nicht-Leerzeichen

    // Durchlaufen des Strings
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ') {
            str[count++] = str[i]; // Verschieben des Zeichens nach vorne
        }
    }

    str[count] = '\0'; // Neues Ende des Strings setzen
}
