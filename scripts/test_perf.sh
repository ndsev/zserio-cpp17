#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"
source "${SCRIPT_DIR}/test_zs.sh"

# Generate C++ files
generate_performance_test()
{
    exit_if_argc_ne $# 8
    local SRC_FILE="$1"; shift
    local BLOB_FULL_NAME="$1"; shift
    local JSON_FILE="$1"; shift
    local BLOB_FILE="$1"; shift
    local LOG_FILE="$1"; shift
    local NUM_ITERATIONS="$1"; shift
    local TEST_CONFIG="$1"; shift
    local PROFILE="$1"; shift

    # use host paths in generated files
    local DISABLE_SLASHES_CONVERSION=1
    posix_to_host_path "${JSON_FILE}" HOST_JSON_FILE ${DISABLE_SLASHES_CONVERSION}
    posix_to_host_path "${BLOB_FILE}" HOST_BLOB_FILE ${DISABLE_SLASHES_CONVERSION}

    local BLOB_INCLUDE_PATH=${BLOB_FULL_NAME//.//}.h
    local BLOB_CLASS_FULL_NAME=${BLOB_FULL_NAME//./::}
    local TOP_LEVEL_PACKAGE_NAME=${BLOB_FULL_NAME%%.*}
    if [[ "${BLOB_FILE}" != "" ]] ; then
        local INPUT_SWITCH="false"
        local INPUT_FILE="${BLOB_FILE}"
    else
        local INPUT_SWITCH="true"
        local INPUT_FILE="${JSON_FILE}"
    fi

    cat > "${SRC_FILE}" << EOF
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/SerializeUtil.h>

#include <${BLOB_INCLUDE_PATH}>

EOF

if [[ ${PROFILE} == 1 ]] ; then
    cat >> "${SRC_FILE}" << EOF
#include <valgrind/callgrind.h>

EOF
fi

cat >> "${SRC_FILE}" << EOF
#if defined(_WIN32) || defined(_WIN64)
#   include <windows.h>
#else
#   include <time.h>
#endif

class PerfTimer
{
public:
    static uint64_t getMicroTime()
    {
#if defined(_WIN32) || defined(_WIN64)
        FILETIME creation, exit, kernelTime, userTime;
        GetThreadTimes(GetCurrentThread(), &creation, &exit, &kernelTime, &userTime);
        return fileTimeToMicro(kernelTime) + fileTimeToMicro(userTime);
#else
        struct timespec ts;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
        return static_cast<uint64_t>(ts.tv_sec) * 1000000 + static_cast<uint64_t>(ts.tv_nsec) / 1000;
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    static uint64_t fileTimeToMicro(const FILETIME& time)
    {
        uint64_t value = time.dwHighDateTime;
        value <<= 8 * sizeof(time.dwHighDateTime);
        value |= static_cast<uint64_t>(time.dwLowDateTime);
        value /= 10;

        return value;
    }
#endif
};
EOF

if [[ "${ZSERIO_EXTRA_ARGS}" == *"polymorphic"* ]] ; then
    cat >> "${SRC_FILE}" << EOF

class TrackerMemoryResource : public zserio::pmr::MemoryResource
{
public:
    size_t getAllocatedSize() const
    {
        return allocatedSize;
    }

    size_t getDeallocatedSize() const
    {
        return deallocatedSize;
    }

private:
    void* doAllocate(size_t bytes, size_t) override
    {
        allocatedSize += bytes;
        return ::operator new(bytes);
    }

    void doDeallocate(void* p, size_t bytes, size_t) override
    {
        deallocatedSize += bytes;
        ::operator delete(p);
    }

    bool doIsEqual(const MemoryResource& other) const noexcept override
    {
        return this == &other;
    }

    size_t allocatedSize = 0;
    size_t deallocatedSize = 0;
};
EOF
fi

    cat >> "${SRC_FILE}" << EOF

using AllocatorType = ${BLOB_CLASS_FULL_NAME}::AllocatorType;
using BitBuffer = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;

static BitBuffer readBlobBuffer(bool inputIsJson, const char* inputPath)
{
    if (inputIsJson)
    {
        // TODO[mikir]: JSON not implemented
        // auto blob = zserio::fromJsonFile<${BLOB_CLASS_FULL_NAME}>(inputPath);

        // serialize to binary file for further analysis
        // zserio::serializeToFile(blob, "${TOP_LEVEL_PACKAGE_NAME}.blob");

        // return zserio::serialize<${BLOB_CLASS_FULL_NAME}, AllocatorType>(blob);
        return BitBuffer();
    }
    else
    {
        // read blob file
        std::ifstream is(inputPath, std::ifstream::binary);
        if (!is)
            throw zserio::CppRuntimeException("Cannot open '") << inputPath << "' for reading!";
        is.seekg(0, is.end);
        const size_t blobByteSize = static_cast<size_t>(is.tellg());
        is.close();

        ${BLOB_CLASS_FULL_NAME} objectData;
        auto objectView = zserio::deserializeFromFile(inputPath, objectData);
        auto bitBuffer = zserio::serialize(objectView);
        if (bitBuffer.getByteSize() != blobByteSize)
        {
            throw zserio::CppRuntimeException("Read only ") << bitBuffer.getByteSize()
                    << "/" << blobByteSize << " bytes!";
        }

        return bitBuffer;
    }
}

int main(int argc, char* argv[])
{
    std::cout << "C++17 Extension Performance Test" << std::endl;

    const char* logPath = "${LOG_FILE}";
    bool inputIsJson = ${INPUT_SWITCH};
    const char* inputPath = "${INPUT_FILE}";
    int numIterations = ${NUM_ITERATIONS};
    if ((argc > 1 && argc < 4) || (argc == 2 && strcmp(argv[1], "-h") == 0))
    {
        std::cerr << "Usage: LOG_PATH (-j|-b) INPUT_PATH [NUM_ITERATIONS]" << std::endl;
        return 1;
    }

    if (argc > 1)
        logPath = argv[1];
    if (argc > 2)
        inputIsJson = strcmp("-j", argv[2]) == 0 ? true : false;
    if (argc > 3)
        inputPath = argv[3];
    if (argc > 4)
        numIterations = atoi(argv[4]);

    if (numIterations <= 0)
    {
        std::cerr << "Num iterations must be a positive integer (" << numIterations << ")!" << std::endl;
        return 1;
    }

    // read blob buffer
    BitBuffer bitBuffer;
    try
    {
        bitBuffer = readBlobBuffer(inputIsJson, inputPath);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
EOF

if [[ "${ZSERIO_EXTRA_ARGS}" == *"polymorphic"* ]]; then
    cat >> "${SRC_FILE}" << EOF

    // calculate blob memory size
    TrackerMemoryResource memoryResource;
    const AllocatorType allocator(&memoryResource);
    zserio::BitStreamReader blobReader(bitBuffer);
    std::unique_ptr<${BLOB_CLASS_FULL_NAME}> memoryBlob(new ${BLOB_CLASS_FULL_NAME}>(blobReader, allocator));
    const size_t blobMemorySize = memoryResource.getAllocatedSize();
    const size_t blobDeallocMemorySize = memoryResource.getDeallocatedSize();
    if (blobDeallocMemorySize != 0)
    {
        std::cerr << "Memory deallocation during blob parsing occurred (" << blobDeallocMemorySize << ")!"
                << std::endl;
        return 1;
    }

EOF
fi

    cat >> "${SRC_FILE}" << EOF
    // run the test
EOF

if [[ "${TEST_CONFIG}" != "WRITE" ]] ; then
    cat >> "${SRC_FILE}" << EOF
    std::vector<${BLOB_CLASS_FULL_NAME}> readData;
    readData.resize(static_cast<size_t>(numIterations));
EOF
else
    cat >> "${SRC_FILE}" << EOF
    ${BLOB_CLASS_FULL_NAME} readData;
    auto readView = zserio::deserialize(bitBuffer, readData);
EOF
fi

if [[ ${PROFILE} == 1 ]] ; then
    cat >> "${SRC_FILE}" << EOF

    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;

EOF
fi

cat >> "${SRC_FILE}" << EOF
    const uint64_t start = PerfTimer::getMicroTime();
    for (size_t i = 0; i < static_cast<size_t>(numIterations); ++i)
    {
EOF

    case "${TEST_CONFIG}" in
        "READ")
            cat >> "${SRC_FILE}" << EOF
        zserio::deserialize(bitBuffer, readData[i]);
EOF
            ;;
        "READ_WRITE")
            cat >> "${SRC_FILE}" << EOF
        auto readView = zserio::deserialize(bitBuffer, readData[i]);
        zserio::serialize(readView);
EOF
            ;;

        "WRITE")
            cat >> "${SRC_FILE}" << EOF
        zserio::serialize(readView);
EOF
            ;;
    esac

cat >> "${SRC_FILE}" << EOF
    }
    const uint64_t stop = PerfTimer::getMicroTime();

EOF

if [[ ${PROFILE} == 1 ]] ; then
    cat >> "${SRC_FILE}" << EOF
    CALLGRIND_STOP_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT;

EOF
fi

cat >> "${SRC_FILE}" << EOF
    // process results
    double totalDuration = static_cast<double>(stop - start) / 1000.;
    double stepDuration = totalDuration / numIterations;
    double blobkBSize = static_cast<double>(bitBuffer.getByteSize()) / 1000.;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Total Duration: " << totalDuration << "ms" << std::endl;
    std::cout << "Iterations:     " << numIterations << std::endl;
    std::cout << "Step Duration:  " << stepDuration << "ms" << std::endl;
    std::cout << "Blob Size:      " << bitBuffer.getBitSize() << " bits" << "(" << blobkBSize << " kB)"
              << std::endl;
EOF

if [[ "${ZSERIO_EXTRA_ARGS}" == *"polymorphic"* ]]; then
    cat >> "${SRC_FILE}" << EOF
    double blobMemorykBSize = static_cast<double>(blobMemorySize) / 1000.;
    std::cout << "Blob in Memory: " << blobMemorySize << " bytes" << "(" << blobMemorykBSize << " kB)"
              << std::endl;
EOF
fi

cat >> "${SRC_FILE}" << EOF

    // write results to file
    std::ofstream logFile(logPath);
    logFile << std::fixed << std::setprecision(3);
    logFile << totalDuration << "ms "
            << numIterations << " "
            << stepDuration << "ms "
            << blobkBSize << "kB "
EOF

if [[ "${ZSERIO_EXTRA_ARGS}" == *"polymorphic"* ]]; then
    cat >> "${SRC_FILE}" << EOF
            << blobMemorykBSize << "kB"
EOF
fi

cat >> "${SRC_FILE}" << EOF
            << std::endl;

    return 0;
}
EOF
}

# Run zserio performance tests.
test_perf()
{
    exit_if_argc_ne $# 15
    local ZSERIO_CPP17_DISTR_DIR="$1"; shift
    local ZSERIO_CPP17_PROJECT_ROOT="$1"; shift
    local ZSERIO_CPP17_BUILD_DIR="$1"; shift
    local TEST_OUT_DIR="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local CPP_TARGETS=("${MSYS_WORKAROUND_TEMP[@]}")
    local SWITCH_DIRECTORY="$1"; shift
    local SWITCH_SOURCE="$1"; shift
    local SWITCH_TEST_NAME="$1"; shift
    local SWITCH_BLOB_NAME="$1"; shift
    local SWITCH_JSON_FILE="$1"; shift
    local SWITCH_BLOB_FILE="$1"; shift
    local SWITCH_NUM_ITERATIONS="$1"; shift
    local SWITCH_TEST_CONFIG="$1"; shift
    local SWITCH_RUN_ONLY="$1"; shift
    local SWITCH_PROFILE="$1"; shift

    # run C++ performance test
    local TEST_SRC_DIR="${TEST_OUT_DIR}/src"
    mkdir -p "${TEST_SRC_DIR}"
    local TEST_SRC_FILE="${TEST_SRC_DIR}/PerformanceTest.cpp"
    local TEST_LOG_DIR="${TEST_OUT_DIR}/log"
    mkdir -p "${TEST_LOG_DIR}"
    if [[ ${SWITCH_RUN_ONLY} == 0 ]] ; then
        generate_performance_test "${TEST_SRC_FILE}" "${SWITCH_BLOB_NAME}" \
                "${SWITCH_JSON_FILE}" "${SWITCH_BLOB_FILE}" "${TEST_LOG_DIR}/PerformanceTest.log" \
                ${SWITCH_NUM_ITERATIONS} ${SWITCH_TEST_CONFIG} ${SWITCH_PROFILE}
    fi
    local CMAKE_ARGS=()
    local CTEST_ARGS=("--verbose")
    if [[ ${SWITCH_PROFILE} == 1 ]] ; then
        CMAKE_ARGS=("-DCMAKE_BUILD_TYPE=RelWithDebInfo")
        CTEST_ARGS+=("-T memcheck")
    fi

    # run test
    test_zs "${ZSERIO_CPP17_DISTR_DIR}" "${ZSERIO_CPP17_PROJECT_ROOT}" "${ZSERIO_CPP17_BUILD_DIR}" \
        "${TEST_OUT_DIR}" PARAM_CPP_TARGET_ARRAY[@] "${SWITCH_SOURCE}" "${SWITCH_DIRECTORY}" \
        "${SWITCH_TEST_NAME}" "${TEST_SRC_FILE}"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    if [[ ${SWITCH_PROFILE} == 1 ]] ; then
        echo
        echo "C++ profiling finished, use one of the following commands for analysis:"
        for CPP_TARGET in "${CPP_TARGETS[@]}" ; do
            local CALLGRIND_FILE=$(${FIND} "${TEST_OUT_DIR}/cpp/${CPP_TARGET}" -name "callgrind.out")
            echo "    kcachegrind ${CALLGRIND_FILE}"
        done
    fi

    # collect results
    echo
    echo "Performance Tests Results - ${SWITCH_TEST_CONFIG}"
    echo "Blob name: ${SWITCH_BLOB_NAME}"
    if [[ "${SWITCH_JSON_FILE}" != "" ]] ; then
        echo "JSON file: ${SWITCH_JSON_FILE##*/}"
    else
        echo "Blob file: ${SWITCH_BLOB_FILE##*/}"
    fi
    for i in {1..103} ; do echo -n "=" ; done ; echo
    printf "| %-21s | %14s | %10s | %15s | %10s | %10s |\n" \
           "Generator" "Total Duration" "Iterations" "Step Duration" "Blob Size" "Blob in Memory"
    echo -n "|" ; for i in {1..101} ; do echo -n "-" ; done ; echo "|"
    for CPP_TARGET in "${CPP_TARGETS[@]}" ; do
        local PERF_TEST_FILE=$(${FIND} "${TEST_OUT_DIR}/cpp/${CPP_TARGET}" -name "PerformanceTest.log")
        local RESULTS=($(cat ${PERF_TEST_FILE}))
        printf "| %-21s | %14s | %10s | %15s | %10s | %14s |\n" \
               "C++ (${CPP_TARGET})" ${RESULTS[0]} ${RESULTS[1]} ${RESULTS[2]} ${RESULTS[3]} ${RESULTS[4]}
    done
    for i in {1..103} ; do echo -n "=" ; done ; echo
    echo

    return 0
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Runs performance tests on given zserio sources using C++17 extension from distr directory.

Usage:
    $0 [-h] [-e] [-p] [-r] [--profile] [-o <dir>] [-d <dir>] [-t <name>] -[n <num>] [-c <config>]
        target... -s <source> -b <blobname> [-f <blobfile> | -j <jsonfile>]

Arguments:
    -h, --help              Show this help.
    -e, --help-env          Show help for enviroment variables.
    -p, --purge             Purge test build directory.
    -r, --run-only          Run already compiled PerformanceTests again.
    --profile               Run the test in profiling mode and produce profiling data.
    -o <dir>, --output-directory <dir>
                            Output directory where tests will be run.
    -d <dir>, --source-dir <dir>
                            Directory with zserio sources. Default is ".".
    -t <name>, --test-name <name>
                            Test name. Optional.
    -n <num>, --num-iterations <num>
                            Number of iterations. Optional, default is 100.
    -c <config>, --test-config <config>
                            Test configuration: READ (default), WRITE, READ_WRITE.
    -s <source>, --source <source>
                            Main zserio source.
    -b <blobname>, --blob-name <blobname>
                            Full name of blob to run performance tests on.
    -f <blobfile>, --blob-file <blobfile>
                            Path to the blob file.
    -j <jsonfile>, --json-file <jsonfile>
                            Path to the JSON file.
    target                  Specify the target to test.

Generator can be:
    cpp-linux32-gcc         Tests for linux32 target using gcc compiler.
    cpp-linux64-gcc         Tests for linux64 target using gcc compiler.
    cpp-linux32-clang       Tests for linux32 target using using Clang compiler.
    cpp-linux64-clang       Tests for linux64 target using Clang compiler.
    cpp-windows64-ming      Tests for windows64 target (MinGW64).
    cpp-windows64-msvc      Tests for windows64 target (MSVC).

Examples:
    $0 cpp-linux64-gcc -d /tmp/zs -s test.zs -b test.Blob -f blob.bin

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
    exit_if_argc_lt $# 13
    local PARAM_CPP_TARGET_ARRAY_OUT="$1"; shift
    local SWITCH_OUT_DIR_OUT="$1"; shift
    local SWITCH_DIRECTORY_OUT="$1"; shift
    local SWITCH_SOURCE_OUT="$1"; shift
    local SWITCH_TEST_NAME_OUT="$1"; shift
    local SWITCH_BLOB_NAME_OUT="$1"; shift
    local SWITCH_JSON_FILE_OUT="$1"; shift
    local SWITCH_BLOB_FILE_OUT="$1"; shift
    local SWITCH_NUM_ITERATIONS_OUT="$1"; shift
    local SWITCH_TEST_CONFIG_OUT="$1"; shift
    local SWITCH_PURGE_OUT="$1"; shift
    local SWITCH_RUN_ONLY_OUT="$1"; shift
    local SWITCH_PROFILE_OUT="$1"; shift

    eval ${SWITCH_DIRECTORY_OUT}="."
    eval ${SWITCH_SOURCE_OUT}=""
    eval ${SWITCH_TEST_NAME_OUT}=""
    eval ${SWITCH_BLOB_NAME_OUT}=""
    eval ${SWITCH_JSON_FILE_OUT}=""
    eval ${SWITCH_BLOB_FILE_OUT}=""
    eval ${SWITCH_NUM_ITERATIONS_OUT}=100 # default
    eval ${SWITCH_TEST_CONFIG_OUT}="READ" # default
    eval ${SWITCH_PURGE_OUT}=0
    eval ${SWITCH_RUN_ONLY_OUT}=0
    eval ${SWITCH_PROFILE_OUT}=0

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

            "-b" | "--blob-name")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing blob name!"
                    echo
                    return 1
                fi
                eval ${SWITCH_BLOB_NAME_OUT}="$2"
                shift 2
                ;;

            "-j" | "--json-file")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing JSON file name!"
                    echo
                    return 1
                fi
                eval ${SWITCH_JSON_FILE_OUT}="$2"
                shift 2
                ;;

            "-f" | "--blob-file")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing BLOB file name!"
                    echo
                    return 1
                fi
                eval ${SWITCH_BLOB_FILE_OUT}="$2"
                shift 2
                ;;

            "-n" | "--num-iterations")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing number of iterations!"
                    echo
                    return 1
                fi
                eval ${SWITCH_NUM_ITERATIONS_OUT}="$2"
                shift 2
                ;;

            "-c" | "--test-config")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing test configuration!"
                    echo
                    return 1
                fi
                eval ${SWITCH_TEST_CONFIG_OUT}="$2"
                shift 2
                ;;

            "-r" | "--run-only")
                eval ${SWITCH_RUN_ONLY_OUT}=1
                shift
                ;;

            "--profile")
                eval ${SWITCH_PROFILE_OUT}=1
                shift
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

    # validate test configuration
    case "${!SWITCH_TEST_CONFIG_OUT}" in
        "READ" | "WRITE" | "READ_WRITE")
            ;;
        *)
            stderr_echo "Invalid test configuration, use one of READ, WRITE, READ_WRITE"
            return 1
            ;;
    esac

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

        if [[ "${!SWITCH_BLOB_NAME_OUT}" == "" ]] ; then
            stderr_echo "Blob name is not set!"
            echo
            return 1
        fi

        if [[ "${!SWITCH_BLOB_FILE_OUT}" == "" && "${!SWITCH_JSON_FILE_OUT}" == "" ]] ; then
            stderr_echo "Neither blob nor JSON filename is set!"
            echo
            return 1
        fi

        if [[ "${!SWITCH_BLOB_FILE_OUT}" != "" && "${!SWITCH_JSON_FILE_OUT}" != "" ]] ; then
            stderr_echo "Set either blob or JSON filename, not both!"
            echo
            return 1
        fi
    else
        if [[ ${!SWITCH_RUN_ONLY_OUT} == 1 ]] ; then
            stderr_echo "Cannot run-only tests when purge is required!"
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
    local SWITCH_DIRECTORY
    local SWITCH_SOURCE
    local SWITCH_TEST_NAME
    local SWITCH_BLOB_NAME
    local SWITCH_JSON_FILE
    local SWITCH_BLOB_FILE
    local SWITCH_NUM_ITERATIONS
    local SWITCH_TEST_CONFIG
    local SWITCH_PURGE
    local SWITCH_RUN_ONLY
    local SWITCH_PROFILE
    parse_arguments PARAM_CPP_TARGET_ARRAY SWITCH_OUT_DIR SWITCH_DIRECTORY SWITCH_SOURCE SWITCH_TEST_NAME \
            SWITCH_BLOB_NAME SWITCH_JSON_FILE SWITCH_BLOB_FILE SWITCH_NUM_ITERATIONS SWITCH_TEST_CONFIG \
            SWITCH_PURGE SWITCH_RUN_ONLY SWITCH_PROFILE "$@"
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

    echo "C++17 Extension Performance Tests"
    echo

    # set global variables
    set_global_common_variables
    if [ $? -ne 0 ] ; then
        return 1
    fi

    set_global_cpp_variables "${ZSERIO_CPP17_PROJECT_ROOT}"
    if [ $? -ne 0 ] ; then
        return 1
    fi

    # cmake needs absolute paths
    convert_to_absolute_path "${SWITCH_OUT_DIR}" SWITCH_OUT_DIR
    if [[ "${SWITCH_DIRECTORY}" != "" ]] ; then
        convert_to_absolute_path "${SWITCH_DIRECTORY}" SWITCH_DIRECTORY
    fi
    if [[ "${SWITCH_JSON_FILE}" != "" ]] ; then
        convert_to_absolute_path "${SWITCH_JSON_FILE}" SWITCH_JSON_FILE
    fi
    if [[ "${SWITCH_BLOB_FILE}" != "" ]] ; then
        convert_to_absolute_path "${SWITCH_BLOB_FILE}" SWITCH_BLOB_FILE
    fi

    # purge if requested and then create test output directory
    local ZSERIO_CPP17_BUILD_DIR="${SWITCH_OUT_DIR}/build"
    local TEST_OUT_DIR="${ZSERIO_CPP17_BUILD_DIR}/test_perf/${SWITCH_TEST_NAME}"
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
    echo "Test config: ${SWITCH_TEST_CONFIG}"
    echo

    # run test
    local ZSERIO_CPP17_DISTR_DIR="${SWITCH_OUT_DIR}/distr"
    test_perf "${ZSERIO_CPP17_DISTR_DIR}" "${ZSERIO_CPP17_PROJECT_ROOT}" "${ZSERIO_CPP17_BUILD_DIR}" \
            "${TEST_OUT_DIR}" PARAM_CPP_TARGET_ARRAY[@] "${SWITCH_DIRECTORY}" "${SWITCH_SOURCE}" \
            "${SWITCH_TEST_NAME}" "${SWITCH_BLOB_NAME}" "${SWITCH_JSON_FILE}" "${SWITCH_BLOB_FILE}" \
             ${SWITCH_NUM_ITERATIONS} ${SWITCH_TEST_CONFIG} ${SWITCH_RUN_ONLY} ${SWITCH_PROFILE}
    if [ $? -ne 0 ] ; then
        return 1
    fi

    return 0
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]] ; then
    main "$@"
fi
