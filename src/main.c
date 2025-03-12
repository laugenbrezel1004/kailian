#define _POSIX_C_SOURCE 200809L
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

    // evtl option für automatisches erstellen von kailian.conf wenn nicht
    // vorhanden

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
    /*printf("Filebuffer -> %s\n", fileBuffer);*/
    return fileBuffer;
}
int main(int argc, char *argv[]) {

    int checkForChat = 0;
    int returnValue = 0;

    // to check for argument size
    if (argc < 2) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stdout, "Try maybe \"-h\"\n");
        return 1;
    }

    // to make sure that something is in argv[1] and check if it is a valid flag
    if (argc == 2 &&
        (strncmp(argv[1], "--", 2) == 0 || strncmp(argv[1], "-", 1) == 0)) {
        checkForChat = checkArgument(argv[1]);
    }

    // If piped input (e.g., tree | ./kailian)
    char *fileBuffer = NULL;
    if (isatty(0) == 0) {
        fileBuffer = readStdin();
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

    // checkArgument return "2" when the argument was "chat" with ai
    if (checkForChat == 2) {

        returnValue = connectToAiChat(promptBuffer, fileBuffer);
    } else {
        returnValue = connectToAi(promptBuffer, fileBuffer);
    }

    free(promptBuffer);
    free(fileBuffer);
    return returnValue;
}
