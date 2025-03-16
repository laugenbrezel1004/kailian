// coffee.c
#define _POSIX_C_SOURCE 199309L
#include "../../include/arguments/coffee.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static void clear_screen(void) { printf("\033[2J\033[1;1H"); }

static void print_coffee(int phase) {
    clear_screen();
    printf("\033[0;37m"); // Weiß für Dampf
    switch (phase % 4) {
    case 0:
        printf("\t      ~    \n\t    ~      \n\t      ~    \n");
        break;
    case 1:
        printf("\t    ~      \n\t      ~    \n\t    ~      \n");
        break;
    case 2:
        printf("\t     ~     \n\t    ~      \n\t     ~     \n");
        break;
    case 3:
        printf("\t      ~    \n\t     ~     \n\t    ~      \n");
        break;
    }
    printf("\033[0;33m"); // Braun für Tasse
    printf("       _____________\n"
           "      <_____________> ___\n"
           "      |             |/ _ \\\n"
           "      |               | | |\n"
           "      |               |_| |\n"
           "   ___|             |\\___/\n"
           "  /    \\___________/    \\\n"
           "  \\_____________________/\n"
           "       \\___________/\n");
    printf("\033[0m"); // Farbe zurücksetzen
}

/**
 * @brief Zeigt eine animierte Kaffeetasse an.
 * @return int 0 bei Erfolg.
 */
int coffee(void) {
    struct timespec delay = {0, 100000000}; // 0.1 Sekunden
    for (int i = 0; i < 40; i++) {          // 40 Frames (~4 Sekunden)
        print_coffee(i);
        fflush(stdout);
        nanosleep(&delay, NULL); // Ignoriert remaining für Einfachheit
    }
    return 0;
}
