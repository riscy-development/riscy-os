MAKEFLAGS := -j 1

# include generated configuration
-include include/config/auto.conf

KBUILD := tools/local/bin/kbuild-mconf
CONFIG := .config .config.cmake
CONFIG_GEN := $(wildcard include/config/*) $(wildcard include/generated/*)


.PHONY: default
default: kernel


# Configuration
.PHONY: menuconfig
menuconfig: $(KBUILD)
	@tools/local/bin/kbuild-mconf Kconfig

.config: menuconfig

.config.cmake: .config
	scripts/config_to_cmake.py .config .config.cmake

# Build
.PHONY: kernel
kernel: $(CONFIG) toolchain
	echo TODO

# Build tools
$(KBUILD):
	tools/build.sh

.PHONY: toolchain _toolchain
toolchain: $(CONFIG)
	$(MAKE) _toolchain

_toolchain:
	toolchain/build.sh $(CONFIG_ARCH_NAME)

# Cleanup
clean:
	rm -f $(CONFIG) $(CONFIG_GEN)

full-clean: clean
	toolchain/clean.sh
	tools/clean.sh

# Deps
-include include/config/auto.conf.cmd
