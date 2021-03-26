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

SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "$ORIGIN/../libs")

option(BUILD_DYNAMIC "Build project linked dynamically with other libs" ON)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(LLVM_BUILD ON)
endif()

if (UNIX)
    if (LLVM_BUILD)
        include(cmake/BuildYAMLClang.cmake)
        include(cmake/BuildFMTClang.cmake)
    else()
        include(cmake/BuildYAMLGCC.cmake)
        include(cmake/BuildFMTGCC.cmake)
    endif()
else()
    include(cmake/BuildYAMLMSVC.cmake)
endif()

add_compile_definitions(META_CORE)
add_subdirectory(metamod)

if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0")
else()
    if (NOT BUILD_DYNAMIC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()
