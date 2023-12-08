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

// Only in kernel.c until we get a framework for output setup
static void fdt_dump(struct fdt *fdt) {
  struct fdt_node *node = fdt_node_begin(fdt);
  int depth = 1;

  puts("FDT {\n");
  while(node != NULL && depth > 0) {
    for(int i = 0; i < depth; i++) {
      putchar('\t');
    }
    puts(fdt_node_name(node));
    putchar('\n');

    struct fdt_node *subnode = fdt_node_subnode_begin(fdt, node);
    if(subnode) {
      depth += 1;
      node = subnode;
      continue;
    }
    struct fdt_node *sibling_node = fdt_node_next_subnode(fdt, node);
    if(sibling_node) {
      node = sibling_node;
      continue;
    }

    node = fdt_next_node(fdt, node);
    depth -= 1;
  }
  puts("}\n");
}


void
kmain(uint64_t hartid, struct fdt* fdt) {
    // Print core ID
    puts("Running on hart ");
    putchar((char)hartid + '0');
    putchar('\n');

    // Check the FDT
    if (fdt_verify(fdt)) {
        puts("Cannot Read the FDT!\n");
        // PANIC! (this is actually unrecoverable)
    }

    fdt_dump(fdt);

    // Testing FDT functions
    struct fdt_node *clint = fdt_find_compatible_node(fdt, NULL, "riscv,clint0");
    if(clint == NULL) {
      puts("Could not find the CLINT!\n");
    } else {
      puts("Found the CLINT!\n");
      puts(fdt_node_name(clint));
      putchar('\n');
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
