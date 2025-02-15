
// logger.h
#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

typedef struct {
    void (*debugConsole)(const char *log_info);
} Log;
Log initLogger();

#endif

