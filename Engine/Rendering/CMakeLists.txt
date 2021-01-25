add_library(Rendering STATIC
    PRIVATE
        Private/RenderSystem.cpp
    PUBLIC
        Public/Rendering/RenderSystem.h
    )

add_library(Atlas::Rendering ALIAS Rendering)

target_include_directories(Rendering
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Rendering Atlas::Core Atlas::Ecs Atlas::Materials Atlas::Geometry Atlas::DebugDraw)
