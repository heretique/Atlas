cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds

project(fcpp)
set(
source_list
  ../../../bgfx/3rdparty/fcpp/FPPBase.h
  ../../../bgfx/3rdparty/fcpp/FPP_protos.h
  ../../../bgfx/3rdparty/fcpp/cpp.h
  ../../../bgfx/3rdparty/fcpp/cpp1.c
  ../../../bgfx/3rdparty/fcpp/cpp2.c
  ../../../bgfx/3rdparty/fcpp/cpp3.c
  ../../../bgfx/3rdparty/fcpp/cpp4.c
  ../../../bgfx/3rdparty/fcpp/cpp5.c
  ../../../bgfx/3rdparty/fcpp/cpp6.c
  ../../../bgfx/3rdparty/fcpp/cppadd.h
  ../../../bgfx/3rdparty/fcpp/cppdef.h
  ../../../bgfx/3rdparty/fcpp/fpp.h
  ../../../bgfx/3rdparty/fcpp/fpp_pragmas.h
)

add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DNINCLUDE=64)
add_definitions(-DNWORK=65536)
add_definitions(-DNBUFF=65536)
add_definitions(-DOLD_PREPROCESSOR=0)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
#  add_definitions(-D_DEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions -Wno-implicit-fallthrough -Wno-incompatible-pointer-types -Wno-parentheses-equality")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64 -Wno-implicit-fallthrough -Wno-incompatible-pointer-types -Wno-parentheses-equality")
#endif()

#if(CMAKE_BUILD_TYPE MATCHES "Release")
#  add_definitions(-DNDEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions -Wno-implicit-fallthrough -Wno-incompatible-pointer-types -Wno-parentheses-equality")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64 -Wno-implicit-fallthrough -Wno-incompatible-pointer-types -Wno-parentheses-equality")
#endif()


add_library(fcpp STATIC ${source_list})


