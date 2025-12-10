#include "o1debug.h"

int o1debugprint = 1;

void o1printf(const char* format, ...) {
    if (o1debugprint) {
        va_list args;

        va_start(args, format);

        vprintf(format, args);

        va_end(args);
    }
}
