#include <string.h>

void*
memcpy(void* restrict dest, const void* restrict src, size_t count) {
    uint8_t* restrict dest_p = dest;
    const uint8_t* restrict src_p = src;

    for (size_t i = 0; i < count; ++i)
        dest_p[i] = src_p[i];

    return dest;
}
