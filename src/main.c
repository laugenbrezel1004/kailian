// main.c
#define _POSIX_C_SOURCE 200809L
#include "../include/manageInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/core/memory.h"
#include "../include/core/config.h"
#include "../include/core/argument_processor.h"

#define MAX_FILE_SIZE 1048576 // 1 MB

/**
 * @brief Liest Eingaben von stdin bis EOF mit Größenbeschränkung.
 * @param max_size Maximale erlaubte Größe in Bytes.
 * @return char* Zeiger auf den allokierten Buffer oder NULL bei Fehler.
 */
static char *readStdin(size_t max_size) {
    if (max_size == 0) {
        fprintf(stderr, "Ungültige maximale Größe\n");
        return NULL;
    }

    char *fileBuffer = calloc(max_size + 1, 1); // Nutze calloc für Nullinitialisierung
    if (!fileBuffer) {
        perror("Speicherallokierung für fileBuffer fehlgeschlagen");
        return NULL;
    }

    size_t fileSize = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        if (fileSize + read > max_size) {
            fprintf(stderr, "Eingabe überschreitet maximale Größe von %zu Bytes\n", max_size);
            goto cleanup;
        }
        
        if (line) {
            memcpy(fileBuffer + fileSize, line, read);
            fileSize += read;
        }
    }

    if (line) {
        free(line);
    }
    return fileBuffer;

cleanup:
    free(line);
    free(fileBuffer);
    return NULL;
}

/**
 * @brief Hauptfunktion zur Verarbeitung von Argumenten und piped Input.
 * @param argc Anzahl der Kommandozeilenargumente.
 * @param argv Array der Kommandozeilenargumente.
 * @return int Fehlercode.
 */
int main(int argc, char *argv[]) {
    ErrorCode error;
    char *file_buffer = NULL;
    ArgumentProcessor *arg_processor = NULL;

    // Initialisiere Argument Processor
    arg_processor = argument_processor_create();
    if (!arg_processor) {
        fprintf(stderr, "Konnte Argument Processor nicht initialisieren\n");
        return 1;
    }

    // Verarbeite stdin wenn vorhanden
    if (!isatty(STDIN_FILENO)) {
        file_buffer = readStdin(MAX_FILE_SIZE);
    }

    // Verarbeite Argumente
    error = argument_processor_handle(arg_processor, argc, argv, file_buffer);

    // Cleanup
    argument_processor_destroy(arg_processor);
    free(file_buffer);

    return error == SUCCESS ? 0 : 1;
}
