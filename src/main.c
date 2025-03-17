// main.c
#define _POSIX_C_SOURCE 200809L
#include "../include/checkArgument.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FILE_SIZE 1048576 // 1 MB

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
    if (argc < 2) {
        fprintf(stderr, "kailian: Too few arguments\nTry 'kailian --help'\n");
        return 1;
    }

    char *file_buffer = NULL;
    if (!isatty(STDIN_FILENO)) {
        file_buffer = readStdin(MAX_FILE_SIZE);
        if (!file_buffer) {
            return 1;
        }
    }

    int result = checkArgument(argc - 1, &argv[1], file_buffer);
    free(file_buffer);
    return result;
}
