
#include <kernel/early_output.h>
#include <stddef.h>

static putchar_t early_putchar_array[EARLY_OUTPUT_MAX_PUTCHAR] = {0};

kerror_t
register_early_putchar(void (*putchar)(char))
{
    int index = -1;

    for (int i = 0; i < EARLY_OUTPUT_MAX_PUTCHAR; i++) {
        if (early_putchar_array[i] == NULL) {
            index = i;
        }
        else if (early_putchar_array[i] == putchar) {
            return KERR_EXIST;
        }
    }

    if (index < 0)
        return KERR_FULL;

    early_putchar_array[index] = putchar;
    return KERR_SUCCESS;
}

kerror_t
unregister_early_putchar(void (*putchar)(char))
{
    for (int i = 0; i < EARLY_OUTPUT_MAX_PUTCHAR; i++) {
        if (early_putchar_array[i] == putchar) {
            early_putchar_array[i] = NULL;
            return KERR_SUCCESS;
        }
    }
    return KERR_NO_EXIST;
}

void
early_putchar(char c)
{
    for (int i = 0; i < EARLY_OUTPUT_MAX_PUTCHAR; i++) {
        putchar_t cb = early_putchar_array[i];
        if (cb == NULL)
            continue;

        (*cb)(c);
    }
}

void
early_puts(const char* s)
{
    while (*s) {
        early_putchar(*s);
        s++;
    }
}
