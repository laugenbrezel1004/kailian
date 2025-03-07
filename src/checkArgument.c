
// Standard-Bibliotheken
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

// Projekt-spezifische Header
#include "../include/arguments/Environment.h"
#include "../include/arguments/argumentList.h"
#include "../include/arguments/coffee.h"
#include "../include/arguments/help.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"

int matchesArgument(const char *argument, const Blaupause *compare) {
    return (strcmp(argument, compare->long_form) == 0 ||
            strcmp(argument, compare->short_form) == 0);
}

void checkArgument(const char *argument) {
    if (matchesArgument(argument, (Blaupause *)&argument_help)) {
        help();
    } else if (matchesArgument(argument, (Blaupause *)&argument_info)) {
        sendArgument(argument_info.long_form);
    } else if (matchesArgument(argument, (Blaupause *)&argument_model)) {
        sendArgument(argument_model.long_form);
    } else if (matchesArgument(argument,
                               (Blaupause *)&argument_showEnvironment)) {
        showEnvironment();
    } else if (matchesArgument(argument, (Blaupause *)&argument_showModels)) {
        sendArgument(argument_showModels.long_form);
    } else if (matchesArgument(argument, (Blaupause *)&argument_coffee)) {
        coffee();
    } else {
        fprintf(stderr,
                "kailian: Unknown argument\nTry 'kailian --help' for more "
                "information\n");
    }
}
