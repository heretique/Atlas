import qbs
import qbs.ModUtils

StaticLibrary {
    id: bgfx
    name: "bgfx"
    files: [
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
    Depends { name: "common" }
    Depends { name: "bx" }
    Depends { name: "bimg" }
//    Depends { name: "easy_profiler" }

    cpp.includePaths: [
        path + "/../../bgfx/include",
        path + "/../../bgfx/3rdparty",
        path + "/../../bgfx/3rdparty/dxsdk/include"
    ]
    cpp.cxxLanguageVersion: "c++14"
    cpp.defines: ["BUILD_WITH_EASY_PROFILER"/*, "BGFX_CONFIG_DEBUG", "__STDC_FORMAT_MACROS"*/]

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.defines:  outer.uniqueConcat(["BGFX_CONFIG_RENDERER_OPENGLES"])
    }

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.includePaths: outer.uniqueConcat([
            path + "/../../bgfx/3rdparty/khronos"
        ])
                //
        cpp.defines:  outer.uniqueConcat(["BGFX_CONFIG_RENDERER_OPENGL"])
    }


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
