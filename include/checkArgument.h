// checkArgument.h
#ifndef CHECKARGUMENT_H
#define CHECKARGUMENT_H

#include "arguments/argumentList.h"

int checkArgument(const char *argument);
int matchesArgument(const char *argument, const Argument *compare);

#endif
