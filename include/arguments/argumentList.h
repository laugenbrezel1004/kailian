
// argumentList.h
#ifndef ARGUMENTLIST_H
#define ARGUMENTLIST_H

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


extern Help argument_help;
extern Info argument_info;
extern Model argument_model;

#endif // ARGUMENTLIST_H

