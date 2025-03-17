#include "../include/checkArgument.h"
#include "../include/argumentList.h"
#include "../include/arguments/coffee.h"
#include "../include/arguments/createEnvironmentConfig.h"
#include "../include/arguments/help.h"
#include "../include/arguments/manageOllamaServer.h"
#include "../include/call_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ErrorCode { SUCCESS = 0, ERR_UNKNOWN_ARG = 1, ERR_MEMORY = 2 };

typedef enum { FLAG, PROMPT } ArgType;

typedef struct {
        const Argument *arg;
        int (*handler)(void);
        ArgType type;
} ArgHandler;

static const ArgHandler handlers[] = {
    {&arguments.help, help, FLAG},
    {&arguments.coffee, coffee, FLAG},
    {&arguments.showEnvironment, showEnvironment, FLAG},
    {&arguments.startOllama, startServer, FLAG},
    {&arguments.killOllama, killServer, FLAG},
    {&arguments.createConfig, createConfig, FLAG},
    {&arguments.info, NULL, FLAG},
    {&arguments.showCurrentModel, NULL, FLAG},
    {&arguments.showModels, NULL, FLAG},
};

static int matchesArgument(const char *arg, const Argument *compare) {
    return !strcmp(arg, compare->long_form) ||
           !strcmp(arg, compare->short_form);
}

static char *buildPrompt(int argc, char *argv[]) {
    size_t prompt_len = 0;
    for (int i = 0; i < argc; i++) {
        prompt_len += strlen(argv[i]) + 1;
    }

    char *prompt = malloc(prompt_len);
    if (!prompt) {
        perror("kailian: malloc failed");
        return NULL;
    }

    char *ptr = prompt;
    for (int i = 0; i < argc; i++) {
        ptr += snprintf(ptr, prompt_len - (ptr - prompt), "%s%s", argv[i],
                        (i < argc - 1) ? " " : "");
    }
    return prompt;
}

int checkArgument(int argc, char *argv[], const char *file_buffer) {
    validateArguments();

    // Einzelnes Flag-Argument
    if (argc == 1) {
        for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
            if (matchesArgument(argv[0], handlers[i].arg)) {
                if (handlers[i].type == FLAG) {
                    if (handlers[i].handler) {
                        return handlers[i].handler();
                    }
                    return connectToAi(NULL, file_buffer,
                                       handlers[i].arg->long_form);
                }
            }
        }
        fprintf(stderr,
                "kailian: Unknown argument '%s'\nTry 'kailian --help'\n",
                argv[0]);
        return ERR_UNKNOWN_ARG;
    }

    // Mehrere Argumente als Prompt behandeln
    char *prompt = buildPrompt(argc, argv);
    if (!prompt) {
        return ERR_MEMORY;
    }

    int result = connectToAi(prompt, file_buffer, NULL);
    free(prompt);
    return result;
}
