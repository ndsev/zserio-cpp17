add_library(with_type_info_code_zs STATIC ${TEST_ZS_ROOT}/with_type_info_code.zs)
zserio_generate_cpp(
    TARGET with_type_info_code_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen_type_info_code
    EXTRA_ARGS -allowImplicitArrays -withTypeInfoCode ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES_TYPE_INFO_CODE
    OUTPUT_VAR ZSERIO_LOG_TYPE_INFO_CODE
    ERROR_VAR ZSERIO_LOG_TYPE_INFO_CODE
)
target_link_libraries(with_type_info_code_zs PUBLIC ZserioCpp17Runtime)
target_include_directories(with_type_info_code_zs SYSTEM PRIVATE ${SQLITE_INCDIR})
if (ZSERIO_LOG_TYPE_INFO_CODE)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_type_info_code.txt "${ZSERIO_LOG_TYPE_INFO_CODE}")
    check_zserio_warnings("${ZSERIO_LOG_TYPE_INFO_CODE}" 0)
endif ()

add_library(type_info_string_expressions_zs STATIC ${TEST_ZS_ROOT}/type_info_string_expressions.zs)
zserio_generate_cpp(
    TARGET type_info_string_expressions_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen_string_expressions
    EXTRA_ARGS -withTypeInfoCode ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES_STRING_EXPRESSIONS
    OUTPUT_VAR ZSERIO_LOG_STRING_EXPRESSIONS
    ERROR_VAR ZSERIO_LOG_STRING_EXPRESSIONS
)
target_link_libraries(type_info_string_expressions_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG_STRING_EXPRESSIONS)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log_string_expressions.txt "${ZSERIO_LOG_STRING_EXPRESSIONS}")
    check_zserio_warnings("${ZSERIO_LOG_STRING_EXPRESSIONS}" 0)
endif ()

add_custom_test(with_type_info_code
    DEPENDS
        with_type_info_code_zs
        type_info_string_expressions_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/DebugStringTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/TypeInfoStringExpressionsTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/WithTypeInfoCodeCreator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/WithTypeInfoCodeCreator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/WithTypeInfoCodeTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES_TYPE_INFO_CODE}
        ${GENERATED_SOURCES_STRING_EXPRESSIONS}
)
