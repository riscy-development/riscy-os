# ----- Linker script support -----

function(add_linker_script target_name script_name)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${target_name}
    # TODO maybe run the linker scripts through CPP?
    COMMAND cp ${script_name} ${CMAKE_CURRENT_BINARY_DIR}/${target_name}
    DEPENDS ${script_name}
    VERBATIM
  )
endfunction()
