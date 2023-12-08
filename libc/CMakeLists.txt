# ----- Sources -----

# Set up sources and private includes
list(APPEND LIBC_SOURCES)
list(APPEND LIBC_PRIVATE_INCLUDES)

# Add submodules
include(cmake/modules.cmake)
add_subdirectory(src)

# Set up includes
list(
  APPEND LIBC_INCLUDES
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

message("${ARCH_LIBK_INCLUDES}")
list(
  APPEND LIBK_INCLUDES
  ${LIBC_INCLUDES}
  "${CMAKE_CURRENT_SOURCE_DIR}/../include" # kernel includes
  "${ARCH_LIBK_INCLUDES}"                  # arch-specific includes
)

# ----- Targets -----
# Create targets
add_library(riscy-libc STATIC ${LIBC_SOURCES})  # TODO how do we build shared libc?
add_library(riscy-libk STATIC ${LIBC_SOURCES})

# Set include paths
target_include_directories(riscy-libc PUBLIC ${LIBC_INCLUDES})
target_include_directories(riscy-libk PUBLIC ${LIBK_INCLUDES})

target_include_directories(riscy-libc PRIVATE ${LIBC_PRIVATE_INCLUDES})
target_include_directories(riscy-libk PRIVATE ${LIBC_PRIVATE_INCLUDES})

# Link to libgcc
target_link_libraries(riscy-libc libgcc)
target_link_libraries(riscy-libk libgcc)

# Set compile defs
target_compile_definitions(riscy-libk PRIVATE KERNEL) # -D KERNEL

# Force include autoconf.h
set(AUTOCONF_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/../include/generated/autoconf.h")

set(CMAKE_C_FLAGS "-include ${AUTOCONF_HEADER} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-include ${AUTOCONF_HEADER} ${CMAKE_CXX_FLAGS}")

# Build libk with kernel flags
separate_arguments(ARCH_LIBK_C_FLAGS UNIX_COMMAND ${ARCH_KERN_C_FLAGS})
separate_arguments(ARCH_LIBK_CXX_FLAGS UNIX_COMMAND ${ARCH_KERN_CXX_FLAGS})

target_compile_options(
  riscy-libk PRIVATE
  $<$<COMPILE_LANGUAGE:C>:${ARCH_LIBK_C_FLAGS}>
  $<$<COMPILE_LANGUAGE:CXX>:${ARCH_LIBK_CXX_FLAGS}>
)
