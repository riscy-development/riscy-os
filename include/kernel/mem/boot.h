#ifndef __KERNEL_MEM_BOOT_H__
#define __KERNEL_MEM_BOOT_H__

#include<stdint.h>
#include<stddef.h>
#include<kernel/error.h>

/*
 * ---Boot Allocator Front End---
 *
 * These functions can have multiple implementations selected by
 * which file(s) are compiled and linked in from the folder "kernel/mem/boot/"
 *
 * The memory detection system at boot should use "boot_free" to add memory
 * to the system and needs to be careful never to free 
 * the same address more than once.
 * (Even if some backends such as bitmaps could support that)
 *
 * NOTE:
 *       boot_alloc should be used sparingly and for permanent allocations.
 *       boot_free is meant more for giving more memory to the boot allocator
 *       than it is for freeing allocations made by boot_alloc (but theoretically could be used for that purpose).
*
 */

/*
 * Try to allocate "size" bytes with alignment "2**alignment",
 *
 * Returns a pointer to the region or NULL if the allocation failed.
 *
 */
void * boot_alloc(size_t size, unsigned int alignment);

/*
 * Tries to add the memory at physical address "start" of length "size" to the free list
 *
 * Returns:
 *     KERR_SUCCESS on success
 *     KERR_EXIST - if a double free is detected
 *     KERR_EMPTY - if the region is too small to free
 *     KERR_FULL - if the region is too large to free
 *     KERR_UNKNOWN - any other error (we don't know what the backend is so there could be weird reasons we can anticipate)
 *
 */
kerror_t boot_free(void *start, size_t size);

#endif
