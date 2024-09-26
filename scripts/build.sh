#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"

# Print help message.
print_help()
{
    cat << EOF
Description:
    Builds Zserio into the distr directory.

Usage:
    $0 [-h] [-e] [-c] [-p] [-o <dir>] package...

Arguments:
    -h, --help               Show this help.
    -e, --help-env           Show help for enviroment variables.
    -c, --clean              Clean package instead of build.
    -p, --purge              Purge build and distr directories before build.
    -o <dir>, --output-directory <dir>
                             Output directory where build and distr will be located.
    package                  Specify the package to build or clean.

Package can be the combination of:
    cpp                      Zserio C++ extension.
    cpp_rt-linux32-gcc       Zserio C++ extension runtime library for native linux32 (gcc).
    cpp_rt-linux64-gcc       Zserio C++ extension runtime library for native linux64 (gcc).
    cpp_rt-linux32-clang     Zserio ASIL C++ extension runtime library for native linux32 (Clang).
    cpp_rt-linux64-clang     Zserio ASIL C++ extension runtime library for native linux64 (Clang).
    cpp_rt-windows64-mingw   Zserio C++ extension runtime library for windows64 target (MinGW64).
    cpp_rt-windows64-msvc    Zserio C++ extension runtime library for windows64 target (MSVC).

Examples:
    $0 cpp cpp_rt-linux64-gcc

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
    local NUM_OF_ARGS=6
    exit_if_argc_lt $# ${NUM_OF_ARGS}
    local PARAM_CPP_OUT="$1"; shift
    local PARAM_CPP_TARGET_ARRAY_OUT="$1"; shift
    local PARAM_ZSERIO_OUT="$1"; shift
    local PARAM_OUT_DIR_OUT="$1"; shift
    local SWITCH_CLEAN_OUT="$1"; shift
    local SWITCH_PURGE_OUT="$1"; shift

    eval ${PARAM_CPP_OUT}=0
    eval ${PARAM_ZSERIO_OUT}=0
    eval ${SWITCH_CLEAN_OUT}=0
    eval ${SWITCH_PURGE_OUT}=0

    local NUM_PARAMS=0
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
                eval ${PARAM_OUT_DIR_OUT}="$2"
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
            "cpp")
                eval ${PARAM_CPP_OUT}=1
                ;;

            "cpp_rt-linux32-"* | "cpp_rt-linux64-"* | "cpp_rt-windows64-"*)
                eval ${PARAM_CPP_TARGET_ARRAY_OUT}[${NUM_CPP_TARGETS}]="${PARAM#cpp_rt-}"
                NUM_CPP_TARGETS=$((NUM_CPP_TARGETS + 1))
                ;;

            "zserio")
                eval ${PARAM_ZSERIO_OUT}=1
                ;;

            *)
                stderr_echo "Invalid argument '${PARAM}'!"
                echo
                return 1
        esac
    done

    if [[ ${!PARAM_CPP_OUT} == 0 &&
          ${NUM_CPP_TARGETS} == 0 &&
          ${PARAM_ZSERIO_OUT} == 0 &&
          ${!SWITCH_PURGE_OUT} == 0 ]] ; then
        stderr_echo "Package to build is not specified!"
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
    local PARAM_CPP
    local PARAM_CPP_TARGET_ARRAY=()
    local PARAM_ZSERIO
    local PARAM_OUT_DIR="${ZSERIO_CPP17_PROJECT_ROOT}"
    local SWITCH_CLEAN
    local SWITCH_PURGE
    parse_arguments PARAM_CPP PARAM_CPP_TARGET_ARRAY PARAM_ZSERIO \
                    PARAM_OUT_DIR SWITCH_CLEAN SWITCH_PURGE "$@"
    local PARSE_RESULT=$?
    if [ ${PARSE_RESULT} -eq 2 ] ; then
        print_help
        return 0
    elif [ ${PARSE_RESULT} -eq 3 ] ; then
        print_help_env
        return 0
    elif [ ${PARSE_RESULT} -ne 0 ] ; then
        return 1
    fi

    # set global variables if needed
    set_global_common_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    if [[ ${PARAM_CPP} == 1 ||
          ${PARAM_ZSERIO} == 1 ]] ; then
        set_global_java_variables
        if [ $? -ne 0 ] ; then
            return 1
        fi
    fi

    if [[ ${#PARAM_CPP_TARGET_ARRAY[@]} -ne 0 ]] ; then
        set_global_cpp_variables
        if [ $? -ne 0 ] ; then
            return 1
        fi
    fi

    # purge if requested and then create build and distr directories
    local ZSERIO_CPP17_BUILD_DIR="${PARAM_OUT_DIR}/build"
    local ZSERIO_CPP17_DISTR_DIR="${PARAM_OUT_DIR}/distr"
    if [[ ${SWITCH_PURGE} == 1 ]] ; then
        echo "Purging build and distr directories."
        echo
        rm -rf "${ZSERIO_CPP17_BUILD_DIR}/"
        rm -rf "${ZSERIO_CPP17_DISTR_DIR}/"
    fi
    mkdir -p "${ZSERIO_CPP17_BUILD_DIR}"
    mkdir -p "${ZSERIO_CPP17_DISTR_DIR}"

    # extensions need absolute paths
    convert_to_absolute_path "${ZSERIO_CPP17_BUILD_DIR}" ZSERIO_CPP17_BUILD_DIR
    convert_to_absolute_path "${ZSERIO_CPP17_DISTR_DIR}" ZSERIO_CPP17_DISTR_DIR

    # get action name and description
    if [[ ${SWITCH_CLEAN} == 1 ]] ; then
        local CPP_TARGET="clean"
        local JAVA_TARGET="clean"
        local ACTION_DESCRIPTION="Cleaning"
    else
        local CPP_TARGET="install"
        local JAVA_TARGET="test"
        local ACTION_DESCRIPTION="Building"
    fi

    local ZSERIO_CPP17_ANT_PROPS=(
            -Dzserio_cpp17.build_dir="${ZSERIO_CPP17_BUILD_DIR}/extension"
            -Dzserio_cpp17.install_dir="${ZSERIO_CPP17_DISTR_DIR}"
    )

    # build Zserio C++ extension
    if [[ ${PARAM_CPP} == 1 ]] ; then
        echo "${ACTION_DESCRIPTION} Zserio C++ extension."
        echo
        compile_java "${ZSERIO_CPP17_PROJECT_ROOT}/extension/build.xml" ZSERIO_CPP17_ANT_PROPS[@] ${JAVA_TARGET}
        if [ $? -ne 0 ] ; then
            return 1
        fi
        echo
    fi

    # build Zserio C++ runtime library
    if [ ${#PARAM_CPP_TARGET_ARRAY[@]} -ne 0 ] ; then
        echo "${ACTION_DESCRIPTION} Zserio C++ runtime library."
        echo
        local CMAKELISTS_DIR="${ZSERIO_CPP17_PROJECT_ROOT}/runtime"
        local CPP_BUILD_DIR="${ZSERIO_CPP17_BUILD_DIR}/runtime_lib"
        local CMAKE_ARGS=(-DCMAKE_INSTALL_PREFIX="${ZSERIO_CPP17_DISTR_DIR}/runtime_lib")
        local IS_COVERAGE_ENABLED=0
        if [[ ! -z "${GCOVR_BIN}" && "${PARAM_CPP_TARGET_ARRAY[@]}" == *"gcc"* ]] ; then
            local IS_COVERAGE_ENABLED=1
            CMAKE_ARGS+=(
                    -DGCOVR_BIN="${GCOVR_BIN}"
            )
        fi
        if [[ ! -z "${LLVM_PROFDATA_BIN}" && ! -z "${LLVM_COV_BIN}" &&
                "${PARAM_CPP_TARGET_ARRAY[@]}" == *"clang"* ]] ; then
            local IS_COVERAGE_ENABLED=1
            CMAKE_ARGS+=(
                    -DLLVM_PROFDATA_BIN="${LLVM_PROFDATA_BIN}"
                    -DLLVM_COV_BIN="${LLVM_COV_BIN}"
            )
        fi
        if [ ${IS_COVERAGE_ENABLED} -eq 1 ] ; then
            CMAKE_ARGS+=(-DZSERIO_CODE_COVERAGE_ENABLE=ON)
        else
            CMAKE_ARGS+=(-DZSERIO_CODE_COVERAGE_ENABLE=OFF)
        fi
        local CTEST_ARGS=()
        compile_cpp "${ZSERIO_CPP17_PROJECT_ROOT}" "${CPP_BUILD_DIR}" "${CMAKELISTS_DIR}" \
                PARAM_CPP_TARGET_ARRAY[@] CMAKE_ARGS[@] CTEST_ARGS[@] ${CPP_TARGET}
        if [ $? -ne 0 ] ; then
            return 1
        fi
        echo
    fi

    # bundle Zserio with C++17 extension
    if [[ ${PARAM_ZSERIO} == 1 ]] ; then
        echo "${ACTION_DESCRIPTION} Zserio bundle."
        echo
        compile_java "${ZSERIO_CPP17_PROJECT_ROOT}/extension/build.xml" ZSERIO_CPP17_ANT_PROPS[@] \
                zserio_bundle.${JAVA_TARGET}
        if [ $? -ne 0 ] ; then
            return 1
        fi
        echo
    fi

    return 0
}

# call main function
main "$@"
