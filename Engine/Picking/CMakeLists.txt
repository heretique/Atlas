add_library(Picking STATIC
    PRIVATE
        Private/PickingSystem.cpp
    PUBLIC
        Public/Picking/PickingSystem.h
    )

add_library(Atlas::Picking ALIAS Picking)


target_include_directories(Picking
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Picking Atlas::Core Atlas::Input Atlas::Ecs Atlas::DebugDraw Atlas::Engine)
