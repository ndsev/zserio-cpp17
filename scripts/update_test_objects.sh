#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"

# Set and check global variables.
set_test_objects_global_variables()
{
    # GIT to use, defaults to "git" if not set
    GIT="${GIT:-git}"
    if [ ! -f "`which "${GIT}"`" ] ; then
        stderr_echo "Cannot find git! Set GIT environment variable."
        return 1
    fi

    return 0
}

# Print help on the environment variables used for this script.
print_test_objects_help_env()
{
    cat << EOF
Uses the following environment variables:
    GIT      Git executable to use. Default is "git".

    Either set these directly, or create 'scripts/build-env.sh' that sets these.
    It's sourced automatically if it exists.

EOF
}

# Run zserio tool with specified sources and arguments
run_zserio_tool()
{
    exit_if_argc_ne $# 6
    local ZSERIO_RELEASE_ROOT="$1"; shift
    local BUILD_DIR="$1"; shift # for logging
    local ZSERIO_SOURCE_DIRECTORY="$1"; shift
    local ZSERIO_SOURCE="$1"; shift
    local SWITCH_WERROR="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local ZSERIO_ARGS=("${MSYS_WORKAROUND_TEMP[@]}")

    local ZSERIO="${ZSERIO_RELEASE_ROOT}/zserio.jar"
    local ZSERIO_LOG="${BUILD_DIR}/zserio_log.txt"
    local MESSAGE="Compilation of zserio '${ZSERIO_SOURCE_DIRECTORY}/${ZSERIO_SOURCE}'"
    echo "STARTING - ${MESSAGE}"

    # build directory must be created in advance because tee will fail to create a new zserio log file
    mkdir -p "${BUILD_DIR}"
    "${JAVA_BIN}" -jar "${ZSERIO_JAR}" "-src" "${ZSERIO_SOURCE_DIRECTORY}" "${ZSERIO_SOURCE}" \
            "${ZSERIO_ARGS[@]}" 2>&1 | tee ${ZSERIO_LOG}

    if [ ${PIPESTATUS[0]} -ne 0 ] ; then
        stderr_echo "${MESSAGE} failed!"
        return 1
    fi

    if [ ${SWITCH_WERROR} -ne 0 ] ; then
        grep -q "\[WARNING\]" ${ZSERIO_LOG}
        if [ $? -eq 0 ] ; then
            stderr_echo "${MESSAGE} failed because warnings are treated as errors! "
            return 1
        fi
    fi

    echo -e "FINISHED - ${MESSAGE}\n"

    return 0
}

