include_guard(GLOBAL)

# Building YAML
include(FetchContent)
FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        7.1.3
        GIT_SHALLOW    ON
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE INTERNAL "")
set(FMT_DOC OFF CACHE INTERNAL "")
set(FMT_TEST OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS ${BUILD_DYNAMIC} CACHE INTERNAL "")

FetchContent_MakeAvailable(fmtlib)

target_compile_options(fmt PRIVATE -m32)
target_link_options(fmt PRIVATE -m32)

set(FMT_INCLUDE_DIR ${fmtlib_SOURCE_DIR}/include)
set(FMT_LIBRARIES fmt)
