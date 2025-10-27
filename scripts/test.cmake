include_guard()

include(scripts/target.cmake)

function(md_add_test TEST_NAME TEST_SOURCE_FILE)
    set(OPTIONS "")
    set(KEYWORDS "")
    set(MULTI_VALUE_KEYWORDS "CONFIGURATIONS")

    cmake_parse_arguments(
        PARSE_ARGV 2
        TEST
        "${OPTIONS}"
        "${KEYWORDS}"
        "${MULTI_VALUE_KEYWORDS}"
    )

    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
    md_configure_target(${TEST_NAME})

    target_link_libraries(${TEST_NAME} PUBLIC moondance)

    set_target_properties(
        ${TEST_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY test
    )

    if(DEFINED TEST_CONFIGURATIONS)
        set(ARG_CONFIGURATIONS "CONFIGURATIONS ${TEST_CONFIGURATIONS}")
    endif()

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
        ${ARG_CONFIGURATIONS}
    )
endfunction()
