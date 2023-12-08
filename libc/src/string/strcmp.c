#include <string.h>

int
strcmp(const char* lhs, const char* rhs) {
    const unsigned char* lhs_p = (const unsigned char*)lhs;
    const unsigned char* rhs_p = (const unsigned char*)rhs;

    while (*lhs_p && *rhs_p) {
        if (*lhs_p < *rhs_p)
            return -1;

        if (*lhs_p > *rhs_p)
            return 1;
    }

    return 0;
}
