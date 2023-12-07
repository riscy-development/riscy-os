# Bitness configs
if (CONFIG_RISCV64)
  set(RISCV_MARCH "rv64gc")
  set(RISCV_MABI  "lp64d")
else() # riscv32
  set(RISCV_MARCH "rv32gc")
  set(RISCV_MABI  "ilp32d")
endif()

# Universal
#set(ARCH_CFLAGS
