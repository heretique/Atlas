cmake_minimum_required(VERSION 3.5)

add_library(bimg STATIC "")
add_library(atlas::bimg ALIAS bimg)


target_sources(bimg PRIVATE
    ../../bimg/3rdparty/astc-codec/src/decoder/astc_file.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/astc_file.h
    ../../bimg/3rdparty/astc-codec/src/decoder/codec.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/codec.h
    ../../bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/endpoint_codec.h
    ../../bimg/3rdparty/astc-codec/src/decoder/footprint.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/footprint.h
    ../../bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/integer_sequence_codec.h
    ../../bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/intermediate_astc_block.h
    ../../bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/logical_astc_block.h
    ../../bimg/3rdparty/astc-codec/src/decoder/partition.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/partition.h
    ../../bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/physical_astc_block.h
    ../../bimg/3rdparty/astc-codec/src/decoder/quantization.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/quantization.h
    ../../bimg/3rdparty/astc-codec/src/decoder/weight_infill.cc
    ../../bimg/3rdparty/astc-codec/src/decoder/weight_infill.h
    ../../bimg/src/image_decode.cpp
    ../../bimg/src/image_encode.cpp
    ../../bimg/src/image_gnf.cpp
    ../../bimg/src/image.cpp
    ../../bimg/src/image_cubemap_filter)

target_include_directories(bimg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../bimg/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../bimg/src
    ../../bimg/3rdparty
    ../../bimg/3rdparty/iqa/include
    ../../bimg/3rdparty/astc-codec
    ../../bimg/3rdparty/astc-codec/include)

target_link_libraries(bimg bx)
