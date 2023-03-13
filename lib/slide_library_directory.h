#ifndef __SLIDE_LIBRARY_DIRECTORY_H__
#define __SLIDE_LIBRARY_DIRECTORY_H__

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

struct Directory {
    // Slide name (NUL terminated) (32 bytes)
    uint8_t name[32];
    // Address of slide within library file (4 bytes)
    uint32_t addr;
};

#endif // __SLIDE_LIBRARY_DIRECTORY_H__
