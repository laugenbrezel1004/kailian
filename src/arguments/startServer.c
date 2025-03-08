#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void startServer() {
    pid_t pid;

    // first fork
    pid = fork();
    if (pid < 0) {
        perror("fork syscall went wrong");
        exit(1);
    }
    if (pid > 0) {
        exit(0); // kill parentprocess
    }

    // create new sesssion
    if (setsid() < 0) {
        perror("setsid went wrong");
        exit(1);
    }

    // second fork
    pid = fork();
    if (pid < 0) {
        perror("fork syscall went wrong");
        exit(1);
    }
    if (pid > 0) {
        exit(0); // kill second parentprocess
    }

    // change working directory
    chdir("/");

    // redirect fd's
    /*close(STDIN_FILENO);*/
    /*open("/dev/null", O_RDWR); // STDIN to /dev/null*/
    /*int log_fd = open("/run/ollama.log", O_WRONLY | O_CREAT | O_APPEND,
     * 0644);*/
    /*if (log_fd < 0) {*/
    /*    fprintf(stderr, "unable to write to ollama.log");*/
    /*    exit(1);*/
    /*}*/
    /*dup2(log_fd, STDOUT_FILENO);*/
    /*dup2(log_fd, STDERR_FILENO);*/
    /*close(log_fd);*/

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDWR); // STDIN == 0 smallest free fd
    dup(0); // STDOUT copies 0 and creates new smallest fd (1) which points to 0
    dup(0); // STDERR copies 0 and creates new smallest fd (2) which points to 0

    // start "ollama serve"
    execl("/usr/local/bin/ollama", "ollama", "serve", (char *)NULL);

    // something weng wrong
    perror("execl fehlgeschlagen");
    exit(1);
}
int killServer() {
    return execl("/usr/bin/killall", "/usr/bin/killall", "ollama");
}
