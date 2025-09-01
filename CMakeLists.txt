cmake_minimum_required(VERSION 3.31)

project(moondance)
enable_testing()
include(scripts/test.cmake)

add_library(moondance STATIC src/test.c)

target_compile_options(
    moondance
    PUBLIC
        "-Wall"
        "-Werror"
        "-Wextra"
        "-pedantic"
)

target_include_directories(
    moondance
    PUBLIC
        "include"
        "${CMAKE_CURRENT_BINARY_DIR}/include"
)

target_compile_features(moondance PUBLIC c_std_23)

configure_file(
    include/moondance/version.h.in
    include/moondance/version.h
)

md_add_test(test_test test/test.c)
