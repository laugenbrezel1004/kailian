// main.c
#define _POSIX_C_SOURCE 200809L
#include "../include/askError.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FILE_SIZE 1048576 // 1 MB

// Fehlercodes
enum ErrorCode { SUCCESS = 0, ERR_MEMORY = 1, ERR_INPUT = 2, ERR_API = 3 };

/**
 * @brief Liest Eingaben von stdin bis EOF mit Größenbeschränkung.
 * @param max_size Maximale erlaubte Größe in Bytes.
 * @return char* Zeiger auf den allokierten Buffer oder NULL bei Fehler.
 */
static char *readStdin(size_t max_size) {
    char *fileBuffer = malloc(max_size + 1); // Direkt allokieren
    if (!fileBuffer) {
        perror("malloc failed for fileBuffer");
        return NULL;
    }
    size_t fileSize = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        if (fileSize + read > max_size) {
            fprintf(stderr, "Input exceeds maximum size of %zu bytes\n",
                    max_size);
            free(line);
            free(fileBuffer);
            return NULL;
        }
        memcpy(fileBuffer + fileSize, line, read);
        fileSize += read;
    }
    free(line);
    fileBuffer[fileSize] = '\0'; // Null-Terminierung
    return fileBuffer;
}

/**
 * @brief Hauptfunktion zur Verarbeitung von Argumenten und piped Input.
 * @param argc Anzahl der Kommandozeilenargumente.
 * @param argv Array der Kommandozeilenargumente.
 * @return int Fehlercode.
 */
int main(int argc, char *argv[]) {
    char *fileBuffer = NULL;
    char *promptBuffer = NULL;
    int returnValue = SUCCESS;

    // Argumente prüfen
    if (argc < 2) {
        fprintf(stderr, "%s: Too few arguments!\nTry 'kailian --help'\n",
                argv[0]);
        return ERR_INPUT;
    }

    if (argc == 2 && (argv[1][0] == '-' || strncmp(argv[1], "--", 2) == 0)) {
        return checkArgument(argv[1]);
    }

    // Piped Input lesen
    if (!isatty(STDIN_FILENO)) {
        fileBuffer = readStdin(MAX_FILE_SIZE);
        if (!fileBuffer) {
            return ERR_INPUT;
        }
    }

    // Prompt aus Argumenten zusammenbauen
    size_t prompt_len = 0;
    for (int i = 1; i < argc; i++) {
        prompt_len += strlen(argv[i]) + 1;
    }
    promptBuffer = malloc(prompt_len);
    if (!promptBuffer) {
        perror("malloc failed for promptBuffer");
        free(fileBuffer);
        return ERR_MEMORY;
    }

    char *ptr = promptBuffer;
    for (int i = 1; i < argc; i++) {
        size_t arg_len = strlen(argv[i]);
        memcpy(ptr, argv[i], arg_len);
        ptr += arg_len;
        if (i < argc - 1)
            *ptr++ = ' ';
    }
    *ptr = '\0';

    // API-Aufruf
    returnValue = connectToAi(promptBuffer, fileBuffer, NULL);

    // Aufräumen
    free(promptBuffer);
    free(fileBuffer);
    return returnValue;
}
