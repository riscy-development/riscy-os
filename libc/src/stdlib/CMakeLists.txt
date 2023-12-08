list(
  APPEND STDLIB_SOURCES

  abort.c
)

list(
  APPEND STDLIB_INCLUDES
  # Nothing
) 

add_libc_module(
  NAME "stdlib"
  SOURCES ${STDLIB_SOURCES}
  INCLUDE_DIRS ${STDLIB_INCLUDES}
)
