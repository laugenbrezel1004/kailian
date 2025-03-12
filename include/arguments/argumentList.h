
// argumentList.h
#ifndef ARGUMENTLIST_H
#define ARGUMENTLIST_H

typedef struct {
    const char *name;       // Descriptive name of the argument
    const char *long_form;  // Long form (e.g., "--help")
    const char *short_form; // Short form (e.g., "-h")
} Argument;

typedef struct {
    Argument askError;
    Argument help;
    Argument info;
    Argument model;
    Argument showEnvironment;
    Argument showModels;
    Argument coffee;
    Argument startOllama;
    Argument killOllama;
    Argument createConfig;
    Argument chat;
    Argument cleanChat;
} Arguments;

// Declare the global variable for arguments
extern Arguments arguments;

#endif // ARGUMENTLIST_H
