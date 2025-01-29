#!/bin/bash

# Source build-env.sh if found.
SCRIPT_DIR=`dirname $0`
if [ -e "${SCRIPT_DIR}/build-env.sh" ] ; then
    source "${SCRIPT_DIR}/build-env.sh"
fi

# Set and check global variables for Java projects.
set_global_common_variables()
{
    # bash command find to use, defaults to "/usr/bin/find" if not set
    # (bash command find makes trouble under MinGW because it clashes with Windows find command)
    FIND="${FIND:-/usr/bin/find}"
    if [ ! -f "`which "${FIND}"`" ] ; then
        stderr_echo "Cannot find bash command find! Set FIND environment variable."
        return 1
    fi

    # clang-format binary to use for formatting check, by default is empty
    CLANG_FORMAT_BIN="${CLANG_FORMAT_BIN:-""}"
    if [[ (! -z "${CLANG_FORMAT_BIN}" && ! -f "`which "${CLANG_FORMAT_BIN}"`") ]] ; then
        stderr_echo "Provided CLANG_FORMAT_BIN=\"${CLANG_FORMAT_BIN}\" does not exist!"
        return 1
    fi

    # clang-format force flag to use for formatting check, by default is 0 (false)
    CLANG_FORMAT_FORCE="${CLANG_FORMAT_FORCE:-0}"
}

# Set and check global variables for Java projects.
set_global_java_variables()
{
    # ANT to use, defaults to "ant" if not set
    ANT="${ANT:-ant}"
    if [ ! -f "`which "${ANT}"`" ] ; then
        stderr_echo "Cannot find Ant! Set ANT environment variable."
        return 1
    fi

    # Ant extra arguments are empty by default
    ANT_EXTRA_ARGS="${ANT_EXTRA_ARGS:-""}"

    # check java binary
    if [ -n "${JAVA_HOME}" ] ; then
        JAVA_BIN="${JAVA_HOME}/bin/java"
    fi
    JAVA_BIN="${JAVA_BIN:-java}"
    if [ ! -f "`which "${JAVA_BIN}"`" ] ; then
        stderr_echo "Cannot find java! Set JAVA_HOME or JAVA_BIN environment variable."
        return 1
    fi

    # check javac binary
    if [ -n "${JAVA_HOME}" ] ; then
        JAVAC_BIN="${JAVA_HOME}/bin/javac"
    fi
    JAVAC_BIN="${JAVAC_BIN:-javac}"
    if [ ! -f "`which "${JAVAC_BIN}"`" ] ; then
        stderr_echo "Cannot find java compiler! Set JAVA_HOME or JAVAC_BIN environment variable."
        return 1
    fi

    # spotbugs home directory is empty by default
    SPOTBUGS_HOME="${SPOTBUGS_HOME:-""}"
}

