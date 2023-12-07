# Generate linker script
set(KERN_LINKER_SCRIPT "${CMAKE_BINARY_DIR}/kernel.ld")
add_linker_script(${KERN_LINKER_SCRIPT} ${ARCH_KERN_LINKER_SCRIPT})

add_custom_target(
  foo ALL
  DEPENDS
  "${KERN_LINKER_SCRIPT}"
)
