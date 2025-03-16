// call_api.c
// imports
#include <fcntl.h>     // Für Dateideskriptoren (falls benötigt)
#include <stddef.h>    // Für size_t
#include <sys/types.h> // Für Systemdatentypen
#include <unistd.h>    // Für POSIX-Funktionen wie close()
#define _POSIX_C_SOURCE                                                        \
    200809L // Aktiviert POSIX-Features (strdup, getline, etc.)
#include "../include/arguments/argumentList.h" // Argument-Definitionen
#include "../include/checkArgument.h"          // Argument-Prüfungen
#include "../include/loadEnv.h"                // Für readEnv und freeEnv
#include <cjson/cJSON.h>                       // JSON-Verarbeitung
#include <curl/curl.h>                         // CURL-Bibliothek
#include <curl/easy.h>                         // Einfache CURL-Funktionen
#include <stdbool.h>                           // Für bool-Typ
#include <stdio.h>                             // Standard-Ein-/Ausgabe
#include <stdlib.h>                            // Speicher- und Exit-Funktionen
#include <string.h>                            // String-Manipulation

// Prototypen
/** @brief Callback-Funktion für CURL, um Argument-Antworten zu verarbeiten. */
static size_t cbSendArgument(void *data, size_t size, size_t nmemb,
                             void *userp);

/** @brief Funktion zum Verbinden mit der KI-API. */
int connectToAi(const char *bufferPrompt, const char *bufferFile,
                const char *argument);

/** @brief Callback-Funktion für CURL, um KI-Antworten zu verarbeiten. */
static size_t cbAi(void *data, size_t size, size_t nmemb, void *userp);

// Macros
/** @brief Makro zur Ausgabe von Fehlermeldungen mit Datei- und Zeilenangabe.
 * @param text Der Fehlermeldungstext.
 */
#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

/**
 * @brief Hilfsfunktion zum Abrufen eines Werts aus dem env-Array anhand eines
 * Schlüssels.
 *
 * Durchsucht das env-Array nach einem Schlüssel und gibt den zugehörigen Wert
 * zurück.
 *
 * @param config Das env-Array aus readEnv.
 * @param env_count Anzahl der Einträge im env-Array.
 * @param key Der Schlüssel, nach dem gesucht wird (z. B. "name").
 * @return const char* Der Wert, oder NULL, wenn nicht gefunden.
 */
static const char *getEnvValue(env *config, size_t env_count, const char *key) {
    for (size_t i = 0; i < env_count; i++) { // Iteriere über alle Einträge
        if (config[i].key &&
            strcmp(config[i].key, key) == 0) { // Schlüsselvergleich
            return config[i].value;            // Wert gefunden
        }
    }
    return NULL; // Schlüssel nicht gefunden
}

/**
 * @brief Callback-Funktion für CURL, um Antworten auf Argument-Anfragen zu
 * verarbeiten.
 *
 * Verarbeitet die vom Server empfangenen Daten, parst sie als JSON und gibt sie
 * formatiert aus.
 *
 * @param data Empfangene Daten vom Server.
 * @param size Größe eines Datenblocks.
 * @param nmemb Anzahl der Datenblöcke.
 * @param userp Benutzerdaten (hier: Zeiger auf showModels-Flag).
 * @return size_t Anzahl der verarbeiteten Bytes oder 0 bei Fehler.
 */
static size_t cbSendArgument(void *data, size_t size, size_t nmemb,
                             void *userp) {
    size_t realsize = size * nmemb; // Gesamtgröße der empfangenen Daten
    int *showModels =
        (int *)userp; // Flag, ob nur Modellnamen angezeigt werden sollen

    // Allokiere Speicher für die empfangenen Daten plus Nullterminator
    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed\n");
        return 0; // Fehler: Speicherallokierung fehlgeschlagen
    }
    memcpy(buffer, data, realsize); // Kopiere Daten in den Buffer
    buffer[realsize] = 0;           // Nullterminator hinzufügen

    // Parse die Daten als JSON
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        MELDUNG("JSON parsing failed");
        free(buffer);
        return 1; // Fehler: JSON-Parsing fehlgeschlagen
    }

    if (*showModels) { // Nur Modellnamen anzeigen
        cJSON *models = cJSON_GetObjectItemCaseSensitive(json, "models");
        if (cJSON_IsArray(models)) { // Prüfe, ob "models" ein Array ist
            int modelCount = cJSON_GetArraySize(models); // Anzahl der Modelle
            for (int j = 0; j < modelCount; j++) { // Iteriere über Modelle
                cJSON *model = cJSON_GetArrayItem(models, j);
                cJSON *name = cJSON_GetObjectItemCaseSensitive(model, "name");
                if (cJSON_IsString(name) && name->valuestring) { // Prüfe String
                    fprintf(stdout, "Model Name: %s\n", name->valuestring);
                }
            }
        } else {
            fprintf(stderr, "No models array found\n");
        }
    } else { // Vollständiges JSON ausgeben
        char *jsonString = cJSON_Print(json);
        if (jsonString) {
            fprintf(stdout, "%s\n", jsonString); // Formatiertes JSON ausgeben
            free(jsonString);                    // Speicher freigeben
        } else {
            fprintf(stderr, "Failed to print JSON\n");
            MELDUNG("Error");
        }
    }

    cJSON_Delete(json); // JSON-Objekt freigeben
    free(buffer);       // Buffer freigeben
    fflush(stdout);     // Ausgabe sofort anzeigen
    return realsize;    // Erfolg: Alle Bytes verarbeitet
}

