// main.c

// includes
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

// Defines
#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

#define MAX_FILE_SIZE 1048576 // 1 MB

/**
 * @brief Reads input from stdin until EOF, with a size limit.
 * If the programm gets input from a a filedecriptor not terminal, than this
 * funciton gets called, which reads input and stores it as "fileBuffer".
 * @param max_size Maximum allowed size in bytes.
 * @return Pointer to the allocated buffer, or NULL on error.
 */
char *readStdin(size_t max_size) {
    char *fileBuffer = NULL;
    size_t fileSize = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        // Check if adding this line exceeds the maximum size
        if (fileSize + read > max_size) {
            fprintf(stderr, "Input exceeds maximum size of %zu bytes\n",
                    max_size);
            free(line);
            free(fileBuffer);
            return NULL;
        }

        // Reallocate memory to accommodate the new line
        char *temp = realloc(fileBuffer, fileSize + read + 1);
        if (!temp) {
            fprintf(stderr, "Error reallocing memory\n");
            free(line);
            free(fileBuffer);
            return NULL;
        }
        fileBuffer = temp;

        // Append the line to the buffer
        memcpy(fileBuffer + fileSize, line, read);
        fileSize += read;
        fileBuffer[fileSize] = '\0'; // Null-terminate, preserving newlines
    }

    free(line); // Free the line buffer allocated by getline
    return fileBuffer;
}

/**
 * @brief Main function.
 * Main function to process command-line arguments and piped input,
 * then connect to an AI service.
 * @param argc Count of passed arguments from the shell.
 * @param *argv[] Saves the different arguments passed from the shell.
 * @return The error code from kailian.
 */
int main(int argc, char *argv[]) {
    // Handle flags (e.g., -h or --help) when exactly one argument is provided
    if (argc == 2 &&
        (strncmp(argv[1], "--", 2) == 0 || strncmp(argv[1], "-", 1) == 0)) {
        return checkArgument(argv[1]);
    } else if (argc < 2) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stdout, "Try maybe \"-h\"\n");
        return 1;
    }
    // Read piped input if stdin is not a terminal (e.g., echo "test" |
    // ./program)
    char *fileBuffer = NULL;
    if (isatty(0) == 0) {
        fileBuffer = readStdin(MAX_FILE_SIZE);
        if (!fileBuffer) {
            return 1; // Exit with error if reading fails
        }
    }

    // Calculate the total length needed for the prompt (arguments + spaces)
    size_t prompt_len = 0;
    for (int i = 1; i < argc; i++) {
        prompt_len += strlen(argv[i]) + 1; // +1 for space or null terminator
    }

    // Allocate memory for the prompt
    char *promptBuffer = malloc(prompt_len);
    if (!promptBuffer) {
        fprintf(stderr, "malloc failed for promptBuffer\n");
        free(fileBuffer);
        return 1;
    }

    // Build the prompt efficiently using a pointer
    char *ptr = promptBuffer;
    for (int i = 1; i < argc; i++) {
        size_t arg_len = strlen(argv[i]);
        memcpy(ptr, argv[i], arg_len);
        ptr += arg_len;
        if (i < argc - 1) {
            *ptr = ' ';
            ptr++;
        }
    }
    *ptr = '\0'; // Null-terminate the prompt

    // Connect to the AI service and get the return value
    int returnValue = connectToAi(promptBuffer, fileBuffer, NULL);

    // Clean up allocated memory
    free(promptBuffer);
    free(fileBuffer);

    return returnValue;
}
