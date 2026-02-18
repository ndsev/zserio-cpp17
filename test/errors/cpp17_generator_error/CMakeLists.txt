set(ZSERIO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)

add_library(template_parameter_choice_array_clash_error_zs INTERFACE
    ${TEST_ZS_ROOT}/template_parameter_clashing/choice_array_clash_error.zs
)
zserio_generate_cpp(
    TARGET template_parameter_choice_array_clash_error_zs
    SRC_DIR ${TEST_ZS_ROOT}/template_parameter_clashing
    GEN_DIR ${ZSERIO_GEN_DIR}/template_parameter_clashing
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    OUTPUT_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    ERROR_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    RESULT_VAR ZSERIO_RESULT
)
if (ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_template_parameter_choice_array_clash.txt
        "${ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH}")
    if (${ZSERIO_RESULT} EQUAL 0)
        message(FATAL_ERROR "Zserio tool is expected to fail!")
    endif ()
endif ()

add_library(template_parameter_struct_array_clash_error_zs INTERFACE
    ${TEST_ZS_ROOT}/template_parameter_clashing/struct_array_clash_error.zs
)
zserio_generate_cpp(
    TARGET template_parameter_struct_array_clash_error_zs
    SRC_DIR ${TEST_ZS_ROOT}/template_parameter_clashing
    GEN_DIR ${ZSERIO_GEN_DIR}/template_parameter_clashing
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    OUTPUT_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    ERROR_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    RESULT_VAR ZSERIO_RESULT
)
if (ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_template_parameter_struct_array_clash.txt
        "${ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH}")
    if (${ZSERIO_RESULT} EQUAL 0)
        message(FATAL_ERROR "Zserio tool is expected to fail!")
    endif ()
endif ()

add_library(template_parameter_struct_clash_error_zs INTERFACE
    ${TEST_ZS_ROOT}/template_parameter_clashing/struct_clash_error.zs
)
zserio_generate_cpp(
    TARGET template_parameter_struct_clash_error_zs
    SRC_DIR ${TEST_ZS_ROOT}/template_parameter_clashing
    GEN_DIR ${ZSERIO_GEN_DIR}/template_parameter_clashing
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    OUTPUT_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    ERROR_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    RESULT_VAR ZSERIO_RESULT
)
if (ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_template_parameter_struct_clash.txt
        "${ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH}")
    if (${ZSERIO_RESULT} EQUAL 0)
        message(FATAL_ERROR "Zserio tool is expected to fail!")
    endif ()
endif ()

add_library(template_parameter_struct_other_clash_error_zs INTERFACE
    ${TEST_ZS_ROOT}/template_parameter_clashing/struct_other_clash_error.zs
)
zserio_generate_cpp(
    TARGET template_parameter_struct_other_clash_error_zs
    SRC_DIR ${TEST_ZS_ROOT}/template_parameter_clashing
    GEN_DIR ${ZSERIO_GEN_DIR}/template_parameter_clashing
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    OUTPUT_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    ERROR_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    RESULT_VAR ZSERIO_RESULT
)
if (ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_template_parameter_struct_other_clash.txt
        "${ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH}")
    if (${ZSERIO_RESULT} EQUAL 0)
        message(FATAL_ERROR "Zserio tool is expected to fail!")
    endif ()
endif ()

add_library(template_parameter_union_array_clash_error_zs INTERFACE
    ${TEST_ZS_ROOT}/template_parameter_clashing/union_array_clash_error.zs
)
zserio_generate_cpp(
    TARGET template_parameter_union_array_clash_error_zs
    SRC_DIR ${TEST_ZS_ROOT}/template_parameter_clashing
    GEN_DIR ${ZSERIO_GEN_DIR}/template_parameter_clashing
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    OUTPUT_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    ERROR_VAR ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH
    RESULT_VAR ZSERIO_RESULT
)
if (ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_template_parameter_union_array_clash.txt
        "${ZSERIO_LOG_TEMPLATE_PARAMETER_CLASH}")
    if (${ZSERIO_RESULT} EQUAL 0)
        message(FATAL_ERROR "Zserio tool is expected to fail!")
    endif ()
endif ()

add_custom_test(cpp17_generator_error
    DEPENDS
        template_parameter_choice_array_clash_error_zs
        template_parameter_struct_array_clash_error_zs
        template_parameter_struct_clash_error_zs
        template_parameter_struct_other_clash_error_zs
        template_parameter_union_array_clash_error_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/Cpp17GeneratorErrorTest.cpp
)
