include_guard(GLOBAL)

# Building YAML
include(FetchContent)
FetchContent_Declare(
        yamlcpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        yaml-cpp-0.6.3
        GIT_SHALLOW    ON
)

set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "")
set(YAML_BUILD_SHARED_LIBS ${DYNAMIC_BUILD} CACHE INTERNAL "")
set(YAML_CPP_INSTALL OFF CACHE INTERNAL "")

if (WIN32)
    set(YAML_MSVC_SHARED_RT ${DYNAMIC_BUILD} CACHE INTERNAL "")
endif()

FetchContent_MakeAvailable(yamlcpp)

if (UNIX)
    target_compile_options(yaml-cpp PRIVATE -Wno-shadow)

    target_compile_options(yaml-cpp PUBLIC -m32)
    target_link_options(yaml-cpp PUBLIC -m32
                                 PRIVATE -Wl,--disable-new-dtags)

    if (NOT DYNAMIC_BUILD)
        set_target_properties(yaml-cpp PROPERTIES
                            POSITION_INDEPENDENT_CODE ON)
    endif()

    if (IS_CLANG_COMPILER)
        target_compile_options(yaml-cpp PRIVATE -Wno-pedantic -stdlib=libc++)

        if (DYNAMIC_BUILD)
            target_link_options(yaml-cpp PUBLIC -fuse-ld=${LLD} -stdlib=libc++ --rtlib=compiler-rt)
            target_link_libraries(yaml-cpp PUBLIC c++ c++abi unwind)
        endif()
    else()
        target_compile_options(yaml-cpp PRIVATE -Wno-effc++)
    endif()
else()
    target_compile_options(yaml-cpp PRIVATE /wd4251 /wd4275)
endif()

set(YAML_CPP_INCLUDE_DIR ${yamlcpp_SOURCE_DIR}/include)
set(YAML_CPP_LIBRARIES yaml-cpp)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS ON)

set_target_properties(yaml-cpp
        PROPERTIES
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH ON
        BUILD_RPATH_USE_ORIGIN ON
        INSTALL_RPATH $ORIGIN)

if (DYNAMIC_BUILD)
    install(TARGETS yaml-cpp
        RUNTIME DESTINATION bin/lib
        LIBRARY DESTINATION bin/lib)
endif()

#SDK
install(TARGETS yaml-cpp
        RUNTIME DESTINATION sdk/lib
        LIBRARY DESTINATION sdk/lib
        ARCHIVE DESTINATION sdk/lib)

install(DIRECTORY ${yamlcpp_SOURCE_DIR}/include/
        DESTINATION sdk/include
        FILES_MATCHING PATTERN "*.h")
