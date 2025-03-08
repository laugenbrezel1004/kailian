#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int startServer() {
    pid_t pid;

    // Schritt 1: Erster Fork
    pid = fork();
    if (pid < 0) {
        perror("Erster Fork fehlgeschlagen");
        exit(1);
    }
    if (pid > 0) {
        printf("Elternprozess (PID %d) beendet sich.\n", getpid());
        exit(0); // Elternprozess beenden
    }

    // Schritt 2: Neue Session erstellen
    if (setsid() < 0) {
        perror("setsid fehlgeschlagen");
        exit(1);
    }

    // Schritt 3: Zweiter Fork
    pid = fork();
    if (pid < 0) {
        perror("Zweiter Fork fehlgeschlagen");
        exit(1);
    }
    if (pid > 0) {
        exit(0); // Zweiter Elternprozess beenden
    }

    // Schritt 4: Arbeitsverzeichnis ändern
    chdir("/");

    // Schritt 5: Dateideskriptoren umleiten
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDWR); // STDIN
    dup(0);                    // STDOUT
    dup(0);                    // STDERR

    // Schritt 6: ollama serve starten
    execl("/usr/local/bin/ollama", "ollamad", "serve", (char *)NULL);

    // Wenn execl fehlschlägt, wird dieser Code erreicht
    perror("execl fehlgeschlagen");
    exit(1);

    return 0; // Wird nie erreicht, da execl den Prozess ersetzt
}
