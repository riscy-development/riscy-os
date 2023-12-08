#include <stdint.h>
#include <string.h>

void*
memset(void* dest, int ch, size_t count) {
    uint8_t* dest_p = dest;
    uint8_t val = (uint8_t)ch;

    for (size_t i = 0; i < count; ++i)
        dest_p[i] = val;

    return dest;
}
