#include "../../include/server/ollama_server.h"
#include "../../include/config/constants.h"
#include "../../include/utils/error_handling.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static int create_daemon(void);
static int setup_daemon_environment(void);
static int setup_logging(void);

int start_ollama_server(void) {
    if (create_daemon() < 0) {
        log_error("server", "Daemon-Erstellung fehlgeschlagen");
        return ERROR_GENERAL;
    }

    if (setup_daemon_environment() < 0) {
        log_error("server", "Daemon-Umgebung konnte nicht eingerichtet werden");
        return ERROR_GENERAL;
    }

    execl(OLLAMA_PATH, "ollama", "serve", (char *)NULL);
    log_error_with_errno("server", "Ollama-Server-Start fehlgeschlagen");
    return ERROR_GENERAL;
}

static int setup_logging(void) {
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    
    int log_fd = open(LOG_PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd < 0) {
        log_error_with_errno("server", "Log-Datei konnte nicht geöffnet werden");
        return ERROR_IO;
    }
    
    if (dup2(log_fd, STDERR_FILENO) < 0) {
        close(log_fd);
        return ERROR_IO;
    }
    
    close(log_fd);
    return ERROR_SUCCESS;
}

// ... rest of the implementation 