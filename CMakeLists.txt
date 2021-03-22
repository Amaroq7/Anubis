cmake_minimum_required(VERSION 3.17)

project(metamod
        VERSION 0.1.0
        DESCRIPTION "HL1 Server Plugin Manager"
        HOMEPAGE_URL "https://github.com/Amaroq7/metamod-cpp"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_STANDARD 17)
option(BUILD_DYNAMIC "Build project linked dynamically with other libs" ON)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if (UNIX)
    if (LLVM_BUILD)
        include(cmake/BuildYAMLClang.cmake)
    else()
        include(cmake/BuildYAMLGCC.cmake)
    endif()
else()
    include(cmake/BuildYAMLMSVC.cmake)
endif()

add_definitions(-DMETA_CORE)
add_subdirectory(src metamod)
