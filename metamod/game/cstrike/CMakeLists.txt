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
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE META_DONT_INCLUDE_ENUM_HELPERS)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        VISIBILITY_INLINES_HIDDEN ON
        CXX_VISIBILITY_PRESET hidden)
