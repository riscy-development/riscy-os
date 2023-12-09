
#include <kernel/mem/boot.h>

// Linked list of free memory regions
static boot_free_region * boot_free_list = NULL;

// How big will the region be if we allocate from the end?
static ssize_t region_size_alloc_after(struct boot_free_region *region, size_t size, unsigned int alignment)
{
  uintptr_t end = (uintptr_t)region + region->size;
  // Get how far off from being aligned it is
  size_t unalignment = (end-size) & ((1ull<<alignment)-1ull);
  return region->size - size - unalignment;
}

// How big will the region be if we allocate from the start?
static ssize_t region_size_alloc_before(struct boot_free_region *region, size_t size, unsigned int alignment)
{
  uintptr_t start = (uintptr_t)region;
  // Get how far off from being aligned it is
  size_t unalignment = (1ull<<alignment) - (start & ((1ull<<alignment)-1ull));
  return region->size - size - unalignment;
}

static void *
alloc_after(struct boot_free_region *region, size_t size, unsigned int alignment)
{
  uintptr_t end = (uintptr_t)region + region->size;
  // Get how far off from being aligned it is
  size_t unalignment = (end-size) & ((1ull<<alignment)-1ull);
  region->size -= (size + unalignment);

  if(region->size < sizeof(struct boot_free_region)) {
    // Remove this region from the list
    if(region->prev != NULL) {
      region->prev->next = region->next;
    } else {
      // If we don't have a previous, we are the boot_free_list
      assert(boot_free_list == region);
      boot_free_list = region->next;
    }
    if(region->next != NULL) {
      region->next->prev = region->prev;
    }
  }

  return (void*)region + region->size;
}

static void *
alloc_before(struct boot_free_region * region, size_t size, unsigned int alignment)
{
  uintptr_t start = (uintptr_t)region;
  // Get how far off from being aligned it is
  size_t unalignment = (1ull<<alignment) - (start & ((1ull<<alignment)-1ull));
  size_t new_size = region->size - (size + unalignment);
  
  if(new_size < sizeof(struct boot_free_region)) {
    // Remove this region from the list
    if(region->prev) {
      region->prev->next = region->next;
    } else {
      // If we don't have a previous, we are the boot free list
      assert(boot_free_list == region);
      boot_free_list = region->next;
    }
    if(region->next) {
      region->next->prev = region->prev;
    }
  }
  else {
    // We need to move the region
    struct boot_free_region *prev = region->prev;
    struct boot_free_region *next = region->next;
    struct boot_free_region *new_region = (struct boot_free_region*)((void*)region + (size + unalignment));
    new_region->size = new_size;
    new_region->prev = prev;
    new_region->next = next;

    if(prev != NULL) {
      prev->next = new_region;
    } else {
      // If we don't have a previous, we are the boot free list
      assert(boot_free_list == region);
      boot_free_list = new_region;
    }
    if(next != NULL) {
      next->prev = new_region;
    }
  }
  return (void*)region + unalignment;
}

void * 
boot_alloc(size_t size, unsigned int alignment) 
{
  struct boot_free_region *smallest_fitting = NULL;
  size_t smallest_fit = (size_t)(-1);
  bool should_alloc_before = false;

  struct boot_free_region *curr = boot_free_list_begin();
  while(curr != NULL) {
    ssize_t after = region_size_alloc_after(curr,size,alignment);
    ssize_t before = region_size_alloc_before(curr,size,alignment);
    ssize_t greater = after > before ? after : before;
    if(greater >= 0) {
      if(smallest_fit > greater) {
        smallest_fit = greater;
        smallest_fitting = curr;
        if(before == greater) {
          should_alloc_before = true;
        } else {
          should_alloc_before = false;
        }
      }
    }
    curr = boot_free_list_next(curr);
  }

  if(smallest_fitting == NULL) {
    return NULL;
  }

  if(should_alloc_before) {
    return alloc_before(smallest_fitting,size,alignment);
  } else {
    return alloc_after(smallest_fitting,size,alignment);
  }
}

kerror_t
boot_free(void *start, size_t size)
{
  // TODO: This function could be waaaaaay smarter (see note in header)

  void *end = start + size;
  struct boot_free_region *region = (struct boot_free_region *)start;

  if(boot_free_list == NULL) {

    if(size < sizeof(struct boot_free_region)) {
      return KERR_EMPTY;
    }

    region->prev = NULL;
    region->next = NULL;
    region->size = size;
    boot_free_list = region;
    return KERR_SUCCESS;
  }

  // We need to make sure that there are no overlapping regions

  struct boot_free_region *fully_before = NULL;
  struct boot_free_region *full_after = NULL;

  struct boot_free_region *curr = boot_free_list_begin();
  while(curr != NULL) {
    void *curr_end = curr + curr->size;
    if(curr_end == start) {
      // Special case: We can fully merge with the one before
      struct boot_free_region *next = boot_free_list_next(curr);
      if(next != NULL && (void*)next == end) {
        // Extra special case: We can fully merge both before and after
        curr->size += (size + next->size);

        // Get rid of the "next" region
        curr->next = next->next;
        if(curr->next) {
          curr->next->prev = next->prev;
        }
        return KERR_SUCCESS;

      } else if(next == NULL || (void*next) > end) {
       // Can only merge with before
       curr->size += size;
       return KERR_SUCCESS;
      } else {
        // Overlap with after (double free error)
        return KERR_EXIST;
      }
    } else if(curr_end < start) {
      fully_before = curr;
    } else if((void*)curr > end) {
      fully_after = curr;
      // Regions should be in order so if we've come across our first fully after, then we've found all we need to
      break;
    } else {
      // It's not fully before or fully after: OVERLAP!
      return KERR_EXIST;
    }
  }

  // fully_before and/or fully_after should now be populated
  if(fully_before == NULL && fully_after == NULL) {
    // This shouldn't happen but let's be safe
    return KERR_EXIST;
  }

  region->before = fully_before;
  region->after = fully_after;
  region->size = size;
  return KERR_SUCCESS;
}

struct boot_free_region *
boot_free_list_begin(void)
{
  return boot_free_list;
}

struct boot_free_region *
boot_free_region_next(struct boot_free_region *region)
{
  return (struct boot_free_region*)region->next;
}

