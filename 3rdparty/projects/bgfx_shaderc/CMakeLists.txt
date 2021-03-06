cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds

project(bgfx_shaderc)

add_subdirectory(spirv-opt)
add_subdirectory(spirv-cross)
add_subdirectory(glslang)
add_subdirectory(glsl-optimizer)
add_subdirectory(fcpp)

set(
source_list
  ../../bgfx/src/shader_spirv.cpp
  ../../bgfx/src/shader_spirv.h
  ../../bgfx/src/vertexlayout.cpp
  ../../bgfx/src/vertexlayout.h
  ../../bgfx/tools/shaderc/shaderc.cpp
  ../../bgfx/tools/shaderc/include/shaderc/shaderc.h
  ../../bgfx/tools/shaderc/shaderc_private.h
  ../../bgfx/tools/shaderc/shaderc_glsl.cpp
  ../../bgfx/tools/shaderc/shaderc_hlsl.cpp
  ../../bgfx/tools/shaderc/shaderc_metal.cpp
  ../../bgfx/tools/shaderc/shaderc_pssl.cpp
  ../../bgfx/tools/shaderc/shaderc_spirv.cpp
)

include_directories(../../bx/include)
include_directories(../../bimg/include)
include_directories(../../bgfx/include)
include_directories(../../bgfx/3rdparty/dxsdk/include)
include_directories(../../bgfx/3rdparty/fcpp)
include_directories(../../bgfx/3rdparty/glslang/glslang/Public)
include_directories(../../bgfx/3rdparty/glslang/glslang/Include)
include_directories(../../bgfx/3rdparty/glslang)
include_directories(../../bgfx/3rdparty/glsl-optimizer/include)
include_directories(../../bgfx/3rdparty/glsl-optimizer/src/glsl)
include_directories(../../bgfx/3rdparty/spirv-cross)
include_directories(../../bgfx/3rdparty/spirv-tools/include)
include_directories(../../bgfx/3rdparty/webgpu/include)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
#  add_definitions(-D_DEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -fomit-frame-pointer -g -m64 -std=c++17 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64")
#endif()

#if(CMAKE_BUILD_TYPE MATCHES "Release")
#  add_definitions(-DNDEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -fomit-frame-pointer -g -O3 -m64 -std=c++17 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64")
#endif()

#add_library(shaderc_lib STATIC ${source_list})
#target_include_directories(shaderc_lib PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../bgfx/tools/shaderc/include>
#    $<INSTALL_INTERFACE:include>
#    PRIVATE ../../bgfx/tools/shaderc/include/shaderc)
#target_link_libraries(shaderc_lib bx fcpp glslang glsl-optimizer spirv-opt spirv-cross)

add_executable(shaderc ${source_list})
target_compile_definitions(shaderc PRIVATE SHADERC_STANDALONE)
target_include_directories(shaderc PRIVATE ../../bgfx/tools/shaderc/include/shaderc)
target_link_libraries(shaderc bx bgfx fcpp glslang glsl-optimizer spirv-opt spirv-cross)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    find_package(Threads REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_library(DL_LIBRARY dl)
    find_library(RT_LIBRARY rt)

    target_link_libraries(shaderc Threads::Threads ${RT_LIBRARY} ${DL_LIBRARY} EGL GLESv2 ${X11_LIBRARIES})
endif()

#add_custom_command(TARGET shaderc
#                   POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:shaderc> $<TARGET_FILE_DIR:shaderc>/../../../Engine)
