add_library(choice_types_zs STATIC ${TEST_ZS_ROOT}/choice_types.zs)
zserio_generate_cpp(
    TARGET choice_types_zs
    SRC_DIR ${TEST_ZS_ROOT}
    GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen
    EXTRA_ARGS ${ZSERIO_EXTRA_ARGS}
    GENERATED_SOURCES_VAR GENERATED_SOURCES
    OUTPUT_VAR ZSERIO_LOG
    ERROR_VAR ZSERIO_LOG
)
target_link_libraries(choice_types_zs PUBLIC ZserioCpp17Runtime)
if (ZSERIO_LOG)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/zserio_log.txt "${ZSERIO_LOG}")
    check_zserio_warnings("${ZSERIO_LOG}" 0)
endif ()

list(FILTER GENERATED_SOURCES INCLUDE REGEX
    ".*/empty_choice/.*|.*/uint64_param_choice/.*|.*/choice_with_array/.*|.*/multiple_param_choice/.*")
set_target_properties(choice_types_zs PROPERTIES SOURCES "${GENERATED_SOURCES}")

add_custom_test(choice_types
    DEPENDS
        choice_types_zs
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ChoiceWithArrayTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/EmptyChoiceTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/MultipleParamChoiceTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp/UInt64ParamChoiceTest.cpp
    GENERATED_SOURCES
        ${GENERATED_SOURCES}
)
