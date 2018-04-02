import qbs

StaticLibrary {
    id: glslang
	name: "glslang"

    files: [
        "../../bgfx/3rdparty/glslang/OGLCompilersDLL/InitializeDll.cpp",
        "../../bgfx/3rdparty/glslang/OGLCompilersDLL/InitializeDll.h",
        "../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.AMD.h",
        "../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.EXT.h",
        "../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.KHR.h",
        "../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.NV.h",
        "../../bgfx/3rdparty/glslang/SPIRV/GLSL.std.450.h",
        "../../bgfx/3rdparty/glslang/SPIRV/GlslangToSpv.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/GlslangToSpv.h",
        "../../bgfx/3rdparty/glslang/SPIRV/InReadableOrder.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/Logger.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/Logger.h",
        "../../bgfx/3rdparty/glslang/SPIRV/SPVRemapper.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/SPVRemapper.h",
        "../../bgfx/3rdparty/glslang/SPIRV/SpvBuilder.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/SpvBuilder.h",
        "../../bgfx/3rdparty/glslang/SPIRV/bitutils.h",
        "../../bgfx/3rdparty/glslang/SPIRV/disassemble.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/disassemble.h",
        "../../bgfx/3rdparty/glslang/SPIRV/doc.cpp",
        "../../bgfx/3rdparty/glslang/SPIRV/doc.h",
        "../../bgfx/3rdparty/glslang/SPIRV/hex_float.h",
        "../../bgfx/3rdparty/glslang/SPIRV/spvIR.h",
        "../../bgfx/3rdparty/glslang/glslang/GenericCodeGen/CodeGen.cpp",
        "../../bgfx/3rdparty/glslang/glslang/GenericCodeGen/Link.cpp",
        "../../bgfx/3rdparty/glslang/glslang/Include/BaseTypes.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/Common.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/ConstantUnion.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/InfoSink.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/InitializeGlobals.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/PoolAlloc.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/ResourceLimits.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/ShHandle.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/Types.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/arrays.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/intermediate.h",
        "../../bgfx/3rdparty/glslang/glslang/Include/revision.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Constant.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/InfoSink.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Initialize.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Initialize.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/IntermTraverse.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Intermediate.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/LiveTraverser.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ParseContextBase.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ParseHelper.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ParseHelper.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/PoolAlloc.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/RemoveTree.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/RemoveTree.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Scan.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Scan.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ScanContext.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ShaderLang.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/SymbolTable.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/SymbolTable.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Versions.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Versions.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/attribute.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/attribute.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/gl_types.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/intermOut.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/iomapper.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/iomapper.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/limits.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/linkValidate.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/localintermediate.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/parseConst.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/parseVersions.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.h",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/reflection.cpp",
        "../../bgfx/3rdparty/glslang/glslang/MachineIndependent/reflection.h",
        "../../bgfx/3rdparty/glslang/glslang/OSDependent/osinclude.h",
        "../../bgfx/3rdparty/glslang/glslang/Public/ShaderLang.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslAttributes.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslAttributes.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslGrammar.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslGrammar.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslOpMap.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslOpMap.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslParseHelper.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslParseHelper.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslParseables.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslParseables.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslScanContext.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslScanContext.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslTokenStream.cpp",
        "../../bgfx/3rdparty/glslang/hlsl/hlslTokenStream.h",
        "../../bgfx/3rdparty/glslang/hlsl/hlslTokens.h",
        ]


	Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++14"
    cpp.defines: [
        "__STDC_LIMIT_MACROS",
        "__STDC_FORMAT_MACROS",
        "__STDC_CONSTANT_MACROS",
        "_DEBUG",
        "ENABLE_HLSL=1",
        ]
    cpp.includePaths: [
        "../../../3rdparty/glslang",
        ]

    Properties {
        condition: qbs.targetOS.contains("windows")
        files: outer.uniqueConcat([
                                      "../../bgfx/3rdparty/glslang/glslang/OSDependent/Windows/ossource.cpp",
                                  ])
    }

    Properties {
        condition: qbs.targetOS.contains("osx")
        files: outer.uniqueConcat([
                                        "../../bgfx/3rdparty/glslang/glslang/OSDependent/Unix/ossource.cpp",
                                  ])
        cpp.commonCompilerFlagss: outer.uniqueConcat([
                                                         "-Wno-ignored-qualifiers",
                                                         "-Wno-missing-field-initializers",
                                                         "-Wno-reorder",
                                                         "-Wno-return-type",
                                                         "-Wno-shadow",
                                                         "-Wno-sign-compare",
                                                         "-Wno-undef",
                                                         "-Wno-unknown-pragmas",
                                                         "-Wno-unused-function",
                                                         "-Wno-unused-parameter",
                                                         "-Wno-unused-variable",
                                                         ])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }

}
