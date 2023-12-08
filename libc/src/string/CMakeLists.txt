list(
  APPEND STRING_SOURCES
  # String manipulation

  # String examination
  strlen.c
  strcmp.c

  # Character array manipulation
  memcpy.c
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
