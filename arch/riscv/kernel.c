#include <assert.h>
#include <kernel/early_output.h>
#include <kernel/of/fdt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Init and Fini arrays */
typedef void (*func_ptr)(void);

extern func_ptr __preinit_array_start; // NOLINT
extern func_ptr __preinit_array_end;   // NOLINT
extern func_ptr __init_array_start;    // NOLINT
extern func_ptr __init_array_end;      // NOLINT
extern func_ptr __fini_array_start;    // NOLINT
extern func_ptr __fini_array_end;      // NOLINT

static void
process_func_array(func_ptr* start, func_ptr* end)
{
    for (func_ptr* func = start; func != end; func++)
        (*func)();
}

static void
preinit()
{
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
init()
{
    process_func_array(&__init_array_start, &__init_array_end);
}

static void
fini()
{
    process_func_array(&__fini_array_start, &__fini_array_end);
}

/* UART Setup */
unsigned char* UART = (unsigned char*)0x10000000; // NOLINT

void
dead_simple_uart_putchar(char c)
{
    *UART = c;
}

__attribute__((constructor)) void
foo(void)
{
    printk("Hello from a global constructor!\n");
}

__attribute__((destructor)) void
bar(void)
{
    printk("Hello from a global destructor!\n");
}

// Only in kernel.c until we get a framework for output setup
static void
fdt_dump(struct fdt* fdt)
{
    struct fdt_node* node = fdt_node_begin(fdt);

    int depth = 0;

    printk("FDT = (%p) {\n", fdt);
    while (node != NULL && depth >= 0) {
        for (int i = 0; i < depth; i++) {
            printk("\t");
        }
        printk("%s\n", fdt_node_name(node));
        node = fdt_next_node(fdt, node, &depth);
    }
    printk("}\n");
}

void
kmain(uint64_t hartid, struct fdt* fdt)
{
    // Register the callback
    register_early_putchar(dead_simple_uart_putchar);

    // Print core ID
    printk("Running on hart (%c)\n", (char)hartid + '0');

    // Check the FDT
    if (fdt_verify(fdt) != FDT_VALID) {
        printk("Cannot Read the FDT!\n");
        kernel_panic();
    }

    fdt_dump(fdt);

    printk("FOODBADD = %p\n", (void*)0xf00dbadd);

    // Testing FDT functions
    struct fdt_node* node = fdt_find_node_by_unit_name(fdt, NULL, "cpu-map");
    if (node == NULL) {
        printk("Could not find node!\n");
    }
    else {
        printk("Found node! (%s)\n", fdt_node_name(node));

#define NUM_PARENTS 3
        struct fdt_node* parents[NUM_PARENTS];
        size_t parents_read = fdt_node_get_parents(fdt, node, parents, NUM_PARENTS);
        assert(parents_read <= NUM_PARENTS);

        printk("Parents of (%s)\n", fdt_node_name(node));
        for (size_t i = 0; i < parents_read; i++) {
            printk("Parent[%p] = %s\n", (void*)(uintptr_t)i, fdt_node_name(parents[i]));
        }
#undef NUM_PARENTS
    }

    // Call global ctors
    preinit();
    init();

    for (;;) {
        printk("%c", *UART);

        if (*UART == '%')
            break;

        if (*UART == '!')
            abort();

        char old_uart = *UART;
        while (old_uart == *UART)
            ;
    }

    fini();
}
