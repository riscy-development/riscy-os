
#include <kernel/of/fdt_mem.h>

/*
 * Use the FDT to hand off free memory regions to the "boot" memory allocator
 */
kerror_t
fdt_boot_mem_init(struct fdt* fdt)
{
    struct fdt_node* mem_node = fdt_find_node_by_device_type(fdt, NULL, "memory");
    while (mem_node != NULL) {
        mem_node = fdt_find_node_by_device_type(fdt, mem_node, "memory");
    }
}
