#include "../include/manageInput.h"
#include "../include/argumentList.h"
#include "../include/arguments/coffee.h"
#include "../include/arguments/createEnvironmentConfig.h"
#include "../include/arguments/help.h"
#include "../include/arguments/manageOllamaServer.h"
#include "../include/arguments/showEnvironmentConfig.h"
#include "../include/call_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Konstanten an den Anfang der Datei
#define MAX_HANDLERS 16
#define ERROR_INVALID_ARGS "kailian: Ungültige Argumente\n"
#define ERROR_UNKNOWN_ARG "kailian: Unbekanntes Argument '%s'\nVersuchen Sie 'kailian --help'\n"

// Funktionsprototypen am Anfang der Datei hinzufügen (nach den includes)
static char *buildPrompt(int argc, char *argv[]);
static int handle_flag_argument(const char *arg, const char *file_buffer);
static int handle_prompt_arguments(int argc, char *argv[], const char *file_buffer);

typedef enum { FLAG = 0, PROMPT = 1 } ArgType;

typedef struct {
        const Argument *arg;
        int (*handler)(void);
        ArgType type;
        const char *description;  // Hinzufügen von Dokumentation
} ArgHandler;

static const ArgHandler handlers[] = {
    {&arguments.help, help, FLAG, "Zeigt Hilfe an"},
    {&arguments.coffee, coffee, FLAG, "Zeigt Coffee-Animation"},
    {&arguments.showEnvironment, showEnvironment, FLAG, "Zeigt Umgebungskonfiguration an"},
    {&arguments.startOllama, startServer, FLAG, "Startet Ollama-Server"},
    {&arguments.killOllama, killServer, FLAG, "Beendet Ollama-Server"},
    {&arguments.createConfig, createConfig, FLAG, "Erstellt Konfiguration"},
    {&arguments.info, NULL, FLAG, "Zeigt Informationen an"},
    {&arguments.showCurrentModel, NULL, FLAG, "Zeigt aktuelles Modell an"},
    {&arguments.showModels, NULL, FLAG, "Zeigt verfügbare Modelle an"},
};

static int matchesArgument(const char *arg, const Argument *compare) {
    return !strcmp(arg, compare->long_form) ||
           !strcmp(arg, compare->short_form);
}

static char *buildPrompt(int argc, char *argv[]) {
    if (!argv || argc < 1) {
        return NULL;
    }

    size_t prompt_len = 0;
    for (int i = 0; i < argc; i++) {
        if (!argv[i]) {
            return NULL;
        }
        prompt_len += strlen(argv[i]) + 1;
    }

    char *prompt = calloc(prompt_len, sizeof(char));  // Nutze calloc für Nullinitialisierung
    if (!prompt) {
        return NULL;
    }

    char *ptr = prompt;
    for (int i = 0; i < argc; i++) {
        ptr += snprintf(ptr, prompt_len - (ptr - prompt), "%s%s", argv[i],
                        (i < argc - 1) ? " " : "");
    }
    return prompt;
}

int manageInput(int argc, char *argv[], const char *file_buffer) {
    if (!argv || argc < 1) {
        fprintf(stderr, ERROR_INVALID_ARGS);
        return 1;
    }

    // Validiere Argumente beim Start
    validateArguments();

    const bool is_flag = (argv[0][0] == '-');
    
    if (argc == 1 && is_flag) {
        return handle_flag_argument(argv[0], file_buffer);
    }

    return handle_prompt_arguments(argc, argv, file_buffer);
}

static int handle_flag_argument(const char *arg, const char *file_buffer) {
    if (!arg) {
        fprintf(stderr, "kailian: Ungültiges Flag-Argument\n");
        return 1;
    }

    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
        if (matchesArgument(arg, handlers[i].arg)) {
            if (handlers[i].type == FLAG) {
                return handlers[i].handler ? 
                       handlers[i].handler() : 
                       connectToAi(NULL, file_buffer, handlers[i].arg->long_form);
            }
        }
    }
    
    fprintf(stderr, ERROR_UNKNOWN_ARG, arg);
    return 1;
}

static int handle_prompt_arguments(int argc, char *argv[], const char *file_buffer) {
    if (!argv || argc < 1) {
        fprintf(stderr, "kailian: Ungültige Prompt-Argumente\n");
        return 1;
    }

    char *prompt = buildPrompt(argc, argv);
    if (!prompt) {
        return 1;
    }

    int result = connectToAi(prompt, file_buffer, NULL);
    free(prompt);
    return result;
}
