#include "../include/askError.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (strncmp(argv[1], "--", 2) == 0 || strncmp(argv[1], "-", 1) == 0) {
            /*printf("Argument == %s\n", argv[1]);*/
            checkArgument(argv[1]);
        }

        char *buffer = malloc(sizeof(char));
        int strlength = 0;
        for (int i = 1; i < argc; i++) {
            strlength += strlen(argv[i]);
        }
        strlength += argc - 2; // Platz für Leerzeichen

        buffer = realloc(buffer, strlength + 1); // +1 für Null-Terminator
        if (buffer == NULL) {
            fprintf(stderr, "Error while allocating memory");
            return EXIT_FAILURE;
        }

        buffer[0] = '\0'; // Initialisiere den String als leer
        for (int i = 1; i < argc; i++) {
            if (i > 1)
                strcat(
                    buffer,
                    " "); // Füge Leerzeichen hinzu, außer vor dem ersten Wort
            strcat(buffer, argv[i]);
            /*printf("%s", argv[i]);*/
        }

        /*printf("%s \n", buffer);*/
        connectToKi(buffer);
        free(buffer);
        return EXIT_SUCCESS;
    }
    ask_error();

    /*fprintf(stderr, "kailian: missing operand\nTry 'kailian --help' for more
     * "*/
    /*                "information\n");*/
    return EXIT_FAILURE;
}
