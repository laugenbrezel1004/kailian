#include "../../include/arguments/argumentList.h"
#include <stdio.h>
#include <stdlib.h>

int help() {
    fprintf(stdout, "Usage: \n\tkailian [ARGUMENT]\n\tkailian "
                    "[YOUR_QUESTION]\n\n\n");
    /*fprintf(*/
    /*    stdout,*/
    /*    "%s\t\t\tJust call kailian without anything. He than looks up the last
     * "*/
    /*    "shellcommand to provide you some feedback.\n",*/
    /*    arguments.help.name);*/
    fprintf(stdout, "%s,\t%s\t\t\tget the help side\n",
            arguments.help.short_form, arguments.help.long_form);
    fprintf(stdout, "%s,\t%s\t\t\tget generall infos\n",
            arguments.info.short_form, arguments.info.long_form);
    fprintf(stdout,
            "%s,\t%s\t\t\tget running model info -> Ask a question if 'model' "
            "is empty\n",
            arguments.model.short_form, arguments.model.long_form);
    fprintf(stdout, "%s,\t%s\tshow the current environment settings\n",
            arguments.showEnvironment.short_form,
            arguments.showEnvironment.long_form);
    fprintf(stdout, "%s,\t%s\t\tshow the available models\n",
            arguments.showModels.short_form, arguments.showModels.long_form);
    fprintf(stdout, "%s,\t%s\t\tcoffee\n", arguments.coffee.short_form,
            arguments.coffee.long_form);
    fprintf(stdout, "%s,\t%s\t\tstart local ollama instance\n",
            arguments.startOllama.short_form, arguments.startOllama.long_form);
    fprintf(stdout, "%s,\t%s\t\tkill local ollama instance\n\n",
            arguments.killOllama.short_form, arguments.killOllama.long_form);
    fprintf(stdout,
            "Exaple: \t\t\tman man | kailian explain me the man command\n");
    exit(0);

    /*return EXIT_FAILURE;*/
}
