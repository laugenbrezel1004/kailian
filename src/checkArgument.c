
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

int matchesArgument(const char *argument, const Arguments *compare) {
    return (strcmp(argument, compare->long_form) == 0 ||
            strcmp(argument, compare->short_form) == 0);
}

void checkArgument(const char *argument) {
    if (matchesArgument(argument, (Arguments *)arguments.help)) {
        help();
    } else if (matchesArgument(argument, arguments.info)) {
        sendArgument(arguments.info.long_form);
    } else if (matchesArgument(argument, arguments.model)) {
        sendArgument(arguments.model.long_form);
    } else if (matchesArgument(argument, arguments.showEnvironment)) {
        showEnvironment();
    } else if (matchesArgument(argument, arguments.showModels)) {
        sendArgument(arguments.showModels.long_form);
    } else if (matchesArgument(argument, arguments.coffee)) {
        coffee();
    } else {
        fprintf(stderr,
                "kailian: Unknown argument\nTry 'kailian --help' for more "
                "information\n");
    }
}
