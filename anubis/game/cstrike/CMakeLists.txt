project(cstrike_api)

set(SRC_FILES
        VFuncCallbacks.cpp
        EntitiesHooks.cpp
        EntityHolder.cpp
        AnubisExports.cpp
        BaseEntity.cpp
        BasePlayer.cpp
        ValveInterface.cpp
        Rules.cpp
        BasePlayerItem.cpp
        BasePlayerWeapon.cpp
        BasePlayerAmmo.cpp
        Hooks.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        SYSTEM
        PRIVATE
        ${CMAKE_SOURCE_DIR}/anubis
        ${CMAKE_SOURCE_DIR}/anubis/game/entities
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/common
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/dlls
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/engine
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/game_shared
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/pm_shared
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/public
        ${CMAKE_BINARY_DIR}/anubis)

target_compile_definitions(${PROJECT_NAME} PRIVATE ANUBIS_ENTITY_DLL ANUBIS_DONT_INCLUDE_ENUM_HELPERS)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -Wl,--disable-new-dtags)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Wall -Werror -Wextra -Wpedantic -pedantic-errors)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)

    if (IS_CLANG_COMPILER)
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
        target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=${LLD})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LIBCPP_LIB} ${LLVM_LIBCPPABI_LIB} ${LLVM_UNWIND_LIB})
    endif ()
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:external
            /external:I ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk
            /external:W0
            /W4 /WX /wd4250)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif ()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        VISIBILITY_INLINES_HIDDEN ON
        SKIP_BUILD_RPATH OFF
        BUILD_WITH_INSTALL_RPATH ON
        BUILD_RPATH_USE_ORIGIN ON
        INSTALL_RPATH \$ORIGIN/../lib
        CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin/dlls
        LIBRARY DESTINATION bin/dlls
        )
