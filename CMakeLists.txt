cmake_minimum_required(VERSION 3.17)

project(Anubis
        VERSION 2.0.0
        DESCRIPTION "HL1 Server Plugin Manager"
        HOMEPAGE_URL "https://github.com/Amaroq7/Anubis"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)

if (WIN32)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(FATAL_ERROR "Only MSVC is supported.")
    endif ()

    include(InstallRequiredSystemLibraries)

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_INSTALL_DEBUG_LIBRARIES_ONLY TRUE)
    endif ()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else ()
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(IS_CLANG_COMPILER ON)

        string(FIND ${CMAKE_CXX_COMPILER_VERSION} "." FIRST_DOT)
        string(SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 ${FIRST_DOT} CLANG_MAJOR_VER)
        find_program(LLD NAMES lld-${CLANG_MAJOR_VER} lld REQUIRED)
        get_filename_component(LLD ${LLD} NAME)

        find_library(LLVM_LIBCPP_LIB c++ REQUIRED)
        find_library(LLVM_LIBCPPABI_LIB c++abi REQUIRED)
        find_library(LLVM_UNWIND_LIB unwind REQUIRED)
    endif ()
endif ()

include(cmake/BuildFMT.cmake)
include(cmake/BuildYAML.cmake)

add_subdirectory(anubis)

find_package(Doxygen)

# Generate docs if doxygen is present
if (DOXYGEN_FOUND)
    get_target_property(DoxygenBin Doxygen::doxygen LOCATION)
    execute_process(COMMAND ${DoxygenBin}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html
            DESTINATION sdk/docs)

    install(FILES ${CMAKE_SOURCE_DIR}/docs/index_placeholder.html
            DESTINATION sdk/docs
            RENAME index.html)

    # Generate internal docs
    execute_process(COMMAND ${DoxygenBin} Doxyfile_internal
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html_internal
            DESTINATION sdk/docs_internal)

    install(FILES ${CMAKE_SOURCE_DIR}/docs/index_internal_placeholder.html
            DESTINATION sdk/docs_internal
            RENAME index.html)
endif ()
