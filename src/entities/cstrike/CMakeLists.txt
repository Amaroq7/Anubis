project(cstrike_api)

set(SRC_FILES
        BaseEntity.cpp
        BasePlayer.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/common
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/dlls
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/engine
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/game_shared
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/pm_shared
        ${CMAKE_SOURCE_DIR}/regamedll_cs_sdk/public)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror -fPIC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()