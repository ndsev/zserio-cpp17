# Zserio C++17 runtime library test.
#
# This CMake file defines an executable which contains the zserio C++ runtime tests.
#
# This CMake file is NOT designed to be included directly without any further dependencies.
#

project(ZserioCpp17RuntimeTest)

# add gtest library
include(gtest_utils)
gtest_add_library("${ZSERIO_CPP17_PROJECT_ROOT}/3rdparty/cpp/googletest")

compiler_set_warnings()
compiler_set_warnings_as_errors()
if (SANITIZERS_ENABLED)
    compiler_set_undefined_sanitizer()
endif ()

set(ZSERIO_CPP17_RUNTIME_TEST_SRCS
    zserio/BitBufferTest.cpp
    zserio/BitFieldUtilTest.cpp
    zserio/BitPositionUtilTest.cpp
    zserio/BitSizeOfCalculatorTest.cpp
    zserio/BitStreamReaderTest.cpp
    zserio/BitStreamTest.cpp
    zserio/BitStreamWriterTest.cpp
    zserio/BuiltInOperatorsTest.cpp
    zserio/CppRuntimeExceptionTest.cpp
    zserio/CppRuntimeVersionTest.cpp
    zserio/ConstraintExceptionTest.cpp
    zserio/FileUtilTest.cpp
    zserio/FloatUtilTest.cpp
    zserio/HashCodeUtilTest.cpp
    zserio/OutOfRangeExceptionTest.cpp
    zserio/SizeConvertUtilTest.cpp
    zserio/SpanTest.cpp
    zserio/StringConvertUtilTest.cpp
    zserio/TrackingAllocator.h
    zserio/TypesTest.cpp
)

add_executable(${PROJECT_NAME} ${ZSERIO_CPP17_RUNTIME_TEST_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE .)
target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_NAME} PRIVATE ZserioCpp17Runtime gtest_main)

gtest_add_tests(${PROJECT_NAME} "--gtest_output=xml" ${ZSERIO_CPP17_RUNTIME_TEST_SRCS})
