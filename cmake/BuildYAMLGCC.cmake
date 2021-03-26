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
set(YAML_BUILD_SHARED_LIBS ${BUILD_DYNAMIC} CACHE INTERNAL "")

FetchContent_MakeAvailable(yamlcpp)
target_compile_options(yaml-cpp PRIVATE -m32 -Wno-effc++ -Wno-shadow)
target_link_options(yaml-cpp PRIVATE -m32)

set(YAML_CPP_INCLUDE_DIR ${yamlcpp_SOURCE_DIR}/include)
set(YAML_CPP_LIBRARIES yaml-cpp)
