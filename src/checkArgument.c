#include "../include/checkArgument.h"
#include "../include/arguments/help.h"
#include "../include/call_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Für strcmp
#include <sys/socket.h>

const char *HELP = "--help";
const char *H = "-h";
const char *VERSION = "--version";
const char *V = "-v";
const char *INFO = "--info";
const char *I = "-i";
const char *MODEL = "--model";
const char *R = "-r";

void checkArgument(char *argument) {
    if (strcmp(argument, HELP) == 0 || strcmp(argument, H) == 0) {
        help();
    } else if (strcmp(argument, VERSION) == 0 || strcmp(argument, V) == 0) {
        // version();
        ;
    } else if (strcmp(argument, INFO) == 0 || strcmp(argument, I) == 0) {
        sendArgument(INFO);
    } else if (strcmp(argument, MODEL) == 0 || strcmp(argument, R) == 0) {
        sendArgument(MODEL);
    } else {
        printf("kailian: Unknown argument\nTry 'kailian --help' for more "
               "information");
    }
    exit(0);
}