# Update C++ test objects.
update_cpp_test_objects()
{
    exit_if_argc_ne $# 4
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift
    local SWITCH_PURGE="$1"; shift
    local ZSERIO_JAR="$1"; shift
    local UPDATE_OUT_DIR="$1"; shift

    local CPP17_RUNTIME_TEST_DIR="${ZSERIO_CPP17_PROJECT_ROOT}/runtime/test"
    local CPP17_RUNTIME_TEST_OBJECT_DIR="${CPP17_RUNTIME_TEST_DIR}/test_object"
    if [[ ${SWITCH_PURGE} == 1 ]] ; then
        echo "Purging test objects."
        echo
        rm -rf "${CPP17_RUNTIME_TEST_OBJECT_DIR}/pmr_allocator"
        rm -rf "${CPP17_RUNTIME_TEST_OBJECT_DIR}/ppmr_allocator"
        rm -rf "${CPP17_RUNTIME_TEST_OBJECT_DIR}/std_allocator"
    fi
    local ZSERIO_ARGS_BASE=("-cpp17" "${CPP17_RUNTIME_TEST_DIR}"
            "-withTypeInfoCode -withoutSourcesAmalgamation")
    local SWITCH_WERROR=1

    local ZSERIO_ARGS_STD=(${ZSERIO_ARGS_BASE[@]} "-setCppAllocator" "std"
            "-setTopLevelPackage" "test_object.std_allocator")
    run_zserio_tool "${UNPACKED_ZSERIO_RELEASE_DIR}" "${UPDATE_OUT_DIR}/cpp/std" \
            "${CPP17_RUNTIME_TEST_OBJECT_DIR}" "test_object.zs" ${SWITCH_WERROR} ZSERIO_ARGS_STD[@]
    if [ $? -ne 0 ] ; then
        stderr_echo "Zserio tool failed!"
        return 1
    fi

    local ZSERIO_ARGS_PMR=(${ZSERIO_ARGS_BASE[@]} "-setCppAllocator" "pmr"
            "-setTopLevelPackage" "test_object.pmr_allocator")
    run_zserio_tool "${UNPACKED_ZSERIO_RELEASE_DIR}" "${UPDATE_OUT_DIR}/cpp/pmr" \
            "${CPP17_RUNTIME_TEST_OBJECT_DIR}" "test_object.zs" ${SWITCH_WERROR} \
            ZSERIO_ARGS_PMR[@]
    if [ $? -ne 0 ] ; then
        stderr_echo "Zserio tool failed!"
        return 1
    fi

    local ZSERIO_ARGS_PPMR=(${ZSERIO_ARGS_BASE[@]} "-setCppAllocator" "ppmr"
            "-setTopLevelPackage" "test_object.ppmr_allocator")
    run_zserio_tool "${UNPACKED_ZSERIO_RELEASE_DIR}" "${UPDATE_OUT_DIR}/cpp/pmr" \
            "${CPP17_RUNTIME_TEST_OBJECT_DIR}" "test_object.zs" ${SWITCH_WERROR} \
            ZSERIO_ARGS_PPMR[@]
    if [ $? -ne 0 ] ; then
        stderr_echo "Zserio tool failed!"
        return 1
    fi

    # remove runtime version check from generated headers
    echo "Removing runtime version check from generated test objects."
    for GENERATED_HEADER in "${CPP17_RUNTIME_TEST_OBJECT_DIR}/"**/*.h ; do
        sed -i '/#include <zserio\/CppRuntimeVersion.h>/,+5d' "${GENERATED_HEADER}"
        if [ $? -ne 0 ] ; then
            stderr_echo "Sed tool failed!"
            return 1
        fi
    done
    echo

    return 0
}

# Commit all updated files into the Git repository.
commit_updated_files()
{
    exit_if_argc_ne $# 1
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift

    local TEST_OBJECTS_DIRS="${ZSERIO_CPP17_PROJECT_ROOT}/runtime/test/test_object"
    "${GIT}" diff --exit-code ${TEST_OBJECTS_DIRS} > /dev/null
    if [ $? -ne 0 ] ; then
        echo "STARTING - Committing all updated generated files"
        "${GIT}" commit -m "Update all generated sources in the runtime test" ${TEST_OBJECTS_DIRS}
        local GIT_RESULT=$?
        if [ ${GIT_RESULT} -ne 0 ] ; then
            stderr_echo "Git failed with return code ${GIT_RESULT}!"
            return 1
        fi
        echo "FINISHED - Committing all updated generated files"
    fi

    return 0
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Update test objects in the runtime library.

Usage:
    $0 [-h] [-e] [-p] [-m] [-o <dir>]

Arguments:
    -h, --help      Show this help.
    -e, --help-env  Show help for enviroment variables.
    -p, --purge     Purge test objects together with output directory before update.
    -m, --commit    Commit updated version into the Git repository.
    -o <dir>, --output-directory <dir>
                    Output directory for updating.

Examples:
    $0
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
    exit_if_argc_lt $# 3
    local PARAM_OUT_DIR_OUT="$1"; shift
    local SWITCH_PURGE_OUT="$1"; shift
    local SWITCH_COMMIT_OUT="$1"; shift

    eval ${SWITCH_PURGE_OUT}=0
    eval ${SWITCH_COMMIT_OUT}=0

    local ARG="$1"
    while [ $# -ne 0 ] ; do
        case "${ARG}" in
            "-h" | "--help")
                return 2
                ;;

            "-e" | "--help-env")
                return 3
                ;;

            "-p" | "--purge")
                eval ${SWITCH_PURGE_OUT}=1
                shift
                ;;

            "-m" | "--commit")
                eval ${SWITCH_COMMIT_OUT}=1
                shift
                ;;

            "-o" | "--output-directory")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing output directory!"
                    echo
                    return 1
                fi
                eval ${PARAM_OUT_DIR_OUT}="$2"
                shift 2
                ;;

            "-"*)
                stderr_echo "Invalid switch '${ARG}'!"
                echo
                return 1
                ;;

            *)
                stderr_echo "Invalid argument '${ARG}'!"
                echo
                return 1
                shift
                ;;
        esac
        ARG="$1"
    done

    return 0
}

main()
{
    # get the project root, absolute path is necessary only for CMake
    local ZSERIO_CPP17_PROJECT_ROOT
    convert_to_absolute_path "${SCRIPT_DIR}/.." ZSERIO_CPP17_PROJECT_ROOT

    # parse command line arguments
    local PARAM_OUT_DIR="${ZSERIO_CPP17_PROJECT_ROOT}"
    local SWITCH_PURGE
    local SWITCH_COMMIT
    parse_arguments PARAM_OUT_DIR SWITCH_PURGE SWITCH_COMMIT "$@"
    local PARSE_RESULT=$?
    if [ ${PARSE_RESULT} -eq 2 ] ; then
        print_help
        return 0
    elif [ ${PARSE_RESULT} -eq 3 ] ; then
        print_help_env
        print_test_objects_help_env
        return 0
    elif [ ${PARSE_RESULT} -ne 0 ] ; then
        return 1
    fi

    echo "Updating test objects in runtime library."
    echo

    # set global variables
    set_global_common_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    set_global_java_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    set_test_objects_global_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    set_global_cpp_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    # extension need absolute paths
    convert_to_absolute_path "${PARAM_OUT_DIR}" PARAM_OUT_DIR

    # purge if requested and then create output directory
    local UPDATE_OUT_DIR="${PARAM_OUT_DIR}/build/update_objects"
    if [[ ${SWITCH_PURGE} == 1 ]] ; then
        echo "Purging output directory."
        echo
        rm -rf "${UPDATE_OUT_DIR}/"
    fi
    mkdir -p "${UPDATE_OUT_DIR}"

    # print information
    local ZSERIO_JAR="${PARAM_OUT_DIR}/distr/zserio.jar"
    echo "Used Zserio jar: ${ZSERIO_JAR}"
    echo "Output directory: ${UPDATE_OUT_DIR}"
    echo

    # update C++ test objects
    local MESSAGE="Update of C++ test objects in runtime library"
    echo "STARTING - ${MESSAGE}"
    echo
    update_cpp_test_objects "${ZSERIO_CPP17_PROJECT_ROOT}" ${SWITCH_PURGE} "${ZSERIO_JAR}" "${UPDATE_OUT_DIR}"
    echo "FINISHED - ${MESSAGE}"
    echo

    # commit all updated files
    if [[ ${SWITCH_COMMIT} != 0 ]] ; then
        commit_updated_files "${ZSERIO_CPP17_PROJECT_ROOT}"
        if [ $? -ne 0 ] ; then
            return 1
        fi
    fi

    return 0
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]] ; then
    main "$@"
fi
