#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"
source "${SCRIPT_DIR}/test.sh"

# Run custom integration test
test_zs()
{
    exit_if_argc_ne $# 9
    local ZSERIO_CPP17_DISTR_DIR="$1"; shift
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift
    local ZSERIO_CPP17_BUILD_DIR="$1"; shift
    local TEST_OUT_DIR="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CPP_TARGETS=("${MSYS_WORKAROUND_TEMP[@]}")
    local SWITCH_SOURCE="$1"; shift
    local SWITCH_DIRECTORY="$1"; shift
    local SWITCH_TEST_NAME="$1"; shift
    local SWITCH_TEST_FILE="$1"; shift

    # run custom integration tests
    local MESSAGE="Zserio C++17 custom integration test"
    echo "STARTING - ${MESSAGE}"

    local TEST_SUITES_LIST="external"
    local CMAKE_ARGS=("-DZSERIO_RELEASE_ROOT=${ZSERIO_CPP17_BUILD_DIR}/extension/download"
                      "-DZSERIO_CPP17_RELEASE_ROOT=${ZSERIO_CPP17_DISTR_DIR}"
                      "-DZSERIO_TEST_SUITES=${TEST_SUITES_LIST}"
                      "-DZSERIO_TEST_EXTERN_SCHEMA=${SWITCH_SOURCE}"
                      "-DZSERIO_TEST_EXTERN_SCHEMA_ROOT=${SWITCH_DIRECTORY}"
                      "-DZSERIO_TEST_EXTERN_SCHEMA_NAME=${SWITCH_TEST_NAME}"
                      "-DZSERIO_TEST_EXTERN_SCHEMA_TEST=${SWITCH_TEST_FILE}")
    local CTEST_ARGS=(--verbose)
    local CPP_TARGET="all"
    local TEST_SRC_DIR="${ZSERIO_CPP17_PROJECT_ROOT}/test"
    compile_cpp "${ZSERIO_CPP17_PROJECT_ROOT}" "${TEST_OUT_DIR}" "${TEST_SRC_DIR}" CPP_TARGETS[@] \
                CMAKE_ARGS[@] CTEST_ARGS[@] ${CPP_TARGET}
    if [ $? -ne 0 ] ; then
        stderr_echo "${MESSAGE} failed!"
        return 1
    fi

    echo -e "FINISHED - ${MESSAGE}\n"

    return 0
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Runs given integration test on given zserio sources using C++17 extension from distr directory.

Usage:
    $0 [-h] [-e] [-p] [-o <dir>] [-d <dir>] [-s <source>] [-t <name>] [-f <file>] target

Arguments:
    -h, --help            Show this help.
    -e, --help-env        Show help for enviroment variables.
    -p, --purge           Purge test build directory.
    -o <dir>, --output-directory <dir>
                          Output directory where tests will be run.
    -d <dir>, --source-dir <dir>
                          Directory with zserio sources. Default is ".".
    -s <source>, --source <source>
                          Main zserio source to test.
    -t <name>, --test-name <name>
                          Test name. Optional.
    -f <file>, --test-file <file>
                          Test source file. Default is empty test.
    target                Specify the target to test.

Target can be a combination of:
    cpp-linux32-gcc      Tests for linux32 target using gcc compiler.
    cpp-linux64-gcc      Tests for linux64 target using gcc compiler.
    cpp-linux32-clang    Tests for linux32 target using using Clang compiler.
    cpp-linux64-clang    Tests for linux64 target using Clang compiler.
    cpp-windows64-ming   Tests for windows64 target (MinGW64).
    cpp-windows64-msvc   Tests for windows64 target (MSVC).

Examples:
    $0 cpp-linux64-gcc -d /tmp/zs -s test.zs

EOF
}

