import qbs
import qbs.ModUtils

StaticLibrary {
    id: bgfx
    name: "bgfx"
    files: [
        // "bgfx/src/amalgamated.cpp",
        "../../bgfx/include/bgfx/defines.h",
        "../../bgfx/include/bgfx/platform.h",
        "../../bgfx/include/bgfx/bgfx.h",
        "../../bgfx/src/bgfx.cpp",
        "../../bgfx/src/glcontext_egl.cpp",
        "../../bgfx/src/glcontext_glx.cpp",
        "../../bgfx/src/glcontext_ppapi.cpp",
        "../../bgfx/src/glcontext_wgl.cpp",
        "../../bgfx/src/hmd.cpp",
        "../../bgfx/src/hmd_ovr.cpp",
        "../../bgfx/src/hmd_openvr.cpp",
        "../../bgfx/src/debug_renderdoc.cpp",
        "../../bgfx/src/renderer_d3d11.cpp",
        "../../bgfx/src/renderer_d3d12.cpp",
        "../../bgfx/src/renderer_d3d9.cpp",
        "../../bgfx/src/renderer_gl.cpp",
        "../../bgfx/src/renderer_gnm.cpp",
        "../../bgfx/src/renderer_noop.cpp",
        "../../bgfx/src/renderer_vk.cpp",
        "../../bgfx/src/shader.cpp",
        "../../bgfx/src/shader_dx9bc.cpp",
        "../../bgfx/src/shader_dxbc.cpp",
        "../../bgfx/src/shader_spirv.cpp",
        "../../bgfx/src/vertexdecl.cpp",
        "../../bgfx/src/topology.cpp"
    ]

    Properties {
        condition: qbs.targetOS.contains("osx")
        files: outer.uniqueConcat([
                                      "../../bgfx/src/glcontext_eagl.mm",
                                      "../../bgfx/src/glcontext_nsgl.mm",
                                      "../../bgfx/src/renderer_mtl.mm",
                                  ])
    }

    Depends { name: "cpp" }
    Depends { name: "bx" }
    Depends { name: "bimg" }
    cpp.includePaths: [
        path + "/../../bgfx/include",
        path + "/../../bgfx/3rdparty",
        path + "/../../bgfx/3rdparty/khronos",
        path + "/../../bgfx/3rdparty/dxsdk/include"
    ]
    cpp.cxxLanguageVersion: "c++14"
    //    cpp.defines: ["BGFX_CONFIG_DEBUG", "__STDC_FORMAT_MACROS"]
    //    cpp.defines: ["BGFX_CONFIG_RENDERER_OPENGLES"]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            path + "/../../bgfx/include",
            path + "/../../bgfx/3rdparty",
            path + "/../../bgfx/3rdparty/khronos",
            path + "/../../bgfx/3rdparty/dxsdk/include"
        ]

    }




    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
