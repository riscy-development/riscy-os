# ----- Linker script support -----

macro(add_linker_script target_name script_name)
  add_custom_command(
    OUTPUT "${target_name}"
    # TODO maybe run the linker scripts through CPP?
    COMMAND cp "${script_name}" "${target_name}"
    DEPENDS "${script_name}"
    COMMENT "Generate linker script ${target_name}"
    VERBATIM USES_TERMINAL
  )
endmacro()