# Set and check global variables for C++ projects.
set_global_cpp_variables()
{
    # CMake to use, defaults to "cmake" if not set
    CMAKE="${CMAKE:-cmake}"
    if [ ! -f "`which "${CMAKE}"`" ] ; then
        stderr_echo "Cannot find CMake! Set CMAKE environment variable."
        return 1
    fi

    # CMake extra arguments are empty by default
    CMAKE_EXTRA_ARGS="${CMAKE_EXTRA_ARGS:-""}"

    # CTest to use, defaults to "ctest" if not set
    CTEST="${CTEST:-ctest}"
    if [ ! -f "`which "${CTEST}"`" ] ; then
        stderr_echo "Cannot find CTest! Set CTEST environment variable."
        return 1
    fi

    # Doxygen to use, defaults to "doxygen" if not set
    DOXYGEN="${DOXYGEN:-doxygen}"
    if [ ! -f "`which "${DOXYGEN}"`" ] ; then
        stderr_echo "Cannot find Doxygen! Set DOXYGEN environment variable."
        return 1
    fi

    MAKE_CMAKE_GENERATOR="${MAKE_CMAKE_GENERATOR:-Unix Makefiles}"
    MSVC_CMAKE_GENERATOR="${MSVC_CMAKE_GENERATOR:-Visual Studio 17 2022}"
    MSVC_CMAKE_TOOLSET="${MSVC_CMAKE_TOOLSET:-v141}"

    # Extra arguments to be passed by CMake to a native build tool
    CMAKE_BUILD_OPTIONS="${CMAKE_BUILD_OPTIONS:-""}"

    # gcovr binary to use for coverage report (gcc), by default is empty
    GCOVR_BIN="${GCOVR_BIN:-""}"
    if [[ (! -z "${GCOVR_BIN}" && ! -f "`which "${GCOVR_BIN}"`") ]] ; then
        stderr_echo "Provided GCOVR_BIN=\"${GCOVR_BIN}\" does not exist!"
        return 1
    fi

    # llvm-profdata binary to use for coverage report (clang), by default is empty
    LLVM_PROFDATA_BIN="${LLVM_PROFDATA_BIN:-""}"
    if [[ (! -z "${LLVM_PROFDATA_BIN}" && ! -f "`which "${LLVM_PROFDATA_BIN}"`") ]] ; then
        stderr_echo "Provided LLVM_PROFDATA_BIN=\"${LLVM_PROFDATA_BIN}\" does not exist!"
        return 1
    fi

    # llvm-cov binary to use for coverage report (clang), by default is empty
    LLVM_COV_BIN="${LLVM_COV_BIN:-""}"
    if [[ (! -z "${LLVM_COV_BIN}" && ! -f "`which "${LLVM_COV_BIN}"`") ]] ; then
        stderr_echo "Provided LLVM_COV_BIN=\"${LLVM_COV_BIN}\" does not exist!"
        return 1
    fi

    if [[ (! -z "${LLVM_PROFDATA_BIN}" && -z "${LLVM_COV_BIN}") ||
            (-z "${LLVM_PROFDATA_BIN}" && ! -z "${LLVM_COV_BIN}") ]] ; then
        stderr_echo "Both LLVM_PROFDATA_BIN and LLVM_COV_BIN environment variable must be set!"
        return 1
    fi

    # Sanitizers configuration - sanitizers disabled by default
    SANITIZERS_ENABLED="${SANITIZERS_ENABLED:-0}"

    # clang-tidy binary to use for static code check, by default is empty
    CLANG_TIDY_BIN="${CLANG_TIDY_BIN:-""}"
    if [[ (! -z "${CLANG_TIDY_BIN}" && ! -f "`which "${CLANG_TIDY_BIN}"`") ]] ; then
        stderr_echo "Provided CLANG_TIDY_BIN=\"${CLANG_TIDY_BIN}\" does not exist!"
        return 1
    fi

    return 0
}

# Print help on the environment variables used.
print_help_env()
{
    cat << EOF
Uses the following environment variables for building:
    ANT                    Ant executable to use. Default is "ant".
    ANT_EXTRA_ARGS         Extra arguments to Ant. Default is empty string.
    CLANG_FORMAT_BIN       Name of clang-format binary. If not set, clang-format tool is not called.
    CLANG_FORMAT_FORCE     Defines whether to force formatting changes. Default is 0 (disabled).
    CLANG_TIDY_BIN         Name of clang-tidy binary. If not set, clang-tidy tool is not called.
    CLANG_VERSION_SUFFIX   Clang compiler version suffix. Default is empty.
                           Set e.g. "-8" to use "clang-8" instead of "clang".
    CMAKE                  CMake executable to use. Default is "cmake".
    CMAKE_BUILD_OPTIONS    Arguments to be passed by CMake to a native build tool.
    CMAKE_EXTRA_ARGS       Extra arguments to CMake. Default is empty string.
    CTEST                  Ctest executable to use. Default is "ctest".
    DOT                    Dot executable to use. Default is "dot".
    DOXYGEN                Doxygen executable to use. Default is 'doxygen".
    GCC_VERSION_SUFFIX     Gcc compiler version suffix. Default is empty.
                           Set e.g. "-11" to use "gcc-11" instead of "gcc".
    GCOVR_BIN              Gcovr binary to use for coverage report generation (gcc).
                           Default is empty string.
    JAVAC_BIN              Java compiler executable to use. Default is "javac".
    JAVA_BIN               Java executable to use. Default is "java".
    LLVM_PROFDATA_BIN      llvm-profdata  binary to use for coverage report generation (clang).
                           Default is empty string.
    LLVM_COV_BIN           llvm-cov  binary to use for coverage report generation (clang).
                           Default is empty string.
    MAKE_CMAKE_GENERATOR   CMake generator to use for build using Makefiles. Default is
                           "Unix Makefiles".
    MSVC_CMAKE_GENERATOR   CMake generator to use with MSVC compiler. Default is
                           "Visual Studio 17 2022". Note that CMake option "-A x64"
                           is added automatically for windows64-mscv target.
    MSVC_CMAKE_TOOLSET     MSVC toolset specification for CMake generator.
                           Default is "v141". Note that "v141" is for VS 2017,
                           "v142" is for VS 2019.
    SANITIZERS_ENABLED     Defines whether to use sanitizers. Default is 0 (disabled).

    Either set these directly, or create 'scripts/build-env.sh' that sets
    these. It's sourced automatically if it exists.

EOF
}

