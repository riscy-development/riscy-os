list(
  APPEND STDIO_SOURCES

  printf.c
)

list(
  APPEND STDIO_INCLUDES
  # Nothing
) 

add_libc_module(
  NAME "stdio"
  SOURCES ${STDIO_SOURCES}
  INCLUDE_DIRS ${STDIO_INCLUDES}
)
