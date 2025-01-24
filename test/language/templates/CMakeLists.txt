add_library(templates_zs STATIC ${TEST_ZS_ROOT}/templates.zs)
zserio_generate_cpp(
    TARGET templates_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(templates_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

add_custom_test(templates
    DEPENDS
        templates_zs
    SOURCES
        # TODO[mikir]: Template integration tests are not implemented!
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
