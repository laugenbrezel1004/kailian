#include "../include/checkArgument.h"
#include "../include/arguments/Environment.h"
#include "../include/arguments/argumentList.h"
#include "../include/arguments/coffee.h"
#include "../include/arguments/help.h"
#include "../include/arguments/startServer.h"
#include "../include/call_api.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int matchesArgument(const char *argument, const Argument *compare) {
    // maybe with strncmp to make code better
    return (strcmp(argument, compare->long_form) == 0 ||
            strcmp(argument, compare->short_form) == 0);
}

int checkArgument(const char *argument) {
    if (matchesArgument(argument, &arguments.help)) {
        help();
    } else if (matchesArgument(argument, &arguments.info)) {
        connectToAi(NULL, NULL, arguments.info.long_form);
    } else if (matchesArgument(argument, &arguments.model)) {
        connectToAi(NULL, NULL, arguments.model.long_form);
    } else if (matchesArgument(argument, &arguments.showEnvironment)) {
        showEnvironment();
    } else if (matchesArgument(argument, &arguments.showModels)) {
        connectToAi(NULL, NULL, arguments.showModels.long_form);
    } else if (matchesArgument(argument, &arguments.coffee)) {
        coffee();
    } else if (matchesArgument(argument, &arguments.startOllama)) {
        startServer();
    } else if (matchesArgument(argument, &arguments.createConfig)) {
        return createConfig();
    } else if (matchesArgument(argument, &arguments.chat)) {
        return 2;
    } else if (matchesArgument(argument, &arguments.killOllama)) {
        return killServer();
    }

    else {
        fprintf(stderr,
                "kailian: Unknown argument\nTry 'kailian --help' for more "
                "information\n");
    }
    return 1;
}
