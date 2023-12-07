include(CMakeParseArguments)


macro(add_kernel_module)
  cmake_parse_arguments(
    TARGET                 # prefix of output variables
    "CORE;LOADED"          # Boolean arguments
    "NAME"                 # Single value arguments
    "SOURCES;INCLUDE_DIRS" # multi value arguments

    ${ARGN}   # arguments of the function to parse, here we take the all original ones
  )
  # TODO: Implement module loading
  if (TARGET_LOADED)
    message(FATAL_ERROR "Loading modules is not supported yet")
    return()
  endif()

  # Make sure a name was provided
  if(NOT TARGET_NAME)
    message(FATAL_ERROR "You must provide a name for this kernel module.")
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
  message(STATUS "Added module ${TARGET_NAME} with ${TARGET_SOURCES}")

  # Append to kernel sources
  # TODO module loading
  list(
    APPEND NEW_SOURCES
    ${KERNEL_SOURCES} ${TARGET_SOURCES}
  )
  set(KERNEL_SOURCES ${NEW_SOURCES} PARENT_SCOPE)

  # Set include directories
  list(
    APPEND NEW_INCLUDES
    ${KERNEL_INCLUDE_DIRS} ${TARGET_INCLUDE_DIRS}
  )
  set(KERNEL_INCLUDE_DIRS ${NEW_INCLUDES} PARENT_SCOPE)
endmacro()
