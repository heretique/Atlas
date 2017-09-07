import qbs
import qbs.ModUtils

StaticLibrary {
    id: bgfx
    name: "bgfx"
    files: [
        // "bgfx/src/amalgamated.cpp",
        "../../bgfx/include/bgfx/*.h",
        "../../bgfx/src/*.cpp"
    ]
    excludeFiles: [
        "../../bgfx/src/amalgamated.cpp"
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
