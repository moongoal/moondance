include_guard()

function(md_configure_target TARGET_NAME)
  set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 99)

  target_compile_options(
   ${TARGET_NAME} 
    PRIVATE 
      "-Wall"
      "-Werror"
      "-Wextra"
      "-pedantic"
  )
endfunction()
