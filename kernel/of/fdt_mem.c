
#include <kernel/of/fdt_mem.h>

// Try to free this region (return the number of bytes successfully freed)
static size_t
try_boot_free_region(struct fdt* fdt, void* start, size_t size)
{
    void* end = start + size;

    // See if we overlap any of the reserve_entries
    struct fdt_reserve_entry* entry = fdt_reserve_entry_begin(fdt);
    while (entry != NULL) {
        void* resv_start = fdt_reserve_entry_address(entry);
        size_t resv_size = fdt_reserve_entry_size(entry);

        void* resv_end = resv_start + resv_size;

        if (resv_end <= start || resv_start > end) {
            // No overlap with this reserve entry
        }
        else if (resv_start <= start) {
            // Fully covering or trim from below
            if (resv_end >= end) {
                // Fully covering (this region is entirely reserved)
                return (size_t)0;
            }
            else {
                // Trim from below
                start = resv_end;
                size = (size_t)(end - resv_end);
            }
        }
        else {
            // Split into two or trim from above
            if (resv_end < end) {
                // Split into two
                // Region1 = [start,resv_start)
                // Region2 = [resv_end,end)
                return try_boot_free_region(fdt, start, (size_t)(resv_start - start))
                       + try_boot_free_region(fdt, resv_end, (size_t)(end - resv_end));
            }
            else {
                // Trim from above
                end = resv_start;
                size = (size_t)(end - start);
            }
        }

        entry = fdt_next_reserve_entry(fdt, entry);
    }

    // We made it through (possibly shrunk)
    if (boot_free(start, size) != KERR_SUCCESS) {
        // We couldn't free for some reason
        return (size_t)0;
    }
    return size;
}

/*
 * Use the FDT to hand off free memory regions to the "boot" memory allocator
 */
kerror_t
fdt_boot_mem_init(struct fdt* fdt)
{
    struct fdt_node* mem_node = fdt_find_node_by_device_type(fdt, NULL, "memory");
    while (mem_node != NULL) {
        // TODO: Actually free memory
        mem_node = fdt_find_node_by_device_type(fdt, mem_node, "memory");
    }
    return KERR_SUCCESS;
}
