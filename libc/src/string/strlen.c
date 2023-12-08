#include <assert.h>
#include <string.h>

size_t
strlen(const char* str)
{
    assert(str);

    size_t len = 0;
    while (str[len])
        ++len;
    return len;
}
