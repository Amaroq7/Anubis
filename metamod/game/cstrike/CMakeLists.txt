project(cstrike_api)

set(SRC_FILES
        VFuncCallbacks.cpp
        EntitiesHooks.cpp
        EntityHolder.cpp
        MetaExports.cpp
        BaseEntity.cpp
        BasePlayer.cpp
        ValveInterface.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        SYSTEM
        PRIVATE
        ${CMAKE_SOURCE_DIR}/metamod
        ${CMAKE_SOURCE_DIR}/metamod/game/entities
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/common
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/dlls
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/engine
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/game_shared
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/pm_shared
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/public)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -Wl,--disable-new-dtags)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Wall -Werror -Wextra -Wpedantic -pedantic-errors)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)

    if (NOT DYNAMIC_BUILD)
        if (IS_CLANG_COMPILER)
            target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
            target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=${LLD} -Wl,--as-needed)
            target_link_libraries(${PROJECT_NAME} PRIVATE c++.a c++abi.a unwind.a m pthread)
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
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE META_DONT_INCLUDE_ENUM_HELPERS)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        VISIBILITY_INLINES_HIDDEN ON
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH ON
        BUILD_RPATH_USE_ORIGIN ON
        INSTALL_RPATH ../libs
        CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin/dlls
        LIBRARY DESTINATION bin/dlls
)
