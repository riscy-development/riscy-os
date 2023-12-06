# TODO configurable by arch
TOOLCHAIN := toolchain/local/bin/riscv64-elf-gcc toolchain/local/bin/riscv64-elf-gdb toolchain/local/bin/riscv64-elf-ld
KBUILD := tools/local/bin/kbuild-mconf


.PHONY: all
all: .config $(TOOLCHAIN)


# Configuration
.PHONY: menuconfig
menuconfig: $(KBUILD)
	@tools/local/bin/kbuild-mconf Kconfig

.config: menuconfig Kconfig


# Build tools
$(KBUILD):
	@tools/build.sh

$(TOOLCHAIN):
	@toolchain/build.sh
