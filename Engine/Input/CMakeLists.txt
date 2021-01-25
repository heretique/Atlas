add_library(Input STATIC
    PRIVATE
        Private/InputManager.cpp
    PUBLIC
        Public/Input/InputManager.h
    )

add_library(Atlas::Input ALIAS Input)

target_include_directories(Input
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Input Atlas::Core Atlas::SDL)
