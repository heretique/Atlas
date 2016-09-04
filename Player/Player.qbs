import qbs

Product {
    type: "application"
    name: "AtlasPlayer"
    consoleApplication: false
    files: [
        "*.cpp",
        "*.h",
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        ".",
        "../Engine/Core",
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
        "../3rdparty/bgfx/3rdparty",
        "../3rdparty/bgfx/3rdparty/khronos",
        "../3rdparty/bgfx/3rdparty/dxsdk/include",
        "../3rdparty/bgfx/examples/17-drawstress",
        "../Window",
        "../3rdparty/sdl/include",
    ]
    cpp.cxxLanguageVersion: "c++11"

    Depends { name: "AtlasWindow" }
    Depends { name: "bgfx" }
    Depends { name: "imgui" }


    Properties {
        condition: qbs.targetOS.contains("windows")

        property string arch: {
            if (qbs.architecture == "x86")
                return "x86"
            if (qbs.architecture == "x86_64")
                return "x64"
        }
        property string toolchain: {
            if (qbs.toolchain.contains("msvc"))
                return "msvc"
            if (qbs.toolchain.contains("mingw"))
                return "mingw"
        }


        cpp.dynamicLibraries: [ "psapi", "gdi32", "user32", "ole32", "winmm", "imm32", "oleaut32", "version", "shell32" ]
        cpp.systemIncludePaths: outer.uniqueConcat(["../../bx/include/compat/" + toolchain])
        cpp.libraryPaths: outer.uniqueConcat(["../3rdparty/sdl/win/" + toolchain + "/" + arch])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
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
