import qbs
import qbs.ModUtils

StaticLibrary {
    id: angelscript
    name: "angelscript"
    files: [
        "../../angelscript/angelscript/source/*.h",
        "../../angelscript/angelscript/source/*.cpp",
    ]
    Depends { name: "cpp" }

    cpp.includePaths: [ 
        path + "/../../angelscript/angelscript/include",
    ]
    cpp.cxxLanguageVersion: "c++14"


    Properties{
        condition: qbs.architecture.contains("arm")
        files: outer.uniqueConcat([
                                      "../../angelscript/angelscript/source/as_callfunc_arm_gcc.S",
                                  ])
    }

    Properties{
        condition: qbs.architecture.contains("x86_64") && qbs.toolchain.contains("msvc")
        files: outer.uniqueConcat([
                                      "../../angelscript/angelscript/source/as_callfunc_x64_msvc_asm.asm",
                                  ])
    }

    Properties{
        condition: qbs.architecture.contains("x86") && qbs.toolchain.contains("msvc")
        files: outer.uniqueConcat([
                                      "../../angelscript/angelscript/source/as_callfunc_arm_msvc.asm",
                                  ])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
