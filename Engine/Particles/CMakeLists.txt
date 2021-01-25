add_library(Particles STATIC
    PRIVATE
        Private/ParticleEffect.cpp
        Private/ParticleSystem.cpp
    PUBLIC
        Public/Particles/ParticleEffect.h
        Public/Particles/ParticleEffectComponent.h
        Public/Particles/ParticleSystem.h
    )

add_library(Atlas::Particles ALIAS Particles)

target_include_directories(Particles
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Particles Atlas::Assets Atlas::Ecs)
