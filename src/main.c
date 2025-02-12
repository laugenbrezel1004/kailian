
#include "../include/api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    char *buffer = malloc(sizeof(char));
    if (argc > 1) {
        int strlength = 0;
        for (int i = 1; i < argc; i++) {
            strlength += strlen(argv[i]);
        }
        strlength += argc - 2; // Platz für Leerzeichen

        buffer = realloc(buffer, strlength + 1); // +1 für Null-Terminator
        if (buffer == NULL) {
            perror("Error while allocating memory");
            return EXIT_FAILURE;
        }

        buffer[0] = '\0'; // Initialisiere den String als leer
        for (int i = 1; i < argc; i++) {
            if (i > 1)
                strcat(
                    buffer,
                    " "); // Füge Leerzeichen hinzu, außer vor dem ersten Wort
            strcat(buffer, argv[i]);
        }

        printf("länge = %d\n", strlength);
        printf("%s\n", buffer);
    }
    connectToKi(buffer);
    free(buffer);
    return EXIT_SUCCESS;
}
