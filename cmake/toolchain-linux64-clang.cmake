# set Clang
if (DEFINED ENV{CLANG_COMPILER_ROOT})
    set(COMPILER_PATH_PREFIX "$ENV{CLANG_COMPILER_ROOT}/")
else ()
    set(COMPILER_PATH_PREFIX "")
endif()

set(CMAKE_C_COMPILER "${COMPILER_PATH_PREFIX}clang$ENV{CLANG_VERSION_SUFFIX}")
set(CMAKE_CXX_COMPILER "${COMPILER_PATH_PREFIX}clang++$ENV{CLANG_VERSION_SUFFIX}")
set(CMAKE_LINKER "${COMPILER_PATH_PREFIX}clang++$ENV{CLANG_VERSION_SUFFIX}")

# set -m64
set(COMPILER_M64_FLAG "--target=x86_64-linux")

# set C flags
set(CMAKE_C_FLAGS_INIT "${COMPILER_M64_FLAG}")

# set CXX flags
set(CMAKE_CXX_FLAGS_INIT "${COMPILER_M64_FLAG}")
