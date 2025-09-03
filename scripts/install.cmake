include_guard()

include(CMakePackageConfigHelpers)

function(md_configure_install)
  set(config_file_install_dir lib/cmake/moondance)
  set(export_name MoondanceTargets)
  set(config_file_name MoondanceConfig.cmake)

  install(
    TARGETS moondance
    EXPORT ${export_name}
    FILE_SET headers
  )

  install(
    EXPORT ${export_name}
    NAMESPACE moondance::
    DESTINATION lib/cmake/moondance
  )

  configure_package_config_file(
    scripts/${config_file_name}.in
    ${config_file_name}
    INSTALL_DESTINATION ${config_file_install_dir}
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
    NO_SET_AND_CHECK_MACRO
  )

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${config_file_name}
    DESTINATION ${config_file_install_dir}
  )
endfunction()
