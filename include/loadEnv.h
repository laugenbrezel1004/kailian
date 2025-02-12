#ifndef LOADENV_H
#define LOADENV_H
#endif 

typedef struct {
        char name[100];
        char endpoint[100];
} Env;



Env readEnv();
