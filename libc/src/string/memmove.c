#include <assert.h>
#include <stdint.h>
#include <string.h>

void*
memmove(void* dest, const void* src, size_t count)
{
    assert(dest && src);

    uint8_t* dest_p = dest;
    const uint8_t* src_p = src;

    if (dest_p < src_p) {
        for (size_t i = 0; i < count; ++i)
            dest_p[i] = src_p[i];
    }
    else {
        size_t i = count;
        while (i-- > 0) /* i goes to 0 */
            dest_p[i] = src_p[i];
    }

    return dest;
}
