#ifndef MODE_H
#define MODE_H

#include <stdbool.h>

enum special_modes { NONE, BASE64, BASE16 };

#define SIZEOF_NA_CHARS 64

struct mode
{
    /* flags for basic generation */
    bool lower   : 1;
    bool upper   : 1;
    bool numbers : 1;
    bool symbols : 1;
    bool spaces  : 1;

    /* special generation modes */
    enum special_modes special;

    /* other */
    bool debug : 1;
    char characters_not_allowed[SIZEOF_NA_CHARS];
    int length;
};

#endif /* MODE_H */
