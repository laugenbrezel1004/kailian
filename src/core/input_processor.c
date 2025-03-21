#include "../../include/core/input_processor.h"
#include "../../include/handlers/handler_manager.h"

typedef struct {
    HandlerManager *handler_manager;
    ApplicationConfig config;
} InputProcessor;

static InputProcessor *processor = NULL;

ErrorCode initialize_input_processor(const char *config_path) {
    if (processor) {
        RETURN_ERROR(ERROR_INITIALIZATION_FAILED, "Prozessor bereits initialisiert");
    }

    processor = calloc(1, sizeof(InputProcessor));
    if (!processor) {
        RETURN_ERROR(ERROR_MEMORY_ALLOCATION, "Prozessor-Allokierung fehlgeschlagen");
    }

    ErrorCode error = load_config(config_path, &processor->config);
    if (error != SUCCESS) {
        free(processor);
        processor = NULL;
        return error;
    }

    processor->handler_manager = handler_manager_create();
    if (!processor->handler_manager) {
        free_config(&processor->config);
        free(processor);
        processor = NULL;
        RETURN_ERROR(ERROR_INITIALIZATION_FAILED, "Handler-Manager-Erstellung fehlgeschlagen");
    }

    return SUCCESS;
}

ErrorCode process_input(int argc, char *argv[], const char *file_buffer) {
    if (!processor) {
        RETURN_ERROR(ERROR_INITIALIZATION_FAILED, "Prozessor nicht initialisiert");
    }

    if (!argv || argc < 1) {
        RETURN_ERROR(ERROR_INVALID_INPUT, "Ungültige Eingabeparameter");
    }

    const bool is_flag = (argv[0][0] == '-');
    
    if (argc == 1 && is_flag) {
        return handler_manager_execute(processor->handler_manager, argv[0], file_buffer);
    }

    return process_prompt_arguments(argc, argv, file_buffer);
} 