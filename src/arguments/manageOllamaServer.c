// startServer.c
#include "../../include/arguments/manageOllamaServer.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOG_PATH "/tmp/kailian.log"
#define OLLAMA_PATH "/usr/local/bin/ollama"
#define KILLALL_PATH "/usr/bin/killall"

// Prototypen
static int create_daemon(void);
static int start_ollama_server(void);  // Neue Funktion
static int kill_ollama_server(void);   // Neue Funktion
static int setup_daemon_environment(void);
static int setup_logging(void);

/**
 * @brief Startet den Ollama-Server als Daemon-Prozess.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */
int startServer(void) {
    return start_ollama_server();
}

/**
 * @brief Beendet den Ollama-Server.
 * @return int 0 bei Erfolg, 1 bei Fehler.
 */
int killServer(void) {
    return kill_ollama_server();
}

static int start_ollama_server(void) {
    if (create_daemon() < 0) {
        return 1;
    }

    if (setup_daemon_environment() < 0) {
        return 1;
    }

    execl(OLLAMA_PATH, "ollama", "serve", (char *)NULL);
    perror("execl fehlgeschlagen");
    return 1;
}

static int setup_daemon_environment(void) {
    umask(0);
    if (chdir("/") < 0) {
        perror("chdir fehlgeschlagen");
        return -1;
    }

    if (setup_logging() < 0) {
        return -1;
    }

    return 0;
}

static int setup_logging(void) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    
    int log_fd = open(LOG_PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        perror("Öffnen der Log-Datei fehlgeschlagen");
        return -1;
    }
    
    if (dup2(log_fd, STDERR_FILENO) < 0) {
        perror("dup2 fehlgeschlagen");
        close(log_fd);
        return -1;
    }
    close(log_fd);
    return 0;
}

static int kill_ollama_server(void) {
    int ret = execl(KILLALL_PATH, "killall", "ollama", (char *)NULL);
    if (ret < 0) {
        perror("killall fehlgeschlagen");
        return 1;
    }
    return 0;
}

static int create_daemon(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Erster fork fehlgeschlagen");
        return -1;
    }
    if (pid > 0) exit(0);

    if (setsid() < 0) {
        perror("setsid fehlgeschlagen");
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        perror("Zweiter fork fehlgeschlagen");
        return -1;
    }
    if (pid > 0) exit(0);

    return 0;
}
