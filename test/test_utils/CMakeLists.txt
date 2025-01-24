# Zserio C++ integration test utility library.
#
# This CMake file defines utility library which is used for all C++ integration tests.
#

project(ZserioCppTestUtils)

set(ZSERIO_CPP_TEST_UTILS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Assertions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ComparisonOperatorsTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/HashTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/LocalServiceClient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ReadTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TestPubsub.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TestUtility.h
    ${CMAKE_CURRENT_SOURCE_DIR}/WriteReadFileTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/WriteReadTest.h)

clang_format_add_custom_target(${PROJECT_NAME}-clang-format
    SOURCES "${ZSERIO_CPP_TEST_UTILS_SOURCES}"
    CONFIG_FILE "${ZSERIO_CPP17_PROJECT_ROOT}/.clang-format"
    WERROR ON)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
