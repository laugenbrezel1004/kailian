#ifndef CONSTANTS_H
#define CONSTANTS_H

// Dateipfade
#define CONFIG_PATH "/etc/kailian/kailian.conf"
#define LOG_PATH "/tmp/kailian.log"
#define OLLAMA_PATH "/usr/local/bin/ollama"
#define KILLALL_PATH "/usr/bin/killall"

// Größenbeschränkungen
#define MAX_FILE_SIZE (1024 * 1024)  // 1 MB
#define MAX_BUFFER_SIZE 256
#define MAX_ARG_LENGTH 32

// Fehlercodes
#define ERROR_SUCCESS 0
#define ERROR_GENERAL 1
#define ERROR_MEMORY -1
#define ERROR_IO -2
#define ERROR_INVALID_ARG -3

#endif // CONSTANTS_H 