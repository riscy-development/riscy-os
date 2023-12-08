#include <assert.h>
#include <string.h>

int
strcmp(const char* lhs, const char* rhs)
{
    assert(lhs && rhs);

    const unsigned char* lhs_p = (const unsigned char*)lhs;
    const unsigned char* rhs_p = (const unsigned char*)rhs;

    while (*lhs_p == *rhs_p && *lhs_p) {
        ++lhs_p;
        ++rhs_p;
    }

    return *lhs_p - *rhs_p;
}
