cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds


project(spirv-cross)
set(
source_list
  ../../../bgfx/3rdparty/spirv-cross/spirv.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cfg.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cfg.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_common.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cpp.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cpp.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cross.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cross.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cross_parsed_ir.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cross_parsed_ir.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cross_util.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_cross_util.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_glsl.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_glsl.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_hlsl.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_hlsl.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_msl.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_msl.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_parser.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_parser.hpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_reflect.cpp
  ../../../bgfx/3rdparty/spirv-cross/spirv_reflect.hpp
)

include_directories(../../../bgfx/3rdparty/spirv-cross/include)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
#  add_definitions(-D_DEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64")
#endif()

#if(CMAKE_BUILD_TYPE MATCHES "Release")
#  add_definitions(-DNDEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64")
#endif()


add_library(spirv-cross STATIC ${source_list})

