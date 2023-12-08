include(CMakeParseArguments)

macro(add_libc_module)
  cmake_parse_arguments(
    TARGET                 # prefix of output variables
    ""                     # boolean arguments
    "NAME"                 # Single value arguments
    "SOURCES;INCLUDE_DIRS" # multi value arguments

    ${ARGN}   # arguments of the function to parse, here we take the all original ones
  )

  # Make sure a name was provided
  if(NOT TARGET_NAME)
    message(FATAL_ERROR "You must provide a name for this libc module.")
    return()
  endif()

  # Prepend source dir to sources and includes
  list(
    TRANSFORM TARGET_SOURCES
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/"
  )
  list(
    TRANSFORM TARGET_INCLUDE_DIRS
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/"
  )

  # Log
  list(LENGTH TARGET_SOURCES _num_sources)
  message(STATUS "Added libc module ${TARGET_NAME} with ${_num_sources} sources")

  # Append to lib{c,k} sources
  list(
    APPEND NEW_SOURCES
    ${LIBC_SOURCES} ${TARGET_SOURCES}
  )
  set(LIBC_SOURCES ${NEW_SOURCES} PARENT_SCOPE)

  # Set include directories
  list(
    APPEND NEW_INCLUDES
    ${LIBC_PRIVATE_INCLUDES} ${TARGET_INCLUDE_DIRS}
  )
  set(LIBC_PRIVATE_INCLUDES ${NEW_INCLUDES} PARENT_SCOPE)
endmacro()
