// src/arguments/coffee.c
#define _POSIX_C_SOURCE                                                        \
    199309L // Enable POSIX.1b (real-time extensions) for nanosleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void clear_screen() {
    printf("\033[2J\033[1;1H"); // Clear screen and move cursor to top-left
}

void print_coffee(int steam_phase) {
    clear_screen();

    // Steam animation (white)
    printf("\033[0;37m");      // White color for steam
    switch (steam_phase % 4) { // Increased to 4 phases for clearer animation
    case 0:
        printf("\t      ~    \n");
        printf("\t    ~      \n");
        printf("\t      ~    \n");
        break;
    case 1:
        printf("\t    ~      \n");
        printf("\t      ~    \n");
        printf("\t    ~      \n");
        break;
    case 2:
        printf("\t     ~     \n");
        printf("\t    ~      \n");
        printf("\t     ~     \n");
        break;
    case 3:
        printf("\t      ~    \n");
        printf("\t     ~     \n");
        printf("\t    ~      \n");
        break;
    }

    // Coffee cup
    printf("\033[0;33m"); // Brown color
    printf("       _____________\n");
    printf("      <_____________> ___\n");
    printf("      |             |/ _ \\\n");
    printf("      |               | | |\n");
    printf("      |               |_| |\n");
    printf("   ___|             |\\___/\n");
    printf("  /    \\___________/    \\\n");
    printf("  \\_____________________/\n");
    printf("       \\___________/\n");
    printf("\033[0m"); // Reset color
}

int coffee() {
    int frames = 4000; // More frames for smoother animation
    int i;
    struct timespec request = {
        0, 400000000}; // 0.1 seconds (100 million nanoseconds)
    struct timespec remaining;

    for (i = 0; i < frames; i++) {
        print_coffee(i);
        fflush(stdout); // Force output to display immediately
        nanosleep(&request, &remaining);
    }

    return 0;
}
