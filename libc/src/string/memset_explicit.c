#include <assert.h>
#include <string.h>

void*
memset_explicit(void* dest, int ch, size_t count)
{
    assert(dest);
    return memset(dest, ch, count);
}
