include_guard()

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

    set(EXE_NAME ${TEST_NAME})

    add_executable(${EXE_NAME} ${TEST_SOURCE_FILE})

    target_link_libraries(${EXE_NAME} PUBLIC moondance)

    set_target_properties(
        ${EXE_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY test
    )

    if(DEFINED TEST_CONFIGURATIONS)
        set(ARG_CONFIGURATIONS "CONFIGURATIONS ${TEST_CONFIGURATIONS}")
    endif()

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${EXE_NAME}
        ${ARG_CONFIGURATIONS}
    )
endfunction()
