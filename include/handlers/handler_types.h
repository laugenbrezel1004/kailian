#ifndef HANDLER_TYPES_H
#define HANDLER_TYPES_H

#include "../core/error.h"

// Funktionstypen für Handler
typedef int (*ArgumentHandler)(void);
typedef ErrorCode (*ArgumentValidationFunc)(const char *);

// Handler-Struktur
typedef struct {
    const char *name;
    ArgumentHandler handler;
    const char *description;
    ArgumentValidationFunc validator;
} ArgHandler;

#endif /* HANDLER_TYPES_H */ 