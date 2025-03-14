#ifndef LOADENV_H
#define LOADENV_H

#define MAXSIZE 300

typedef struct {
        char name[MAXSIZE];
        char endpoint_generate[MAXSIZE];
        char endpoint_info[MAXSIZE];
        char endpoint_running_model[MAXSIZE];
        char endpoint_ollama_version[MAXSIZE];
        char endpoint_chat[MAXSIZE];
        char endpoint_show[MAXSIZE];
        char system[MAXSIZE];
} Env;


Env readEnv();
void removeSpaces(char *str);
#endif 
