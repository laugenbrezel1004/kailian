#include "../../include/handlers/handler_manager.h"
#include <stdlib.h>

struct HandlerManager {
    ArgHandler *handlers;
    size_t capacity;
    size_t count;
};

HandlerManager *handler_manager_create(void) {
    HandlerManager *manager = calloc(1, sizeof(HandlerManager));
    if (!manager) {
        return NULL;
    }

    manager->capacity = 16;
    manager->handlers = calloc(manager->capacity, sizeof(ArgHandler));
    if (!manager->handlers) {
        free(manager);
        return NULL;
    }

    return manager;
}

void handler_manager_destroy(HandlerManager *manager) {
    if (manager) {
        free(manager->handlers);
        free(manager);
    }
}

ErrorCode handler_manager_register(HandlerManager *manager, const ArgHandler *handler) {
    if (!manager || !handler) {
        RETURN_ERROR(ERROR_INVALID_INPUT, "Ungültige Handler-Parameter");
    }

    if (manager->count >= manager->capacity) {
        size_t new_capacity = manager->capacity * 2;
        ArgHandler *new_handlers = reallocarray(manager->handlers, 
                                              new_capacity, 
                                              sizeof(ArgHandler));
        if (!new_handlers) {
            RETURN_ERROR(ERROR_MEMORY_ALLOCATION, "Handler-Array-Erweiterung fehlgeschlagen");
        }
        manager->handlers = new_handlers;
        manager->capacity = new_capacity;
    }

    manager->handlers[manager->count++] = *handler;
    return SUCCESS;
} 