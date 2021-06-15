add_library(Core STATIC
    PRIVATE
        Private/Logger.cpp
        Private/MemoryManager.cpp
        Private/ReflectHq.cpp
        Private/SimpleMeshVertex.cpp
        Private/TransformComponent.cpp
    PUBLIC
        Public/Core/Component.h
        Public/Core/Debug.h
        Public/Core/Delegate.h
        Public/Core/Logger.h
        Public/Core/MemoryManager.h
        Public/Core/SimpleMeshVertex.h
        Public/Core/TransformComponent.h
    )
add_library(Atlas::Core ALIAS Core)

target_include_directories(Core
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        ../../3rdparty
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
    )




if (MSVC)
    target_compile_options(Core PRIVATE /bigobj)
endif()

#target_compile_definitions(Core PUBLIC BGFX_SHARED_LIB_USE=1)

target_link_libraries(Core hq Atlas::BGFX Atlas::Reflection)
