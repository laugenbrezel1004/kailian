// loadEnv.c
#include "../include/loadEnv.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Struktur für ein Schlüssel-Wert-Paar aus der Konfigurationsdatei.
 *
 * Diese Struktur repräsentiert einen einzelnen Eintrag in der Konfiguration,
 * bestehend aus einem Schlüssel (key) und einem Wert (value).
 */
/*typedef struct {*/
/*        char *key;   < zeiger auf den schlüssel (z. b. "name"). */
/*        char *value; < zeiger auf den wert (z. b. "mistral"). */
/*} env;*/

/**
 * @brief Makro zur Ausgabe von Fehlermeldungen mit Datei- und
 * Zeileninformationen.
 *
 * @param text Der Text der Fehlermeldung, der auf stderr ausgegeben wird.
 */
#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

/**
 * @brief Schneidet führende und abschließende Leerzeichen aus einem String und
 * kopiert ihn.
 *
 * Diese Funktion entfernt Leerzeichen am Anfang und Ende des Eingabestrings
 * und kopiert das Ergebnis in den Zielbuffer.
 *
 * @param dest Zielbuffer, in den der getrimmte String geschrieben wird.
 * @param src Quellstring, der getrimmt werden soll.
 * @param dest_size Maximale Größe des Zielbuffers, um Überläufe zu vermeiden.
 */
static void trim_copy(char *dest, const char *src, size_t dest_size) {
    const char *start = src;
    while (*start == ' ')
        start++; // Überspringe führende Leerzeichen
    const char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ')
        end--; // Überspringe abschließende Leerzeichen
    size_t len = end - start + 1;
    if (len >= dest_size)
        len = dest_size - 1;  // Verhindere Bufferüberlauf
    memcpy(dest, start, len); // Kopiere getrimmten String
    dest[len] = '\0';         // Null-Terminierung hinzufügen
}

/**
 * @brief Liest die Konfigurationsdatei und gibt ein dynamisch allokiertes Array
 * von env-Struct zurück.
 *
 * Diese Funktion öffnet die Datei "/etc/kailian/kailian.conf", liest sie Zeile
 * für Zeile, parst Schlüssel-Wert-Paare und speichert sie in einem dynamisch
 * wachsenden Array.
 *
 * @param out_size Pointer auf eine Variable, die die Anzahl der gelesenen
 * Einträge zurückgibt.
 * @return env* Pointer auf das dynamisch allokierte Array von env-Struct, oder
 * NULL bei Fehlern.
 * @note Der Aufrufer ist dafür verantwortlich, den Speicher mit freeEnv()
 * freizugeben.
 */
env *readEnv(size_t *out_size) {
    FILE *fptr;          /**< Dateizeiger für die Konfigurationsdatei. */
    char line[256];      /**< Buffer für eine einzelne Zeile aus der Datei. */
    char key[256];       /**< Temporärer Buffer für den Schlüssel. */
    char value[256];     /**< Temporärer Buffer für den Wert. */
    size_t capacity = 8; /**< Aktuelle Kapazität des env-Arrays (wächst bei
                            Bedarf). Letzer Eintrag für Markierung des Endes*/
    size_t index = 0;    /**< Index des nächsten freien Slots im Array. */

    // Initiales Array im Heap allokieren
    env *envs = malloc(capacity * sizeof(env));
    if (!envs) {
        MELDUNG("Speicherallokierung für envs fehlgeschlagen!");
        exit(ENOMEM); // Beende das Programm bei Speicherfehler
    }
    // Initialisiere alle Einträge mit NULL, um spätere Freigabe zu vereinfachen
    for (size_t i = 0; i < capacity; i++) {
        envs[i].key = NULL;
        envs[i].value = NULL;
    }

    // Öffne die Konfigurationsdatei im Lesemodus
    fptr = fopen("/etc/kailian/kailian.conf", "r");
    if (fptr == NULL) {
        fprintf(
            stderr,
            "kailian: /etc/kailian/kailian.conf: No such file or directory\n");
        free(envs); // Freigabe des Arrays bei Fehler
        exit(ENOENT);
    }

    // Lese die Datei Zeile für Zeile
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0';     // Entferne Zeilenumbruch
        char *equal_sign = strchr(line, '='); // Finde das erste '='
        if (equal_sign != NULL) {
            *equal_sign = '\0'; // Teile die Zeile in Schlüssel und Wert
            trim_copy(key, line, sizeof(key));               // Trim Schlüssel
            trim_copy(value, equal_sign + 1, sizeof(value)); // Trim Wert

            // Prüfe, ob das Array erweitert werden muss
            if (index >= capacity) {
                capacity *= 2; // Verdopple die Kapazität
                env *temp = realloc(envs, capacity * sizeof(env));
                if (!temp) {
                    MELDUNG("Reallokierung fehlgeschlagen!");
                    freeEnv(envs, index); // Bereinige bei Fehler
                    fclose(fptr);
                    exit(ENOMEM);
                }
                envs = temp;
                // Initialisiere neue Einträge mit NULL
                for (size_t i = index; i < capacity; i++) {
                    envs[i].key = NULL;
                    envs[i].value = NULL;
                }
            }

            // Allokiere Speicher für key und value im Heap
            envs[index].key = malloc(strlen(key) + 1);
            envs[index].value = malloc(strlen(value) + 1);
            if (!envs[index].key || !envs[index].value) {
                MELDUNG("Speicherallokierung fehlgeschlagen!");
                freeEnv(envs, index); // Bereinige bei Fehler
                fclose(fptr);
                exit(ENOMEM);
            }
            strcpy(envs[index].key, key);     // Kopiere Schlüssel
            strcpy(envs[index].value, value); // Kopiere Wert
            index++;                          // Gehe zum nächsten Slot
        }
    }

    fclose(fptr);      // Schließe die Datei
    *out_size = index; // Setze die Anzahl der gelesenen Einträge
    return envs;       // Gib das Array zurück
}

/**
 * @brief Gibt den Speicher des env-Arrays frei.
 *
 * Diese Funktion gibt den Speicher für alle Schlüssel und Werte sowie das Array
 * selbst frei.
 *
 * @param envs Pointer auf das zu befreiende env-Array.
 * @param size Anzahl der Einträge im Array, die freigegeben werden sollen.
 */
void freeEnv(env *envs, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (envs[i].key) {
            free(envs[i].key);  // Gib Speicher für Schlüssel frei
            envs[i].key = NULL; // Vermeide doppelte Freigabe
        }
        if (envs[i].value) {
            free(envs[i].value); // Gib Speicher für Wert frei
            envs[i].value = NULL;
        }
    }
    free(envs); // Gib das Array selbst frei
}
