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

    while ((read = getline(&line, &len, stdin)) != -1) {
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
        fileBuffer[fileSize] = '\0'; // Keep newlines in place
    }
    free(line);
    return fileBuffer;
}
int main(int argc, char *argv[]) {
    char *fileBuffer = NULL;
    if (isatty(0) == 0) { // If piped input (e.g., tree | ./kailian)
        fileBuffer = readStdin();
    }

    if (argc < 2) {
        fprintf(stderr, "Too few arguments!\n");
        free(fileBuffer);
        return 1;
    }

    // Build prompt from argv
    size_t prompt_len = 0;
    for (int i = 1; i < argc; i++) {
        prompt_len += strlen(argv[i]) + 1; // Space between args
    }
    char *promptBuffer = malloc(prompt_len);
    if (!promptBuffer) {
        fprintf(stderr, "malloc failed for promptBuffer\n");
        free(fileBuffer);
        return 1;
    }
    promptBuffer[0] = '\0';
    for (int i = 1; i < argc; i++) {
        strcat(promptBuffer, argv[i]);
        if (i < argc - 1)
            strcat(promptBuffer, " ");
    }

    int result = connectToKi(promptBuffer, fileBuffer);
    free(promptBuffer);
    free(fileBuffer);
    return result;
}
