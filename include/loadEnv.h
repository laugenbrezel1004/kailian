#ifndef LOADENV_H
#define LOADENV_H

#define MAXSIZE 100

typedef struct {
        char name[MAXSIZE];
        char endpoint[MAXSIZE];
        char info_endpoint[MAXSIZE];
        char running_model_endpoint[MAXSIZE];
        char ollama_version_endpoint[MAXSIZE];
} Env;


Env readEnv();
void removeSpaces(char *str);
#endif 
