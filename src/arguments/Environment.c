
#include <asm-generic/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void showEnvironment(void) {

    FILE *ptr = fopen("/etc/kailian/kailian.conf", "r");
    if (ptr == NULL) {
        fprintf(
            stderr,
            "kailian: /etc/kailian/kailian.conf: No such file or directory\n");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), ptr)) {
        printf("%s", buffer);
    }

    fclose(ptr);
}
