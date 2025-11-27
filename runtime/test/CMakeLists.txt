# Zserio C++17 runtime library test.
#
# This CMake file defines an executable which contains the zserio C++ runtime tests.
#
# This CMake file is NOT designed to be included directly without any further dependencies.
#

cmake_minimum_required(VERSION 3.21.0)

project(ZserioCpp17RuntimeTest)

# add gtest library
include(gtest_utils)
gtest_add_library("${ZSERIO_CPP17_PROJECT_ROOT}/3rdparty/cpp/googletest")

compiler_set_warnings()
compiler_set_warnings_as_errors()
if (SANITIZERS_ENABLED)
    compiler_set_undefined_sanitizer()
endif ()

set(TEST_OBJECT_SRCS
    test_object/pmr_allocator/CreatorBitmask.cpp
    test_object/pmr_allocator/CreatorBitmask.h
    test_object/pmr_allocator/CreatorEnum.cpp
    test_object/pmr_allocator/CreatorEnum.h
    test_object/pmr_allocator/CreatorNested.cpp
    test_object/pmr_allocator/CreatorNested.h
    test_object/pmr_allocator/CreatorObject.cpp
    test_object/pmr_allocator/CreatorObject.h
    test_object/pmr_allocator/CreatorUnsignedEnum.cpp
    test_object/pmr_allocator/CreatorUnsignedEnum.h
    test_object/pmr_allocator/DebugStringObject.cpp
    test_object/pmr_allocator/DebugStringObject.h
    test_object/pmr_allocator/DebugStringParamObject.cpp
    test_object/pmr_allocator/DebugStringParamObject.h
    test_object/pmr_allocator/ReflectableBitmask.cpp
    test_object/pmr_allocator/ReflectableBitmask.h
    test_object/pmr_allocator/ReflectableChoice.cpp
    test_object/pmr_allocator/ReflectableChoice.h
    test_object/pmr_allocator/ReflectableEnum.cpp
    test_object/pmr_allocator/ReflectableEnum.h
    test_object/pmr_allocator/ReflectableNested.cpp
    test_object/pmr_allocator/ReflectableNested.h
    test_object/pmr_allocator/ReflectableObject.cpp
    test_object/pmr_allocator/ReflectableObject.h
    test_object/pmr_allocator/ReflectableUnion.cpp
    test_object/pmr_allocator/ReflectableUnion.h
    test_object/pmr_allocator/ReflectableUtilBitmask.cpp
    test_object/pmr_allocator/ReflectableUtilBitmask.h
    test_object/pmr_allocator/ReflectableUtilEnum.cpp
    test_object/pmr_allocator/ReflectableUtilEnum.h
    test_object/pmr_allocator/ReflectableUtilChoice.cpp
    test_object/pmr_allocator/ReflectableUtilChoice.h
    test_object/pmr_allocator/ReflectableUtilObject.cpp
    test_object/pmr_allocator/ReflectableUtilObject.h
    test_object/pmr_allocator/ReflectableUtilUnion.cpp
    test_object/pmr_allocator/ReflectableUtilUnion.h
    test_object/pmr_allocator/WalkerBitmask.cpp
    test_object/pmr_allocator/WalkerBitmask.h
    test_object/pmr_allocator/WalkerChoice.cpp
    test_object/pmr_allocator/WalkerChoice.h
    test_object/pmr_allocator/WalkerNested.cpp
    test_object/pmr_allocator/WalkerNested.h
    test_object/pmr_allocator/WalkerObject.cpp
    test_object/pmr_allocator/WalkerObject.h
    test_object/pmr_allocator/WalkerUnion.cpp
    test_object/pmr_allocator/WalkerUnion.h

    test_object/std_allocator/CreatorBitmask.cpp
    test_object/std_allocator/CreatorBitmask.h
    test_object/std_allocator/CreatorEnum.cpp
    test_object/std_allocator/CreatorEnum.h
    test_object/std_allocator/CreatorNested.cpp
    test_object/std_allocator/CreatorNested.h
    test_object/std_allocator/CreatorObject.cpp
    test_object/std_allocator/CreatorObject.h
    test_object/std_allocator/CreatorUnsignedEnum.cpp
    test_object/std_allocator/CreatorUnsignedEnum.h
    test_object/std_allocator/DebugStringObject.cpp
    test_object/std_allocator/DebugStringObject.h
    test_object/std_allocator/DebugStringParamObject.cpp
    test_object/std_allocator/DebugStringParamObject.h
    test_object/std_allocator/ReflectableBitmask.cpp
    test_object/std_allocator/ReflectableBitmask.h
    test_object/std_allocator/ReflectableChoice.cpp
    test_object/std_allocator/ReflectableChoice.h
    test_object/std_allocator/ReflectableEnum.cpp
    test_object/std_allocator/ReflectableEnum.h
    test_object/std_allocator/ReflectableNested.cpp
    test_object/std_allocator/ReflectableNested.h
    test_object/std_allocator/ReflectableObject.cpp
    test_object/std_allocator/ReflectableObject.h
    test_object/std_allocator/ReflectableUnion.cpp
    test_object/std_allocator/ReflectableUnion.h
    test_object/std_allocator/ReflectableUtilBitmask.cpp
    test_object/std_allocator/ReflectableUtilBitmask.h
    test_object/std_allocator/ReflectableUtilEnum.cpp
    test_object/std_allocator/ReflectableUtilEnum.h
    test_object/std_allocator/ReflectableUtilChoice.cpp
    test_object/std_allocator/ReflectableUtilChoice.h
    test_object/std_allocator/ReflectableUtilObject.cpp
    test_object/std_allocator/ReflectableUtilObject.h
    test_object/std_allocator/ReflectableUtilUnion.cpp
    test_object/std_allocator/ReflectableUtilUnion.h
    test_object/std_allocator/WalkerBitmask.cpp
    test_object/std_allocator/WalkerBitmask.h
    test_object/std_allocator/WalkerChoice.cpp
    test_object/std_allocator/WalkerChoice.h
    test_object/std_allocator/WalkerNested.cpp
    test_object/std_allocator/WalkerNested.h
    test_object/std_allocator/WalkerObject.cpp
    test_object/std_allocator/WalkerObject.h
    test_object/std_allocator/WalkerUnion.cpp
    test_object/std_allocator/WalkerUnion.h

    test_object/ppmr_allocator/CreatorBitmask.cpp
    test_object/ppmr_allocator/CreatorBitmask.h
    test_object/ppmr_allocator/CreatorEnum.cpp
    test_object/ppmr_allocator/CreatorEnum.h
    test_object/ppmr_allocator/CreatorNested.cpp
    test_object/ppmr_allocator/CreatorNested.h
    test_object/ppmr_allocator/CreatorObject.cpp
    test_object/ppmr_allocator/CreatorObject.h
    test_object/ppmr_allocator/CreatorUnsignedEnum.cpp
    test_object/ppmr_allocator/CreatorUnsignedEnum.h
    test_object/ppmr_allocator/DebugStringObject.cpp
    test_object/ppmr_allocator/DebugStringObject.h
    test_object/ppmr_allocator/DebugStringParamObject.cpp
    test_object/ppmr_allocator/DebugStringParamObject.h
    test_object/ppmr_allocator/ReflectableBitmask.cpp
    test_object/ppmr_allocator/ReflectableBitmask.h
    test_object/ppmr_allocator/ReflectableChoice.cpp
    test_object/ppmr_allocator/ReflectableChoice.h
    test_object/ppmr_allocator/ReflectableEnum.cpp
    test_object/ppmr_allocator/ReflectableEnum.h
    test_object/ppmr_allocator/ReflectableNested.cpp
    test_object/ppmr_allocator/ReflectableNested.h
    test_object/ppmr_allocator/ReflectableObject.cpp
    test_object/ppmr_allocator/ReflectableObject.h
    test_object/ppmr_allocator/ReflectableUnion.cpp
    test_object/ppmr_allocator/ReflectableUnion.h
    test_object/ppmr_allocator/ReflectableUtilBitmask.cpp
    test_object/ppmr_allocator/ReflectableUtilBitmask.h
    test_object/ppmr_allocator/ReflectableUtilEnum.cpp
    test_object/ppmr_allocator/ReflectableUtilEnum.h
    test_object/ppmr_allocator/ReflectableUtilChoice.cpp
    test_object/ppmr_allocator/ReflectableUtilChoice.h
    test_object/ppmr_allocator/ReflectableUtilObject.cpp
    test_object/ppmr_allocator/ReflectableUtilObject.h
    test_object/ppmr_allocator/ReflectableUtilUnion.cpp
    test_object/ppmr_allocator/ReflectableUtilUnion.h
    test_object/ppmr_allocator/WalkerBitmask.cpp
    test_object/ppmr_allocator/WalkerBitmask.h
    test_object/ppmr_allocator/WalkerChoice.cpp
    test_object/ppmr_allocator/WalkerChoice.h
    test_object/ppmr_allocator/WalkerNested.cpp
    test_object/ppmr_allocator/WalkerNested.h
    test_object/ppmr_allocator/WalkerObject.cpp
    test_object/ppmr_allocator/WalkerObject.h
    test_object/ppmr_allocator/WalkerUnion.cpp
    test_object/ppmr_allocator/WalkerUnion.h
)

