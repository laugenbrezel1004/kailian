// argumentList.c
#include "../../include/arguments/argumentList.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Globale Argumentliste
Arguments arguments = {
    .askError = {"AskError", NULL, NULL},
    .help = {"help", "--help", "-h"},
    .info = {"info", "--info", "-i"},
    .showCurrentModel = {"showCurrentModel", "--current-model", "-m"},
    .showEnvironment = {"showEnvironment", "--show-environment", "-e"},
    .showModels = {"showModels", "--show-models", "-s"},
    .coffee = {"coffee", "--coffee", "-C"},
    .startOllama = {"startOllama", "--start-ollama", "-o"},
    .killOllama = {"killOllama", "--kill-ollama", "-k"},
    .createConfig = {"createConfig", "--create-config", "-c"},
    .chat = {"chat", "--chat", "-p"},
    .cleanChat = {"cleanChat", "--clean-chat", "-t"}};

// Optional: Funktion zur Validierung der Argumentliste
void validateArguments(void) {
    // Sicherstellen, dass alle long_form und short_form gesetzt sind
    const Argument *args[] = {&arguments.help,
                              &arguments.info,
                              &arguments.showCurrentModel,
                              &arguments.showEnvironment,
                              &arguments.showModels,
                              &arguments.coffee,
                              &arguments.startOllama,
                              &arguments.killOllama,
                              &arguments.createConfig,
                              &arguments.chat,
                              &arguments.cleanChat};
    for (size_t i = 0; i < sizeof(args) / sizeof(args[0]); i++) {
        if (!args[i]->long_form || !args[i]->short_form) {
            fprintf(stderr,
                    "Argument '%s' is missing long_form or short_form\n",
                    args[i]->name);
            exit(1);
        }
    }
}
