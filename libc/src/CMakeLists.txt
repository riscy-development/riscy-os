# Add modules
add_subdirectory(stdlib)
add_subdirectory(string)

# Propagate upwards
set(NEW_SOURCES ${LIBC_SOURCES})
set(LIBC_SOURCES ${NEW_SOURCES} PARENT_SCOPE)

set(NEW_INCLUDES ${LIBC_PRIVATE_INCLUDES})
set(LIBC_PRIVATE_INCLUDES ${NEW_INCLUDES} PARENT_SCOPE)
