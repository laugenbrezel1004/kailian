// checkArgument.c
#include "../include/checkArgument.h"
#include "../include/arguments/Environment.h"
#include "../include/arguments/argumentList.h"
#include "../include/arguments/coffee.h"
#include "../include/arguments/help.h"
#include "../include/arguments/startServer.h"
#include "../include/call_api.h"
#include <stdio.h>
#include <string.h>

// Fehlercodes
enum ErrorCode { SUCCESS = 0, ERR_UNKNOWN_ARG = 1 };

/**
 * @brief Prüft, ob ein Argument mit einem bekannten Argument übereinstimmt.
 * @param argument Das zu prüfende Argument.
 * @param compare Das Argument aus der Argumentliste zum Vergleich.
 * @return int 1 wenn Übereinstimmung, sonst 0.
 */
int matchesArgument(const char *argument, const Argument *compare) {
    return (strcmp(argument, compare->long_form) == 0 ||
            strcmp(argument, compare->short_form) == 0);
}

/**
 * @brief Verarbeitet ein Kommandozeilenargument und ruft die passende Funktion
 * auf.
 * @param argument Das zu prüfende Argument.
 * @return int Fehlercode (0 für Erfolg).
 */
int checkArgument(const char *argument) {
    if (matchesArgument(argument, &arguments.help)) {
        return help();
    } else if (matchesArgument(argument, &arguments.coffee)) {
        return coffee();
    } else if (matchesArgument(argument, &arguments.showEnvironment)) {
        return showEnvironment();
    } else if (matchesArgument(argument, &arguments.startOllama)) {
        startServer();
        return SUCCESS; // Annahme: startServer beendet das Programm
    } else if (matchesArgument(argument, &arguments.killOllama)) {
        return killServer();
    } else if (matchesArgument(argument, &arguments.createConfig)) {
        return createConfig();
    } else if (matchesArgument(argument, &arguments.info)) {
        return connectToAi(NULL, NULL, arguments.info.long_form);
    } else if (matchesArgument(argument, &arguments.model)) {
        return connectToAi(NULL, NULL, arguments.model.long_form);
    } else if (matchesArgument(argument, &arguments.showModels)) {
        return connectToAi(NULL, NULL, arguments.showModels.long_form);
    } else {
        fprintf(stderr,
                "kailian: Unknown argument '%s'\nTry 'kailian --help' for more "
                "information\n",
                argument);
        return ERR_UNKNOWN_ARG;
    }
}
