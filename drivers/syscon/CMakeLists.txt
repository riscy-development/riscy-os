list(
  APPEND SYSCON_SOURCES
  # main file
  syscon.c
)

list(
  APPEND SYSCON_INCLUDES
  # nothing
)

add_kernel_module(
  CORE
  NAME "syscon"
  SOURCES ${SYSCON_SOURCES}
  INCLUDE_DIRS ${SYSCON_INCLUDES}
)
