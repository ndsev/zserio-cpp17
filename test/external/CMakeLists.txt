add_library(external_zs STATIC ${ZSERIO_TEST_EXTERN_SCHEMA_ROOT}/${ZSERIO_TEST_EXTERN_SCHEMA})
zserio_generate_cpp(
    TARGET external_zs
    SRC_DIR ${ZSERIO_TEST_EXTERN_SCHEMA_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(external_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
endif ()

add_custom_test(external
    DEPENDS
        external_zs
    SOURCES
        ${ZSERIO_TEST_EXTERN_SCHEMA_TEST}
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