set(ZSERIO_CPP17_RUNTIME_TEST_SRCS
    zserio/ppmr/PropagatingPolymorphicAllocatorTest.cpp
    zserio/AllocatorHolderTest.cpp
    zserio/AnyTest.cpp
    zserio/ArrayLengthExceptionTest.cpp
    zserio/ArrayViewTest.cpp
    zserio/BitBufferTest.cpp
    zserio/BitFieldUtilTest.cpp
    zserio/BitPositionUtilTest.cpp
    zserio/BitmasksTest.cpp
    zserio/BitStreamReaderTest.cpp
    zserio/BitStreamTest.cpp
    zserio/BitStreamWriterTest.cpp
    zserio/BuiltInOperatorsTest.cpp
    zserio/BytesTest.cpp
    zserio/ChoiceCaseExceptionTest.cpp
    zserio/CppRuntimeExceptionTest.cpp
    zserio/CppRuntimeVersionTest.cpp
    zserio/ConstraintExceptionTest.cpp
    zserio/DataViewTest.cpp
    zserio/DebugStringUtilTest.cpp
    zserio/EnumsTest.cpp
    zserio/ExtendedFieldExceptionTest.cpp
    zserio/ExtendedTest.cpp
    zserio/FileUtilTest.cpp
    zserio/FloatUtilTest.cpp
    zserio/GenericAccessorTest.cpp
    zserio/HashCodeUtilTest.cpp
    zserio/IntrospectableViewTest.cpp
    zserio/JsonDecoderTest.cpp
    zserio/JsonEncoderTest.cpp
    zserio/JsonParserTest.cpp
    zserio/JsonReaderTest.cpp
    zserio/JsonTokenizerTest.cpp
    zserio/JsonWriterTest.cpp
    zserio/MissedOptionalExceptionTest.cpp
    zserio/OptionalTest.cpp
    zserio/OutOfRangeExceptionTest.cpp
    zserio/PubsubExceptionTest.cpp
    zserio/ReflectableDataTest.cpp
    zserio/ReflectableUtilTest.cpp
    zserio/SerializeUtilTest.cpp
    zserio/ServiceExceptionTest.cpp
    zserio/SizeConvertUtilTest.cpp
    zserio/SpanTest.cpp
    zserio/StringConvertUtilTest.cpp
    zserio/StringTest.cpp
    zserio/TrackingAllocator.h
    zserio/TraitsTest.cpp
    zserio/TypeInfoTest.cpp
    zserio/TypeInfoUtilTest.cpp
    zserio/TypesTest.cpp
    zserio/UnionCaseExceptionTest.cpp
    zserio/UniquePtrTest.cpp
    zserio/ValidationExceptionTest.cpp
    zserio/VariantTest.cpp
    zserio/WalkerTest.cpp
    zserio/ZserioTreeCreatorTest.cpp
)

if (ZSERIO_CPP17_RUNTIME_SQLITE_ENABLE)
    set(ZSERIO_CPP17_RUNTIME_TEST_SQLITE_SRCS
        zserio/SqliteConnectionTest.cpp
        zserio/ValidationSqliteUtilTest.cpp
    )
endif()

add_executable(${PROJECT_NAME} ${TEST_OBJECT_SRCS} ${ZSERIO_CPP17_RUNTIME_TEST_SRCS} ${ZSERIO_CPP17_RUNTIME_TEST_SQLITE_SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ZSERIO_CPP17_RUNTIME_TEST_SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/test_object FILES ${TEST_OBJECT_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE .)
target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_NAME} PRIVATE ZserioCpp17Runtime gtest_main)

gtest_add_tests(${PROJECT_NAME} "--gtest_output=xml" ${ZSERIO_CPP17_RUNTIME_TEST_SRCS} ${ZSERIO_CPP17_RUNTIME_TEST_SQLITE_SRCS})
