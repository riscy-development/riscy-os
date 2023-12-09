
#include <kernel/of/fdt_mem.h>

/*
 * Use the FDT to hand off free memory regions to the "boot" memory allocator
 */
kerror_t fdt_boot_mem_init(struct fdt *fdt);

