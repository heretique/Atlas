add_library(Lights STATIC
    PRIVATE
        Private/Light.cpp
    PUBLIC
        Public/Lights/Light.h
        Public/Lights/LightComponent.h
    )

add_library(Atlas::Lights ALIAS Lights)

target_include_directories(Lights
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Lights Atlas::Assets)
