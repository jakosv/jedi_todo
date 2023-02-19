#ifndef SUTILS_H_SENTRY
#define SUTILS_H_SENTRY

#ifdef __linux__

#include <stdlib.h>

size_t strlcpy(char *dest, const char *src, size_t size);
size_t strlcat(char *dest, const char *src, size_t count);

#endif

#endif
