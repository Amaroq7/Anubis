project(valve_api)

set(SRC_FILES
        VFuncCallbacks.cpp
        EntitiesHooks.cpp
        BaseEntity.cpp
        MetaExports.cpp
        EntityHolder.cpp
        BasePlayer.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        SYSTEM
        PRIVATE
        ${FMT_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/metamod
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/rehlds/common
        ${CMAKE_SOURCE_DIR}/rehlds/dlls
        ${CMAKE_SOURCE_DIR}/rehlds/engine
        ${CMAKE_SOURCE_DIR}/rehlds/pm_shared
        ${CMAKE_SOURCE_DIR}/rehlds/public)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -Wl,--disable-new-dtags)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Wall -Werror -Wextra -Wpedantic -pedantic-errors)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)

    if (NOT DYNAMIC_BUILD)
        if (IS_CLANG_COMPILER)
            target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
            target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=${LLD} -Wl,--as-needed)
            target_link_libraries(${PROJECT_NAME} PRIVATE c++.a c++abi.a unwind.a m)
        else()
            target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
            if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0.0")
                target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
            endif()
        endif()
    else()
        if (IS_CLANG_COMPILER)
            target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
            target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=${LLD} -Wl,--as-needed)
            target_link_libraries(${PROJECT_NAME} PRIVATE c++ c++abi unwind)
        endif()
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:external
                                                   /external:I ${CMAKE_SOURCE_DIR}/rehlds
                                                   /external:W0
                                                   /W4 /WX /wd4250)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        VISIBILITY_INLINES_HIDDEN ON
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH ON
        BUILD_RPATH_USE_ORIGIN ON
        INSTALL_RPATH ../lib
        CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin/dlls
        LIBRARY DESTINATION bin/dlls
)
