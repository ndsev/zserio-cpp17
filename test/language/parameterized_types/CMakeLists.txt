add_library(parameterized_types_zs STATIC ${TEST_ZS_ROOT}/parameterized_types.zs)
zserio_generate_cpp(
    TARGET parameterized_types_zs
    SRC_DIR ${TEST_ZS_ROOT}
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

list(FILTER GENERATED_SOURCES INCLUDE REGEX
    ".*/array_element_param_with_optional/.*|.*/dynamic_bitfield_param/.*|.*/multiple_params/.*|\
.*/parameterized_nested_in_array/.*|.*/parameterized_param/.*|.*/param_with_optional/.*|\
.*/simple_param/.*")
set_target_properties(parameterized_types_zs PROPERTIES SOURCES "${GENERATED_SOURCES}")

add_custom_test(parameterized_types
    DEPENDS
        parameterized_types_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ArrayElementParamWithOptionalTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/DynamicBitfieldParamTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/MultipleParamsTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParameterizedParamTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParameterizedNestedInArrayTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParamWithOptionalTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/SimpleParamTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
