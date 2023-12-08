#include <kernel/of/fdt.h>
#include <kernel/early_output.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
fdt_dump(struct fdt* fdt) {
    struct fdt_node* node = fdt_node_begin(fdt);
    int depth = 1;

    printk("FDT {\n");
    while (node != NULL && depth > 0) {
        for (int i = 0; i < depth; i++) {
            printk("%c", '\t');
        }
        printk(fdt_node_name(node));
        printk("%c", '\n');

        struct fdt_node* subnode = fdt_node_subnode_begin(fdt, node);
        if (subnode) {
            depth += 1;
            node = subnode;
            continue;
        }
        struct fdt_node* sibling_node = fdt_node_next_subnode(fdt, node);
        if (sibling_node) {
            node = sibling_node;
            continue;
        }

        node = fdt_next_node(fdt, node);
        depth -= 1;
    }
    printk("}\n");
}

void
kmain(uint64_t hartid, struct fdt* fdt)
{
    // Register the callback
    register_early_putchar(dead_simple_uart_putchar);
    
    // Print core ID
    printk("Running on hart ");
    printk("%c", (char)hartid + '0');
    printk("%c", '\n');

    // Check the FDT
    if (fdt_verify(fdt) != FDT_VALID) {
        printk("Cannot Read the FDT!\n");
        kernel_panic();
    }

    fdt_dump(fdt);

    // Testing FDT functions
    struct fdt_node *clint = fdt_find_compatible_node(fdt, NULL, "riscv,clint0");
    if(clint == NULL) {
      printk("Could not find the CLINT!\n");
    } else {
      printk("Found the CLINT!\n");
      printk(fdt_node_name(clint));
      printk("%c", '\n');
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
