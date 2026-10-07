# bHaptics SDK CMake package — lives in the cmake/ folder of a bhaptics-cpp release archive.
#
#   find_package(bhaptics CONFIG REQUIRED)
#   target_link_libraries(app PRIVATE bhaptics::bhaptics)
#
# Point CMake at the extracted archive with -Dbhaptics_DIR=<archive>/cmake or
# CMAKE_PREFIX_PATH=<archive>. On Windows the x64 / x86 library is picked from
# the consuming project's pointer size.

cmake_minimum_required(VERSION 3.14...3.31)

get_filename_component(_bhaptics_prefix "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_bhaptics_libdir "${_bhaptics_prefix}/lib/x64")
    else()
        set(_bhaptics_libdir "${_bhaptics_prefix}/lib/x86")
    endif()
    set(_bhaptics_library "${_bhaptics_libdir}/bhaptics_library.dll")
    set(_bhaptics_implib  "${_bhaptics_libdir}/bhaptics_library.lib")
elseif(APPLE)
    set(_bhaptics_library "${_bhaptics_prefix}/lib/libbhaptics_library.dylib")
else()
    set(_bhaptics_library "${_bhaptics_prefix}/lib/libbhaptics_library.so")
endif()

if(NOT EXISTS "${_bhaptics_library}")
    set(bhaptics_FOUND FALSE)
    set(bhaptics_NOT_FOUND_MESSAGE
        "This bhaptics-cpp archive has no library for this platform / architecture (looked for ${_bhaptics_library}).")
    return()
endif()

if(NOT TARGET bhaptics::bhaptics)
    add_library(bhaptics::bhaptics SHARED IMPORTED)
    set_target_properties(bhaptics::bhaptics PROPERTIES
        IMPORTED_LOCATION "${_bhaptics_library}"
        INTERFACE_INCLUDE_DIRECTORIES "${_bhaptics_prefix}/include"
    )
    if(WIN32)
        set_target_properties(bhaptics::bhaptics PROPERTIES IMPORTED_IMPLIB "${_bhaptics_implib}")
    elseif(APPLE)
        set_target_properties(bhaptics::bhaptics PROPERTIES IMPORTED_SONAME "@rpath/libbhaptics_library.dylib")
    else()
        set_target_properties(bhaptics::bhaptics PROPERTIES IMPORTED_SONAME "libbhaptics_library.so")
    endif()
endif()

unset(_bhaptics_prefix)
unset(_bhaptics_libdir)
unset(_bhaptics_library)
unset(_bhaptics_implib)
