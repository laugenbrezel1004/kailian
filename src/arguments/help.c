#include "../../include/arguments/argumentList.h"
#include <stdio.h>
#include <stdlib.h>

int help() {
    fprintf(stdout,
            "Usage: \n\tkailian [ARGUMENT]\n\tkailian [YOUR_QUESTION]\n\n\n");
    fprintf(stdout, "%s,\t%s\t\t\tget the help side\n",
            argument_help.short_form, argument_help.long_form);
    fprintf(stdout, "%s,\t%s\t\t\tget generall infos\n",
            argument_info.short_form, argument_info.long_form);
    fprintf(stdout,
            "%s,\t%s\t\t\tget running model info -> Ask a question if 'model' "
            "is empty\n",
            argument_model.short_form, argument_model.long_form);
    fprintf(stdout, "%s,\t%s\tshow the current environment settings\n",
            argument_showEnvironment.short_form,
            argument_showEnvironment.long_form);
    fprintf(stdout, "%s,\t%s\t\tshow the available models\n",
            argument_showModels.short_form, argument_showModels.long_form);

    exit(0);

    /*return EXIT_FAILURE;*/
}
