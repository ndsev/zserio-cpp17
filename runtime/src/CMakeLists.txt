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

option(ZSERIO_CPP17_RUNTIME_SQLITE_ENABLE "Build zserio runtime with SQLite dependent parts" ON)
option(ZSERIO_CPP17_RUNTIME_SQLITE_USE_EXTERNAL "Use an external Sqlite3 cmake target" OFF)
set(ZSERIO_CPP17_RUNTIME_SQLITE_ROOT "../../3rdparty/cpp/sqlite3" CACHE PATH "Path to the custom SQLite distribution. You may have to create CMakeLists.txt/sqlite3.h if not present" )

option(ZSERIO_CPP17_RUNTIME_BUILD_SHARED_LIBS "Build zserio runtime as a shared library" OFF)

# add SQLite3 library
if (ZSERIO_CPP17_RUNTIME_SQLITE_ENABLE)
    if (NOT ZSERIO_CPP17_RUNTIME_SQLITE_USE_EXTERNAL)
        message(STATUS "Zserio runtime will be built with SQLite (packaged) support")
        add_subdirectory(${ZSERIO_CPP17_RUNTIME_SQLITE_ROOT} sqlite3) # binary directory is required
    else()
        message(STATUS "Zserio runtime will be built with SQLite (external) support")
    endif()
else()
    message(STATUS "Zserio runtime will be built without SQLite support")
endif()

set(ZSERIO_CPP17_RUNTIME_LIB_SRCS
    zserio/pmr/Any.h
    zserio/pmr/BitBuffer.h
    zserio/pmr/Bytes.h
    zserio/pmr/IReflectableData.h
    zserio/pmr/IService.h
    zserio/pmr/ITypeInfo.h
    zserio/pmr/Map.h
    zserio/pmr/Optional.h
    zserio/pmr/Set.h
    zserio/pmr/String.h
    zserio/pmr/UniquePtr.h
    zserio/pmr/Variant.h
    zserio/pmr/Vector.h
    zserio/ppmr/Any.h
    zserio/ppmr/BitBuffer.h
    zserio/ppmr/Bytes.h
    zserio/ppmr/IReflectableData.h
    zserio/ppmr/IService.h
    zserio/ppmr/ITypeInfo.h
    zserio/ppmr/Map.h
    zserio/ppmr/Optional.h
    zserio/ppmr/PropagatingPolymorphicAllocator.h
    zserio/ppmr/Set.h
    zserio/ppmr/String.h
    zserio/ppmr/UniquePtr.h
    zserio/ppmr/Variant.h
    zserio/ppmr/Vector.h
    zserio/AllocatorHolder.h
    zserio/Any.h
    zserio/ArrayView.h
    zserio/ArrayLengthException.cpp
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
    zserio/ChoiceCaseException.cpp
    zserio/ChoiceCaseException.h
    zserio/ChoiceTag.h
    zserio/ConstraintException.cpp
    zserio/ConstraintException.h
    zserio/CppRuntimeException.cpp
    zserio/CppRuntimeException.h
    zserio/CppRuntimeVersion.h
    zserio/DataView.h
    zserio/DebugStringUtil.h
    zserio/DeltaContext.h
    zserio/Enums.h
    zserio/Extended.h
    zserio/ExtendedFieldException.cpp
    zserio/ExtendedFieldException.h
    zserio/FileUtil.cpp
    zserio/FileUtil.h
    zserio/FloatUtil.cpp
    zserio/FloatUtil.h
    zserio/GenericAccessor.h
    zserio/HashCodeUtil.h
    zserio/IIntrospectableData.h
    zserio/IIntrospectableView.h
    zserio/ItemsAccessor.h
    zserio/IntrospectableDataBase.h
    zserio/IntrospectableView.h
    zserio/IPubsub.h
    zserio/IReflectableData.h
    zserio/IService.h
    zserio/ITypeInfo.h
    zserio/IValidationObserver.h
    zserio/IWalkFilter.h
    zserio/IWalkObserver.h
    zserio/JsonDecoder.h
    zserio/JsonEncoder.cpp
    zserio/JsonEncoder.h
    zserio/JsonParser.h
    zserio/JsonReader.h
    zserio/JsonTokenizer.cpp
    zserio/JsonTokenizer.h
    zserio/JsonWriter.h
    zserio/MissedOptionalException.cpp
    zserio/MissedOptionalException.h
    zserio/Optional.cpp
    zserio/Optional.h
    zserio/OutOfRangeException.cpp
    zserio/OutOfRangeException.h
    zserio/PubsubException.cpp
    zserio/PubsubException.h
    zserio/RebindAlloc.h
    zserio/ReflectableData.h
    zserio/ReflectableUtil.h
    zserio/RuntimeArch.h
    zserio/SerializeUtil.h
    zserio/ServiceException.cpp
    zserio/ServiceException.h
    zserio/SizeConvertUtil.cpp
    zserio/SizeConvertUtil.h
    zserio/Span.h
    zserio/String.cpp
    zserio/String.h
    zserio/StringConvertUtil.h
    zserio/Traits.h
    zserio/TypeInfo.h
    zserio/TypeInfoUtil.cpp
    zserio/TypeInfoUtil.h
    zserio/Types.cpp
    zserio/Types.h
    zserio/UnexpectedOptionalException.cpp
    zserio/UnexpectedOptionalException.h
    zserio/UnionCaseException.cpp
    zserio/UnionCaseException.h
    zserio/UniquePtr.h
    zserio/ValidationException.cpp
    zserio/ValidationException.h
    zserio/Variant.cpp
    zserio/Variant.h
    zserio/Vector.h
    zserio/View.h
    zserio/Walker.h
    zserio/WalkerConst.h
    zserio/ZserioTreeCreator.cpp
    zserio/ZserioTreeCreator.h
)

