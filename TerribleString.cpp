#include "TerribleString.h"

#include <stdio.h>
#include <string.h>

int _hashString(char* str)
{
    unsigned int value = 0;

    while (1)
    {
        value += (unsigned int)*str;
        str++;

        if (*str == 0)
            break;
    }

    return value;
}
