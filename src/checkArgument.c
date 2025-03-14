// checkArgument.c

// include
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

/**
 * @brief Function to make sure argument is valid.
 * This funciton is used by checkArgument() to compare a given
 * argument with the argument struck form argumentList.c to make
 * sure that the argument is valid.
 * @param *argument The given argument to check.
 * @param *compare The entity in the struckt to compare the argument with.
 * @return Integer with non-zero when argument fits
 */
int matchesArgument(const char *argument, const Argument *compare) {
    // maybe with strncmp to make code better
    return (strcmp(argument, compare->long_form) == 0 ||
            strcmp(argument, compare->short_form) == 0);
}

/**
 * @brief Loop throw arguments and call correct functions
 * This function uses matchesArgument() to check the different arguments.
 * It then calles the functions for the arguments, based on the output from
 * matchesArgument().
 * @param *argument The argument from the main().
 * @return Integer error code.
 */
int checkArgument(const char *argument) {
    if (matchesArgument(argument, &arguments.help)) {
        help();
    } else if (matchesArgument(argument, &arguments.coffee)) {
        coffee();
    } else if (matchesArgument(argument, &arguments.showEnvironment)) {
        showEnvironment();
    } else if (matchesArgument(argument, &arguments.startOllama)) {
        startServer();
    } else if (matchesArgument(argument, &arguments.killOllama)) {
        return killServer();
    } else if (matchesArgument(argument, &arguments.createConfig)) {
        return createConfig();
    } else if (matchesArgument(argument, &arguments.info)) {
        connectToAi(NULL, NULL, arguments.info.long_form);
    } else if (matchesArgument(argument, &arguments.model)) {
        connectToAi(NULL, NULL, arguments.model.long_form);
    } else if (matchesArgument(argument, &arguments.showModels)) {
        connectToAi(NULL, NULL, arguments.showModels.long_form);
    } else {
        fprintf(stderr,
                "kailian: Unknown argument\nTry 'kailian --help' for more "
                "information\n");
    }
    return 1;
}
