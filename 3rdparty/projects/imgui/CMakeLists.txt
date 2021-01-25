cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds

add_library(imgui STATIC "")
add_library(Atlas::ImGui ALIAS imgui)

target_sources(imgui PRIVATE
    ../../imgui/imgui_draw.cpp
    ../../imgui/imgui.cpp
    ../../imgui/imgui_demo.cpp
    ../../imgui/imgui_tables.cpp
    ../../imgui/imgui_widgets.cpp)

target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../imgui>
    $<INSTALL_INTERFACE:include>)
