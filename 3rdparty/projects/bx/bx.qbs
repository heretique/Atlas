import qbs
import qbs.ModUtils

StaticLibrary {
    id: bx
    name: "bx"
    files: [
        "../../bx/include/bx/*.h",
        "../../bx/include/compat/freebsd/*.h",
        "../../bx/include/compat/ios/*.h",
        "../../bx/include/compat/mingw/*.h",
        "../../bx/include/compat/msvc/*.h",
        "../../bx/include/compat/osx/*.h",
        "../../bx/include/tinystl/*.h",
        "../../bx/src/*.cpp"
    ]
    excludeFiles : [
        "../../bx/src/amalgamated.cpp"
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        path + "/../../bx/include",
        path + "/../../bx/3rdparty"
    ]

    Depends { name: "common" }

    cpp.cxxLanguageVersion: "c++14"
    cpp.defines: [
        "__STDC_LIMIT_MACROS",
        "__STDC_FORMAT_MACROS",
        "__STDC_CONSTANT_MACROS"
        ]


    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.defines:  outer.uniqueConcat(["BX_PLATFORM_RPI"])
    }

    Properties {
        condition: qbs.toolchain.contains("msvc")
        cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../../bx/include/compat/msvc" ])
    }

    Properties {
        condition: qbs.toolchain.contains("mingw")
        cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../../bx/include/compat/mingw" ])
    }

    Properties {
        condition: qbs.targetOS.contains("darwin")
        cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../../bx/include/compat/osx" ])
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            path + "/../../bx/include"
        ]

        Properties {
            condition: qbs.targetOS.contains("windows")
            cpp.windowsApiCharacterSet: ""
        }

        Properties {
            condition: qbs.toolchain.contains("msvc")
            cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../../bx/include/compat/msvc" ])
        }

        Properties {
            condition: qbs.toolchain.contains("mingw")
            cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../../bx/include/compat/mingw" ])
        }

        Properties {
            condition: qbs.targetOS.contains("darwin")
            cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../../bx/include/compat/osx" ])
        }
    }


    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
