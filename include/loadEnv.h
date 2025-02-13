#ifndef LOADENV_H
#define LOADENV_H
#endif 

typedef struct {
        char name[100];
        char endpoint[100];
        char info_endpoint[100];
        char model_endpoint[100];
        char ollama_version_endpoint[100];
} Env;




Env readEnv();
