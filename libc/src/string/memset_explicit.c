#include <string.h>

void*
memset_explicit(void* dest, int ch, size_t count) {
    return memset(dest, ch, count);
}
