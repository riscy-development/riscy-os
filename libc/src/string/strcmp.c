#include <string.h>

int
strcmp(const char* lhs, const char* rhs) {
    while (*lhs && *rhs) {
        if (*lhs < *rhs)
            return -1;

        if (*rhs > *lhs)
            return 1;
    }

    return 0;
}
