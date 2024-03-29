project(engine_api)

set(SRC_FILES
        Edict.cpp
        Library.cpp
        TraceResult.cpp
        Hooks.cpp
        Callbacks.cpp
        ReHooks.cpp
        GameClient.cpp
        Cvar.cpp
        ValveInterface.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        SYSTEM
        PRIVATE
        ${FMT_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/anubis
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/rehlds
        ${CMAKE_SOURCE_DIR}/rehlds/common
        ${CMAKE_SOURCE_DIR}/rehlds/dlls
        ${CMAKE_SOURCE_DIR}/rehlds/engine
        ${CMAKE_SOURCE_DIR}/rehlds/pm_shared
        ${CMAKE_SOURCE_DIR}/rehlds/public)

target_compile_definitions(${PROJECT_NAME} PRIVATE ANUBIS_CORE)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)

    if (IS_CLANG_COMPILER)
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:external
                                                   /external:I ${CMAKE_SOURCE_DIR}/rehlds
                                                   /external:W0
                                                   /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        VISIBILITY_INLINES_HIDDEN ON
        CXX_VISIBILITY_PRESET hidden)
