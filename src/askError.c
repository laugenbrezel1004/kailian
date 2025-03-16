/*// askError.c*/
/*#include "../include/askError.h"*/
/*#include <readline/history.h>*/
/*#include <readline/readline.h>*/
/*#include <stdio.h>*/
/**/
/*/***/
/* * @brief Fragt den Benutzer nach einem Fehler und zeigt die History an.*/
/* * @return int 0 bei Erfolg, 1 bei Fehler.*/
/* */
/*int ask_error(void) {*/
/*    using_history();*/
/**/
/*    char *input;*/
/*    while ((input = readline("Enter a command: ")) != NULL) {*/
/*        if (*input) {*/
/*            add_history(input);*/
/*            printf("You entered: %s\n", input);*/
/*        }*/
/*        free(input);*/
/*    }*/
/**/
/*    printf("\nCommand History:\n");*/
/*    HIST_ENTRY **hist = history_list();*/
/*    if (hist) {*/
/*        for (int i = 0; hist[i]; i++) {*/
/*            printf("%d: %s\n", i + 1, hist[i]->line);*/
/*        }*/
/*    }*/
/*    return 0;*/
/*}*/
