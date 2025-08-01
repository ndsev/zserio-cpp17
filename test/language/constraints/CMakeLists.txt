add_library(constraints_zs STATIC ${TEST_ZS_ROOT}/constraints.zs)
zserio_generate_cpp(
    TARGET constraints_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(constraints_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

add_custom_test(constraints
    DEPENDS
        constraints_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ArrayLengthofConstraintTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ChoiceConstraintsTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ConstraintsInTemplatesTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/LiteralConstraintTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/OptionalConstraintsTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/StructureBitmaskConstraintsTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/StructureConstraintsTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/UnionConstraintsTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
