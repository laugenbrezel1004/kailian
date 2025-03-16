// help.c
#include "../../include/arguments/help.h"
#include "../../include/arguments/argumentList.h"
#include <stdio.h>

/**
 * @brief Zeigt die Hilfeseite für kailian an.
 * @return int Immer 0 (Erfolg).
 */
int help(void) {
    printf("Usage:\n"
           "\tkailian [ARGUMENT]\n"
           "\tkailian [YOUR_QUESTION]\n\n"
           "Options:\n");
    printf("  %s, %-20s Get this help page\n", arguments.help.short_form,
           arguments.help.long_form);
    printf("  %s, %-20s Get general info\n", arguments.info.short_form,
           arguments.info.long_form);
    printf("  %s, %-20s Get running model info\n",
           arguments.showCurrentModel.short_form,
           arguments.showCurrentModel.long_form);
    printf("  %s, %-20s Show environment settings\n",
           arguments.showEnvironment.short_form,
           arguments.showEnvironment.long_form);
    printf("  %s, %-20s Show available models\n",
           arguments.showModels.short_form, arguments.showModels.long_form);
    printf("  %s, %-20s Display a coffee animation\n",
           arguments.coffee.short_form, arguments.coffee.long_form);
    printf("  %s, %-20s Start local Ollama instance\n",
           arguments.startOllama.short_form, arguments.startOllama.long_form);
    printf("  %s, %-20s Kill local Ollama instance\n",
           arguments.killOllama.short_form, arguments.killOllama.long_form);
    printf("  %s, %-20s Create a new config file\n",
           arguments.createConfig.short_form, arguments.createConfig.long_form);
    printf("  %s, %-20s Chat with context\n", arguments.chat.short_form,
           arguments.chat.long_form);
    printf("  %s, %-20s Clean chat context\n", arguments.cleanChat.short_form,
           arguments.cleanChat.long_form);
    printf("\nExample:\n"
           "\tman man | kailian explain me the man command\n");

    return 0; // Kein exit, damit die Funktion wiederverwendbar bleibt
}
