#include "../../include/arguments/showEnvironmentConfig.h"
#include "../../include/environmentSettings.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int showEnvironment(void) {
    FILE *fp = fopen(configPath, "r");
    if (!fp) {
        fprintf(stderr,
                "kailian: %s: %s\nCreate a new one with --create-config\n",
                configPath, strerror(errno));
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }

    fclose(fp);
    return 0;
}
