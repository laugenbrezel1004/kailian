
// argumentList.h
#ifndef ARGUMENTLIST_H
#define ARGUMENTLIST_H


typedef struct {
    const char *name;
    const char *long_form;
    const char *short_form;
} Blaupause;

typedef struct {
    const char *name;
    const char *long_form;
    const char *short_form;
} Help;

typedef struct {
    const char *name;
    const char *long_form;
    const char *short_form;
} Info;

typedef struct {
    const char *name;
    const char *long_form;
    const char *short_form;
} Model;

typedef struct {
    const char *name;
    const char *long_form;
    const char *short_form;
} ShowEnironment;


extern Help argument_help;
extern Info argument_info;
extern Model argument_model;
extern ShowEnironment argument_showEnvironment;
extern Blaupause argument_blaupause;

#endif // ARGUMENTLIST_H

