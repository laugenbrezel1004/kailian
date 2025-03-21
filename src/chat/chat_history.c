#include "../../include/chat/chat_history.h"
#include "../../include/config/constants.h"
#include "../../include/utils/error_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ChatHistory history = {0};

void chat_history_init(void) {
    FILE *file = fopen(HISTORY_FILE_PATH, "r");
    if (!file) {
        return;
    }

    char line[MAX_BUFFER_SIZE];
    int entry_index = 0;
    ChatEntry *current_entry = NULL;

    while (fgets(line, sizeof(line), file) && entry_index < MAX_HISTORY_ENTRIES) {
        if (strncmp(line, "PROMPT:", 7) == 0) {
            current_entry = &history.entries[entry_index];
            current_entry->prompt = strdup(line + 8);
        } else if (strncmp(line, "RESPONSE:", 9) == 0 && current_entry) {
            current_entry->response = strdup(line + 10);
            current_entry->timestamp = time(NULL);
            entry_index++;
            history.count++;
        }
    }

    fclose(file);
}

void chat_history_add(const char *prompt, const char *response) {
    if (history.count == MAX_HISTORY_ENTRIES) {
        // Entferne den ältesten Eintrag
        free(history.entries[0].prompt);
        free(history.entries[0].response);
        
        // Verschiebe alle Einträge nach vorne
        for (int i = 1; i < MAX_HISTORY_ENTRIES; i++) {
            history.entries[i-1] = history.entries[i];
        }
        history.count--;
    }

    ChatEntry *entry = &history.entries[history.count];
    entry->prompt = strdup(prompt);
    entry->response = strdup(response);
    entry->timestamp = time(NULL);
    history.count++;

    // Speichere History in Datei
    FILE *file = fopen(HISTORY_FILE_PATH, "w");
    if (file) {
        for (int i = 0; i < history.count; i++) {
            fprintf(file, "PROMPT: %s\n", history.entries[i].prompt);
            fprintf(file, "RESPONSE: %s\n", history.entries[i].response);
        }
        fclose(file);
    }
}

char *chat_history_get_context(void) {
    if (history.count == 0) {
        return NULL;
    }

    // Berechne benötigte Größe
    size_t total_size = 0;
    for (int i = 0; i < history.count; i++) {
        total_size += strlen(history.entries[i].prompt) + 
                     strlen(history.entries[i].response) + 50;
    }

    char *context = malloc(total_size);
    if (!context) {
        return NULL;
    }

    char *ptr = context;
    for (int i = 0; i < history.count; i++) {
        ptr += sprintf(ptr, "User: %s\nAssistant: %s\n",
                      history.entries[i].prompt,
                      history.entries[i].response);
    }

    return context;
}

void chat_history_clear(void) {
    for (int i = 0; i < history.count; i++) {
        free(history.entries[i].prompt);
        free(history.entries[i].response);
    }
    history.count = 0;
    remove(HISTORY_FILE_PATH);
}

void chat_history_cleanup(void) {
    chat_history_clear();
} 