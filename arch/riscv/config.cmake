# Bitness configs
if (CONFIG_RISCV64)
  set(RISCV_MARCH "rv64gc")
  set(RISCV_MABI  "lp64d")
else() # riscv32
  set(RISCV_MARCH "rv32gc")
  set(RISCV_MABI  "ilp32d")
endif()

# Universal
add_link_options(
  LINKER:-m elf_${CONFIG_ARCH_NAME} -z max-page-size=0x1000 -nostdlib
)
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mno-relax")

# Kernelspace
set(
  ARCH_KERN_C_FLAGS
  "-mno-relax -march=${RISCV_MARCH} -mabi=${RISCV_MABI} -mcmodel=medany \
   -fno-stack-protector -fno-omit-frame-pointer"
)
set(ARCH_KERN_CXX_FLAGS "${ARCH_KERN_C_FLAGS}")

# Userspace
# TODO add a userspace

# Linker script
set(
  ARCH_KERN_LINKER_SCRIPT
  "${CMAKE_SOURCE_DIR}/arch/riscv/ld/qemu-virt64-kernel.ld"
)

# TODO userspace linker script
