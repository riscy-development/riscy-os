set(
  KERNEL_OF_SOURCES
  # Runtime
  fdt.c
  syscon.c
)

list(APPEND KERNEL_OF_INCLUDES include)

add_kernel_module(
  CORE # core module
  NAME "open_firmware"
  SOURCES ${KERNEL_OF_SOURCES}
  INCLUDE_DIRS ${KERNEL_OF_INCLUDES}
)
