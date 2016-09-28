import qbs
import qbs.ModUtils

StaticLibrary {
    id: bgfx
    name: "bgfx"
    files: [
       // "bgfx/src/amalgamated.cpp",
        "../../bgfx/include/bgfx/bgfxdefines.h",
        "../../bgfx/include/bgfx/bgfxplatform.h",
        "../../bgfx/include/bgfx/bgfx.h",
        "../../bgfx/src/bgfx.cpp",
        "../../bgfx/src/glcontext_egl.cpp",
        "../../bgfx/src/glcontext_glx.cpp",
        "../../bgfx/src/glcontext_ppapi.cpp",
        "../../bgfx/src/glcontext_wgl.cpp",
        "../../bgfx/src/image.cpp",
        "../../bgfx/src/hmd.cpp",
        "../../bgfx/src/hmd_ovr.cpp",
        "../../bgfx/src/hmd_openvr.cpp",
        "../../bgfx/src/debug_renderdoc.cpp",
        "../../bgfx/src/renderer_d3d11.cpp",
        "../../bgfx/src/renderer_d3d12.cpp",
        "../../bgfx/src/renderer_d3d9.cpp",
        "../../bgfx/src/renderer_gl.cpp",
        "../../bgfx/src/renderer_noop.cpp",
        "../../bgfx/src/renderer_vk.cpp",
        "../../bgfx/src/shader_dx9bc.cpp",
        "../../bgfx/src/shader_dxbc.cpp",
        "../../bgfx/src/shader_spirv.cpp",
        "../../bgfx/src/vertexdecl.cpp",
        "../../bgfx/src/topology.cpp"
    ]
    Depends { name: "cpp" }

    cpp.includePaths: [ "../../bx/include",
        "../../bgfx/include",
        "../../bgfx/3rdparty",
        "../../bgfx/3rdparty/khronos",
        "../../bgfx/3rdparty/dxsdk/include"
    ]
    cpp.cxxLanguageVersion: "c++14"
    cpp.defines: ["BGFX_CONFIG_DEBUG", "__STDC_FORMAT_MACROS"]
//    cpp.defines: ["BGFX_CONFIG_RENDERER_OPENGLES"]

    Properties {
        condition: qbs.toolchain.contains("msvc")
        cpp.systemIncludePaths: outer.uniqueConcat([ "../../bx/include/compat/msvc" ])
    }

    Properties {
        condition: qbs.toolchain.contains("mingw")
        cpp.systemIncludePaths: outer.uniqueConcat([ "../../bx/include/compat/mingw" ])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
