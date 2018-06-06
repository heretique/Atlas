cmake_minimum_required(VERSION 3.5)

add_library(imgui STATIC "")
add_library(atlas::imgui ALIAS imgui)

target_sources(imgui PRIVATE ../../imgui/imgui_draw.cpp
    ../../imgui/imgui.cpp)

target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../imgui>
    $<INSTALL_INTERFACE:include>)
