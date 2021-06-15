add_library(Materials STATIC
    PRIVATE
        Private/Material.cpp
        Private/MaterialComponent.cpp
        Private/Shader.cpp
        Private/Texture.cpp
    PUBLIC
        Public/Materials/Material.h
        Public/Materials/MaterialComponent.h
        Public/Materials/Shader.h
        Public/Materials/Texture.h
    )

add_library(Atlas::Materials ALIAS Materials)

target_include_directories(Materials
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Materials Atlas::Core Atlas::Assets Atlas::Utils RTTR::Core)
