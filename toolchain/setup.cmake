# Cross-compilation config for cmake
# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_CROSSCOMPILING ON)

set(CMAKE_SYSROOT "${CMAKE_BINARY_DIR}/root")

# Set processor type
set(CMAKE_SYSTEM_PROCESSOR "${CONFIG_ARCH_NAME}")

# Get path to toolchain bins
# https://stackoverflow.com/a/12854575
set(TOOLCHAIN_PATH "${CMAKE_CURRENT_LIST_DIR}/local/bin")
set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/${CONFIG_ARCH_NAME}-elf")

# Set root path
set(CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/local")

# Adjust behavior of `find_` commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  # only search in host env
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)   # only look in target env
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)   # only look in target env

# Set compiler paths
set(CMAKE_C_COMPILER    ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER  ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER  ${TOOLCHAIN_PREFIX}-as)
set(CMAKE_LINKER        ${TOOLCHAIN_PREFIX}-ld)
set(CMAKE_RANLIB        ${TOOLCHAIN_PREFIX}-ranlib)
set(CMAKE_STRIP         ${TOOLCHAIN_PREFIX}-strip)
set(CMAKE_AR            ${TOOLCHAIN_PREFIX}-ar)

# Output ELF64 if using NASM
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
