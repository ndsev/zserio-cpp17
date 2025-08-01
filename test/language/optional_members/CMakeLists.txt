add_library(optional_members_zs STATIC ${TEST_ZS_ROOT}/optional_members.zs)
zserio_generate_cpp(
    TARGET optional_members_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(optional_members_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

add_custom_test(optional_members
    DEPENDS
        optional_members_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoOptionalBoolTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoOptionalTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalArrayRecursionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalArrayTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalComplexExpressionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalExpressionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalExpressionWithRemovedEnumItemTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalRecursionTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalRecursionInTemplateTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
