add_library(alignment_zs STATIC ${TEST_ZS_ROOT}/alignment.zs)
zserio_generate_cpp(
    TARGET alignment_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(alignment_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

add_custom_test(alignment
    DEPENDS
        alignment_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/AutoOptionalMemberAlignmentTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/BitAlignmentTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalMemberAlignmentTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
