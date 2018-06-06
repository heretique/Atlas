cmake_minimum_required(VERSION 3.5)

add_library(wrenpp STATIC "")
add_library(atlas::wrenpp ALIAS wrenpp)

target_sources(wrenpp PRIVATE ../../wrenpp/Wren++.cpp)

target_include_directories(wrenpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(wrenpp wren)
target_compile_features(wrenpp PUBLIC cxx_std_17)
