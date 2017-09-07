import qbs

Product {
    type: "application"
    name: "AtlasEditor"
    consoleApplication: false
    files: [
        "*.cpp",
        "*.h",
        "../3rdparty/microbench/*.cpp",
        "../3rdparty/microbench/*.h",
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        ".",
        "../Engine",
        "../Engine/Core",
        "../Engine/Managers",
        "../Window"
    ]
    cpp.cxxLanguageVersion: "c++14"
    cpp.architecture: "x86_64"

    Depends { name: "common" }
    Depends { name: "AtlasWindow" }
    Depends { name: "bx" }
    Depends { name: "bgfx" }
    Depends { name: "imgui" }
    Depends { name: "fmt" }
    Depends { name: "wren" }
    Depends { name: "wrenpp" }
    Depends { name: "AtlasEngine" }
    Depends { name: "entityx" }


    Properties {
        condition: qbs.targetOS.contains("windows")

        cpp.dynamicLibraries: [ "psapi", "gdi32", "user32", "ole32", "winmm", "imm32", "oleaut32", "version", "shell32" ]
        cpp.systemIncludePaths: outer.uniqueConcat(["../../bx/include/compat/" + common.toolchain])
        cpp.libraryPaths: outer.uniqueConcat(["../3rdparty/sdl/win/" + common.toolchain + "/" + common.arch])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.dynamicLibraries: [ "rt", "dl", "GL", "pthread", "X11", "SDL2" ]
    }

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.frameworks: [ "Cocoa", "OpenGL", "Metal", "QuartzCore"]
        cpp.libraryPaths: outer.uniqueConcat(["/usr/local/opt/sdl2/lib"])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
