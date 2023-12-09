set(
  KERNEL_MEM_SOURCES
  # Runtime
  boot.c
)

list(APPEND KERNEL_MEM_INCLUDES include)

add_kernel_module(
  CORE # core module
  NAME "memory"
  SOURCES ${KERNEL_OF_SOURCES}
  INCLUDE_DIRS ${KERNEL_OF_INCLUDES}
)
