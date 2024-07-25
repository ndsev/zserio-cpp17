cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
endif()

project(cpp17_poc)

add_subdirectory(runtime)

set(SRC
    main.cpp
    test.zs
)

file(GLOB SRC_GEN RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "gen/*.cpp" "gen/*.h")
file(GLOB SRC_PMR RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "gen/pmr/*.cpp" "gen/pmr/*.h")

add_executable(${PROJECT_NAME} ${SRC} ${SRC_GEN} ${SRC_PMR})

target_link_libraries(${PROJECT_NAME} PRIVATE ZserioCppRuntime)

target_include_directories(${PROJECT_NAME} PUBLIC "gen" ".")

source_group("gen" FILES ${SRC_GEN})
source_group("pmr" FILES ${SRC_PMR})
