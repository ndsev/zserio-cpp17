#!/bin/bash

SCRIPT_DIR=`dirname $0`
source "${SCRIPT_DIR}/common_tools.sh"
source "${SCRIPT_DIR}/test_zs.sh"

# Get the name of the log file which the performance test run with the given index writes.
get_perf_log_name()
{
    exit_if_argc_ne $# 3
    local RUN_INDEX="$1"; shift
    local NUM_RUNS="$1"; shift
    local LOG_NAME_OUT="$1"; shift

    if [[ ${NUM_RUNS} -eq 1 ]] ; then
        eval ${LOG_NAME_OUT}="PerformanceTest.log"
    else
        eval ${LOG_NAME_OUT}="PerformanceTest_$((RUN_INDEX + 1)).log"
    fi
}

# Get the values of the given list without duplicates, in the order of their first occurrence.
get_distinct_values()
{
    exit_if_argc_ne $# 2
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local VALUES=("${MSYS_WORKAROUND_TEMP[@]}")
    local DISTINCT_VALUES_OUT="$1"; shift

    local DISTINCT_VALUES_LOC=()
    local VALUE
    for VALUE in "${VALUES[@]}" ; do
        if [[ " ${DISTINCT_VALUES_LOC[*]} " != *" ${VALUE} "* ]] ; then
            DISTINCT_VALUES_LOC+=("${VALUE}")
        fi
    done

    eval ${DISTINCT_VALUES_OUT}='("${DISTINCT_VALUES_LOC[@]}")'
}

# Get the name of the generated C++ function which runs the given test configuration.
get_perf_test_function_name()
{
    exit_if_argc_ne $# 2
    local TEST_CONFIG="$1"; shift
    local FUNCTION_NAME_OUT="$1"; shift

    case "${TEST_CONFIG}" in
        "READ")
            eval ${FUNCTION_NAME_OUT}="runRead"
            ;;
        "WRITE")
            eval ${FUNCTION_NAME_OUT}="runWrite"
            ;;
        "READ_WRITE")
            eval ${FUNCTION_NAME_OUT}="runReadWrite"
            ;;
    esac
}

