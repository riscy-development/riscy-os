# TODO: Select different files to be used as sources depending on Kconfig's choice of Boot Allocator
set(
  KERNEL_BOOT_ALLOC_SOURCES
  # Runtime
  free_list.c
)

list(APPEND KERNEL_BOOT_ALLOC_INCLUDES include)

add_kernel_module(
  CORE # core module
  NAME "boot_allocator"
  SOURCES ${KERNEL_BOOT_ALLOC_SOURCES}
  INCLUDE_DIRS ${KERNEL_BOOT_ALLOC_INCLUDES}
)
