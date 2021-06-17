project(metamod)

string(TIMESTAMP CURRENT_YEAR %Y)

set(META_RELEASE_MODE ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} ${CMAKE_BUILD_TYPE})

if (DYNAMIC_BUILD)
    string(APPEND META_RELEASE_MODE ";Dynamic")
else()
    string(APPEND META_RELEASE_MODE ";Static")
endif()

# Generate MetaInfo.hpp
find_program(GIT git)
if (GIT AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT} rev-parse --short HEAD OUTPUT_VARIABLE COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)
    configure_file(MetaInfo.hpp.in MetaInfo.hpp @ONLY)
else()
    set(COMMIT "<unknown>")
    configure_file(MetaInfo.hpp.in MetaInfo.hpp @ONLY)
endif()

add_subdirectory(engine)
add_subdirectory(game)

set(SRC_FILES
        EngineExports.cpp
        DllExports.cpp
        Metamod.cpp
        Module.cpp
        MetaConfig.cpp
        Utils.cpp
        MetaCvars.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        SYSTEM
        PRIVATE
        ${CMAKE_BINARY_DIR}/metamod
        ${YAML_CPP_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/rehlds
        ${CMAKE_SOURCE_DIR}/rehlds/common
        ${CMAKE_SOURCE_DIR}/rehlds/dlls
        ${CMAKE_SOURCE_DIR}/rehlds/engine
        ${CMAKE_SOURCE_DIR}/rehlds/pm_shared
        ${CMAKE_SOURCE_DIR}/rehlds/public)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -Wl,--disable-new-dtags)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Wall -Werror -Wextra -Wpedantic -pedantic-errors)

    if (NOT DYNAMIC_BUILD)
        if (NOT IS_CLANG_COMPILER)
            target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
            if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
                target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
            endif()
        else()
            target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
            target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=${LLD} -Wl,--as-needed)
            target_link_libraries(${PROJECT_NAME} PRIVATE c++.a c++abi.a unwind.a m)
        endif()
    else()
        if (IS_CLANG_COMPILER)
            target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
            target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=${LLD} -Wl,--as-needed)
            target_link_libraries(${PROJECT_NAME} PRIVATE c++ c++abi unwind)
        endif()
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86 /EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8 /SECTION:.data,RW)
endif()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        VISIBILITY_INLINES_HIDDEN ON
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH ON
        BUILD_RPATH_USE_ORIGIN ON
        INSTALL_RPATH ../libs
        CXX_VISIBILITY_PRESET hidden)

target_link_libraries(${PROJECT_NAME} PRIVATE engine_api gamelib_api ${YAML_CPP_LIBRARIES} ${FMT_LIBRARIES})
add_dependencies(${PROJECT_NAME} engine_api gamelib_api ${YAML_CPP_LIBRARIES} ${FMT_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin/dlls
        LIBRARY DESTINATION bin/dlls
)

install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/configs
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

#SDK
install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/public/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/sdk/include/metamod
        FILES_MATCHING PATTERN "*.hpp"
)

# TODO: Come up with better solution than just hardcoding paths
if (IS_CLANG_COMPILER)
    if (DYNAMIC_BUILD)
        file(GLOB_RECURSE LIBCXX_LIBS FOLLOW_SYMLINKS
            LIST_DIRECTORIES false
            /usr/lib32/libc++*.so*
            /usr/lib32/libunwind.so
            /usr/lib32/libunwind.so.1*)
        install(FILES ${LIBCXX_LIBS} DESTINATION bin/libs)
    else()
        set(LIBCXX_LIBS /usr/lib32/libc++.a /usr/lib32/libc++abi.a /usr/lib32/libunwind.a)
    endif()
    install(FILES ${LIBCXX_LIBS} DESTINATION sdk/lib)
endif()