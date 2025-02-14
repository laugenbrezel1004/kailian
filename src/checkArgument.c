
// Standard-Bibliotheken
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// Projekt-spezifische Header
#include "../include/arguments/Environment.h"
#include "../include/arguments/argumentList.h"
#include "../include/arguments/help.h"
#include "../include/call_api.h"
#include "../include/checkArgument.h"

/**/
/*const char *HELP = "--help";*/
/*const char *H = "-h";*/
/*const char *VERSION = "--version";*/
/*const char *V = "-v";*/
/*const char *INFO = "--info";*/
/*const char *I = "-i";*/
/*const char *MODEL = "--model";*/
/*const char *R = "-r";*/
/*const char *SETMODEL = "--set-model";*/
/**/
void checkArgument(const char *argument) {
    if (strcmp(argument, argument_help.long_form) == 0 ||
        strcmp(argument, argument_help.short_form) == 0) {
        help();
    } /* else if (strcmp(argument, arguments[1].long_form) == 0 ||
                strcmp(argument, arguments[1].short_form) == 0) {
          version();};*/

    else if (strcmp(argument, argument_info.long_form) == 0 ||
             strcmp(argument, argument_info.short_form) == 0) {
        sendArgument(argument_info.long_form);
    } else if (strcmp(argument, argument_model.long_form) == 0 ||
               strcmp(argument, argument_model.short_form) == 0) {
        sendArgument(argument_model.long_form);
    } else {
        fprintf(stderr,
                "kailian: Unknown argument\nTry 'kailian --help' for more "
                "information");
    }
    exit(0);
}
