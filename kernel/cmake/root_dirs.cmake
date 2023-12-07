macro(add_root_dir dir)
  add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/../${dir}"
    "${CMAKE_CURRENT_BINARY_DIR}/kroot/${dir}"
  )
endmacro()
