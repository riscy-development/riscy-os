# Bitness configs
if (CONFIG_RISCV64)
  set(RISCV_MARCH "rv64gc")
  set(RISCV_MABI  "lp64d")
  set(RISCV_EMODE "elf64lriscv") # linker emulation mode
else() # riscv32
  set(RISCV_MARCH "rv32gc")
  set(RISCV_MABI  "ilp32d")
  set(RISCV_EMODE "elf32lriscv") # linker emulation mode
endif()

# Universal
add_link_options(LINKER:-m${RISCV_EMODE})
add_link_options(LINKER:-zmax-page-size=0x1000)
add_link_options(LINKER:-nostdlib)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mno-relax")

#
# Kernelspace
#

# Flags
set(
  ARCH_KERN_C_FLAGS
  "-mno-relax -march=${RISCV_MARCH} -mabi=${RISCV_MABI} -mcmodel=medany \
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
  "${CMAKE_SOURCE_DIR}/arch/riscv/ld/qemu-virt64-kernel.ld"
)

# TODO userspace linker script
