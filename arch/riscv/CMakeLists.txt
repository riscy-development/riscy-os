set(
  ARCH_RISCV_SOURCES
  # Runtime
  entry.S
  kernel.c
)

list(APPEND ARCH_RISCV_INCLUDES include)

add_kernel_module(
  CORE # core module
  NAME "arch_riscv"
  SOURCES ${ARCH_RISCV_SOURCES}
  INCLUDE_DIRS ${ARCH_RISCV_INCLUDES}
)
