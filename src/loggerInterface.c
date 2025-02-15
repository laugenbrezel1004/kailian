#include "../include/loggerInterface.h"
#include "logger.h"
#include "loggerconf.h"

// debug logik
static void debugConsole(const char *log_info) {
    logger_initConsoleLogger(stderr);
    logger_setLevel(LogLevel_DEBUG);
    LOG_INFO(log_info);
}

// strukt implementieren
Log initLogger() {
    Log logger = {.debugConsole = debugConsole};
    return logger;
};
