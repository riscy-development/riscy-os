#include <string.h>

int
memcmp(const void* lhs, const void* rhs, size_t count)
{
    const uint8_t* lhs_p = lhs;
    const uint8_t* rhs_p = rhs;

    for (size_t i = 0; i < count; ++i) {
        if (lhs_p[i] < rhs_p[i])
            return -1;

        if (rhs_p[i] < lhs_p[i])
            return 1;
    }

    return 0;
}
