
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void changeEnvironment(const char *model) {

    FILE *ptr = fopen(".env", "r");
    if (ptr == NULL) {
        fprintf(stderr, "Error opening the .env file\n");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), ptr)) {
        printf("%s",
               buffer); // fgets behält den Zeilenumbruch, daher kein \n nötig
        if (strncmp(buffer, "name", 4) == 0) {
            printf("Found name: %s", buffer);
        }
    }

    fclose(ptr);

    // Hier können Sie den 'model' Parameter verwenden, z.B.:
    // printf("Received model: %s\n", model);
}

void showEnvironment(void) {

    FILE *ptr = fopen(".env", "r");
    if (ptr == NULL) {
        fprintf(stderr, "Error opening the .env file\n");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), ptr)) {
        if (strncmp(buffer, "#", 1) == 0) {
            continue;
        }
        printf("%s", buffer);
    }

    fclose(ptr);
}
