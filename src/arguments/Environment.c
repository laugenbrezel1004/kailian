// Environment.c
#include "../../include/arguments/Environment.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_PATH "/etc/kailian/kailian.conf"
static const char *templateConfig =
    "name = YOURAI\n"
    "endpoint = http://localhost:11434/api/generate\n"
    "info_endpoint = http://localhost:11434/api/tags\n"
    "running_model_endpoint = http://localhost:11433/api/ps\n"
    "ollama_version_endpoint = http://localhost:11434/api/version\n"
    "system = \"You are a linux admin who answers correctly and without "
    "markdown\"\n";

/**
 * @brief Zeigt den Inhalt der Konfigurationsdatei an.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */
int showEnvironment(void) {
    FILE *ptr = fopen(CONFIG_PATH, "r");
    if (!ptr) {
        perror("Failed to open config file");
        fprintf(stderr, "Create a new one with --create-config\n");
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), ptr)) {
        printf("%s", buffer);
    }

    fclose(ptr);
    return 0;
}

/**
 * @brief Erstellt eine neue Konfigurationsdatei mit Standardwerten.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */
int createConfig(void) {
    int fd = open(CONFIG_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to create config file");
        return 1;
    }

    ssize_t written = write(fd, templateConfig, strlen(templateConfig));
    if (written < 0 || (size_t)written != strlen(templateConfig)) {
        perror("Failed to write config file");
        close(fd);
        return 1;
    }

    close(fd);
    printf("Config created at %s\n", CONFIG_PATH);
    return 0;
}
