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

    // 0, 1,2 are "dead"
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Get user ID
    uid_t userID = getuid();

    // Create the directory path (e.g., "/run/user/1234")
    char dir_path[256];
    snprintf(dir_path, sizeof(dir_path), "/run/user/%u/kailian.log", userID);
    int log_fd = open(dir_path, O_WRONLY | O_CREAT,
                      0644); // log_fd -> 0
    if (log_fd < 0) {
        perror("Failed to open log file");
        exit(1);
    }

    dup2(log_fd, STDERR_FILENO); // 2 -> kailian.log
    close(log_fd);               // kill 0 (stdin)
    // start "ollama serve"
    execl("/usr/local/bin/ollama", "ollama", "serve", (char *)NULL);

    // something weng wrong
    perror("execl fehlgeschlagen");
    exit(1);
}
int killServer() {
    return execl("/usr/bin/killall", "/usr/bin/killall", "ollama");
}
