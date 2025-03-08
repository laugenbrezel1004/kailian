
// argumentList.c
#include "../../include/arguments/argumentList.h"
#include <stdlib.h>

// Define and initialize the global variable for arguments
Arguments arguments = {
    .askError = {.name = "AskError", .long_form = NULL, .short_form = NULL},
    .help = {.name = "help", .long_form = "--help", .short_form = "-h"},
    .info = {.name = "info", .long_form = "--info", .short_form = "-i"},
    .model = {.name = "model", .long_form = "--model", .short_form = "-m"},
    .showEnvironment = {.name = "showEnvironment",
                        .long_form = "--show-environment",
                        .short_form = "-e"},
    .showModels = {.name = "showModels",
                   .long_form = "--show-models",
                   .short_form = "-s"},
    .coffee = {.name = "coffee", .long_form = "--coffee", .short_form = "-c"},
    .startOllama = {.name = "startOllama",
                    .long_form = "--start-ollama",
                    .short_form = "-o"},
    .killOllama = {.name = "killOllama",
                   .long_form = "--kill-ollama",
                   .short_form = "-k"}};
