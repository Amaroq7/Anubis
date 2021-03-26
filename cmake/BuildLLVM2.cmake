include_guard(GLOBAL)

find_program(LLD NAMES lld-${CLANG_MAJOR_VER} lld)
get_filename_component(LLD ${LLD} NAME)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=${LLD} -L${CMAKE_BINARY_DIR}/lib")
include(FetchContent)
set(LLVM_ENABLE_PROJECTS libunwind$<SEMICOLON>libcxx$<SEMICOLON>libcxxabi ON CACHE INTERNAL "")
set(LLVM_BUILD_32_BITS ON CACHE INTERNAL "")

set(LIBUNWIND_STANDALONE_BUILD ON CACHE INTERNAL "")
set(LIBUNWIND_BUILD_32_BITS ON CACHE INTERNAL "")
set(LIBUNWIND_ENABLE_ASSERTIONS OFF CACHE INTERNAL "")
set(LIBUNWIND_USE_COMPILER_RT ON CACHE INTERNAL "")
set(LIBUNWIND_HERMETIC_STATIC_LIBRARY ON CACHE INTERNAL "")
set(LIBUNWIND_INCLUDE_DOCS OFF CACHE INTERNAL "")

set(LIBCXXABI_STANDALONE_BUILD ON CACHE INTERNAL "")
set(LIBCXXABI_USE_COMPILER_RT ON CACHE INTERNAL "")
set(LIBCXXABI_BUILD_32_BITS ON CACHE INTERNAL "")
set(LIBCXXABI_INCLUDE_TESTS OFF CACHE INTERNAL "")
set(LIBCXXABI_ENABLE_ASSERTIONS OFF CACHE INTERNAL "")
set(LIBCXXABI_USE_LLVM_UNWINDER ON CACHE INTERNAL "")
if(${BUILD_DYNAMIC})
    set(LIBCXXABI_ENABLE_STATIC_UNWINDER OFF CACHE INTERNAL "")
else()
    set(LIBCXXABI_ENABLE_STATIC_UNWINDER ON CACHE INTERNAL "")
endif()
set(LIBCXXABI_HERMETIC_STATIC_LIBRARY ON CACHE INTERNAL "")

set(LIBCXX_INCLUDE_BENCHMARKS OFF CACHE INTERNAL "")
set(LIBCXX_STANDALONE_BUILD ON CACHE INTERNAL "")
set(LIBCXX_USE_COMPILER_RT ON CACHE INTERNAL "")
set(LIBCXX_ENABLE_EXPERIMENTAL_LIBRARY ON CACHE INTERNAL "")
set(LIBCXX_CXX_ABI libcxxabi CACHE INTERNAL "")
set(LIBCXX_HERMETIC_STATIC_LIBRARY ON CACHE INTERNAL "")
set(LIBCXX_USE_COMPILER_RT ON CACHE INTERNAL "")
set(LIBCXX_BUILD_32_BITS ON CACHE INTERNAL "")

set(LIBCXXABI_LIBUNWIND_INCLUDES "${llvm_SOURCE_DIR}/include/libunwind/include" CACHE PATH
        "Specify path to libunwind includes." FORCE)
set(LIBCXXABI_LIBUNWIND_PATH "{llvm_SOURCE_DIR}/include/libunwind" CACHE PATH
        "Specify path to libunwind source." FORCE)

set(LIBCXXABI_LIBCXX_INCLUDES ${llvm_SOURCE_DIR}/libcxx/include CACHE INTERNAL "")
set(LIBCXX_CXX_ABI_INCLUDE_PATHS ${llvm_SOURCE_DIR}/libcxxabi/include CACHE INTERNAL "")
set(LIBCXX_HAS_PTHREAD_API ON CACHE INTERNAL "")

FetchContent_Declare(
        llvm
        GIT_REPOSITORY https://github.com/llvm/llvm-project.git
        GIT_TAG        llvmorg-11.1.0
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(llvm)
target_link_options(unwind_shared PRIVATE -fuse-ld=${LLD})
install(TARGETS unwind_shared unwind_static
        EXPORT unwind-targets
        LIBRARY DESTINATION lib COMPONENT unwind
        ARCHIVE DESTINATION lib COMPONENT unwind
)

export(TARGETS unwind_shared unwind_static FILE "${llvm_BINARY_DIR}/unwind-targets.cmake")
export(PACKAGE unwind)
install(EXPORT unwind-targets
        DESTINATION lib/cmake/llvm
)

install(TARGETS cxxabi_shared cxxabi_static
        EXPORT cxxabi-targets
        LIBRARY DESTINATION lib COMPONENT cxxabi
        ARCHIVE DESTINATION lib COMPONENT cxxabi
)

export(TARGETS cxxabi_shared cxxabi_static FILE "${llvm_BINARY_DIR}/cxxabi-targets.cmake")
export(PACKAGE cxxabi)
install(EXPORT cxxabi-targets
        DESTINATION lib/cmake/llvm
)

install(TARGETS cxx_shared cxx_static
        EXPORT cxx-targets
        LIBRARY DESTINATION lib COMPONENT cxx
        ARCHIVE DESTINATION lib COMPONENT cxx
)

export(TARGETS cxx_shared cxx_static FILE "${llvm_BINARY_DIR}/cxx-targets.cmake")
export(PACKAGE cxx)
install(EXPORT cxx-targets
        DESTINATION lib/cmake/llvm
)

set(LLVM_BUILD ON)
if (BUILD_DYNAMIC)
    set(LLVM_LIBS unwind_shared cxxabi_shared cxx_shared)
else()
    set(LLVM_LIBS unwind_static cxxabi_static cxx_static)
endif()

set(LIBCXXABI_INCLUDE_HEADERS "${libcxxabi-llvm_SOURCE_DIR}/include")
set(LIBCXX_INCLUDE_HEADERS "${libcxx-llvm_SOURCE_DIR}/include")
set(LLVM_LIBS_DIR ${CMAKE_BINARY_DIR}/lib)
string(REPLACE " -fuse-ld=${LLD} -L${CMAKE_BINARY_DIR}/lib" "" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")