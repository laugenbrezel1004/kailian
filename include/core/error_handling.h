#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_INPUT = -1,
    ERROR_MEMORY_ALLOCATION = -2,
    ERROR_UNKNOWN_ARGUMENT = -3,
    ERROR_HANDLER_FAILED = -4,
    ERROR_INITIALIZATION_FAILED = -5
} ErrorCode;

// Fehlerbehandlungs-Kontext
typedef struct {
    ErrorCode code;
    char message[256];
    const char *file;
    int line;
} ErrorContext;

void log_error(const ErrorContext *ctx);
ErrorContext create_error(ErrorCode code, const char *message, const char *file, int line);

#define RETURN_ERROR(code, message) \
    do { \
        ErrorContext ctx = create_error(code, message, __FILE__, __LINE__); \
        log_error(&ctx); \
        return code; \
    } while(0)

#endif 