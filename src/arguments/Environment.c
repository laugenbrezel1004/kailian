// Environment.c
#include "../../include/arguments/Environment.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_PATH "/etc/kailian/kailian.conf"
static const char *templateConfig =
    "name = chatgpt\n"
    "endpoint_generate = http://localhost:11434/api/generate\n"
    "endpoint_info = http://localhost:11434/api/tags\n"
    "endpoint_running_model = http://localhost:11434/api/ps\n"
    "endpoint_ollama_version = http://localhost:11434/api/version\n"
    "endpoint_chat = http://localhost:11434/api/chat\n"
    "endpoint_show = http://localhost:11434/api/show\n"
    "system = \"Do not answer in markdown\"";
/**
 * @brief Zeigt den Inhalt der Konfigurationsdatei an.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */
int showEnvironment(void) {
    FILE *fp = fopen(CONFIG_PATH, "r");
    if (!fp) {
        fprintf(stderr,
                "kailian: %s: %s\nCreate a new one with --create-config\n",
                CONFIG_PATH, strerror(errno));
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }

    fclose(fp);
    return 0;
}

int createConfig(void) {
    int fd = open(CONFIG_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "kailian: Failed to create %s: %s\n", CONFIG_PATH,
                strerror(errno));
        return 1;
    }

    size_t len = strlen(templateConfig);
    ssize_t written = write(fd, templateConfig, len);
    if (written < 0 || (size_t)written != len) {
        fprintf(stderr, "kailian: Failed to write %s: %s\n", CONFIG_PATH,
                strerror(errno));
        close(fd);
        return 1;
    }

    close(fd);
    printf("kailian: Config created at %s\n", CONFIG_PATH);
    return 0;
}
