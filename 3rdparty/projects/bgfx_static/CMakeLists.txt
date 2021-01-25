cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds

add_library(bgfx_static STATIC "")

target_sources(bgfx_static PRIVATE ../../bgfx/src/bgfx.cpp
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
    ../../bgfx/src/renderer_nvn.cpp
    ../../bgfx/src/renderer_vk.cpp
    ../../bgfx/src/renderer_webgpu.cpp
    ../../bgfx/src/shader_dx9bc.cpp
    ../../bgfx/src/shader_dxbc.cpp
    ../../bgfx/src/shader_spirv.cpp
    ../../bgfx/src/shader.cpp
    ../../bgfx/src/topology.cpp
    ../../bgfx/src/vertexlayout.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_include_directories(bgfx_static PRIVATE ../../bgfx/3rdparty/dxsdk/include)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#        target_include_directories(bx PUBLIC ../../bx/include/compat/mingw)
    endif()
#    target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_DEBUG=1)
endif()


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_sources(bgfx_static PRIVATE ../../bgfx/src/glcontext_eagl.mm
        ../../bgfx/src/glcontext_nsgl.mm
        ../../bgfx/src/renderer_mtl.mm)

    target_compile_definitions(bgfx_static PRIVATE BGFX_CONFIG_RENDERER_OPENGL)
endif()

target_include_directories(bgfx_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../bgfx/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../bgfx/src
    ../../bgfx/3rdparty
    ../../bgfx/3rdparty/khronos)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_compile_definitions(bgfx_static PRIVATE BGFX_CONFIG_DEBUG=1 BGFX_CONFIG_RENDERER_OPENGLES=31)
endif()

target_link_libraries(bgfx_static bx bimg)
