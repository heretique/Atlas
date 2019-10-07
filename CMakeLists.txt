cmake_minimum_required(VERSION 3.5)

#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

#if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CMAKE_TOOLCHAIN_FILE)
#endif()

project(atlas VERSION 0.1.0 LANGUAGES CXX C)
include(CMakeToolsHelpers OPTIONAL)

add_subdirectory(hq hq)
add_subdirectory(3rdparty/projects/bx)
add_subdirectory(3rdparty/projects/bimg)
add_subdirectory(3rdparty/projects/bgfx)
add_subdirectory(3rdparty/projects/bgfx_shaderc)
#add_subdirectory(3rdparty/projects/easy_profiler)
add_subdirectory(3rdparty/projects/fmt)
add_subdirectory(3rdparty/projects/imgui)
add_subdirectory(3rdparty/projects/wren)
add_subdirectory(3rdparty/projects/wrenpp)
add_subdirectory(Engine)
add_subdirectory(Tests)

