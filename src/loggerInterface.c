
// logger.c
#include "../include/loggerInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Logger {
        FILE *file;
};

Logger *Logger_create(const char *filename) {
    Logger *logger = malloc(sizeof(Logger));
    logger->file = fopen(filename, "a");
    return logger;
}

void Logger_destroy(Logger *logger) {
    if (logger) {
        fclose(logger->file);
        free(logger);
    }
}

void Logger_log(Logger *logger, const char *message) {
    time_t now;
    time(&now);
    fprintf(logger->file, "[%s] %s\n", ctime(&now), message);
    fflush(logger->file);
}