# Print a message to stderr.
stderr_echo()
{
    echo "FATAL ERROR - $@" 1>&2
}

# Exit if number of input arguments is not equal to number required by function.
#
# Usage:
# ------
# exit_if_argc_ne $# 2
#
# Return codes:
# -------------
# 0 - Always success. In case of failure, function exits with error code 3.
exit_if_argc_ne()
{
    local NUM_OF_ARGS=2
    if [ $# -ne ${NUM_OF_ARGS} ] ; then
        stderr_echo "${FUNCNAME[0]}() called with $# arguments but ${NUM_OF_ARGS} is required."
        exit 3
    fi

    local NUM_OF_CALLER_ARGS=$1; shift
    local REQUIRED_NUM_OF_CALLED_ARGS=$1; shift
    if [ ${NUM_OF_CALLER_ARGS} -ne ${REQUIRED_NUM_OF_CALLED_ARGS} ] ; then
        stderr_echo "${FUNCNAME[1]}() called with ${NUM_OF_CALLER_ARGS} arguments but ${REQUIRED_NUM_OF_CALLED_ARGS} is required."
        exit 3
    fi
}

# Exit if number of input arguments is less than number required by function.
#
# Usage:
# ------
# exit_if_argc_lt $# 2
#
# Return codes:
# -------------
# 0 - Always success. In case of failure, function exits with error code 3.
exit_if_argc_lt()
{
    local NUM_OF_ARGS=2
    if [ $# -ne ${NUM_OF_ARGS} ] ; then
        stderr_echo "${FUNCNAME[0]}() called with $# arguments but ${NUM_OF_ARGS} is required."
        exit 3
    fi

    local NUM_OF_CALLER_ARGS=$1; shift
    local REQUIRED_NUM_OF_CALLED_ARGS=$1; shift
    if [ ${NUM_OF_CALLER_ARGS} -lt ${REQUIRED_NUM_OF_CALLED_ARGS} ] ; then
        stderr_echo "${FUNCNAME[1]}() called with ${NUM_OF_CALLER_ARGS} arguments but ${REQUIRED_NUM_OF_CALLED_ARGS} is required."
        exit 3
    fi
}

# Convert input argument to absolute path.
convert_to_absolute_path()
{
    exit_if_argc_ne $# 2
    local PATH_TO_CONVERT="$1"; shift
    local ABSOLUTE_PATH_OUT="$1"; shift

    local DIR_TO_CONVERT="${PATH_TO_CONVERT}"
    local FILE_TO_CONVERT=""
    if [ ! -d "${DIR_TO_CONVERT}" ] ; then
        DIR_TO_CONVERT="${PATH_TO_CONVERT%/*}"
        FILE_TO_CONVERT="${PATH_TO_CONVERT##*/}"
        if [[ "${DIR_TO_CONVERT}" == "${FILE_TO_CONVERT}" ]] ; then
            DIR_TO_CONVERT="."
        else
            if [ ! -d "${DIR_TO_CONVERT}" ] ; then
                stderr_echo "${FUNCNAME[0]}() called with a non-existing directory ${DIR_TO_CONVERT}!"
                return 1
            fi
        fi
    fi

    pushd "${DIR_TO_CONVERT}" > /dev/null
    # don't use "`pwd`" here because it does not work if path contains spaces
    local ABSOLUTE_PATH="'`pwd`'"
    popd > /dev/null

    if [ -n "${FILE_TO_CONVERT}" ] ; then
        ABSOLUTE_PATH="${ABSOLUTE_PATH}/${FILE_TO_CONVERT}"
    fi

    eval ${ABSOLUTE_PATH_OUT}="${ABSOLUTE_PATH}"

    return 0
}

# Compile Java by running Ant target.
compile_java()
{
    exit_if_argc_ne $# 3
    local ANT_BUILD_FILE="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local ANT_PROPS=("${MSYS_WORKAROUND_TEMP[@]}")
    local ANT_TARGET="$1"; shift

    if [ -n "${CLANG_FORMAT_BIN}" ] ; then
        ANT_PROPS+=("-Dclang_format.exec_file=${CLANG_FORMAT_BIN}")
    fi
    if [[ ${CLANG_FORMAT_FORCE} == 1 ]] ; then
        local ANT_PROPS+=("-Dclang_format.force=true")
    fi

    if [ -n "${SPOTBUGS_HOME}" ] ; then
        ANT_PROPS+=("-Dspotbugs.home_dir=${SPOTBUGS_HOME}")
    fi

    "${ANT}" ${ANT_EXTRA_ARGS} -f "${ANT_BUILD_FILE}" "${ANT_PROPS[@]}" ${ANT_TARGET}
    local ANT_RESULT=$?
    if [ ${ANT_RESULT} -ne 0 ] ; then
        stderr_echo "Running ant failed with return code ${ANT_RESULT}!"
        return 1
    fi

    return 0
}

# Compile and test C++ code by running cmake and make for all targets.
compile_cpp()
{
    exit_if_argc_ne $# 7
    local ZSERIO_PROJECT_ROOT="$1"; shift
    local BUILD_DIR="$1"; shift
    local CMAKELISTS_DIR="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local TARGETS=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CMAKE_ARGS=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CTEST_ARGS=("${MSYS_WORKAROUND_TEMP[@]}")
    local MAKE_TARGET="$1"; shift

    local TARGET
    for TARGET in "${TARGETS[@]}" ; do
        compile_cpp_for_target "${ZSERIO_PROJECT_ROOT}" "${BUILD_DIR}/${TARGET}" "${CMAKELISTS_DIR}" \
                               "${TARGET}" CMAKE_ARGS[@] CTEST_ARGS[@] "${MAKE_TARGET}"
        if [ $? -ne 0 ] ; then
            return 1
        fi
    done

    return 0
}

# Compile and test C++ code by running cmake and make for one target.
compile_cpp_for_target()
{
    exit_if_argc_ne $# 7
    local ZSERIO_PROJECT_ROOT="$1"; shift
    local BUILD_DIR="$1"; shift
    local CMAKELISTS_DIR="$1"; shift
    local TARGET="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CMAKE_ARGS=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CTEST_ARGS=("${MSYS_WORKAROUND_TEMP[@]}")
    local MAKE_TARGET="$1"; shift

    local TOOLCHAIN_FILE="${ZSERIO_PROJECT_ROOT}/cmake/toolchain-${TARGET}.cmake"
    CMAKE_ARGS=("--no-warn-unused-cli"
                "${CMAKE_ARGS[@]}"
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
                "-DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE}"
                "-DCMAKE_PREFIX_PATH=${SQLITE_RELEASE_ROOT}/${TARGET}")

    if [ ${SANITIZERS_ENABLED} -eq 1 ] ; then
        CMAKE_ARGS+=("-DSANITIZERS_ENABLED=ON")
    else
        CMAKE_ARGS+=("-DSANITIZERS_ENABLED=OFF")
    fi

    if [ ! -z "${CLANG_TIDY_BIN}" ] ; then
        CMAKE_ARGS+=("-DCLANG_TIDY_BIN=${CLANG_TIDY_BIN}")
    else
        CMAKE_ARGS+=("-UCLANG_TIDY_BIN")
    fi

    if [ ! -z "${CLANG_FORMAT_BIN}" ] ; then
        CMAKE_ARGS+=("-DCLANG_FORMAT_BIN=${CLANG_FORMAT_BIN}")
    else
        CMAKE_ARGS+=("-UCLANG_FORMAT_BIN")
    fi

    if [[ ${CLANG_FORMAT_FORCE} == 1 ]] ; then
        CMAKE_ARGS+=("-DCLANG_FORMAT_FORCE=ON")
    else
        CMAKE_ARGS+=("-DCLANG_FORMAT_FORCE=OFF")
    fi

    # detect build type
    local BUILD_TYPE="Release"
    local BUILD_TYPE_LOWER_CASE="release"
    if [[ "${CMAKE_EXTRA_ARGS}" == *-DCMAKE_BUILD_TYPE=?ebug* ]] ; then
        BUILD_TYPE="Debug";
        BUILD_TYPE_LOWER_CASE="debug"
    fi

    BUILD_DIR="${BUILD_DIR}/${BUILD_TYPE_LOWER_CASE}"

    mkdir -p "${BUILD_DIR}"
    pushd "${BUILD_DIR}" > /dev/null

    if [[ ${MAKE_TARGET} == "install" ]] ; then
        local CMAKE_BUILD_TARGET="all"
    else
        local CMAKE_BUILD_TARGET="${MAKE_TARGET}"
    fi

    # resolve CMake generator
    if [[ ${TARGET} == *"-msvc" ]] ; then
        local CMAKE_BUILD_CONFIG="--config ${BUILD_TYPE}"
        CTEST_ARGS+=("-C ${BUILD_TYPE}")
        if [[ ${CMAKE_BUILD_TARGET} == "all" ]] ; then
            CMAKE_BUILD_TARGET="ALL_BUILD" # all target doesn't exist in MSVC solution
        fi
        local CMAKE_GENERATOR="${MSVC_CMAKE_GENERATOR}";
        CMAKE_ARGS=("${CMAKE_ARGS[@]}" "-A x64" "-T ${MSVC_CMAKE_TOOLSET}")
    else
        local CMAKE_GENERATOR="${MAKE_CMAKE_GENERATOR}"
        local CMAKE_BUILD_CONFIG=""
    fi

    # generate makefile running cmake
    "${CMAKE}" ${CMAKE_EXTRA_ARGS} -G "${CMAKE_GENERATOR}" "${CMAKE_ARGS[@]}" "${CMAKELISTS_DIR}"
    local CMAKE_RESULT=$?
    if [ ${CMAKE_RESULT} -ne 0 ] ; then
        stderr_echo "Running CMake failed with return code ${CMAKE_RESULT}!"
        popd > /dev/null
        return 1
    fi

    # build it running cmake
    "${CMAKE}" --build . --target ${CMAKE_BUILD_TARGET} ${CMAKE_BUILD_CONFIG} -- ${CMAKE_BUILD_OPTIONS}
    local CMAKE_RESULT=$?
    if [ ${CMAKE_RESULT} -ne 0 ] ; then
        stderr_echo "Running CMake failed with return code ${CMAKE_RESULT}!"
        popd > /dev/null
        return 1
    fi

    # only run "make test" if we can actually run it on current host
    can_run_tests "${TARGET}"
    local CAN_RUN_TESTS_RESULT=$?
    if [[ ${MAKE_TARGET} != "clean" && ${CAN_RUN_TESTS_RESULT} == 0 ]] ; then
        CTEST_OUTPUT_ON_FAILURE=1 "${CTEST}" ${CTEST_ARGS[@]}
        local CTEST_RESULT=$?
        if [ ${CTEST_RESULT} -ne 0 ] ; then
            stderr_echo "Tests on target ${TARGET} failed with return code ${CTEST_RESULT}."
            popd > /dev/null
            return 1
        fi

        # run coverage target if it is requested and if exists
        if [[ (! -z "${GCOVR_BIN}" && "${TARGET}" == *"gcc"*) ||
                (! -z "${LLVM_PROFDATA_BIN}" && ! -z "${LLVM_COV_BIN}" && "${TARGET}" == *"clang"*) ]] ; then
            if [[ `cmake --build . -- help | grep coverage` ]] ; then
                "${CMAKE}" --build . --target coverage
                local COVERAGE_RESULT=$?
                if [ ${COVERAGE_RESULT} -ne 0 ] ; then
                    stderr_echo "Generating of coverage report failed with return code ${COVERAGE_RESULT}."
                    popd > /dev/null
                    return 1
                fi
            fi
        fi
    fi

    # install it running cmake
    if [[ ${MAKE_TARGET} == "install" ]] ; then
        "${CMAKE}" --build . --target install ${CMAKE_BUILD_CONFIG} -- ${CMAKE_BUILD_OPTIONS}
        local CMAKE_RESULT=$?
        if [ ${CMAKE_RESULT} -ne 0 ] ; then
            stderr_echo "Running CMake failed with return code ${CMAKE_RESULT}!"
            popd > /dev/null
            return 1
        fi
    fi

    popd > /dev/null

    return 0
}

# Test if it's possible to run tests for given target on current host.
can_run_tests()
{
    exit_if_argc_ne $# 1
    local TARGET_PLATFORM="$1"; shift

    local HOST_PLATFORM
    get_host_platform HOST_PLATFORM
    if [ $? -ne 0 ] ; then
        return 1
    fi

    # assume on 64bit both 32bit and 64bit executables can be run
    case "${HOST_PLATFORM}" in
    ubuntu32)
        [[ "${TARGET_PLATFORM}" == "linux32-"* ]]
        ;;
    ubuntu64)
        [[ "${TARGET_PLATFORM}" == "linux32-"* || "${TARGET_PLATFORM}" = "linux64-"* ]]
        ;;
    windows64)
        [[ "${TARGET_PLATFORM}" == "windows64-"* ]]
        ;;
    *)
        stderr_echo "can_run_tests: unknown current platform ${HOST_PLATFORM}!"
        return 1
    esac
}

