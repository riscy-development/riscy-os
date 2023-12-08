#include <kernel/string.h>

void
memcpy(uint8_t* src, uint8_t* dest, size_t n) {
    // Not ideal but functional
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

int
strlen(const char* c) {
    int count = 0;
    while (*c) {
        count++;
        c++;
    }
    return count;
}

int
strcmp(const char* a, const char* b) {
    while (*a != '\0' && *b != '\0') {
        if (*a < *b) {
            return -1;
        }
        if (*a > *b) {
            return 1;
        }

        a++;
        b++;
    }
    if(*a > *b) {
      return 1;
    }
    if(*a < *b) {
      return -1;
    }
    return 0;
}
