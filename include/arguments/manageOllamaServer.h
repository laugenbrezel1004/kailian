#ifndef MANAGEOLLAMASERVER_H
#define MANAGEOLLAMASERVER_H

/**
 * @brief Startet den Ollama-Server als Daemon-Prozess.
 * @return int 0 bei Erfolg, 1 bei Fehler
 */
int startServer(void);

/**
 * @brief Beendet den laufenden Ollama-Server.
 * @return int 0 bei Erfolg, 1 bei Fehler
 */
int killServer(void);

#endif // MANAGEOLLAMASERVER_H