# Determines the current host platform.
#
# Returns one of the following platforms:
# ubuntu32, ubuntu64, windows32, windows64
get_host_platform()
{
    exit_if_argc_ne $# 1
    local HOST_PLATFORM_OUT="$1"; shift

    local OS=`uname -s`
    local HOST=""
    case "${OS}" in
    Linux)
        HOST="ubuntu"
        ;;
    MINGW*|MSYS*)
        HOST="windows"
        ;;
    *)
        stderr_echo "uname returned unsupported OS!"
        return 1
        ;;
    esac

    if [ "${HOST}" = "windows" ] ; then
        # can't use uname on windows - MSYS always says it's i686
        local CURRENT_ARCH
        CURRENT_ARCH=`wmic OS get OSArchitecture 2> /dev/null`
        if [ $? -ne 0 ] ; then
            # wmic failed, assume it's Windows XP 32bit
            NATIVE_TARGET="windows32"
        else
            case "${CURRENT_ARCH}" in
            *64-bit*)
                NATIVE_TARGET="windows64"
                ;;
            *32-bit*)
                NATIVE_TARGET="windows32"
                ;;
            *)
                stderr_echo "wmic returned unsupported architecture!"
                return 1
            esac
        fi
    else
        local CURRENT_ARCH=`uname -m`
        case "${CURRENT_ARCH}" in
        x86_64)
            NATIVE_TARGET="${HOST}64"
            ;;
        i686)
            NATIVE_TARGET="${HOST}32"
            ;;
        *)
            stderr_echo "unname returned unsupported architecture!"
            return 1
        esac
    fi

    eval ${HOST_PLATFORM_OUT}="${NATIVE_TARGET}"

    return 0
}

