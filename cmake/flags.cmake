set(
  DEFAULT_C_FLAGS
  "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion \
  -Wcast-qual -Wformat=2 -Wundef -Werror=float-equal -Wshadow -Wcast-align \
  -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough \
  -Wextra-semi"
)
set(
  DEFAULT_CXX_FLAGS
  "${DEFAULT_C_FLAGS} -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast"
)

# Set the flags
set(CMAKE_C_FLAGS "${DEFAULT_C_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

# Add debug flags if needed
if (CONFIG_DEBUG_SYMBOLS)
  set(CMAKE_C_FLAGS "-g4 ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "-g4 ${CMAKE_CXX_FLAGS}")
endif()