/**
 * @brief Funktion zum Verbinden mit verschiedenen Ollama-API-Endpunkten.
 *
 * Diese Funktion verbindet sich mit der KI und sendet Daten wie bufferPrompt,
 * bufferFile oder ein Argument an die KI. Verwendet Umgebungsvariablen aus der
 * Konfigurationsdatei.
 *
 * @param bufferPrompt Der Eingabeprompt von der Kommandozeile.
 * @param bufferFile Datei-Eingabe über stdin von der Kommandozeile.
 * @param argument Das Argument von der Kommandozeile, z. B. -m.
 * @return int 0 bei Erfolg, sonst Fehlercode.
 */
int connectToAi(const char *bufferPrompt, const char *bufferFile,
                const char *argument) {
    // Lese die Umgebungsvariablen aus der Konfigurationsdatei
    size_t env_count;                  // Anzahl der geladenen Einträge
    env *config = readEnv(&env_count); // Lade Konfiguration
    if (!config) {
        fprintf(stderr, "Failed to load environment configuration\n");
        return 1; // Fehler: Konfiguration konnte nicht geladen werden
    }

    // Hole die benötigten Werte aus dem env-Array
    const char *name = getEnvValue(config, env_count, "name"); // Modellname
    const char *endpoint_generate = getEnvValue(
        config, env_count, "endpoint_generate"); // Generate-Endpunkt
    const char *endpoint_info =
        getEnvValue(config, env_count, "endpoint_info"); // Info-Endpunkt
    const char *system = getEnvValue(config, env_count, "system"); // Systeminfo
    const char *endpoint_running_model = getEnvValue(
        config, env_count, "endpoint_running_model"); // Aktuell laufende Ki

    // Prüfe, ob die kritischen Werte vorhanden sind
    if (!name || !endpoint_generate || !endpoint_info || !system ||
        !endpoint_running_model) {
        fprintf(stderr, "Missing required environment variables\n");
        freeEnv(config, env_count); // Speicher freigeben
        return 1;                   // Fehler: Fehlende Werte
    }

    // Curl-spezifische Initialisierung
    CURL *curl = NULL;       // CURL-Handle
    CURLcode res = CURLE_OK; // CURL-Ergebniscode

    curl = curl_easy_init(); // Initialisiere CURL
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        freeEnv(config, env_count); // Speicher freigeben
        return 1; // Fehler: CURL-Initialisierung fehlgeschlagen
    }

    // Wenn ein Argument übergeben wurde
    if (argument != NULL) {
        const char *url = NULL; // URL für den API-Aufruf
        int showModels = 0;     // Flag für Modellnamen-Anzeige

        // Bestimme den richtigen API-Endpunkt basierend auf dem Argument
        if (strcmp(argument, arguments.model.long_form) == 0) {
            url = endpoint_running_model; // TODO: Modellwahl-Logik fehlt noch
        } else if (strcmp(argument, arguments.showModels.long_form) == 0) {
            url = endpoint_info; // Info-Endpunkt für Modellliste
            showModels = 1;      // Nur Modellnamen anzeigen
        } else if (strcmp(argument, arguments.info.long_form) == 0) {
            url = endpoint_info; // Info-Endpunkt für volle Info
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argument);
            curl_easy_cleanup(curl);    // CURL aufräumen
            freeEnv(config, env_count); // Speicher freigeben
            return 1;                   // Fehler: Unbekanntes Argument
        }

        // Setze CURL-Optionen für den API-Aufruf
        curl_easy_setopt(curl, CURLOPT_URL, url); // API-URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         cbSendArgument);                       // Callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &showModels); // Übergib Flag

        res = curl_easy_perform(curl); // Führe API-Aufruf aus
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);          // CURL aufräumen
        freeEnv(config, env_count);       // Speicher freigeben
        return (res == CURLE_OK) ? 0 : 1; // Rückgabe: Erfolg oder Fehler
    }

    // Kein Argument -> Normaler API-Generate-Aufruf
    cJSON *root = cJSON_CreateObject(); // Erstelle JSON-Objekt für Anfrage
    if (!root) {
        fprintf(stderr, "cJSON_CreateObject failed\n");
        freeEnv(config, env_count);
        curl_easy_cleanup(curl);
        return 1; // Fehler: JSON-Erstellung fehlgeschlagen
    }

    // Füge Daten zum JSON-Objekt hinzu
    cJSON_AddStringToObject(root, "model", name);    // Modellname aus Config
    cJSON_AddStringToObject(root, "system", system); // System aus Config

    // Kombiniere Prompt und bufferFile mit einem Zeilenumbruch
    char *full_prompt = NULL;
    size_t prompt_len = strlen(bufferPrompt) + 1; // Prompt + Nullterminator
    if (bufferFile)
        prompt_len += strlen(bufferFile) + 1; // Zeilenumbruch + bufferFile
    full_prompt = malloc(prompt_len);         // Allokiere Speicher für Prompt
    if (!full_prompt) {
        fprintf(stderr, "malloc failed for full_prompt\n");
        cJSON_Delete(root);
        freeEnv(config, env_count);
        curl_easy_cleanup(curl);
        return 1; // Fehler: Speicherallokierung fehlgeschlagen
    }
    if (bufferFile) {
        snprintf(full_prompt, prompt_len, "%s\n%s", bufferPrompt,
                 bufferFile); // Kombiniere
    } else {
        strcpy(full_prompt, bufferPrompt); // Nur Prompt kopieren
    }

    cJSON_AddStringToObject(root, "prompt", full_prompt); // Prompt zum JSON
    free(full_prompt);                                    // Speicher freigeben

    char *json_str = cJSON_PrintUnformatted(root); // JSON als String
    if (!json_str) {
        fprintf(stderr, "cJSON_PrintUnformatted failed\n");
        cJSON_Delete(root);
        freeEnv(config, env_count);
        curl_easy_cleanup(curl);
        return 1; // Fehler: JSON-Formatierung fehlgeschlagen
    }

    // Setze CURL-Optionen für den Generate-Aufruf
    curl_easy_setopt(curl, CURLOPT_URL, endpoint_generate); // Generate-Endpunkt
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str); // JSON-Daten als POST
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbAi); // Callback für Antwort

    res = curl_easy_perform(curl); // Führe API-Aufruf aus
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    }
    printf("\n"); // Entfernt das "%" nach der KI-Antwort

    // Aufräumen
    free(json_str);                   // JSON-String freigeben
    cJSON_Delete(root);               // JSON-Objekt freigeben
    curl_easy_cleanup(curl);          // CURL aufräumen
    freeEnv(config, env_count);       // Env-Array freigeben
    return (res == CURLE_OK) ? 0 : 1; // Rückgabe: Erfolg oder Fehler
}

