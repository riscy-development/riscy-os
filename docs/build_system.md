# Build System

## TL;DR

### Files

These are the most important files:

- `toolchain/flags.cmake`: Toolchain-*specific* flags (mostly `-f`)
- `cmake/flags.cmake`: Toolchain-*independent* flags (i.e., warnings)
- `arch/ARCH/config.cmake`: sets up flags for this architecture
- `arch/ARCH/CMakeLists.txt`: specifies how to build the arch-specific code as a
  `CORE` (compiled-in) kernel module
- `kernel/CMakeLists.txt`: adds kernel-specific code to build
- `libc/src/CMakeLists.txt`: listing of all libc modules
- `libc/src/MODULE/CMakeLists.txt`: specifies how to build that module.

### What file should I go to?

- Toolchain-specific flags: `toolchain/flags.cmake`
- Toolchain-independent flags: `cmake/flags.cmake`
- Arch-specific flags: `arch/ARCH/config.cmake`
- Arch-specific code: `arch/ARCH/CMakeLists.txt`
- New kernel code: `kernel/CMakeLists.txt`, either touch `KENREL_SOURCES` or run
  `add_subdirectory`
- New libc code: `libc/src/MODULE/CMakeLists.txt`
- New libc module: `libc/src/CMakeLists.txt`

## Details

### Toolchain

These files set up the toolchain so it can build anything. This is the bare
minimum to get an executable output.

- `toolchain/setup.cmake`: a CMake TOOLCHAIN file, this is run very early and
  tells CMake the paths to the compiler and assorted tools.
- `toolchain/flags.cmake`: sets flags specific to the toolchain, the bare minimum
  for the project to even build.
  
### Top Level

The next thing that runs are the top level CMake files. These do the following:

- `CMakeLists.txt`: language configuration, linter configuration, and then adds
   the architecture, libc, and kernel directories.
- `cmake/flags.cmake`: Toolchain-*independent* flags for things like warnings.
- `cmake/linters.cmake`: Sets up linters. **CURRENTLY BROKEN**
- `cmake/prelude.cmake`: Makes sure no one is building the project in a dumb way,
  also exports compile commands.
- `cmake/lds.cmake`: Add a recipe for "building" linker scripts.

### Architecture

Now, we get to our architecture-specific configuration.

- `arch/ARCH/config.cmake`: sets up linker and C flags, export specific flags for
  the kernel and userspace. Must export the following:
      - `ARCH_KERN_C_FLAGS`
      - `ARCH_KERN_CXX_FLAGS`
      - `ARCH_LIBK_INCLUDES`
      - `ARCH_KERN_LINKER_SCRIPT`: path to kernel linker script
- `arch/ARCH/CMakeLists.txt`: instantiates the architecture-specific configuration
  as a `CORE` kernel module (i.e., compiled in).
  
### Kernel

- `kernel/cmake/modules.cmake`: specifies how to build a kernel module.
- `kernel/cmake/root_dirs.cmake`: helper to add repo root directories.
- `kernel/CMakeLists.txt`: specifies how to build the kernel, runs 
  `add_subdirectory` a whole bunch to add all kernel modules. Shouldn't need to
  touch this much.
  
### Libc

- `libc/cmake/modules.cmake`: specifies how to build a libc module
- `libc/CMakeLists.txt`: specifies how to build both `libc` and `libk`. Shouldn't
  need to touch this much.
- `libc/src/CMakeLists.txt`: listing of all libc modules
- `libc/src/MODULE/CMakeLists.txt`: specifies how to build that module.
