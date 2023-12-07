cmake_minimum_required(VERSION 3.27)

# Configuration
include(cmake/prelude.cmake)

include(.config.cmake)
include(toolchain/setup.cmake)

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

message("CFLAGS: ${CMAKE_C_FLAGS}")
message("CXXFLAGS: ${CMAKE_CXX_FLAGS}")

# ----- ARCHITECTURE CONFIGURATION -----

# Grab arch-specific config
include("arch/${CONFIG_ARCH_DIR}/config.cmake")




# TODO
# Export needed flags
# set(TOOLCHAIN_CFLAGS   "-ffreestanding -nostartfiles -nostdlib -nodefaultlibs -Wl,--gc-sections")
# set(TOOLCHAIN_CXXFLAGS "-fno-rtti -fno-exceptions ${TOOLCHAIN_CFLAGS}")
# set(TOOLCHAIN_LDFLAGS  "--gc-sections")

# if (CONFIG_RISCV)
#   set(TOOLCHAIN_CFLAGS   "-mcmodel=medany ${TOOLCHAIN_CFLAGS}")
#   set(TOOLCHAIN_CXXFLAGS "-mcmodel=medany ${TOOLCHAIN_CXXFLAGS}")
# endif()

