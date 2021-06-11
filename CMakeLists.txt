cmake_minimum_required(VERSION 3.17)

project(MetamodCPP
        VERSION 0.1.0
        DESCRIPTION "HL1 Server Plugin Manager"
        HOMEPAGE_URL "https://github.com/Amaroq7/metamod-cpp"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

option(DYNAMIC_BUILD "Build project linked dynamically with other libs" ON)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if (WIN32)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(FATAL_ERROR "Only MSVC is supported.")
    endif()

    if (NOT DYNAMIC_BUILD)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(IS_CLANG_COMPILER ON)

    string(FIND ${CMAKE_CXX_COMPILER_VERSION} "." FIRST_DOT)
    string(SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 ${FIRST_DOT} CLANG_MAJOR_VER)
    find_program(LLD NAMES lld-${CLANG_MAJOR_VER} lld REQUIRED)
    get_filename_component(LLD ${LLD} NAME)
endif()

include(cmake/BuildFMT.cmake)
include(cmake/BuildYAML.cmake)

add_compile_definitions(META_CORE)
add_subdirectory(metamod)