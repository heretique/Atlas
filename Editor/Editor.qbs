import qbs

Product {
    type: "application"
    name: "AtlasEditor"
    consoleApplication: false
    files: [
        "*.cpp",
        "*.h",
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
        "../3rdparty/bgfx/3rdparty",
        "../3rdparty/bgfx/3rdparty/khronos",
        "../3rdparty/bgfx/3rdparty/dxsdk/include",
        "../3rdparty/bgfx/examples/17-drawstress",
        "../Window",
     ]
    cpp.cxxLanguageVersion: "c++11"


    Depends { name: "Qt"; submodules: ["core", "gui"] }
    Depends { name: "AtlasWindow" }
    Depends { name: "bgfx" }


    Properties {
            condition: qbs.toolchain.contains("msvc")
            cpp.systemIncludePaths: outer.uniqueConcat([ "../3rdparty/bx/include/compat/msvc" ])
        }

        Properties {
            condition: qbs.toolchain.contains("mingw")
            cpp.systemIncludePaths: outer.uniqueConcat([ "../3rdparty/bx/include/compat/mingw" ])
        }



    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.dynamicLibraries: [ "psapi", "gdi32", "user32" ]
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.dynamicLibraries: [ "rt", "dl", "GL", "pthread", "X11" ]
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
