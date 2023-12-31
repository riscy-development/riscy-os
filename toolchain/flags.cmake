# Set some basic compiler flags

set(
  CMAKE_C_FLAGS
  "-ffreestanding -nostdlib -nostdinc -fdiagnostics-color=always -fno-common ${CMAKE_C_FLAGS}"
)
set(
  CMAKE_CXX_FLAGS
  "-ffreestanding -nostdlib -nostdinc -nostdinc++ -fdiagnostics-color=always -fno-common -fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}"
)

add_link_options(LINKER:--gc-sections)
