cmake_minimum_required(VERSION 3.31)

project(moondance)
enable_testing()

set(MD_VERSION_MAJOR 2)
set(MD_VERSION_MINOR 0)
set(MD_VERSION_PATCH 0)
set(MD_VERSION "${MD_VERSION_MAJOR}.${MD_VERSION_MINOR}.${MD_VERSION_PATCH}")

set(CPACK_PACKAGE_VERSION_MAJOR ${MD_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MD_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${MD_VERSION_PATCH})
set(CPACK_PACKAGE_DIRECTORY packages)
include(CPack)

set(C_STANDARD_REQUIRED ON)

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
