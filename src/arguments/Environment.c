
#include <asm-generic/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void changeEnvironment() {
    char model[100];
    const int BUFFER_SIZE = 1024;
    // what do you want to change
    /*fflush(stdin);*/
    /*fprintf(stdout, "What do you want to "*/
    /*                "change?\nname(1)\nendpoint(2)\ninfo_endpoint(3)\nrunning_"*/
    /*                "model_endpoint(4)\nollama_version_endpoint(5)\n");*/
    /*input = fgetc(stdin);*/
    /*switch (input) {*/
    /*case '1':*/
    /*    fprintf(stdout, "case 1");*/
    /*    break;*/
    /*}*/
    fflush(stdin);
    fprintf(stdout, "Enter the name of your model\n");
    if (fgets(model, sizeof(model), stdin) == NULL) {
        fprintf(stderr, "Non valid input");
        exit(EXIT_FAILURE);
    }

    /* File pointer to hold reference of input file */
    FILE *fPtr;
    FILE *fTemp;

    char buffer[BUFFER_SIZE];
    char newline[BUFFER_SIZE];
    int line, count;

    /*  Open all required files */
    fPtr = fopen(".env", "r");
    fTemp = fopen("replace.tmp", "w");

    /* fopen() return NULL if unable to open file in given mode. */
    if (fPtr == NULL || fTemp == NULL) {
        /* Unable to open file hence exit */
        fprintf(stderr, "\nUnable to open file.\n");
        fprintf(stderr,
                "Please check whether file exists and you have read/write "
                "privilege.\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Read line from source file and write to destination
     * file after replacing given line.
     */
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL) {

        /* If current line is line to replace */
        if (strncmp(buffer, "name", 4) == 0)
            fprintf(fTemp, "name = %s", model);
        else
            fputs(buffer, fTemp);
    }

    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);

    /* Delete original source file */
    remove(".env");

    /* Rename temporary file as original file */
    rename("replace.tmp", ".env");

    /*printf("\nSuccessfully replaced '%d' line with '%s'.", line, newline);*/

    fprintf(stdout, "Environment file updated successfully.\n");
}

void showEnvironment(void) {

    FILE *ptr = fopen(".env", "r");
    if (ptr == NULL) {
        fprintf(stderr, "Error opening the .env file\n");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), ptr)) {
        if (strncmp(buffer, "#", 1) == 0) {
            continue;
        }
        printf("%s", buffer);
    }

    fclose(ptr);
}
