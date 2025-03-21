#include "../../include/utils/string_utils.h"
#include <string.h>

StringResult create_string_from_args(int argc, char *argv[]) {
    StringResult result = {NULL, SUCCESS};
    
    if (!argv || argc < 1) {
        result.error = ERROR_INVALID_INPUT;
        return result;
    }

    size_t total_len = 0;
    for (int i = 0; i < argc; i++) {
        if (!argv[i]) {
            result.error = ERROR_INVALID_INPUT;
            return result;
        }
        total_len += strlen(argv[i]) + 1;
    }

    char *buffer = calloc(total_len, sizeof(char));
    if (!buffer) {
        result.error = ERROR_MEMORY_ALLOCATION;
        return result;
    }

    char *ptr = buffer;
    for (int i = 0; i < argc; i++) {
        size_t remaining = total_len - (ptr - buffer);
        int written = snprintf(ptr, remaining, "%s%s", 
                             argv[i], (i < argc - 1) ? " " : "");
        if (written < 0 || written >= (int)remaining) {
            free(buffer);
            result.error = ERROR_INVALID_INPUT;
            return result;
        }
        ptr += written;
    }

    result.content = buffer;
    return result;
} 