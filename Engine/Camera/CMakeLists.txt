add_library(Camera STATIC
    PRIVATE
        Private/Camera.cpp
    PUBLIC
        Public/Camera/Camera.h
    )

add_library(Atlas::Camera ALIAS Camera)

target_include_directories(Camera
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Camera Atlas::Core)
