#include "../include/checkArgument.h"
#include "../include/arguments/help.h"
#include "../include/arguments/info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Für strcmp

const char *HELP = "--help";
const char *H = "-h";
const char *VERSION = "--version";
const char *V = "-v";
const char *INFO = "--info";
const char *I = "-i";
const char *RUNNINGMODEL = "--running-model";
const char *R = "-r";

void checkArgument(char *argument) {
    if (strcmp(argument, HELP) == 0 || strcmp(argument, H) == 0) {
        help();
    } else if (strcmp(argument, VERSION) == 0 || strcmp(argument, V) == 0) {
        // version();
    } else if (strcmp(argument, INFO) == 0 || strcmp(argument, I) == 0) {
        get_info();
    } else {
        printf("kailian: Unknown argument\n");
    }
    exit(0);
}