# Parse all command line arguments.
#
# Return codes:
# -------------
# 0 - Success. Arguments have been successfully parsed.
# 1 - Failure. Some arguments are wrong or missing.
# 2 - Help switch is present. Arguments after help switch have not been checked.
# 3 - Environment help switch is present. Arguments after help switch have not been checked.
parse_arguments()
{
    exit_if_argc_lt $# 7
    local PARAM_CPP_TARGET_ARRAY_OUT="$1"; shift
    local SWITCH_OUT_DIR_OUT="$1"; shift
    local SWITCH_PURGE_OUT="$1"; shift
    local SWITCH_DIRECTORY_OUT="$1"; shift
    local SWITCH_SOURCE_OUT="$1"; shift
    local SWITCH_TEST_NAME_OUT="$1"; shift
    local SWITCH_TEST_FILE_OUT="$1"; shift

    eval ${SWITCH_PURGE_OUT}=0
    eval ${SWITCH_DIRECTORY_OUT}="."
    eval ${SWITCH_SOURCE_OUT}=""
    eval ${SWITCH_TEST_NAME_OUT}=""
    eval ${SWITCH_TEST_FILE_OUT}=""

    local NUM_PARAMS=0
    local PARAM_ARRAY=();
    local ARG="$1"
    while [ $# -ne 0 ] ; do
        case "${ARG}" in
            "-h" | "--help")
                return 2
                ;;

            "-e" | "--help-env")
                return 3
                ;;

            "-o" | "--output-directory")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing output directory!"
                    echo
                    return 1
                fi
                eval ${SWITCH_OUT_DIR_OUT}="$2"
                shift 2
                ;;

            "-p" | "--purge")
                eval ${SWITCH_PURGE_OUT}=1
                shift
                ;;

            "-d" | "--source-dir")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing directory with zserio sources!"
                    echo
                    return 1
                fi
                eval ${SWITCH_DIRECTORY_OUT}="$2"
                shift 2
                ;;

            "-s" | "--source")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing main zserio source!"
                    echo
                    return 1
                fi
                eval ${SWITCH_SOURCE_OUT}="$2"
                shift 2
                ;;

            "-t" | "--test-name")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing test name!"
                    echo
                    return 1
                fi
                eval ${SWITCH_TEST_NAME_OUT}="$2"
                shift 2
                ;;

            "-f" | "--test-file")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing test file!"
                    echo
                    return 1
                fi
                eval ${SWITCH_TEST_FILE_OUT}="$2"
                shift 2
                ;;

            "-"*)
                stderr_echo "Invalid switch '${ARG}'!"
                echo
                return 1
                ;;

            *)
                PARAM_ARRAY[NUM_PARAMS]=${ARG}
                NUM_PARAMS=$((NUM_PARAMS + 1))
                shift
                ;;
        esac
        ARG="$1"
    done

    local NUM_CPP_TARGETS=0
    local PARAM
    for PARAM in "${PARAM_ARRAY[@]}" ; do
        case "${PARAM}" in
            "cpp-linux32-"* | "cpp-linux64-"* | "cpp-windows64-"*)
                eval ${PARAM_CPP_TARGET_ARRAY_OUT}[${NUM_CPP_TARGETS}]="${PARAM#cpp-}"
                NUM_CPP_TARGETS=$((NUM_CPP_TARGETS + 1))
                ;;

            *)
                stderr_echo "Invalid argument '${PARAM}'!"
                echo
                return 1
        esac
    done

    if [[ ${!SWITCH_PURGE_OUT} == 0 ]] ; then
        if [[ ${NUM_CPP_TARGETS} == 0 ]] ; then
            stderr_echo "Target to test is not specified!"
            echo
            return 1
        fi

        if [[ "${!SWITCH_SOURCE_OUT}" == "" ]] ; then
            stderr_echo "Main zserio source is not set!"
            echo
            return 1
        fi
    fi

    # default test name
    if [[ "${!SWITCH_TEST_NAME_OUT}" == "" ]] ; then
        local DEFAULT_TEST_NAME=${!SWITCH_SOURCE_OUT%.*} # strip extension
        DEFAULT_TEST_NAME=${DEFAULT_TEST_NAME//\//_} # all slashes to underscores
        eval ${SWITCH_TEST_NAME_OUT}=${DEFAULT_TEST_NAME}
    fi

    return 0
}

main()
{
    # get the project root, absolute path is necessary only for CMake
    local ZSERIO_CPP17_PROJECT_ROOT
    convert_to_absolute_path "${SCRIPT_DIR}/.." ZSERIO_CPP17_PROJECT_ROOT

    # parse command line arguments
    local PARAM_CPP_TARGET_ARRAY=()
    local SWITCH_OUT_DIR="${ZSERIO_CPP17_PROJECT_ROOT}"
    local SWITCH_PURGE
    local SWITCH_DIRECTORY
    local SWITCH_SOURCE
    local SWITCH_TEST_NAME
    local SWITCH_TEST_FILE
    parse_arguments PARAM_CPP_TARGET_ARRAY SWITCH_OUT_DIR SWITCH_PURGE SWITCH_DIRECTORY SWITCH_SOURCE \
            SWITCH_TEST_NAME SWITCH_TEST_FILE "$@"
    local PARSE_RESULT=$?
    if [ ${PARSE_RESULT} -eq 2 ] ; then
        print_help
        return 0
    elif [ ${PARSE_RESULT} -eq 3 ] ; then
        print_test_help_env
        print_help_env
        return 0
    elif [ ${PARSE_RESULT} -ne 0 ] ; then
        return 1
    fi

    echo "C++17 Extension Custom Integration Tests"
    echo

    # set global variables
    set_global_common_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    set_global_cpp_variables "${ZSERIO_PROJECT_ROOT}"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    # cmake needs absolute paths
    convert_to_absolute_path "${SWITCH_OUT_DIR}" SWITCH_OUT_DIR
    if [[ "${SWITCH_DIRECTORY}" != "" ]] ; then
        convert_to_absolute_path "${SWITCH_DIRECTORY}" SWITCH_DIRECTORY
    fi
    if [[ "${SWITCH_TEST_FILE}" != "" ]] ; then
        convert_to_absolute_path "${SWITCH_TEST_FILE}" SWITCH_TEST_FILE
    fi

    # purge if requested and then create test output directory
    local ZSERIO_CPP17_BUILD_DIR="${SWITCH_OUT_DIR}/build"
    local TEST_OUT_DIR="${ZSERIO_CPP17_BUILD_DIR}/test_zs/${SWITCH_TEST_NAME}"
    if [[ ${SWITCH_PURGE} == 1 ]] ; then
        echo "Purging test directory."
        echo
        rm -rf "${TEST_OUT_DIR}/"

        if [[ ${#PARAM_CPP_TARGET_ARRAY[@]} == 0 ]] ; then
            return 0  # purge only
        fi
    fi
    mkdir -p "${TEST_OUT_DIR}"

    # print information
    echo "Test output directory: ${TEST_OUT_DIR}"
    echo

    # run test
    local ZSERIO_CPP17_DISTR_DIR="${SWITCH_OUT_DIR}/distr"
    test_zs "${ZSERIO_CPP17_DISTR_DIR}" "${ZSERIO_CPP17_PROJECT_ROOT}" "${ZSERIO_CPP17_BUILD_DIR}" \
            "${TEST_OUT_DIR}" PARAM_CPP_TARGET_ARRAY[@] "${SWITCH_SOURCE}" "${SWITCH_DIRECTORY}" \
            "${SWITCH_TEST_NAME}" "${SWITCH_TEST_FILE}"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    return 0
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]] ; then
    main "$@"
fi
