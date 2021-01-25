add_library(Scripting STATIC
    PRIVATE
        Private/Script.cpp
    PUBLIC
        Public/Scripting/Script.h
    )

add_library(Atlas::Scripting ALIAS Scripting)


target_include_directories(Scripting
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Scripting Atlas::Assets)
