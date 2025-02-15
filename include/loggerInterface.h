
// logger.h
#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

typedef struct Logger Logger;

Logger* Logger_create(const char* filename);
void Logger_destroy(Logger* logger);
void Logger_log(Logger* logger, const char* message);

#endif

