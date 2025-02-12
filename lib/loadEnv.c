
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char name[100];
        char endpoint[100];
} Env;

Env readEnv() {
    Env env = {"", ""}; // Initialize with empty strings
    FILE *fptr;
    char line[256];           // Increased buffer size for safety
    const char delim[] = "="; // Use '=' as delimiter
    char *token;

    fptr = fopen(".env", "r");
    if (fptr == NULL) {
        perror("Error reading .env file");
        return env; // Return empty env if file can't be opened
    }

    while (fgets(line, sizeof(line), fptr)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = 0;

        token = strtok(line, delim);
        if (token != NULL) {
            if (strcmp(token, "name") == 0) {
                token = strtok(NULL, delim);
                if (token != NULL) {
                    strncpy(env.name, token, sizeof(env.name) - 1);
                }
            } else if (strcmp(token, "endpoint") == 0) {
                token = strtok(NULL, delim);
                if (token != NULL) {
                    strncpy(env.endpoint, token, sizeof(env.endpoint) - 1);
                }
            }
        }
    }

    fclose(fptr);
    return env;
}

/*int main() {*/
/*    Env env = readEnv();*/
/*    printf("Name: %s\n", env.name);*/
/*    printf("Endpoint: %s\n", env.endpoint);*/
/*    return 0;*/
/*}*/
