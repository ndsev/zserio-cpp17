add_custom_test(cpp17_generator_error
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/Cpp17GeneratorErrorTest.cpp
)

set(ZSERIO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)

set(TYPE_ALIAS_CLASH_TESTS
    choice_tmpl_param_field_clash_error
    struct_field_clash_error
    struct_name_field_clash_error
    struct_name_tmpl_param_clash_error
    struct_tmpl_param_clash_error
    struct_tmpl_param_field_clash_error
    table_tmpl_param_clash_error
    union_tmpl_param_field_clash_error
)
foreach (TEST IN LISTS TYPE_ALIAS_CLASH_TESTS)
    add_library(${TEST}_zs INTERFACE
        ${TEST_ZS_ROOT}/type_alias_clashing/${TEST}.zs
    )
    zserio_generate_cpp(
        TARGET ${TEST}_zs
        SRC_DIR ${TEST_ZS_ROOT}/type_alias_clashing
        GEN_DIR ${ZSERIO_GEN_DIR}/type_alias_clashing
        EXTRA_ARGS -withoutCrossExtensionCheck ${ZSERIO_EXTRA_ARGS} # only interested in cpp17 errors here
        OUTPUT_VAR ZSERIO_LOG_TYPE_ALIAS_CLASH
        ERROR_VAR ZSERIO_LOG_TYPE_ALIAS_CLASH
        RESULT_VAR ZSERIO_RESULT
    )
    if (ZSERIO_LOG_TYPE_ALIAS_CLASH)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_type_alias_${TEST}.txt "${ZSERIO_LOG_TYPE_ALIAS_CLASH}")
        if (${ZSERIO_RESULT} EQUAL 0)
            message(FATAL_ERROR "Zserio tool is expected to fail!")
        endif ()
    endif ()
    add_dependencies(cpp17_generator_error ${TEST}_zs)
endforeach()

set(VIEW_ALIAS_CLASH_TESTS
    struct_name_view_clash_error
    struct_tmpl_param_view_clash_error
    table_tmpl_param_view_clash_error
)
foreach (TEST IN LISTS VIEW_ALIAS_CLASH_TESTS)
    add_library(${TEST}_zs INTERFACE
        ${TEST_ZS_ROOT}/view_alias_clashing/${TEST}.zs
    )
    zserio_generate_cpp(
        TARGET ${TEST}_zs
        SRC_DIR ${TEST_ZS_ROOT}/view_alias_clashing
        GEN_DIR ${ZSERIO_GEN_DIR}/view_alias_clashing
        EXTRA_ARGS -withoutCrossExtensionCheck ${ZSERIO_EXTRA_ARGS} # only interested in cpp17 errors here
        OUTPUT_VAR ZSERIO_LOG_VIEW_ALIAS_CLASH
        ERROR_VAR ZSERIO_LOG_VIEW_ALIAS_CLASH
        RESULT_VAR ZSERIO_RESULT
    )
    if (ZSERIO_LOG_VIEW_ALIAS_CLASH)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_view_alias_${TEST}.txt "${ZSERIO_LOG_VIEW_ALIAS_CLASH}")
        if (${ZSERIO_RESULT} EQUAL 0)
            message(FATAL_ERROR "Zserio tool is expected to fail!")
        endif ()
    endif ()
    add_dependencies(cpp17_generator_error ${TEST}_zs)
endforeach()
