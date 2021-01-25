cmake_minimum_required(VERSION 3.16) # for precompiled headers and unity builds


project(glslang)
set(
source_list
  ../../../bgfx/3rdparty/glslang/OGLCompilersDLL/InitializeDll.cpp
  ../../../bgfx/3rdparty/glslang/OGLCompilersDLL/InitializeDll.h
  ../../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.AMD.h
  ../../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.EXT.h
  ../../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.KHR.h
  ../../../bgfx/3rdparty/glslang/SPIRV/GLSL.ext.NV.h
  ../../../bgfx/3rdparty/glslang/SPIRV/GLSL.std.450.h
  ../../../bgfx/3rdparty/glslang/SPIRV/GlslangToSpv.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/GlslangToSpv.h
  ../../../bgfx/3rdparty/glslang/SPIRV/InReadableOrder.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/Logger.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/Logger.h
  ../../../bgfx/3rdparty/glslang/SPIRV/NonSemanticDebugPrintf.h
  ../../../bgfx/3rdparty/glslang/SPIRV/SPVRemapper.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/SPVRemapper.h
  ../../../bgfx/3rdparty/glslang/SPIRV/SpvBuilder.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/SpvBuilder.h
  ../../../bgfx/3rdparty/glslang/SPIRV/SpvPostProcess.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/SpvTools.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/SpvTools.h
  ../../../bgfx/3rdparty/glslang/SPIRV/bitutils.h
  ../../../bgfx/3rdparty/glslang/SPIRV/disassemble.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/disassemble.h
  ../../../bgfx/3rdparty/glslang/SPIRV/doc.cpp
  ../../../bgfx/3rdparty/glslang/SPIRV/doc.h
  ../../../bgfx/3rdparty/glslang/SPIRV/hex_float.h
  ../../../bgfx/3rdparty/glslang/SPIRV/spvIR.h
  ../../../bgfx/3rdparty/glslang/glslang/GenericCodeGen/CodeGen.cpp
  ../../../bgfx/3rdparty/glslang/glslang/GenericCodeGen/Link.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslAttributes.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslAttributes.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslGrammar.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslGrammar.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslOpMap.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslOpMap.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslParseHelper.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslParseHelper.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslParseables.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslParseables.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslScanContext.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslScanContext.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslTokenStream.cpp
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslTokenStream.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/hlslTokens.h
  ../../../bgfx/3rdparty/glslang/glslang/HLSL/pch.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/BaseTypes.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/Common.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/ConstantUnion.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/InfoSink.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/InitializeGlobals.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/PoolAlloc.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/ResourceLimits.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/ShHandle.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/Types.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/arrays.h
  ../../../bgfx/3rdparty/glslang/glslang/Include/intermediate.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Constant.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/InfoSink.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Initialize.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Initialize.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/IntermTraverse.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Intermediate.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/LiveTraverser.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ParseContextBase.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ParseHelper.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ParseHelper.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/PoolAlloc.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/RemoveTree.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/RemoveTree.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Scan.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Scan.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ScanContext.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/ShaderLang.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/SymbolTable.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/SymbolTable.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Versions.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/Versions.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/attribute.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/attribute.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/gl_types.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/glslang_tab.cpp.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/intermOut.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/iomapper.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/iomapper.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/limits.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/linkValidate.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/localintermediate.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/parseConst.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/parseVersions.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/pch.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpContext.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/preprocessor/PpTokens.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/propagateNoContraction.h
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/reflection.cpp
  ../../../bgfx/3rdparty/glslang/glslang/MachineIndependent/reflection.h
  ../../../bgfx/3rdparty/glslang/glslang/OSDependent/Web/glslang.js.cpp
  ../../../bgfx/3rdparty/glslang/glslang/OSDependent/osinclude.h
  ../../../bgfx/3rdparty/glslang/glslang/Public/ShaderLang.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(source_list ${source_list}
        ../../../bgfx/3rdparty/glslang/glslang/OSDependent/Windows/ossource.cpp
        )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(source_list ${source_list}
        ../../../bgfx/3rdparty/glslang/glslang/OSDependent/Unix/ossource.cpp
        )
endif()

include_directories(../../../bgfx/3rdparty)
include_directories(../../../bgfx/3rdparty/glslang)
include_directories(../../../bgfx/3rdparty/spirv-tools/include)
include_directories(../../../bgfx/3rdparty/spirv-tools/source)
add_definitions(-D__STDC_LIMIT_MACROS)
add_definitions(-D__STDC_FORMAT_MACROS)
add_definitions(-D__STDC_CONSTANT_MACROS)
add_definitions(-DENABLE_OPT=1)
add_definitions(-DENABLE_HLSL=1)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
#  add_definitions(-D_DEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64 -std=c++14 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64")
#endif()

#if(CMAKE_BUILD_TYPE MATCHES "Release")
#  add_definitions(-DNDEBUG)
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64 -std=c++14 -fno-rtti -fno-exceptions")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64")
#endif()

add_library(glslang STATIC ${source_list})

