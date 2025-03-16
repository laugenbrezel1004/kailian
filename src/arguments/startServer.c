// startServer.c
#include "../../include/arguments/startServer.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief Startet einen Ollama-Server als Daemon.
 * @note Beendet das Programm bei Erfolg oder Fehler.
 */
void startServer(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("First fork failed");
        exit(1);
    }
    if (pid > 0)
        exit(0); // Elternprozess beenden

    if (setsid() < 0) {
        perror("setsid failed");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("Second fork failed");
        exit(1);
    }
    if (pid > 0)
        exit(0);

    umask(0);   // Berechtigungen zurücksetzen
    chdir("/"); // Arbeitsverzeichnis auf Root setzen

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    int log_fd = open("/tmp/kailian.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Failed to open log file");
        exit(1);
    }
    dup2(log_fd, STDERR_FILENO);
    close(log_fd);

    execl("/usr/local/bin/ollama", "ollama", "serve", (char *)NULL);
    perror("execl failed");
    exit(1);
}

/**
 * @brief Beendet den Ollama-Server.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */
int killServer(void) {
    int ret = execl("/usr/bin/killall", "killall", "ollama", (char *)NULL);
    if (ret < 0) {
        perror("killall failed");
        return 1;
    }
    return 0;
}
