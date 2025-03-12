#include <asm-generic/errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *configFile = "/etc/kailian/kailian.conf";
const char *templateConfig = "name = YOURAI \n\
endpoint = http://localhost:11434/api/generate \n\
info_endpoint = http://localhost:11434/api/tags \n\
running_model_endpoint = http://localhost:11433/api/ps \n\
ollama_version_endpoint = http://localhost:11434/api/version \n\
system = \"You are a linux admin who answers correctly and without markdown \"";

int showEnvironment(void) {

    FILE *ptr = fopen(configFile, "r");
    if (ptr == NULL) {
        fprintf(stderr, "Config's gone! -> /etc/kailian/kailian.conf: create a "
                        "new one with --create-config\n");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), ptr)) {
        printf("%s", buffer);
    }

    fclose(ptr);
    return 0;
}

int createConfig(void) {
    int fd_configFile = open(configFile, O_WRONLY | O_CREAT, 0644);
    if (fd_configFile < 0) {
        fprintf(stderr, "open() syscall failed\n");
    }
    /*printf("buffer -> %s", templateConfig);*/
    write(fd_configFile, templateConfig, strlen(templateConfig));
    close(fd_configFile);

    return 0;
}
