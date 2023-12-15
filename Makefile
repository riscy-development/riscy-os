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
.PHONY: menuconfig cfg
menuconfig: $(KBUILD)
	@tools/local/bin/kbuild-mconf Kconfig

cfg: menuconfig

.config:
	@echo "Please run \`make menuconfig\` first to configure the build"
	@exit 1

.config.cmake: .config
	scripts/config_to_cmake.py .config .config.cmake

# Build
$(BUILD_SCRIPT): .config.cmake
	mkdir -p $(BUILD)
	cd $(BUILD); cmake -G Ninja ..

.PHONY: kernel
kernel: | toolchain $(BUILD_SCRIPT)
	ninja -C build install

# Run
.PHONY: run format cloc spellcheck

run: kernel
	scripts/run-$(CONFIG_ARCH_NAME).sh

format:
	scripts/format.sh

spellcheck:
	codespell

cloc:
	@scc --exclude-dir 'toolchain/' --exclude-dir 'tools/'

# Build tools
$(KBUILD):
	tools/build.sh

.PHONY: toolchain _toolchain
toolchain: .config.cmake
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
