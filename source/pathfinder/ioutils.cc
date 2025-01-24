#include "ioutils.hh"
#include <stdio.h>

void cout_err(const char *tag, const char *details)
{
    printf("ERROR (%s): %s\n", tag, details);
}

void cout_warn(const char *tag, const char *details)
{
#ifdef LOW_VERBOSE_MODE
    printf("WARNING (%s): %s\n", tag, details);
#endif
}

void cout_debug(const char *tag, const char *details)
{
    printf("INFO (%s): %s\n", tag, details);
}