if (ZSERIO_CPP17_RUNTIME_SQLITE_ENABLE)
    set(ZSERIO_CPP17_RUNTIME_LIB_SQLITE_SRCS
        zserio/ISqliteDatabase.h
        zserio/SqliteConnection.h
        zserio/SqliteException.h
        zserio/SqliteException.cpp
        zserio/SqliteFinalizer.h
        zserio/SqliteUtil.h
        zserio/ValidationSqliteUtil.h
    )
endif()

if (${ZSERIO_CPP17_RUNTIME_BUILD_SHARED_LIBS})
    message(STATUS "Zserio runtime will be built as a shared library") 
    set(ZSERIO_CPP17_LIB_TYPE "SHARED")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
else()
    message(STATUS "Zserio runtime will be built as a static library")
    set(ZSERIO_CPP17_LIB_TYPE "STATIC")
endif()

add_library(${PROJECT_NAME} ${ZSERIO_CPP17_LIB_TYPE} ${ZSERIO_CPP17_RUNTIME_LIB_SRCS} ${ZSERIO_CPP17_RUNTIME_LIB_SQLITE_SRCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/zserio FILES ${ZSERIO_CPP17_RUNTIME_LIB_SRCS} ${ZSERIO_CPP17_RUNTIME_LIB_SQLITE_SRCS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (ZSERIO_CPP17_RUNTIME_SQLITE_ENABLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC SQLite3)
endif()

# install Zserio C++ runtime library (we release sources only)
install(DIRECTORY . DESTINATION .
        FILES_MATCHING
        PATTERN "CMakeLists.txt"
        PATTERN "*.h"
        PATTERN "*.cpp"
        PATTERN "*Sqlite*" EXCLUDE)

if (ZSERIO_CPP17_RUNTIME_SQLITE_ENABLE)
    install(DIRECTORY . DESTINATION .
        FILES_MATCHING
        PATTERN "*Sqlite*.h"
        PATTERN "*Sqlite*.cpp")
endif()
