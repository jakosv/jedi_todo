#include "sutils.h"

#include <stdlib.h>

#ifdef __linux__

size_t strlcpy(char *dest, const char *src, size_t size)
{
    size_t i;
    for (i = 0; i < size - 1 && src[i]; i++)
        dest[i] = src[i];
    dest[i] = 0;
    return i;
}

size_t strlcat(char *dest, const char *src, size_t count)
{
    size_t i, j;
    for (i = 0; i < count - 1 && dest[i]; i++)
        {}
    for (j = 0; i < count - 1 && src[j]; i++, j++)
        dest[i] = src[j];
    dest[i] = 0;
    return j;
}

#endif
