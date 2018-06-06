cmake_minimum_required(VERSION 3.5)

add_library(bimg STATIC "")
add_library(atlas::bimg ALIAS bimg)

target_sources(bimg PRIVATE ../../bimg/src/image_decode.cpp
    ../../bimg/src/image_encode.cpp
    ../../bimg/src/image_gnf.cpp
    ../../bimg/src/image.cpp)

target_include_directories(bimg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../bimg/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../bimg/src
    ../../bimg/3rdparty
    ../../bimg/3rdparty/iqa/include)

target_link_libraries(bimg bx)
