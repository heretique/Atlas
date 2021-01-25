add_library(Engine STATIC
	PRIVATE
        Private/Engine.cpp
	PUBLIC
        Public/Engine/Engine.h
	)

add_library(Atlas::Engine ALIAS Engine)

target_include_directories(Engine
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Engine
    Atlas::Core
    Atlas::Animation
    Atlas::Assets
    Atlas::Camera
    Atlas::Ecs
    Atlas::Geometry
    Atlas::Input
    Atlas::Lights
    Atlas::Materials
    Atlas::Particles
    Atlas::Picking
    Atlas::Plugins
    Atlas::Rendering
    Atlas::Scripting
    Atlas::UI
    Atlas::Utils)
