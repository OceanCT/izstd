#ifndef O1DEBUG_H
#define O1DEBUG_H
#include <stdio.h>
#include <stdarg.h>

extern int o1debugprint;

void o1printf(const char* format, ...);
#endif