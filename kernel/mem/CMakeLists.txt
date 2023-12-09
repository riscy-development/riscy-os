set(
  KERNEL_MEM_SOURCES
  # Runtime
)

add_subdirectory("boot") # Boot Memory Allocator Implementations

list(APPEND KERNEL_MEM_INCLUDES include)

add_kernel_module(
  CORE # core module
  NAME "memory"
  SOURCES ${KERNEL_MEM_SOURCES}
  INCLUDE_DIRS ${KERNEL_MEM_INCLUDES}
)
