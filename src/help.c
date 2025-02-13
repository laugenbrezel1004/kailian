#include <stdio.h>
#include <stdlib.h>

int help() {
    printf("Help is there\n");
    fprintf(stdout, "Usage: kailian [QUESTION]\n\n");
    fprintf(stdout, "-h\t --help\t\t get the help side\n");
    fprintf(stdout, "-v\t --version\t get the version\n");
    fprintf(stdout, "-i\t --info\t\t get model infos\n");
    exit(0);
    /*return EXIT_FAILURE;*/
}
