add_library(offsets_zs STATIC ${TEST_ZS_ROOT}/offsets.zs)
zserio_generate_cpp(
    TARGET offsets_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(offsets_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

add_custom_test(offsets
    DEPENDS
        offsets_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoArrayOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/BitOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/NestedOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OffsetInNestedChoiceTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OffsetInChoiceTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OffsetInNestedChoiceInTemplateTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OffsetInNestedUnionInTemplateTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OffsetInNestedUnionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OffsetInUnionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalMemberOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalMemberOptionalOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalOffsetInTemplateTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PackedAutoArrayOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParameterOffsetInTemplateTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ParameterOffsetTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/UInt64OffsetTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
