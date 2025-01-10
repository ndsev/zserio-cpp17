#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"

# Gets test suites matching the provided patterns.
get_test_suites()
{
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local PATTERNS=("${MSYS_WORKAROUND_TEMP[@]}")
    local TEST_SUITES_OUT="$1"; shift

    local STARTING_POINT="${ZSERIO_CPP17_PROJECT_ROOT}/test"
    local FIND_EXPRESSION=()
    for i in ${!PATTERNS[@]} ; do
        FIND_EXPRESSION+=("(")
    done
    for i in ${!PATTERNS[@]} ; do
        local PATTERN="${PATTERNS[$i]}"
        if [[ $PATTERN == "i:"* ]] ; then
            PATTERN="${PATTERN#i:}"
            if [ $i -gt 0 ] ; then
                FIND_EXPRESSION+=("-o")
            fi
            FIND_EXPRESSION+=("-ipath" "${STARTING_POINT}/${PATTERN}")
        elif [[ $PATTERN == "x:"* ]] ; then
            PATTERN="${PATTERN#x:}"
            if [ $i -gt 0 ] ; then
                FIND_EXPRESSION+=("-a")
            fi
            FIND_EXPRESSION+=("!" "-ipath" "${STARTING_POINT}/${PATTERN}")
        else
            stderr_echo "Unexpected test pattern!"
            return 1
        fi
        FIND_EXPRESSION+=(")")
    done

    local TEST_SUITES_ARR=(
        $(${FIND} ${STARTING_POINT} -mindepth 2 -maxdepth 2 -type d "${FIND_EXPRESSION[@]}" | sort)
    )

    for i in ${!TEST_SUITES_ARR[@]} ; do
        eval ${TEST_SUITES_OUT}[$i]="${TEST_SUITES_ARR[$i]#${STARTING_POINT}/}"
    done

    return 0
}

# Compare BLOBs and JSONs created by tests.
compare_test_data()
{
    exit_if_argc_ne $# 4
    local TEST_SRC_DIR="$1"; shift
    local TEST_OUT_DIR="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local TEST_SUITES=("${MSYS_WORKAROUND_TEMP[@]}")
    local PLATFORM_NAME="$1"; shift

    echo
    echo "Comparing data created by ${PLATFORM_NAME} tests"
    echo

    local TOTAL_BLOBS=0
    local TOTAL_JSONS=0
    for TEST_SUITE in "${TEST_SUITES[@]}"; do
        local TEST_DATA_DIR="${TEST_SRC_DIR}/data/${TEST_SUITE}/data"
        local TEST_SUITE_DIR="${TEST_OUT_DIR}/${TEST_SUITE}"

        # check if test suite exists for this platform
        if [ -d "${TEST_SUITE_DIR}" ] ; then
            local FIND_PARAMS="-name *.blob -o -name *.json -not -name compile_commands.json"
            local TEST_SUITE_FILES=($("${FIND}" "${TEST_SUITE_DIR}" ${FIND_PARAMS} | sort))

            echo -n "${TEST_SUITE} ... "
            local NUM_BLOBS=0
            local NUM_JSONS=0
            local CMP_RESULT=0
            for TEST_SUITE_FILE in ${TEST_SUITE_FILES[@]} ; do
                local FILE_NAME="${TEST_SUITE_FILE##*/}"
                local TEST_DATA_FILE="${TEST_DATA_DIR}/${FILE_NAME}"

                if [ ! -e "${TEST_DATA_FILE}" ] ; then
                    stderr_echo "Data file '${TEST_DATA_FILE}' doesn't exist!"
                    echo
                    return 1
                fi

                if [[ "${FILE_NAME}" == *.blob ]] ; then
                    NUM_BLOBS=$((NUM_BLOBS+1))
                    cmp "${TEST_SUITE_FILE}" "${TEST_DATA_FILE}"
                    if [ $? -ne 0 ] ; then
                        CMP_RESULT=1
                    fi
                else
                    NUM_JSONS=$((NUM_JSONS+1))
                    diff --strip-trailing-cr "${TEST_SUITE_FILE}" "${TEST_DATA_FILE}"
                    if [ $? -ne 0 ] ; then
                        CMP_RESULT=1
                    fi
                fi
            done

            if [ ${CMP_RESULT} -ne 0 ] ; then
                stderr_echo "Comparison failed!"
                echo
                return 1
            fi

            if [ ${NUM_BLOBS} -eq 0 -a ${NUM_JSONS} -eq 0 ] ; then
                echo "N/A"
            else
                echo "${NUM_BLOBS} BLOBs, ${NUM_JSONS} JSONs"
            fi

            TOTAL_BLOBS=$((TOTAL_BLOBS+NUM_BLOBS))
            TOTAL_JSONS=$((TOTAL_JSONS+NUM_JSONS))
        fi
    done

    if [[ $((TOTAL_BLOBS+TOTAL_JSONS)) -gt 0 ]] ; then
        echo "Successfully compared ${TOTAL_BLOBS} BLOBs and ${TOTAL_JSONS} JSONs."
    else
        echo "Nothing to compare."
    fi
}

