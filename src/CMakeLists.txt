project(metamod)

add_subdirectory(entities)
add_subdirectory(engine)
add_subdirectory(gamelib)

set(SRC_FILES
        EngineExports.cpp
        DllExports.cpp
        Metamod.cpp
        Module.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${YAML_CPP_INCLUDE_DIR}
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
    target_link_libraries(${PROJECT_NAME} PRIVATE dl ${YAML_CPP_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${YAML_CPP_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86 /EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8 /SECTION:.data,RW)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE engine_api gamelib_api cstrike_api valve_api ${YAML_CPP_LIBRARIES})

add_dependencies(${PROJECT_NAME} engine_api gamelib_api cstrike_api valve_api ${YAML_CPP_LIBRARIES})
