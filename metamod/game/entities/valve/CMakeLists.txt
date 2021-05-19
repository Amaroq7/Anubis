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
        ${CMAKE_SOURCE_DIR}/metamod/game/entities
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/rehlds/common
        ${CMAKE_SOURCE_DIR}/rehlds/dlls
        ${CMAKE_SOURCE_DIR}/rehlds/engine
        ${CMAKE_SOURCE_DIR}/rehlds/pm_shared
        ${CMAKE_SOURCE_DIR}/rehlds/public)

if (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror -Wall -Wextra -Wpedantic -pedantic-errors)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        VISIBILITY_INLINES_HIDDEN ON
        CXX_VISIBILITY_PRESET hidden)