# Returns path according to the current host.
#
# On Linux the given path is unchanged, on Windows the path is converted to windows path.
posix_to_host_path()
{
    exit_if_argc_lt $# 2
    local POSIX_PATH="$1"; shift
    local HOST_PATH_OUT="$1"; shift
    local DISABLE_SLASHES_CONVERSION=0
    if [ $# -ne 0 ] ; then
        DISABLE_SLASHES_CONVERSION="$1"; shift # optional, default is false
    fi

    local HOST_PLATFORM
    get_host_platform HOST_PLATFORM
    if [[ "${HOST_PLATFORM}" == "windows"* ]] ; then
        # change drive specification in case of full path, e.g. '/d/...' to 'd:/...'
        local SEARCH_PATTERN="/?/"
        if [ "${POSIX_PATH}" != "${POSIX_PATH/${SEARCH_PATTERN}/}" ] ; then
            POSIX_PATH="${POSIX_PATH:1:1}:${POSIX_PATH:2}"
        fi

        if [ ${DISABLE_SLASHES_CONVERSION} -ne 1 ] ; then
            # replace all Posix '/' to Windows '\'
            local SEARCH_PATTERN="/"
            local REPLACE_PATTERN="\\"
            POSIX_PATH="${POSIX_PATH//${SEARCH_PATTERN}/${REPLACE_PATTERN}}"
        fi
    fi

    eval ${HOST_PATH_OUT}="'${POSIX_PATH}'"
}

# Returns executable according to the current host.
#
# On Linux the given executable is unchanged, on Windows the executable is appended by extension if it is given
# without any extension.
get_executable()
{
    exit_if_argc_lt $# 2
    local EXECUTABLE="$1"; shift
    local HOST_EXECUTABLE_OUT="$1"; shift

    local HOST_PLATFORM
    get_host_platform HOST_PLATFORM
    if [[ "${HOST_PLATFORM}" == "windows"* ]] ; then
        local HOST_EXECUTABLE="`ls --append-exe "${EXECUTABLE}"`"
    else
        local HOST_EXECUTABLE="${EXECUTABLE}"
    fi

    eval ${HOST_EXECUTABLE_OUT}="'${HOST_EXECUTABLE}'"
}
