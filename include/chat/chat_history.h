#ifndef CHAT_HISTORY_H
#define CHAT_HISTORY_H

#include <time.h>

#define MAX_HISTORY_ENTRIES 5
#define HISTORY_FILE_PATH "/tmp/kailian_chat_history"

typedef struct {
    char *prompt;
    char *response;
    time_t timestamp;
} ChatEntry;

typedef struct {
    ChatEntry entries[MAX_HISTORY_ENTRIES];
    int count;
} ChatHistory;

// Funktionen für Chat-History-Management
void chat_history_init(void);
void chat_history_add(const char *prompt, const char *response);
char *chat_history_get_context(void);
void chat_history_clear(void);
void chat_history_cleanup(void);

#endif // CHAT_HISTORY_H 