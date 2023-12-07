MAKEFLAGS := -j 1

# include generated configuration
-include include/config/auto.conf

KBUILD := tools/local/bin/kbuild-mconf
CONFIG := .config .config.cmake
CONFIG_GEN := $(wildcard include/config/*) $(wildcard include/generated/*)

BUILD := build
BUILD_SCRIPT := $(BUILD)/build.ninja

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
$(BUILD_SCRIPT): $(CONFIG) toolchain
	mkdir -p $(BUILD)
	cd $(BUILD); cmake -G Ninja ..

.PHONY: kernel
kernel: $(BUILD)/build.ninja
	ninja -C build install

# Run
run: kernel
	scripts/run.sh

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
	rm -rf $(BUILD)

full-clean: clean
	toolchain/clean.sh
	tools/clean.sh

# Deps
-include include/config/auto.conf.cmd
