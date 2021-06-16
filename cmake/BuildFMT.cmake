include_guard(GLOBAL)

# Building YAML
include(FetchContent)
FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        7.1.3
        GIT_SHALLOW    ON
)

set(FMT_DOC OFF CACHE INTERNAL "")
set(FMT_TEST OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS ${DYNAMIC_BUILD} CACHE INTERNAL "")

FetchContent_MakeAvailable(fmtlib)

if (UNIX)
    target_compile_options(fmt PUBLIC -m32)
    target_link_options(fmt PUBLIC -m32
                            PRIVATE -Wl,--disable-new-dtags)

    if (NOT DYNAMIC_BUILD)
        set_target_properties(fmt PROPERTIES
                POSITION_INDEPENDENT_CODE ON)
    endif()

    if (IS_CLANG_COMPILER)
        target_compile_options(fmt PRIVATE -stdlib=libc++ -Wno-pedantic -Wno-user-defined-literals)

        if (DYNAMIC_BUILD)
            target_link_options(fmt PUBLIC -fuse-ld=${LLD} -stdlib=libc++ --rtlib=compiler-rt)
        endif()
    endif()
endif()

set(FMT_INCLUDE_DIR ${fmtlib_SOURCE_DIR}/include)
set(FMT_LIBRARIES fmt)

set_target_properties(fmt
        PROPERTIES
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH ON
        BUILD_RPATH_USE_ORIGIN ON
        INSTALL_RPATH $ORIGIN)

install(TARGETS fmt
        RUNTIME DESTINATION bin/libs
        LIBRARY DESTINATION bin/libs)