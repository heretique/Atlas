add_library(Plugins STATIC
    PRIVATE
        Private/PluginManager.cpp
    PUBLIC
        Public/Plugins/PluginManager.h
    )

add_library(Atlas::Plugins ALIAS Plugins)


target_include_directories(Plugins
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Plugins Atlas::Core)
