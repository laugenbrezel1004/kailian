#ifndef ARGUMENT_CONTEXT_H
#define ARGUMENT_CONTEXT_H

// Kontext für die Argumentverarbeitung
typedef struct {
    const char *name;
    const char *value;
    const char *file_buffer;
    void *user_data;
} ArgumentContext;

#endif /* ARGUMENT_CONTEXT_H */ 