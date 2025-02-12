#include "../include/api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
int main(int argc, char *argv[]) {
    int strlength = 0;
    int count = 0;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            strlength += strlen(argv[i]);
            count++;
        }
        char *buffer = malloc(strlength + sizeof(char));
        if (buffer == NULL)
            perror("Error while allocating memory");

        for (int i = 0; i < count; i++) {
            strcpy(buffer, argv[i]);
        }

        printf("länge = %d", strlength);
        printf("%s", buffer);
    }
    connectToKi();
    return EXIT_SUCCESS;
}
