# Set some basic compiler flags

set(AUTOCONF "${CMAKE_CURRENT_SOURCE_DIR}/include/generated/autoconf.h")

set(
  CMAKE_C_FLAGS
  "-ffreestanding -nostdlib -nostdinc -fdiagnostics-color=always -fno-common -include ${AUTOCONF} -Wno-pointer-arith ${CMAKE_C_FLAGS}"
)
set(
  CMAKE_CXX_FLAGS
  "-ffreestanding -nostdlib -nostdinc -nostdinc++ -fdiagnostics-color=always -fno-common -fno-rtti -fno-exceptions -include ${AUTOCONF} ${CMAKE_CXX_FLAGS}"
)

add_link_options(LINKER:--gc-sections)
