#include "../include/askError.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"
#include "../include/loggerInterface.h"
#include <ctype.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

#define MAX_FILE_SIZE 1048576 // 1 MB

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);
    return content;
}

char *create_json_body(const char *file_content) {
    size_t content_len = strlen(file_content);
    char *json_body = malloc(content_len * 2 + 200); // Großzügige Schätzung
    char *p = json_body;

    p += sprintf(
        p, "{\"model\":\"deepseek-r1:1.5b\",\"prompt\":\"Analyze this "
           "file:\",\"system\":\"You are a helpful "
           "assistant.\",\"stream\":false,\"options\":{\"file_content\":\"");

    for (size_t i = 0; i < content_len; i++) {
        switch (file_content[i]) {
        case '\\':
            *p++ = '\\';
            *p++ = '\\';
            break;
        case '"':
            *p++ = '\\';
            *p++ = '"';
            break;
        case '\b':
            *p++ = '\\';
            *p++ = 'b';
            break;
        case '\f':
            *p++ = '\\';
            *p++ = 'f';
            break;
        case '\n':
            *p++ = '\\';
            *p++ = 'n';
            break;
        case '\r':
            *p++ = '\\';
            *p++ = 'r';
            break;
        case '\t':
            *p++ = '\\';
            *p++ = 't';
            break;
        default:
            *p++ = file_content[i];
        }
    }

    strcpy(p, "\"}}");
    printf("%s", json_body);
    return json_body;
}
int main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (strncmp(argv[1], "--", 2) == 0 || strncmp(argv[1], "-", 1) == 0) {
            /*printf("Argument == %s\n", argv[1]);*/
            checkArgument(argv[1]);
        }

        char *buffer = malloc(sizeof(char));
        int strlength = 0;
        for (int i = 1; i < argc; i++) {
            strlength += strlen(argv[i]);
        }
        strlength += argc - 2; // Platz für Leerzeichen

        buffer = realloc(buffer, strlength + 1); // +1 für Null-Terminator
        if (buffer == NULL) {
            /*LOG("Error while realloc()");*/
            return EXIT_FAILURE;
        }

        buffer[0] = '\0'; // Initialisiere den String als leer
        for (int i = 1; i < argc; i++) {
            if (i > 1)
                strcat(
                    buffer,
                    " "); // Füge Leerzeichen hinzu, außer vor dem ersten Wort
            strcat(buffer, argv[i]);
        }

        connectToKi(buffer);
        free(buffer);
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}
