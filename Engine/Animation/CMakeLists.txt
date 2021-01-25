add_library(Animation STATIC
    PRIVATE
        Private/AnimationSystem.cpp
    PUBLIC
        Public/Animation/AnimationSystem.h
    )
add_library(Atlas::Animation ALIAS Animation)

target_include_directories(Animation
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>)

target_link_libraries(Animation Atlas::Core Atlas::Ecs)
