#include "kernel/early_output.h"
#include "kernel/mem/boot.h"
#include "kernel/of/fdt.h"
#include "kernel/of/fdt_mem.h"

#include "drivers/syscon.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Init and Fini arrays */
typedef void (*func_ptr)(void);

EXTERN_PTR_SYMBOL(__preinit_array_start, func_ptr); // NOLINT
EXTERN_PTR_SYMBOL(__preinit_array_end, func_ptr);   // NOLINT
EXTERN_PTR_SYMBOL(__init_array_start, func_ptr);    // NOLINT
EXTERN_PTR_SYMBOL(__init_array_end, func_ptr);      // NOLINT
EXTERN_PTR_SYMBOL(__fini_array_start, func_ptr);    // NOLINT
EXTERN_PTR_SYMBOL(__fini_array_end, func_ptr);      // NOLINT

static void
process_func_array(func_ptr* start, func_ptr* end)
{
    for (func_ptr* func = start; func != end; func++)
        (*func)();
}

static void
preinit()
{
    process_func_array(__preinit_array_start, __preinit_array_end);
}

static void
init()
{
    process_func_array(__init_array_start, __init_array_end);
}

static void
fini()
{
    process_func_array(__fini_array_start, __fini_array_end);
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

    word_t depth = 0;

    printk("FDT = (%p) {\n", fdt);
    while (node != NULL && depth >= 0) {
        for (word_t i = 0; i < depth; i++) {
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
    kerror_t err;

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

    // Initialize the boot memory allocator using the FDT
    fdt_boot_mem_init(fdt);

    // Testing the boot memory allocator

    // Dump the mem map
    boot_mem_dump();

    // Allocate a page aligned to 16 bytes
    void* alloc = boot_alloc(0x1000, 4);
    printk("alloc = %p\n", alloc);

    boot_mem_dump();

    // Set up syscon reboot
    bool syscon_reboot_ok = true;
    err = syscon_reboot_init(fdt);

    if (err) {
        printk("Error setting up syscon reboot\n");
        syscon_reboot_ok = false;
    }

    // Set up syscon shutdown
    bool syscon_shutdown_ok = true;
    err = syscon_shutdown_init(fdt);

    if (err) {
        printk("Error setting up syscon shutdown\n");
        syscon_shutdown_ok = false;
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

        if (syscon_reboot_ok && *UART == '^')
            syscon_reboot();

        if (syscon_shutdown_ok && *UART == '&')
            syscon_shutdown();

        char old_uart = *UART;
        while (old_uart == *UART)
            ;
    }

    fini();
}
