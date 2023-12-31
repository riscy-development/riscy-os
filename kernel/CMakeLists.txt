include(cmake/root_dirs.cmake)

# ----- Dependencies -----

# Generate linker script
set(KERN_LINKER_SCRIPT "${CMAKE_BINARY_DIR}/kernel.ld")
add_linker_script(${KERN_LINKER_SCRIPT} ${ARCH_KERN_LINKER_SCRIPT})

# Add target
add_custom_target(
  kernel_linker_script
  DEPENDS ${KERN_LINKER_SCRIPT}
)

# ----- Flags -----

# Force include autoconf.h
set(AUTOCONF_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../include/generated/autoconf.h")

set(KERN_C_FLAGS "-include ${AUTOCONF_HEADER}")
set(KERN_CXX_FLAGS "-include ${AUTOCONF_HEADER}")

# Set full command line
set(CMAKE_C_FLAGS "${KERN_C_FLAGS} ${ARCH_KERN_C_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${KERN_CXX_FLAGS} ${ARCH_KERN_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

# Run checks
include(cmake/checks.cmake)

# ----- Sources -----

# Get list of sources
set(
  KERNEL_SOURCES

  # Main script
  #kmain.cpp
  early_output.c
)

# Get list of include directories
list(
  APPEND KERNEL_INCLUDE_DIRS

  # Main includes
  "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

# Add other directories
include(cmake/modules.cmake)

add_root_dir("arch/${CONFIG_ARCH_DIR}")  # architecture directory
add_root_dir("drivers")

# Internal modules
add_subdirectory("of") # Open Firmware Support
add_subdirectory("mem") # Memory Framework / Memory Allocators 

# ----- Add target -----

# Create executable
add_executable(riscy-os ${KERNEL_SOURCES})

# Libraries
target_link_libraries(riscy-os libgcc riscy-libk)

# Set include directories
target_include_directories(riscy-os PUBLIC ${KERNEL_INCLUDE_DIRS})

# Add defines
target_compile_definitions(riscy-os PUBLIC KERNEL) # -D KERNEL

# Set up linking
target_link_options(riscy-os PUBLIC "LINKER:-T${KERN_LINKER_SCRIPT}")

# Set properties of target
set_target_properties(
  riscy-os PROPERTIES
  OUTPUT_NAME "riscy-os.elf"                      # Output file name
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"  # Output directory
  LINK_DEPENDS ${KERN_LINKER_SCRIPT}              # Dependency on linker script
)

add_dependencies(riscy-os kernel_linker_script)

# Install the executable to /boot
install(
  TARGETS riscy-os
  DESTINATION boot
)
