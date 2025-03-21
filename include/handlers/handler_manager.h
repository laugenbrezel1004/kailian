#ifndef HANDLER_MANAGER_H
#define HANDLER_MANAGER_H

#include "../core/error.h"

typedef struct HandlerManager HandlerManager;

HandlerManager *handler_manager_create(void);
void handler_manager_destroy(HandlerManager *manager);
ErrorCode handler_manager_register(HandlerManager *manager, const ArgHandler *handler);
ErrorCode handler_manager_execute(HandlerManager *manager, const char *arg, const char *file_buffer);

#endif 