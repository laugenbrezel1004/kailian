#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>

void log_error(const char *module, const char *message);
void log_error_with_errno(const char *module, const char *message);
void log_info(const char *module, const char *message);

#endif // ERROR_HANDLING_H 