cmake_minimum_required(VERSION 3.25)

# Configuration
include(cmake/prelude.cmake)
include(.config.cmake)

# Set up toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain/setup.cmake")
include(toolchain/flags.cmake)

# Create project
project(
  riscy-os
  VERSION 0.1.0
  DESCRIPTION "An OS for RISC-V machines."
  HOMEPAGE_URL "https://github.com/riscy-development/riscy-os"
  LANGUAGES C CXX ASM
)

# ----- LANGUAGE CONFIGURATION -----

# Safety
include(cmake/flags.cmake)
include(cmake/linters.cmake)

# Language config
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
# TODO: use proper build types
if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
  message(FATAL
    ": Don't use CMAKE_BUILD_TYPE when building riscy-os.\n"
    "The default build type is optimized with debug info and asserts enabled,\n"
    "and that's all there is."
  )
endif()


# ----- ARCHITECTURE CONFIGURATION -----

# Grab arch-specific config
include("arch/${CONFIG_ARCH_DIR}/config.cmake")

# Set up linker scripts
include("cmake/lds.cmake")

# add_linker_script("user.ld" ${ARCH_USER_LINKER_SCRIPT})
# set(USER_LINKER_SCRIPT "${CMAKE_BINARY_DIR}/kernel.ld")

# Set up installation
set(CMAKE_STAGING_PREFIX "${CMAKE_SYSROOT}")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SYSROOT}")

# Set output directories
# This is for userspace bins
set(
  EXECUTABLE_OUTPUT_PATH
  "${CMAKE_SYSROOT}/bin" CACHE PATH "Build directory" FORCE
)
set(
  LIBRARY_OUTPUT_PATH
  "${CMAKE_SYSROOT}/usr/lib" CACHE PATH "Build directory" FORCE
)

# ----- KERNEL -----

# Set up libgcc
add_library("libgcc" STATIC IMPORTED GLOBAL)
set_property(
  TARGET "libgcc"
  PROPERTY IMPORTED_LOCATION
  "${CMAKE_SOURCE_DIR}/toolchain/build/${CMAKE_SYSTEM_PROCESSOR}/gcc/gcc/libgcc.a"
)

# Add the kernel
add_subdirectory(kernel)

# ----- USERSPACE -----

# TODO
