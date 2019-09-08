cmake_minimum_required(VERSION 3.5)

add_library(bx STATIC "")
add_library(atlas::bx ALIAS bx)

target_sources(bx PRIVATE ../../bx/src/allocator.cpp
    ../../bx/src/bx.cpp
    ../../bx/src/commandline.cpp
    ../../bx/src/crtnone.cpp
    ../../bx/src/debug.cpp
    ../../bx/src/dtoa.cpp
    ../../bx/src/easing.cpp
    ../../bx/src/file.cpp
    ../../bx/src/filepath.cpp
    ../../bx/src/hash.cpp
    ../../bx/src/math.cpp
    ../../bx/src/mutex.cpp
    ../../bx/src/os.cpp
    ../../bx/src/process.cpp
    ../../bx/src/semaphore.cpp
    ../../bx/src/settings.cpp
    ../../bx/src/sort.cpp
    ../../bx/src/string.cpp
    ../../bx/src/thread.cpp
    ../../bx/src/timer.cpp
    ../../bx/src/url.cpp)

target_include_directories(bx PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../bx/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../bx/src ../../bx/3rdparty)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_include_directories(bx PUBLIC ../../bx/include/compat/msvc)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_include_directories(bx PUBLIC ../../bx/include/compat/mingw)
    endif()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_include_directories(bx PUBLIC ../../bx/include/compat/osx)
endif()

target_compile_features(bx PUBLIC cxx_std_14)

target_compile_definitions(bx PUBLIC __STDC_LIMIT_MACROS
    __STDC_FORMAT_MACROS
    __STDC_CONSTANT_MACROS)
