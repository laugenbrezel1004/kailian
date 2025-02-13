#include <stdio.h>
#include <stdlib.h>

int help() {
    printf("Help is there\n");
    fprintf(stdout,
            "Usage: \n\tkailian [ARGUMENT]\n\tkailian [YOUR_QUESTION]\n\n\n");
    fprintf(stdout, "-h\t --help\t\t get the help side\n");
    fprintf(stdout, "-v\t --version\t get the version\n");
    fprintf(stdout, "-i\t --info\t\t get generall infos\n");
    fprintf(stdout,
            "-r\t --model\t get running model info -> Ask a question if "
            "'model' is empty\n");

    exit(0);

    /*return EXIT_FAILURE;*/
}
