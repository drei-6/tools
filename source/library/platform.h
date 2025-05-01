#ifndef PLATFORM_H
#define PLATFORM_H

#include "definitions.h"

#ifdef _WIN32
    #define WINDOWS_OS
#else
    #error
#endif

#define PLATFORM_API

#define Print PlatformPrint
#define Log Print

typedef struct
{
    void *Memory;
    uptr Size;
} platform_file;

#endif