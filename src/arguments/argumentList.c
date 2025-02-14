#include "../../include/arguments/argumentList.h"
#include <stdlib.h>

Blaupause argument_blaupause = {
    .name = NULL, .long_form = NULL, .short_form = NULL};

Help argument_help = {
    .name = "help", .long_form = "--help", .short_form = "-h"};

Info argument_info = {
    .name = "info", .long_form = "--info", .short_form = "-i"};

Model argument_model = {
    .name = "model", .long_form = "--model", .short_form = "-m"};

ShowEnironment argument_showEnvironment = {.name = "showEnvironment",
                                           .long_form = "--show-environment",
                                           .short_form = "-e"};
