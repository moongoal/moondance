cmake_minimum_required(VERSION 3.31)

project(moondance VERSION 2.0.0)
enable_testing()

set(C_STANDARD_REQUIRED ON)

include(CPack)
include(scripts/test.cmake)
include(scripts/install.cmake)
include(scripts/target.cmake)

add_library(moondance STATIC src/test.c)
md_configure_target(moondance)

target_include_directories(
  moondance
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

configure_file(
  include/moondance/version.h.in
  include/moondance/version.h
)

target_sources(
  moondance
  PUBLIC
  FILE_SET headers
  TYPE HEADERS

  BASE_DIRS
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include

  FILES
    include/moondance/test.h
    include/moondance/util.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/moondance/version.h
)

md_add_test(test_test test/test.c)

md_configure_install()
