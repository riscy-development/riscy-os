
# Universal
add_link_options(LINKER:-zmax-page-size=0x1000)
add_link_options(LINKER:-nostdlib)

#
# Kernelspace
#

# Flags
set(
  ARCH_KERN_C_FLAGS
  "-mcmodel=large \
   -fno-stack-protector -fno-omit-frame-pointer"
)
set(ARCH_KERN_CXX_FLAGS "${ARCH_KERN_C_FLAGS}")

# Includes
list(
  APPEND ARCH_KERN_INCLUDES
  include
)

set(ARCH_LIBK_INCLUDES ${ARCH_KERN_INCLUDES})
list(
  TRANSFORM ARCH_LIBK_INCLUDES
  PREPEND "${CMAKE_CURRENT_LIST_DIR}/"  # since this file is included
)

# Userspace
# TODO add a userspace

# Linker script
set(
  ARCH_KERN_LINKER_SCRIPT
  "${CMAKE_SOURCE_DIR}/arch/arm64/ld/qemu-virt64-kernel.ld"
)

# TODO userspace linker script
