#include "../include/api.h"
#include "../include/checkArgument.h"
#include "../include/help.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

char *argumentstartDoubleDash = "--";
char *argumentstartSingelDash = "-";
int main(int argc, char *argv[]) {
    if (argc >= 2) {
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
            if ((argv[i] == argumentstartDoubleDash ||
                 argv[i] == argumentstartSingelDash) == 0) {
                /*checkArgument(argv[i]);*/
                checkArgument();
            }
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
    fprintf(stderr, "kailian: missing operand\n");
    return EXIT_FAILURE;
}
