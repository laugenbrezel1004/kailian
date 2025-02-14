#ifndef LOADENV_H
#define LOADENV_H

#define MAXSIZE 100

typedef struct {
        char name[MAXSIZE];
        char endpoint[MAXSIZE];
        char info_endpoint[MAXSIZE];
        char model_endpoint[MAXSIZE];
        char ollama_version_endpoint[MAXSIZE];
        char set_mdoel[MAXSIZE];
} Env;


Env readEnv();
void removeSpaces(char *str);
#endif 