# Append the C++ function which runs one test configuration on any blob to the performance test.
generate_performance_test_run()
{
    exit_if_argc_ne $# 3
    local SRC_FILE="$1"; shift
    local TEST_CONFIG="$1"; shift
    local PROFILE="$1"; shift

    local FUNCTION_NAME
    get_perf_test_function_name ${TEST_CONFIG} FUNCTION_NAME

    cat >> "${SRC_FILE}" << EOF

template <typename Blob>
static int ${FUNCTION_NAME}(const char* logPath, bool inputIsJson, const char* inputPath, int numIterations)
{
    // read blob buffer
    BitBuffer bitBuffer;
    try
    {
        bitBuffer = readBlobBuffer<Blob>(inputIsJson, inputPath);
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
    zserio::BitStreamReader blobReader(bitBuffer, zserio::ArrayPreallocation(1024*1024*1024));
    std::unique_ptr<Blob> memoryBlob(new Blob>(blobReader, allocator));
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
    std::vector<Blob> readData;
    readData.resize(static_cast<size_t>(numIterations));
EOF
else
    cat >> "${SRC_FILE}" << EOF
    Blob readData;
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
        zserio::BitStreamReader reader(bitBuffer, zserio::ArrayPreallocation(1024*1024*1024));
        zserio::detail::read(reader, readData[i]);
EOF
            ;;
        "READ_WRITE")
            cat >> "${SRC_FILE}" << EOF
        zserio::BitStreamReader reader(bitBuffer, zserio::ArrayPreallocation(1024*1024*1024));
        auto readView = zserio::detail::read(reader, readData[i]);
        zserio::BitStreamWriter writer(bitBuffer);
        zserio::detail::write(writer, readView);
EOF
            ;;

        "WRITE")
            cat >> "${SRC_FILE}" << EOF
        zserio::BitStreamWriter writer(bitBuffer);
        zserio::detail::write(writer, readView);
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

# Generate C++ files
#
# One performance test runs every test configuration on every blob, configurations first. The n-th blob
# name pairs with the n-th JSON or blob file, where an empty JSON file means the blob file is used.
generate_performance_test()
{
    exit_if_argc_ne $# 8
    local SRC_FILE="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local BLOB_NAMES=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local JSON_FILES=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local BLOB_FILES=("${MSYS_WORKAROUND_TEMP[@]}")
    local LOG_DIR="$1"; shift
    local NUM_ITERATIONS="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local TEST_CONFIGS=("${MSYS_WORKAROUND_TEMP[@]}")
    local PROFILE="$1"; shift

    local NUM_BLOBS=${#BLOB_NAMES[@]}
    local NUM_RUNS=$((${#TEST_CONFIGS[@]} * NUM_BLOBS))
    local DISTINCT_BLOB_NAMES
    get_distinct_values BLOB_NAMES[@] DISTINCT_BLOB_NAMES
    local DISTINCT_TEST_CONFIGS
    get_distinct_values TEST_CONFIGS[@] DISTINCT_TEST_CONFIGS

    cat > "${SRC_FILE}" << EOF
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/SerializeUtil.h>

EOF

    local BLOB_NAME
    for BLOB_NAME in "${DISTINCT_BLOB_NAMES[@]}" ; do
        cat >> "${SRC_FILE}" << EOF
#include <${BLOB_NAME//.//}.h>
EOF
    done

    cat >> "${SRC_FILE}" << EOF

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

// all blobs are generated with the same allocator
using AllocatorType = ${DISTINCT_BLOB_NAMES[0]//./::}::allocator_type;
using BitBuffer = zserio::BasicBitBuffer<zserio::RebindAlloc<AllocatorType, uint8_t>>;

template <typename Blob>
static BitBuffer readBlobBuffer(bool inputIsJson, const char* inputPath)
{
    if (inputIsJson)
    {
        // TODO[mikir]: JSON not implemented
        // auto blob = zserio::fromJsonFile<Blob>(inputPath);

        // serialize to binary file for further analysis
        // zserio::serializeToFile(blob, "<top level package>.blob");

        // return zserio::serialize<Blob, AllocatorType>(blob);
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

        Blob objectData;
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
EOF

    local TEST_CONFIG
    for TEST_CONFIG in "${DISTINCT_TEST_CONFIGS[@]}" ; do
        generate_performance_test_run "${SRC_FILE}" ${TEST_CONFIG} ${PROFILE}
    done

    cat >> "${SRC_FILE}" << EOF

struct PerfTestRun
{
    const char* name;
    const char* logPath;
    bool inputIsJson;
    const char* inputPath;
    int (*run)(const char* logPath, bool inputIsJson, const char* inputPath, int numIterations);
};

int main(int argc, char* argv[])
{
    std::cout << "C++17 Extension Performance Test" << std::endl;

    PerfTestRun runs[] = {
EOF

    # use host paths in generated files (needed for Windows)
    local DISABLE_SLASHES_CONVERSION=1
    local RUN_INDEX=0
    for TEST_CONFIG in "${TEST_CONFIGS[@]}" ; do
        local FUNCTION_NAME
        get_perf_test_function_name ${TEST_CONFIG} FUNCTION_NAME
        local BLOB_INDEX
        for (( BLOB_INDEX=0; BLOB_INDEX < NUM_BLOBS; BLOB_INDEX++ )) ; do
            local INPUT_SWITCH="false"
            local INPUT_FILE="${BLOB_FILES[${BLOB_INDEX}]}"
            if [[ "${INPUT_FILE}" == "" ]] ; then
                INPUT_SWITCH="true"
                INPUT_FILE="${JSON_FILES[${BLOB_INDEX}]}"
            fi
            posix_to_host_path "${INPUT_FILE}" INPUT_FILE ${DISABLE_SLASHES_CONVERSION}
            local LOG_NAME
            get_perf_log_name ${RUN_INDEX} ${NUM_RUNS} LOG_NAME
            local LOG_FILE
            posix_to_host_path "${LOG_DIR}/${LOG_NAME}" LOG_FILE ${DISABLE_SLASHES_CONVERSION}
            local BLOB_NAME="${BLOB_NAMES[${BLOB_INDEX}]}"
            cat >> "${SRC_FILE}" << EOF
        {"${TEST_CONFIG} ${BLOB_NAME} ${INPUT_FILE##*/}", "${LOG_FILE}", ${INPUT_SWITCH}, "${INPUT_FILE}",
                &${FUNCTION_NAME}<${BLOB_NAME//./::}>},
EOF
            RUN_INDEX=$((RUN_INDEX + 1))
        done
    done

    cat >> "${SRC_FILE}" << EOF
    };
    const size_t numRuns = sizeof(runs) / sizeof(runs[0]);
    int numIterations = ${NUM_ITERATIONS};
    if ((argc > 1 && argc < 4) || (argc == 2 && strcmp(argv[1], "-h") == 0))
    {
        std::cerr << "Usage: LOG_PATH (-j|-b) INPUT_PATH [NUM_ITERATIONS]" << std::endl;
        return 1;
    }

    if (argc > 1 && numRuns != 1)
    {
        std::cerr << "Arguments can be given only to a test with a single blob and test config!" << std::endl;
        return 1;
    }

    if (argc > 1)
        runs[0].logPath = argv[1];
    if (argc > 2)
        runs[0].inputIsJson = strcmp("-j", argv[2]) == 0 ? true : false;
    if (argc > 3)
        runs[0].inputPath = argv[3];
    if (argc > 4)
        numIterations = atoi(argv[4]);

    if (numIterations <= 0)
    {
        std::cerr << "Num iterations must be a positive integer (" << numIterations << ")!" << std::endl;
        return 1;
    }

    for (const PerfTestRun& run : runs)
    {
        if (numRuns != 1)
            std::cout << std::endl << "Test run: " << run.name << std::endl;
        if (run.run(run.logPath, run.inputIsJson, run.inputPath, numIterations) != 0)
            return 1;
    }

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
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local BLOB_NAMES=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local JSON_FILES=("${MSYS_WORKAROUND_TEMP[@]}")
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local BLOB_FILES=("${MSYS_WORKAROUND_TEMP[@]}")
    local SWITCH_NUM_ITERATIONS="$1"; shift
    local MSYS_WORKAROUND_TEMP=("${!1}"); shift
    local TEST_CONFIGS=("${MSYS_WORKAROUND_TEMP[@]}")
    local SWITCH_RUN_ONLY="$1"; shift
    local SWITCH_PROFILE="$1"; shift

    # run C++ performance tests
    local TEST_LOG_FILE_SUBDIR="log"
    for CPP_TARGET in "${CPP_TARGETS[@]}" ; do
        local TARGET_TEST_OUT_DIR="${TEST_OUT_DIR}/${CPP_TARGET}"
        local TEST_SRC_DIR="${TARGET_TEST_OUT_DIR}/src"
        mkdir -p "${TEST_SRC_DIR}"
        local TEST_SRC_FILE="${TEST_SRC_DIR}/PerformanceTest.cpp"
        local TEST_LOG_DIR="${TARGET_TEST_OUT_DIR}/${TEST_LOG_FILE_SUBDIR}"
        mkdir -p "${TEST_LOG_DIR}"
        mkdir -p "${TARGET_TEST_OUT_DIR}"
        if [[ ${SWITCH_RUN_ONLY} == 0 ]] ; then
            generate_performance_test "${TEST_SRC_FILE}" BLOB_NAMES[@] JSON_FILES[@] BLOB_FILES[@] \
                    "${TEST_LOG_DIR}" ${SWITCH_NUM_ITERATIONS} TEST_CONFIGS[@] ${SWITCH_PROFILE}
        fi

        # run external integration test
        test_zs "${ZSERIO_CPP17_DISTR_DIR}" "${ZSERIO_CPP17_PROJECT_ROOT}" "${ZSERIO_CPP17_BUILD_DIR}" \
            "${TEST_OUT_DIR}" CPP_TARGET "${SWITCH_SOURCE}" "${SWITCH_DIRECTORY}" \
            "${SWITCH_TEST_NAME}" "${TEST_SRC_FILE}" ${SWITCH_PROFILE}
        if [ $? -ne 0 ] ; then
            return 1
        fi
    done

    # collect results
    local NUM_BLOBS=${#BLOB_NAMES[@]}
    local NUM_RUNS=$((${#TEST_CONFIGS[@]} * NUM_BLOBS))
    local RUN_INDEX=0
    local TEST_CONFIG
    for TEST_CONFIG in "${TEST_CONFIGS[@]}" ; do
        local BLOB_INDEX
        for (( BLOB_INDEX=0; BLOB_INDEX < NUM_BLOBS; BLOB_INDEX++ )) ; do
            local TEST_LOG_FILE_NAME
            get_perf_log_name ${RUN_INDEX} ${NUM_RUNS} TEST_LOG_FILE_NAME
            echo
            echo "Performance Tests Results - ${TEST_CONFIG}"
            echo "Blob name: ${BLOB_NAMES[${BLOB_INDEX}]}"
            if [[ "${JSON_FILES[${BLOB_INDEX}]}" != "" ]] ; then
                echo "JSON file: ${JSON_FILES[${BLOB_INDEX}]##*/}"
            else
                echo "Blob file: ${BLOB_FILES[${BLOB_INDEX}]##*/}"
            fi
            for i in {1..103} ; do echo -n "=" ; done ; echo
            printf "| %-21s | %14s | %10s | %15s | %10s | %10s |\n" \
                   "Generator" "Total Duration" "Iterations" "Step Duration" "Blob Size" "Blob in Memory"
            echo -n "|" ; for i in {1..101} ; do echo -n "-" ; done ; echo "|"
            for CPP_TARGET in "${CPP_TARGETS[@]}" ; do
                local PERF_TEST_FILE="${TEST_OUT_DIR}/${CPP_TARGET}/${TEST_LOG_FILE_SUBDIR}/${TEST_LOG_FILE_NAME}"
                local RESULTS=($(cat ${PERF_TEST_FILE}))
                printf "| %-21s | %14s | %10s | %15s | %10s | %14s |\n" \
                       "C++ (${CPP_TARGET})" ${RESULTS[0]} ${RESULTS[1]} ${RESULTS[2]} ${RESULTS[3]} ${RESULTS[4]}
            done
            for i in {1..103} ; do echo -n "=" ; done ; echo
            echo
            RUN_INDEX=$((RUN_INDEX + 1))
        done
    done

    return 0
}

# Print help message.
print_help()
{
    cat << EOF
Description:
    Runs performance tests on given zserio sources using C++17 extension from distr directory.

Usage:
    $0 [-h] [-e] [-p] [-r] [-l] [-o <dir>] [-d <dir>] [-t <name>] -[n <num>] [-c <config>]...
        target... -s <source> (-b <blobname> (-f <blobfile> | -j <jsonfile>))...

Arguments:
    -h, --help              Show this help.
    -e, --help-env          Show help for enviroment variables.
    -p, --purge             Purge test build directory.
    -r, --run-only          Run already compiled PerformanceTests again.
    -l, --profile           Run the test in profiling mode and produce profiling data.
    -o <dir>, --output-directory <dir>
                            Output directory where tests will be run.
    -d <dir>, --source-dir <dir>
                            Directory with zserio sources. Default is ".".
    -t <name>, --test-name <name>
                            Test name. Optional.
    -n <num>, --num-iterations <num>
                            Number of iterations. Optional, default is 100.
    -c <config>, --test-config <config>
                            Test configuration: READ (default), WRITE, READ_WRITE. Can be repeated.
    -s <source>, --source <source>
                            Main zserio source.
    -b <blobname>, --blob-name <blobname>
                            Full name of blob to run performance tests on.
    -f <blobfile>, --blob-file <blobfile>
                            Path to the blob file.
    -j <jsonfile>, --json-file <jsonfile>
                            Path to the JSON file.
                            Blob name and blob or JSON file can be repeated, the n-th blob name pairs
                            with the n-th blob or JSON file. The sources are generated and compiled once,
                            the test runs each test configuration on each pair and writes
                            PerformanceTest_<n>.log, a single run writes PerformanceTest.log.
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
    $0 cpp-linux64-gcc -d /tmp/zs -s test.zs -b test.Blob -f blob1.bin -b test.Blob -f blob2.bin \
        -c READ -c WRITE

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
    eval ${SWITCH_BLOB_NAME_OUT}="()"
    eval ${SWITCH_JSON_FILE_OUT}="()"
    eval ${SWITCH_BLOB_FILE_OUT}="()"
    eval ${SWITCH_NUM_ITERATIONS_OUT}=100 # default
    eval ${SWITCH_TEST_CONFIG_OUT}="()"
    eval ${SWITCH_PURGE_OUT}=0
    eval ${SWITCH_RUN_ONLY_OUT}=0
    eval ${SWITCH_PROFILE_OUT}=0

    local NUM_PARAMS=0
    local PARAM_ARRAY=()
    local NUM_BLOB_NAMES=0
    local NUM_INPUT_FILES=0
    local NUM_TEST_CONFIGS=0
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
                eval ${SWITCH_BLOB_NAME_OUT}[${NUM_BLOB_NAMES}]='"$2"'
                NUM_BLOB_NAMES=$((NUM_BLOB_NAMES + 1))
                shift 2
                ;;

            "-j" | "--json-file")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing JSON file name!"
                    echo
                    return 1
                fi
                eval ${SWITCH_JSON_FILE_OUT}[${NUM_INPUT_FILES}]='"$2"'
                eval ${SWITCH_BLOB_FILE_OUT}[${NUM_INPUT_FILES}]=""
                NUM_INPUT_FILES=$((NUM_INPUT_FILES + 1))
                shift 2
                ;;

            "-f" | "--blob-file")
                if [ $# -eq 1 ] ; then
                    stderr_echo "Missing BLOB file name!"
                    echo
                    return 1
                fi
                eval ${SWITCH_BLOB_FILE_OUT}[${NUM_INPUT_FILES}]='"$2"'
                eval ${SWITCH_JSON_FILE_OUT}[${NUM_INPUT_FILES}]=""
                NUM_INPUT_FILES=$((NUM_INPUT_FILES + 1))
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
                eval ${SWITCH_TEST_CONFIG_OUT}[${NUM_TEST_CONFIGS}]='"$2"'
                NUM_TEST_CONFIGS=$((NUM_TEST_CONFIGS + 1))
                shift 2
                ;;

            "-r" | "--run-only")
                eval ${SWITCH_RUN_ONLY_OUT}=1
                shift
                ;;

            "-l" | "--profile")
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

    # validate test configurations
    if [[ ${NUM_TEST_CONFIGS} -eq 0 ]] ; then
        eval ${SWITCH_TEST_CONFIG_OUT}[0]="READ" # default
        NUM_TEST_CONFIGS=1
    fi
    local TEST_CONFIG_INDEX
    for (( TEST_CONFIG_INDEX=0; TEST_CONFIG_INDEX < NUM_TEST_CONFIGS; TEST_CONFIG_INDEX++ )) ; do
        local TEST_CONFIG_VAR="${SWITCH_TEST_CONFIG_OUT}[${TEST_CONFIG_INDEX}]"
        case "${!TEST_CONFIG_VAR}" in
            "READ" | "WRITE" | "READ_WRITE")
                ;;
            *)
                stderr_echo "Invalid test configuration, use one of READ, WRITE, READ_WRITE"
                return 1
                ;;
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

        if [[ ${NUM_BLOB_NAMES} -eq 0 ]] ; then
            stderr_echo "Blob name is not set!"
            echo
            return 1
        fi

        if [[ ${NUM_INPUT_FILES} -eq 0 ]] ; then
            stderr_echo "Neither blob nor JSON filename is set!"
            echo
            return 1
        fi

        if [[ ${NUM_BLOB_NAMES} -ne ${NUM_INPUT_FILES} ]] ; then
            stderr_echo "Each blob name needs exactly one blob or JSON filename!" \
                    "(${NUM_BLOB_NAMES} blob names, ${NUM_INPUT_FILES} filenames)"
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
    local SWITCH_BLOB_NAMES=()
    local SWITCH_JSON_FILES=()
    local SWITCH_BLOB_FILES=()
    local SWITCH_NUM_ITERATIONS
    local SWITCH_TEST_CONFIGS=()
    local SWITCH_PURGE
    local SWITCH_RUN_ONLY
    local SWITCH_PROFILE
    parse_arguments PARAM_CPP_TARGET_ARRAY SWITCH_OUT_DIR SWITCH_DIRECTORY SWITCH_SOURCE SWITCH_TEST_NAME \
            SWITCH_BLOB_NAMES SWITCH_JSON_FILES SWITCH_BLOB_FILES SWITCH_NUM_ITERATIONS SWITCH_TEST_CONFIGS \
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
    local BLOB_INDEX
    for (( BLOB_INDEX=0; BLOB_INDEX < ${#SWITCH_BLOB_NAMES[@]}; BLOB_INDEX++ )) ; do
        if [[ "${SWITCH_JSON_FILES[${BLOB_INDEX}]}" != "" ]] ; then
            convert_to_absolute_path "${SWITCH_JSON_FILES[${BLOB_INDEX}]}" "SWITCH_JSON_FILES[${BLOB_INDEX}]"
        fi
        if [[ "${SWITCH_BLOB_FILES[${BLOB_INDEX}]}" != "" ]] ; then
            convert_to_absolute_path "${SWITCH_BLOB_FILES[${BLOB_INDEX}]}" "SWITCH_BLOB_FILES[${BLOB_INDEX}]"
        fi
    done

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
    echo "Test config: ${SWITCH_TEST_CONFIGS[*]}"
    echo

    # run test
    local ZSERIO_CPP17_DISTR_DIR="${SWITCH_OUT_DIR}/distr"
    test_perf "${ZSERIO_CPP17_DISTR_DIR}" "${ZSERIO_CPP17_PROJECT_ROOT}" "${ZSERIO_CPP17_BUILD_DIR}" \
            "${TEST_OUT_DIR}" PARAM_CPP_TARGET_ARRAY[@] "${SWITCH_DIRECTORY}" "${SWITCH_SOURCE}" \
            "${SWITCH_TEST_NAME}" SWITCH_BLOB_NAMES[@] SWITCH_JSON_FILES[@] SWITCH_BLOB_FILES[@] \
             ${SWITCH_NUM_ITERATIONS} SWITCH_TEST_CONFIGS[@] ${SWITCH_RUN_ONLY} ${SWITCH_PROFILE}
    if [ $? -ne 0 ] ; then
        return 1
    fi

    return 0
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]] ; then
    main "$@"
fi
