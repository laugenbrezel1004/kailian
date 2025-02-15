#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H


typedef struct {
    void (*debugConsole)(const char *log_info);
} Log;

Log initLogger();
/*void debugConsole(const char *log_info);*/
#endif // !LOGGERINTERFACE_H
