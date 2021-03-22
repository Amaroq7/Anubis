project(engine_api)

set(SRC_FILES
        Edict.cpp
        EntVars.cpp
        Funcs.cpp
        Engine.cpp
        Globals.cpp
        TraceResult.cpp
        Hooks.cpp
        Callbacks.cpp
        ValveInterface.cpp
        ReFuncs.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/public
        ${CMAKE_SOURCE_DIR}/rehlds
        ${CMAKE_SOURCE_DIR}/rehlds/common
        ${CMAKE_SOURCE_DIR}/rehlds/dlls
        ${CMAKE_SOURCE_DIR}/rehlds/engine
        ${CMAKE_SOURCE_DIR}/rehlds/pm_shared
        ${CMAKE_SOURCE_DIR}/rehlds/public)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror -fPIC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _vsnprintf=vsnprintf)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
endif()