/**
 * @brief Callback-Funktion für CURL, um KI-Antworten vom Generate-Endpunkt zu
 * verarbeiten.
 *
 * Verarbeitet die empfangenen Daten, parst sie als JSON und gibt die Antwort
 * aus.
 *
 * @param data Empfangene Daten vom Server.
 * @param size Größe eines Datenblocks.
 * @param nmemb Anzahl der Datenblöcke.
 * @param userp Benutzerdaten (hier ungenutzt).
 * @return size_t Anzahl der verarbeiteten Bytes oder 0 bei Fehler.
 */
static size_t cbAi(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb; // Gesamtgröße der empfangenen Daten

    // Allokiere temporären Buffer für empfangene Daten plus Nullterminator
    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed in callback\n");
        MELDUNG("error");
        return 0; // Fehler: Speicherallokierung fehlgeschlagen
    }

    memcpy(buffer, data, realsize); // Kopiere Daten in den Buffer
    buffer[realsize] = '\0';        // Nullterminator hinzufügen

    cJSON *json = cJSON_Parse(buffer); // Parse als JSON
    const char *errorKi = "{\"error}"; // Prüfstring für Fehler
    if (json) {
        if (strncmp(errorKi, buffer, 6) == 0) { // Prüfe auf Fehlerantwort
            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            exit(1); // Beende Programm bei Fehler
        }

        // Extrahiere die Antwort aus dem JSON
        cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (response && cJSON_IsString(response)) {
            printf("%s", response->valuestring); // Antwort ausgeben
            fflush(stdout);                      // Sofort anzeigen
        }
        cJSON_Delete(json); // JSON-Objekt freigeben
    } else {
        fflush(stdout); // Bei Nicht-JSON: Ausgabe flushen
    }
    free(buffer);    // Buffer freigeben
    return realsize; // Erfolg: Alle Bytes verarbeitet
}
