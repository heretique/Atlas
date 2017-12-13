import qbs

Product {
    type: "application"
    name: "AtlasPlayer"
    consoleApplication: true
    files: [
        "*.cpp",
        "*.h",
    ]

    Depends { name: "cpp" }
//    cpp.architecture: "x86_64"
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
    cpp.cxxLanguageVersion: "c++14"

    Depends { name: "common" }
    Depends { name: "AtlasWindow" }
    Depends { name: "bgfx" }
    Depends { name: "imgui" }
    Depends { name: "fmt" }
    Depends { name: "wren" }
    Depends { name: "wrenpp" }


    Properties {
        condition: qbs.targetOS.contains("windows")

        cpp.dynamicLibraries: [ "psapi", "gdi32", "user32", "ole32", "winmm", "imm32", "oleaut32", "version", "shell32" ]
        cpp.systemIncludePaths: outer.uniqueConcat(["../../bx/include/compat/" + common.toolchain])
        cpp.libraryPaths: outer.uniqueConcat(["../3rdparty/sdl/win/" + common.toolchain + "/" + common.arch])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.dynamicLibraries: [ "rt", "dl", "EGL", "GLESv2", "pthread",  "SDL2" ]
    }

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.architecture: "x86_64"
        cpp.frameworks: [ "Cocoa", "OpenGL" ]
        cpp.libraryPaths: outer.uniqueConcat(["/usr/local/opt/sdl2/lib"])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "home/pi/atlas"
    }
}
