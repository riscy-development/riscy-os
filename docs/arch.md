# Architecture-specific code

Here is all that must be defined in `arch/ARCH/` for a new architecture.

1. Linker scripts.
2. CMake files (see [Build System Docs](./build_system.md)).
3. Processing of `_preinit`, `_init`, and `_fini` as defined by GCC. 
4. Kernel entrypoint (`kmain(...)` and `entry.S`).
5. A putchar must be registered with `early_putchar`
