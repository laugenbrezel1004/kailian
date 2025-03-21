#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>
#include "../core/error.h"

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} SafeString;

SafeString* string_create(const char *initial_value);
void string_destroy(SafeString **str);
ErrorCode string_append(SafeString *str, const char *append_value);
ErrorCode string_trim(SafeString *str);
ErrorCode string_format(SafeString *str, const char *format, ...);

#endif 