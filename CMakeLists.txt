cmake_minimum_required(VERSION 3.21)
project(tact_cpp2_examples LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The SDK in sdk/ — or point bhaptics_DIR at any extracted bhaptics-cpp release.
find_package(bhaptics CONFIG REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/sdk")

# bhaptics.h and the CMake package version must come from the same SDK release.
get_target_property(bhaptics_include bhaptics::bhaptics INTERFACE_INCLUDE_DIRECTORIES)
file(READ "${bhaptics_include}/bhaptics/bhaptics.h" bhaptics_header)
foreach(part MAJOR MINOR PATCH)
    string(REGEX MATCH "#define BHAPTICS_VERSION_${part} ([0-9]+)" _ "${bhaptics_header}")
    if(NOT "${CMAKE_MATCH_1}" STREQUAL "${bhaptics_VERSION_${part}}")
        message(FATAL_ERROR "bhaptics.h and the bhaptics CMake package (${bhaptics_VERSION}) are from different releases")
    endif()
endforeach()

foreach(source examples/hello.c examples/play_events.cpp examples/motor_control.cpp)
    get_filename_component(name "${source}" NAME_WE)
    add_executable(${name} "${source}")
    target_link_libraries(${name} PRIVATE bhaptics::bhaptics)

    if(MSVC)
        target_compile_options(${name} PRIVATE /W4 /permissive-)
        target_compile_definitions(${name} PRIVATE _CRT_SECURE_NO_WARNINGS)  # getenv
    else()
        target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(WIN32)
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${name}> $<TARGET_FILE_DIR:${name}>
            COMMAND_EXPAND_LISTS)
    endif()
endforeach()
