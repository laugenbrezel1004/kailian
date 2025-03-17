// createEnvironmentConfig.c
#include "../../include/arguments/createEnvironmentConfig.h"
#include "../../include/environmentSettings.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Zeigt den Inhalt der Konfigurationsdatei an.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */

int createConfig(void) {
    int fd = open(configPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "kailian: Failed to create %s: %s\n", configPath,
                strerror(errno));
        return 1;
    }

    size_t len = strlen(templateConfig);
    ssize_t written = write(fd, templateConfig, len);
    if (written < 0 || (size_t)written != len) {
        fprintf(stderr, "kailian: Failed to write %s: %s\n", configPath,
                strerror(errno));
        close(fd);
        return 1;
    }

    close(fd);
    printf("kailian: Config created at %s\n", configPath);
    return 0;
}
