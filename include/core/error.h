#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

typedef int32_t ErrorCode;

// Fehlercodes definieren
#define DECLARE_ERROR_CODES(X) \
    X(SUCCESS,                    0,  "Erfolg") \
    X(ERROR_INVALID_INPUT,       -1,  "Ungültige Eingabe") \
    X(ERROR_MEMORY_ALLOCATION,   -2,  "Speicherallokierung fehlgeschlagen") \
    X(ERROR_UNKNOWN_ARGUMENT,    -3,  "Unbekanntes Argument") \
    X(ERROR_HANDLER_FAILED,      -4,  "Handler fehlgeschlagen") \
    X(ERROR_INITIALIZATION_FAILED, -5, "Initialisierung fehlgeschlagen")

#define ERROR_ENUM(name, code, msg) name = code,
enum {
    DECLARE_ERROR_CODES(ERROR_ENUM)
};

// Fehlerkontext-Struktur
typedef struct {
    ErrorCode code;
    char message[256];
    const char *file;
    int line;
} ErrorContext;

// Funktionsdeklarationen
void log_error(const ErrorContext *ctx);
ErrorContext create_error(ErrorCode code, const char *message, const char *file, int line);

// Makro für Fehlerrückgabe
#define RETURN_ERROR(code, message) \
    do { \
        ErrorContext ctx = create_error(code, message, __FILE__, __LINE__); \
        log_error(&ctx); \
        return code; \
    } while(0)

#endif /* ERROR_H */ 