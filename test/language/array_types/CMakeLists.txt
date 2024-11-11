add_library(array_types_zs STATIC ${TEST_ZS_ROOT}/array_types.zs)
zserio_generate_cpp(
    TARGET array_types_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(array_types_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

list(FILTER GENERATED_SOURCES INCLUDE REGEX
    ".*/auto_array_bitfield_param/.*|.*/auto_array_struct_recursion/.*|.*/auto_array_uint8/.*|.*/fixed_array_uint8/.*|.*/packed_auto_array_bitfield_param/.*|.*/packed_variable_array_struct_recursion/.*|.*/variable_array_subtyped_struct/.*|.*/variable_array_ternary_operator/.*")
set_target_properties(array_types_zs PROPERTIES SOURCES "${GENERATED_SOURCES}")

add_custom_test(array_types
    DEPENDS
        array_types_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoArrayBitfieldParamTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoArrayStructRecursionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoArrayUInt8Test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/FixedArrayUInt8Test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PackedAutoArrayBitfieldParamTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PackedVariableArrayStructRecursionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/VariableArraySubtypedStructTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/VariableArrayTernaryOperatorTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
