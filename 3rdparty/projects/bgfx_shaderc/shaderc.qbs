import qbs

CppApplication {
    consoleApplication: true
	name: "shaderc"
    cpp.cxxLanguageVersion: "c++14"
	Depends { name: "cpp" }
    Depends { name: "common" }
	Depends { name: "bx" }
	Depends { name: "fcpp" }
	Depends { name: "glslang" }
	Depends { name: "glsl-optimizer" }


    files: [
        "../../bgfx/src/shader_spirv.cpp",
        "../../bgfx/src/shader_spirv.h",
        "../../bgfx/src/vertexdecl.cpp",
        "../../bgfx/src/vertexdecl.h",
        "../../bgfx/tools/shaderc/shaderc.cpp",
        "../../bgfx/tools/shaderc/shaderc.h",
        "../../bgfx/tools/shaderc/shaderc_glsl.cpp",
        "../../bgfx/tools/shaderc/shaderc_hlsl.cpp",
        "../../bgfx/tools/shaderc/shaderc_pssl.cpp",
        "../../bgfx/tools/shaderc/shaderc_spirv.cpp",
        ]

    cpp.windowsApiCharacterSet: ""
    cpp.defines: [
        "__STDC_LIMIT_MACROS",
        "__STDC_FORMAT_MACROS",
        "__STDC_CONSTANT_MACROS"
        ]
    cpp.includePaths: [
        "../../bimg/include",
        "../../bx/include",
        "../../bgfx/3rdparty/dxsdk/include",
        "../../bgfx/3rdparty/fcpp",
        "../../bgfx/3rdparty/glsl-optimizer/include",
        "../../bgfx/3rdparty/glsl-optimizer/src/glsl",
        "../../bgfx/3rdparty/glslang",
        "../../bgfx/3rdparty/glslang/glslang/Include",
        "../../bgfx/3rdparty/glslang/glslang/Public",
        "../../bgfx/include",
        ]


    Properties {
        condition: qbs.targetOS.contains("darwin")
        cpp.frameworks: [ "Cocoa"]
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "home/pi/atlas"
    }

}
