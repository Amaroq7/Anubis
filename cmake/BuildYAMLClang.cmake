include_guard(GLOBAL)

# Building YAML
include(FetchContent)
FetchContent_Declare(
        yaml-cpp-project
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        yaml-cpp-0.6.3
        GIT_SHALLOW    ON
)

set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "")
set(YAML_BUILD_SHARED_LIBS ${BUILD_DYNAMIC} CACHE INTERNAL "")
FetchContent_MakeAvailable(yaml-cpp-project)
target_compile_options(yaml-cpp PRIVATE -m32 -Wno-pedantic -Wno-shadow -idirafter ${CMAKE_BINARY_DIR})
target_link_options(yaml-cpp PUBLIC -m32 -fuse-ld=lld -stdlib=libc++ --rtlib=compiler-rt -Wl,--as-needed)

set_target_properties(yaml-cpp
        PROPERTIES
        INSTALL_RPATH "$ORIGIN"
)

set(YAML_CPP_INCLUDE_DIR ${yaml-cpp-project_SOURCE_DIR}/include)
set(YAML_CPP_LIBRARIES yaml-cpp)
