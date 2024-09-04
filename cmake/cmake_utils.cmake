# CMake helpers

# Force Release build when nothing is selected
if (NOT MSVC)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        message(STATUS "Build type not specified, using: \"Release\"")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif ()

    set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING
            "Build type, options are: Default, Debug, Release, RelWithDebInfo, MinSizeRel")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Default" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif ()
