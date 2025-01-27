# Zserio C++17 runtime library.
#
# This CMake file defines a static library target named ZserioCpp17Runtime that contains the zserio C++ runtime.
#
# This CMake file is designed to be included directly without any further dependencies.
#
# An example application CMakeLists.txt follows:
#
# project(ZserioTestApp)
#
# add_subdirectory(runtime_lib ZserioCpp17Runtime)
#
# add_executable(ZserioTestApp HelloWorld.cpp)
# target_link_libraries(ZserioTestApp ZserioCpp17Runtime)

cmake_minimum_required(VERSION 3.15.0)

project(ZserioCpp17Runtime)

set(ZSERIO_CPP17_RUNTIME_LIB_SRCS
    zserio/AllocatorHolder.h
    zserio/Array.h
    zserio/ArrayLengthException.h
    zserio/ArrayTraits.h
    zserio/BitBuffer.h
    zserio/BitFieldUtil.cpp
    zserio/BitFieldUtil.h
    zserio/Bitmasks.h
    zserio/BitPositionUtil.h
    zserio/BitFieldUtil.h
    zserio/BitSize.h
    zserio/BitStreamReader.cpp
    zserio/BitStreamReader.h
    zserio/BitStreamWriter.cpp
    zserio/BitStreamWriter.h
    zserio/BuiltInOperators.cpp
    zserio/BuiltInOperators.h
    zserio/Bytes.cpp
    zserio/Bytes.h
    zserio/ChoiceCaseException.h
    zserio/ChoiceTag.h
    zserio/ConstraintException.h
    zserio/CppRuntimeException.cpp
    zserio/CppRuntimeException.h
    zserio/CppRuntimeVersion.h
    zserio/DataView.h
    zserio/DeltaContext.h
    zserio/Enums.h
    zserio/Extended.h
    zserio/ExtendedFieldException.h
    zserio/FileUtil.cpp
    zserio/FileUtil.h
    zserio/FloatUtil.cpp
    zserio/FloatUtil.h
    zserio/HashCodeUtil.h
    zserio/IPubsub.h
    zserio/IService.h
    zserio/MissedOptionalException.h
    zserio/Optional.h
    zserio/OutOfRangeException.h
    zserio/PubsubException.h
    zserio/RebindAlloc.h
    zserio/RuntimeArch.h
    zserio/SerializeUtil.h
    zserio/ServiceException.h
    zserio/SizeConvertUtil.cpp
    zserio/SizeConvertUtil.h
    zserio/Span.h
    zserio/String.cpp
    zserio/String.h
    zserio/StringConvertUtil.h
    zserio/Traits.h
    zserio/Types.cpp
    zserio/Types.h
    zserio/UnionCaseException.h
    zserio/ValidationException.h
    zserio/Variant.h
    zserio/Vector.h
    zserio/View.h
)

add_library(${PROJECT_NAME} STATIC ${ZSERIO_CPP17_RUNTIME_LIB_SRCS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
