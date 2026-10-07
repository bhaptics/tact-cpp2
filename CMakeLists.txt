cmake_minimum_required(VERSION 3.21)
project(tact_cpp2_examples LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The SDK in sdk/ — or point bhaptics_DIR at any extracted bhaptics-cpp release.
find_package(bhaptics CONFIG REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/sdk")

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
