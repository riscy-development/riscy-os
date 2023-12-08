set(
  ARCH_RISCV_SOURCES
  # Runtime
  entry.S
  kernel.c
)

add_kernel_module(
  CORE # core module
  NAME "arch_riscv"
  SOURCES ${ARCH_RISCV_SOURCES}
  INCLUDE_DIRS ${__ARCH_KERN_INCLUDES}
)
