cmake_minimum_required(VERSION 3.5)

add_library(bgfx STATIC "")
add_library(atlas::bgfx ALIAS bgfx)

target_sources(bgfx PRIVATE ../../bgfx/src/bgfx.cpp
    ../../bgfx/src/debug_renderdoc.cpp
    ../../bgfx/src/dxgi.cpp
    ../../bgfx/src/glcontext_egl.cpp
    ../../bgfx/src/glcontext_glx.cpp
    ../../bgfx/src/glcontext_wgl.cpp
    ../../bgfx/src/nvapi.cpp
    ../../bgfx/src/renderer_d3d9.cpp
    ../../bgfx/src/renderer_d3d11.cpp
    ../../bgfx/src/renderer_d3d12.cpp
    ../../bgfx/src/renderer_gl.cpp
    ../../bgfx/src/renderer_gnm.cpp
    ../../bgfx/src/renderer_noop.cpp
    ../../bgfx/src/renderer_vk.cpp
    ../../bgfx/src/shader_dx9bc.cpp
    ../../bgfx/src/shader_dxbc.cpp
    ../../bgfx/src/shader_spirv.cpp
    ../../bgfx/src/shader.cpp
    ../../bgfx/src/topology.cpp
    ../../bgfx/src/vertexdecl.cpp)

message("System name: ${CMAKE_SYSTEM_NAME}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_sources(bgfx PRIVATE ../../bgfx/src/glcontext_eagl.mm
        ../../bgfx/src/glcontext_nsgl.mm
        ../../bgfx/src/renderer_mtl.mm)

    target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_RENDERER_OPENGL)
endif()

target_include_directories(bgfx PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../bgfx/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../bgfx/src
    ../../bgfx/3rdparty
    ../../bgfx/3rdparty/khronos)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_include_directories(bgfx BEFORE PUBLIC /opt/vc/include)
    target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_RENDERER_OPENGLES)
endif()

target_link_libraries(bgfx bx bimg)
