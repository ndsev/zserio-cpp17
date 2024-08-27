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
# add_subdirectory(runtime_libs/cpp ZserioCpp17Runtime)
#
# add_executable(ZserioTestApp HelloWorld.cpp)
# target_link_libraries(ZserioTestApp ZserioCpp17Runtime)

cmake_minimum_required(VERSION 3.15.0)

project(ZserioCpp17Runtime)

set(ZSERIO_CPP17_RUNTIME_LIB_SRCS
    zserio/BuiltInOperators.cpp
    zserio/BuiltInOperators.h
    zserio/CppRuntimeVersion.h
    zserio/Span.h
    zserio/Types.h
)

add_library(${PROJECT_NAME} STATIC ${ZSERIO_CPP17_RUNTIME_LIB_SRCS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
