add_library(parameterized_types_zs STATIC zs/parameterized_types.zs)
zserio_generate_cpp(
    TARGET parameterized_types_zs
    SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zs
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(parameterized_types_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

add_custom_test(parameterized_types
    DEPENDS
        parameterized_types_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/SimpleParamTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParameterizedParamTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParamWithOptionalTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
