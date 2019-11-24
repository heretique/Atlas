cmake_minimum_required(VERSION 3.5)

add_library(fmt STATIC "")
add_library(atlas::fmt ALIAS fmt)

target_sources(fmt PRIVATE ../../fmt/format.cc
    ../../fmt/ostream.cc
    ../../fmt/posix.cc)

target_include_directories(fmt PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../..)

target_compile_features(fmt PUBLIC cxx_std_17)
