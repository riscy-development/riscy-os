
#include "kernel/of/fdt_mem.h"

#include <stdio.h>

// Helper function which decides how to deal with overlapping reserved regions
// Returns true if split, else false
static bool
trim_or_split_around_reserved(
    void** start_ptr,
    size_t* size_ptr, // Original Region to be trimmed or split
    void** split_start_ptr,
    size_t* split_size_ptr, // Storage for second region if split occurs
    void* resv_start,
    size_t resv_size // The reserved region to trim or split around
)
{
    void* start = *start_ptr;
    size_t size = *size_ptr;

    void* end = start + size;
    void* resv_end = resv_start + resv_size;

    if (resv_end <= start || resv_start > end) {
        // No overlap with this reserve entry
        return false;
    }
    else if (resv_start <= start) {
        // Fully covering or trim from below
        if (resv_end >= end) {
            // Fully covering (this region is entirely reserved)
            *start_ptr = NULL;
            *size_ptr = 0;
            return false;
        }

        // Trim from below
        *start_ptr = resv_end;
        *size_ptr = (size_t)(end - resv_end);
        return false;
    }

    // Split into two or trim from above
    if (resv_end < end) {
        // Split into two
        // Region1 = [start,resv_start)
        // Region2 = [resv_end,end)
        *size_ptr = (size_t)(resv_start - start);
        *split_start_ptr = resv_end;
        *split_size_ptr = (size_t)(end - *split_start_ptr);
        return true;
    }

    // Trim from above
    end = resv_start;
    *size_ptr = (size_t)(end - start);
    return false;
}

// Try to free this region (return the number of bytes successfully freed)
static size_t
try_boot_free_region(struct fdt* fdt, void* start, size_t size)
{
    // Should be unnecessary (but it's worth checking in case someone messes up)
    if (size == 0) {
        return (size_t)0;
    }

    // Buffers for a second region if we get split into two
    void* split_start;
    size_t split_size;

    // Check if this is a region before the end of the kernel (protects the kernel and
    // firmware which is before it)
    EXTERN_SYMBOL(__kernel_end);

    if (start < (void*)__kernel_end) {
        if ((void*)(start + size) <= (void*)__kernel_end) {
            // Entirely before the kernel
            return (size_t)0;
        }
        // Need to trim before
        size = (size_t)((start + size) - (void*)__kernel_end);
        start = (void*)__kernel_end;
    }
    // No addresses lower than the end of the kernel (so no overlap)

    // Handle the reserve entries in the FDT
    struct fdt_reserve_entry* entry = fdt_reserve_entry_begin(fdt);
    while (entry != NULL) {
        void* resv_start = fdt_reserve_entry_address(entry);
        size_t resv_size = fdt_reserve_entry_size(entry);

        if (trim_or_split_around_reserved(
                &start, &size, &split_start, &split_size, resv_start, resv_size
            )) {
            // We split into 2 regions
            return try_boot_free_region(fdt, start, size)
                   + try_boot_free_region(fdt, split_start, split_size);
        }
        if (size == 0) {
            // We got fully trimmed
            return (size_t)0;
        }
        // We're still one contiguous region (continue)

        entry = fdt_next_reserve_entry(fdt, entry);
    }

    // Make sure we don't free the FDT blob itself
    void* fdt_resv_start = (void*)fdt;
    size_t fdt_resv_size = fdt_size(fdt);

    if (trim_or_split_around_reserved(
            &start, &size, &split_start, &split_size, fdt_resv_start, fdt_resv_size
        )) {
        // We split into 2 regions
        return try_boot_free_region(fdt, start, size)
               + try_boot_free_region(fdt, split_start, split_size);
    }

    if (size == 0) {
        // We got fully trimmed
        return (size_t)0;
    }
    // We're still one contiguous region (continue)

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

    size_t total_mem_seen = 0;
    size_t total_mem_freed = 0;

    while (mem_node != NULL) {
        // TODO: Actually free memory
        size_t num_register_blocks = fdt_node_num_register_blocks(fdt, mem_node);

        for (size_t i = 0; i < num_register_blocks; i++) {
            void* address;
            size_t size;

            fdt_node_get_register_block(fdt, mem_node, 0, &address, &size);
            total_mem_seen += size;

            printk(
                "memory_node: (%s)[%c] {addr = %p, size = %p}\n",
                fdt_node_name(mem_node),
                '0' + (unsigned char)i,
                (void*)address,
                (void*)(uintptr_t)size
            );

            size_t freed = try_boot_free_region(fdt, address, size);
            total_mem_freed += freed;
        }

        mem_node = fdt_find_node_by_device_type(fdt, mem_node, "memory");
    }

    printk("[FDT] Freed Memory Regions:\n");
    printk("[FDT] \tTotal Memory = (%p bytes)\n", (void*)(uintptr_t)total_mem_seen);
    printk("[FDT] \tFreed Memory = (%p bytes)\n", (void*)(uintptr_t)total_mem_freed);

    return KERR_SUCCESS;
}
