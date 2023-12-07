#include <kernel/of/fdt.h>
#include <kernel/stdint.h>
#include <kernel/string.h>

/* Init and Fini arrays */
typedef void (*func_ptr)(void);

extern func_ptr __preinit_array_start; // NOLINT
extern func_ptr __preinit_array_end;   // NOLINT
extern func_ptr __init_array_start;    // NOLINT
extern func_ptr __init_array_end;      // NOLINT
extern func_ptr __fini_array_start;    // NOLINT
extern func_ptr __fini_array_end;      // NOLINT

static void
process_func_array(func_ptr* start, func_ptr* end) {
    for (func_ptr* func = start; func != end; func++)
        (*func)();
}

static void
preinit() {
    /* why do we pass as pointers?
     *
     * otherwise gcc dereferences...
     * but the linker gives us the pointer
     *
     * its really horrible, but it was the only way that worked
     */
    process_func_array(&__preinit_array_start, &__preinit_array_end);
}

static void
init() {
    process_func_array(&__init_array_start, &__init_array_end);
}

static void
fini() {
    process_func_array(&__fini_array_start, &__fini_array_end);
}

/* UART Setup */
unsigned char* UART = (unsigned char*)0x10000000; // NOLINT

void
putchar(char c) {
    *UART = c;
}

void
puts(const char* str) {
    while (*str != '\0') {
        putchar(*str);
        str++;
    }
}

__attribute__((constructor)) void
foo(void) {
    puts("Hello from a global constructor!\n");
}

__attribute__((destructor)) void
bar(void) {
    puts("Hello from a global destructor!\n");
}

void
kmain(uint64_t hartid, struct fdt* fdt) {
    // Print core ID
    puts("Running on hart ");
    putchar((char)hartid + '0');
    putchar('\n');

    // Check the FDT
    if (verify_fdt(fdt)) {
        puts("Using a supported FDT Version\n");
    } else {
        puts("Cannot Read the FDT (Unsupported Version!)\n");
    }

    // Call global ctors
    preinit();
    init();

    for (;;) {
        putchar(*UART);

        if (*UART == '%')
            break;

        char old_uart = *UART;
        while (old_uart == *UART)
            ;
    }

    fini();
}
