add_library(Ecs STATIC
    PRIVATE
        Private/EcsManager.cpp
    PUBLIC
        Public/Ecs/EcsManager.h
        Public/Ecs/ISystem.h
    )

add_library(Atlas::Ecs ALIAS Ecs)

target_include_directories(Ecs
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Ecs Atlas::Core Atlas::Camera Atlas::Utils)
