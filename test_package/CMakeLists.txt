cmake_minimum_required(VERSION 4.0)

project(MoondanceTestPackage)

find_package(moondance REQUIRED)

add_executable(test_exe main.c)
target_compile_features(test_exe PRIVATE c_std_23)
target_link_libraries(test_exe PRIVATE moondance::moondance)
