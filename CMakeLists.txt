cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds

#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

#if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CMAKE_TOOLCHAIN_FILE)
#endif()

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Atlas VERSION 0.1.0 LANGUAGES CXX C)
include(CMakeToolsHelpers OPTIONAL)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#option(ATLAS_SHARED_LIBS "Implement modules as shared libs" ON)
option(ATLAS_USE_PCH "Use precompiled headers" ON)
option(ATLAS_USE_UNITY_BUILDS "Use unity builds" OFF)
option(ATLAS_BUILD_TESTS "Enable this to build tests" ON)
option(ATLAS_BUILD_PLAYGROUND "Enable this to build playground projects" ON)

#if (USE_PCH)
#  if (BUILD_STATIC OR BUILD_WITH_STATIC_RUNTIME_LIBS)
#    set(USE_PCH FALSE)
#  endif()
#endif()

#set(LIB_TYPE STATIC)

#if (ATLAS_SHARED_LIBS)
#    set(LIB_TYPE SHARED)
#    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#endif()

add_subdirectory(3rdparty/hq)
add_subdirectory(3rdparty/psmove)
add_subdirectory(3rdparty/projects/bx)
add_subdirectory(3rdparty/projects/bimg)
add_subdirectory(3rdparty/projects/bgfx)
#add_subdirectory(3rdparty/projects/bgfx_static)
add_subdirectory(3rdparty/projects/bgfx_shaderc)
#add_subdirectory(3rdparty/projects/easy_profiler)
add_subdirectory(Engine)

if (ATLAS_BUILD_TESTS)
    add_subdirectory(Tests)
endif()

if (ATLAS_BUILD_PLAYGROUND)
    add_subdirectory(Playground)
endif()

