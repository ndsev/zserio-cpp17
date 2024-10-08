add_library(enumeration_types_zs STATIC  ${TEST_ZS_ROOT}/enumeration_types.zs)
zserio_generate_cpp(
    TARGET enumeration_types_zs
    SRC_DIR  ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(enumeration_types_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

list(FILTER GENERATED_SOURCES INCLUDE REGEX
    ".*/uint8_enum/.*")
set_target_properties(enumeration_types_zs PROPERTIES SOURCES "${GENERATED_SOURCES}")

add_custom_test(enumeration_types
    DEPENDS
        enumeration_types_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/UInt8EnumTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
