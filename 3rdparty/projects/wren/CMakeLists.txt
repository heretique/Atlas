cmake_minimum_required(VERSION 3.5)

add_library(wren STATIC "")
add_library(atlas::wren ALIAS wren)

target_sources(wren PRIVATE ../../wren/src/optional/wren_opt_meta.c
    ../../wren/src/optional/wren_opt_random.c
    ../../wren/src/vm/wren_compiler.c
    ../../wren/src/vm/wren_core.c
    ../../wren/src/vm/wren_debug.c
    ../../wren/src/vm/wren_primitive.c
    ../../wren/src/vm/wren_utils.c
    ../../wren/src/vm/wren_value.c
    ../../wren/src/vm/wren_vm.c)

target_include_directories(wren PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../wren/src/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE ../../wren/src/vm
    ../../wren/src/optional)
