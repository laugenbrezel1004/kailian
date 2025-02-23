#define _POSIX_C_SOURCE 200809L
#include "../include/arguments/help.h"
#include "../include/askError.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

#define MAX_FILE_SIZE 1048576 // 1 MB

char *readStdin() {
    char *fileBuffer = NULL;
    size_t fileSize = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_count = 0;

    while ((read = getline(&line, &len, stdin)) != -1) {
        line_count++;
        fprintf(stderr, "Line %d: %zd bytes: [%s]\n", line_count, read, line);
        // Trim newline from line
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }
        char *temp = realloc(fileBuffer, fileSize + read + 1);
        if (!temp) {
            fprintf(stderr, "Error reallocing memory\n");
            free(line);
            free(fileBuffer);
            exit(1);
        }
        fileBuffer = temp;
        memcpy(fileBuffer + fileSize, line, read);
        fileSize += read;
        fileBuffer[fileSize] = '\0'; // Null-terminate at the new end
        fprintf(stderr, "Buffer after line %d: [%s]\n", line_count, fileBuffer);
    }
    fprintf(stderr, "Total lines: %d, Final buffer: [%s]\n", line_count,
            fileBuffer ? fileBuffer : "NULL");
    free(line);
    printf("FileBuffer -> %s\n", fileBuffer ? fileBuffer : "NULL");
    return fileBuffer;
}
int main(int argc, char *argv[]) {
    // ungleich 0 ist true
    char *fileBuffer = NULL;
    // if stdin is not a tty, read the stdin (like when it is from a pipe)
    if (isatty(0) == 0) {
        fileBuffer = readStdin();
    }

    if (argc < 2) {
        fprintf(stderr, "To few arguments!\n");
        help();
        return EXIT_FAILURE;
    }

    // to make sure that something is in argv[1]
    if (argc == 2 &&
        (strncmp(argv[1], "--", 2) == 0 || strncmp(argv[1], "-", 1) == 0)) {
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
    char *promptbuffer = malloc(strlength + 1); // +1 for null terminator
    if (!promptbuffer) {
        MELDUNG("Failed to allocate memory for promptbuffer");
        free(promptbuffer);
        free(fileBuffer);
        return EXIT_FAILURE;
    }
    promptbuffer[0] = '\0';

    // Concatenate arguments with spaces
    for (int i = 1; i < argc; i++) {
        if (i > 1) {
            strcat(promptbuffer, " ");
        }
        strcat(promptbuffer, argv[i]);
    }

    // Send to API

    int result = connectToKi(promptbuffer, fileBuffer);
    if (result) {
        fprintf(stderr, "Something went wrong!!!\n");
        free(promptbuffer);
        free(fileBuffer);
        return EXIT_FAILURE;
    }

    free(promptbuffer);
    free(fileBuffer);

    /*free(buffer);*/
    return EXIT_SUCCESS;
}
