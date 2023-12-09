#ifndef __KERNEL_MEM_BOOT_H__
#define __KERNEL_MEM_BOOT_H__

#include<stdint.h>
#include<kernel/error.h>

// Doubly-linked list of free regions in physical memory
struct boot_free_region {
  struct boot_free_region *next;
  struct boot_free_region *prev;
  size_t size;
};

/*
 * Try to allocate "size" bytes with alignment "2**alignment",
 * shrinking the boot_free_list.
 *
 * Returns a pointer to the region or NULL if the allocation failed.
 *
 * Invalidates all struct boot_free_region pointers
 */
void * boot_alloc(size_t size, unsigned int alignment);

/*
 * Tries to add the memory at physical address "start" of length "size" to the free list
 *
 * Returns:
 *     KERR_SUCCESS on success
 *     KERR_EXIST if the region overlaps an existing region
 *     KERR_EMPTY if the region is too small (size < sizeof(struct boot_free_region))
 *
 * NOTE: boot_free is not perfect,
 *       boot_alloc should be used sparingly and for permenant allocations
 *       boot_free is meant more for giving more memory to the boot allocator
 *       than it is for freeing allocations made by boot_alloc.
 */
kerror_t boot_free(void *start, size_t size);

/*
 * Get the first (lowest address) region in the free list
 */
struct boot_free_region * boot_free_list_begin(void);

/*
 * Get the next region (next higher address) in the free list (or NULL)
 */
struct boot_free_region * boot_free_list_next(struct boot_free_region *region);


#endif
