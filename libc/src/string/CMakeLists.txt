list(
  APPEND STRING_SOURCES
  # TODO multiple files
  string.c
)

list(
  APPEND STRING_INCLUDES
  # Nothing
) 

add_libc_module(
  NAME "string"
  SOURCES ${STRING_SOURCES}
  INCLUDE_DIRS ${STRING_INCLUDES}
)
