cmake_minimum_required(VERSION 3.5)

add_library(easy_profiler STATIC "")
add_library(atlas::easy_profiler ALIAS easy_profiler)

target_sources(easy_profiler PRIVATE ../../easy_profiler/easy_profiler_core/block.cpp
    ../../easy_profiler/easy_profiler_core/easy_socket.cpp
    ../../easy_profiler/easy_profiler_core/event_trace_win.cpp
    ../../easy_profiler/easy_profiler_core/nonscoped_block.cpp
    ../../easy_profiler/easy_profiler_core/profile_manager.cpp
    ../../easy_profiler/easy_profiler_core/reader.cpp
    ../../easy_profiler/easy_profiler_core/thread_storage.cpp)

target_include_directories(easy_profiler PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../easy_profiler/easy_profiler_core/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../easy_profiler/easy_profiler_core)

target_compile_features(easy_profiler PUBLIC cxx_std_17)

target_compile_definitions(easy_profiler PRIVATE EASY_PROFILER_STATIC
    BUILD_WITH_EASY_PROFILER
    EASY_PROFILER_VERSION_MAJOR=1
    EASY_PROFILER_VERSION_MINOR=3
    EASY_PROFILER_VERSION_PATCH=0
    # a hack to make easy_profiler compile
    EASY_CONSTEXPR=const
    EASY_STATIC_CONSTEXPR=static const
    EASY_CONSTEXPR_FCN=)
