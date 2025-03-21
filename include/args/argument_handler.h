#ifndef ARGUMENT_HANDLER_H
#define ARGUMENT_HANDLER_H

#include <stdbool.h>

typedef enum {
    ARG_TYPE_FLAG,
    ARG_TYPE_PROMPT,
    ARG_TYPE_INVALID
} ArgumentType;

typedef struct {
    ArgumentType type;
    const char *value;
} ParsedArgument;

ParsedArgument parse_argument(const char *arg);
bool is_valid_argument(const char *arg);
const char *get_argument_description(const char *arg);

#endif // ARGUMENT_HANDLER_H 