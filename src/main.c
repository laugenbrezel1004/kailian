#include "../include/arguments/help.h"
#include "../include/askError.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

#define MAX_FILE_SIZE 1048576 // 1 MB

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "To few arguments!");
        fprintf(stderr, "Usage: \n");
        help();
        return EXIT_FAILURE;
    }

    if (strncmp(argv[1], "--", 2) == 0 || strncmp(argv[1], "-", 1) == 0) {
        checkArgument(argv[1]);
    }
    // Calculate total length of arguments
    size_t strlength = 0;
    for (int i = 1; i < argc; i++) {
        strlength += strlen(argv[i]);
    }
    strlength += argc - 2; // Spaces between arguments

    if (strlength > MAX_FILE_SIZE) {
        fprintf(stderr, "Input exceeds max size (%d bytes)\n", MAX_FILE_SIZE);
        return EXIT_FAILURE;
    }

    // Allocate buffer directly
    char *buffer = malloc(strlength + 1); // +1 for null terminator
    if (!buffer) {
        MELDUNG("Failed to allocate memory for buffer");
        return EXIT_FAILURE;
    }
    buffer[0] = '\0';

    // Concatenate arguments with spaces
    for (int i = 1; i < argc; i++) {
        if (i > 1) {
            strcat(buffer, " ");
        }
        strcat(buffer, argv[i]);
    }

    // Send to API
    connectToKi(buffer);

    free(buffer);
    return EXIT_SUCCESS;
}
