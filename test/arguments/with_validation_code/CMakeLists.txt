add_library(with_validation_code_zs STATIC ${TEST_ZS_ROOT}/with_validation_code.zs)
zserio_generate_cpp(
    TARGET with_validation_code_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(with_validation_code_zs PUBLIC ZserioCpp17Runtime)
target_include_directories(with_validation_code_zs SYSTEM PRIVATE ${SQLITE_INCDIR})
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

# Note that validation is always enabled in C++17 generator!
add_custom_test(with_validation_code
    DEPENDS
        with_validation_code_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/BlobTableValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ColumnTypeValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ConstraintTableValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ControlValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/DependTableValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/FullRangeTableValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/HiddenColumnTableValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/SimpleTableValidationTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/VirtualTableValidationTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
