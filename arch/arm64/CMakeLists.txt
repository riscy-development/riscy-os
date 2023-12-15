set(
  ARCH_ARM64_SOURCES
  # Runtime
  entry.S
  kernel.c
)

add_kernel_module(
  CORE # core module
  NAME "arch_arm64"
  SOURCES ${ARCH_ARM64_SOURCES}
  INCLUDE_DIRS ${__ARCH_KERN_INCLUDES}
)
