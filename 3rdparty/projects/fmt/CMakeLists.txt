cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds

add_library(fmt STATIC "")
add_library(Atlas::Fmt ALIAS fmt)

target_sources(fmt PRIVATE
    ../../fmt/format.cc
    ../../fmt/os.cc)

target_include_directories(fmt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../..)

target_compile_features(fmt PUBLIC cxx_std_17)