# Run integration tests
test_cpp()
{
    exit_if_argc_ne $# 7
    local ZSERIO_CPP17_DISTR_DIR="$1"; shift
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift
    local ZSERIO_CPP17_BUILD_DIR="$1"; shift
    local TEST_OUT_DIR="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CPP_TARGETS=("${MSYS_WORKAROUND_TEMP[@]}")
    local SWITCH_CLEAN="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local TEST_SUITES=("${MSYS_WORKAROUND_TEMP[@]}")

    # run integration tests
    local MESSAGE="Zserio C++17 integration tests"
    echo "STARTING - ${MESSAGE}"

    local TEST_SUITES_LIST=""
    for i in ${!TEST_SUITES[@]} ; do
        if [ ${i} -gt 0 ] ; then
            TEST_SUITES_LIST+=";"
        fi
        TEST_SUITES_LIST+="${TEST_SUITES[i]}"
    done

    local CMAKE_ARGS=("-DZSERIO_RELEASE_ROOT=${ZSERIO_CPP17_BUILD_DIR}/extension/download"
                      "-DZSERIO_CPP17_RELEASE_ROOT=${ZSERIO_CPP17_DISTR_DIR}"
                      "-DZSERIO_TEST_SUITES=${TEST_SUITES_LIST}")
    local CTEST_ARGS=()
    if [[ ${SWITCH_CLEAN} == 1 ]] ; then
        local CPP_TARGET="clean"
    else
        local CPP_TARGET="all"
    fi
    local TEST_SRC_DIR="${ZSERIO_CPP17_PROJECT_ROOT}/test"
    compile_cpp "${ZSERIO_CPP17_PROJECT_ROOT}" "${TEST_OUT_DIR}" "${TEST_SRC_DIR}" CPP_TARGETS[@] \
                CMAKE_ARGS[@] CTEST_ARGS[@] ${CPP_TARGET}
    if [ $? -ne 0 ] ; then
        stderr_echo "${MESSAGE} failed!"
        return 1
    fi

    if [[ ${SWITCH_CLEAN} != 1 ]] ; then
        for TARGET in "${CPP_TARGETS[@]}"; do
            local BUILD_TYPE="release"
            if [[ "${CMAKE_EXTRA_ARGS}" == *-DCMAKE_BUILD_TYPE=?ebug* ]] ; then
                BUILD_TYPE="debug"
            fi

            compare_test_data "${TEST_SRC_DIR}" "${TEST_OUT_DIR}/${TARGET}/${BUILD_TYPE}" TEST_SUITES[@] \
                    "C++ ${TARGET}"
            if [ $? -ne 0 ] ; then
                stderr_echo "${MESSAGE} failed!"
                return 1
            fi
        done
    fi

    echo -e "FINISHED - ${MESSAGE}\n"

    return 0
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Runs integration tests using C++17 extension from distr directory.

Usage:
    $0 [-h] [-e] [-c] [-p] [-o <dir>] [-i <pattern>]... [-x <pattern>]... target...

Arguments:
    -h, --help           Show this help.
    -e, --help-env       Show help for enviroment variables.
    -c, --clean          Clean package instead of build.
    -p, --purge          Purge test build directory.
    -o <dir>, --output-directory <dir>
                         Output directory where tests will be run.
    -i <pattern>, --include <pattern>
                         Include internal tests matching the specified pattern. Can be specified multiple times.
    -x <pattern>, --exclude <pattern>
                         Exclude internal tests matching the specified pattern. Can be specified multiple times.
    target               Specify the target to test.

Target can be a combination of:
    cpp-linux32-gcc      Tests for linux32 target using gcc compiler.
    cpp-linux64-gcc      Tests for linux64 target using gcc compiler.
    cpp-linux32-clang    Tests for linux32 target using using Clang compiler.
    cpp-linux64-clang    Tests for linux64 target using Clang compiler.
    cpp-windows64-ming   Tests for windows64 target (MinGW64).
    cpp-windows64-msvc   Tests for windows64 target (MSVC).

Examples:
    $0 cpp-linux64-gcc

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
    exit_if_argc_lt $# 5
    local PARAM_CPP_TARGET_ARRAY_OUT="$1"; shift
    local SWITCH_OUT_DIR_OUT="$1"; shift
    local SWITCH_CLEAN_OUT="$1"; shift
    local SWITCH_PURGE_OUT="$1"; shift
    local SWITCH_TEST_SUITES_ARRAY_OUT="$1"; shift

    eval ${SWITCH_CLEAN_OUT}=0
    eval ${SWITCH_PURGE_OUT}=0

    local NUM_PARAMS=0
    local NUM_PATTERNS=0
    local PARAM_ARRAY=()
    local ARG="$1"
    while [ $# -ne 0 ] ; do
        case "${ARG}" in
            "-h" | "--help")
                return 2
                ;;

            "-e" | "--help-env")
                return 3
                ;;

            "-c" | "--clean")
                eval ${SWITCH_CLEAN_OUT}=1
                shift
                ;;

            "-p" | "--purge")
                eval ${SWITCH_PURGE_OUT}=1
                shift
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

            "-i" | "--include")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing test include pattern!"
                    echo
                    return 1
                fi
                eval ${SWITCH_TEST_SUITES_ARRAY_OUT}[${NUM_PATTERNS}]="i:$2"
                NUM_PATTERNS=$((NUM_PATTERNS + 1))
                shift 2
                ;;

            "-x" | "--exclude")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing test exclude pattern!"
                    echo
                    return 1
                fi
                eval ${SWITCH_TEST_SUITES_ARRAY_OUT}[${NUM_PATTERNS}]="x:$2"
                NUM_PATTERNS=$((NUM_PATTERNS + 1))
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

    if [[ ${NUM_CPP_TARGETS} -eq 0 &&
          ${!SWITCH_PURGE_OUT} == 0 ]] ; then
        stderr_echo "Package to test is not specified!"
        echo
        return 1
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
    local SWITCH_CLEAN
    local SWITCH_PURGE
    local SWITCH_TEST_PATTERN_ARRAY=()
    # note that "$@" must have qoutes to prevent expansion of include/exclude patterns
    parse_arguments PARAM_CPP_TARGET_ARRAY SWITCH_OUT_DIR SWITCH_CLEAN SWITCH_PURGE SWITCH_TEST_PATTERN_ARRAY \
            "$@"
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

    echo "C++17 Extension Integration Tests"
    echo

    # set global variables
    set_global_common_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    if [[ ${#PARAM_CPP_TARGET_ARRAY[@]} -ne 0 ]] ; then
        set_global_cpp_variables
        if [ $? -ne 0 ] ; then
            return 1
        fi
    fi

    # cmake needs absolute paths
    convert_to_absolute_path "${SWITCH_OUT_DIR}" SWITCH_OUT_DIR

    # purge if requested and then create test output directory
    local ZSERIO_CPP17_BUILD_DIR="${SWITCH_OUT_DIR}/build"
    local TEST_OUT_DIR="${ZSERIO_CPP17_BUILD_DIR}/test"
    if [[ ${SWITCH_PURGE} == 1 ]] ; then
        echo "Purging test directory."
        echo
        rm -rf "${TEST_OUT_DIR}/"

        if [[ ${#PARAM_CPP_TARGET_ARRAY[@]} == 0 ]] ; then
            return 0 # purge only
        fi
    fi
    mkdir -p "${TEST_OUT_DIR}"

    # get test suites to run
    local TEST_SUITES=()
    get_test_suites "${ZSERIO_CPP17_PROJECT_ROOT}" SWITCH_TEST_PATTERN_ARRAY[@] TEST_SUITES
    if [ $? -ne 0 ] ; then
        return 1
    fi
    if [ ${#TEST_SUITES[@]} -eq 0 ] ; then
        echo "No test suites found."
        return 0
    fi

    # run test
    local ZSERIO_CPP17_DISTR_DIR="${SWITCH_OUT_DIR}/distr"
    test_cpp "${ZSERIO_CPP17_DISTR_DIR}" "${ZSERIO_CPP17_PROJECT_ROOT}" "${ZSERIO_CPP17_BUILD_DIR}" \
        "${TEST_OUT_DIR}" PARAM_CPP_TARGET_ARRAY[@] ${SWITCH_CLEAN} TEST_SUITES[@]
    if [ $? -ne 0 ] ; then
        return 1
    fi

    return 0
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]] ; then
    main "$@"
fi
