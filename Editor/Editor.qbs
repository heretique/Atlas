import qbs

Product {
    type: "application"
    name: "AtlasEditor"
    consoleApplication: true
    files: [
        "*.cpp",
        "*.h",
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        ".",
        "../Engine/Core",
        "../Window"
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
        cpp.dynamicLibraries: [ "rt", "dl", "GL", "pthread", "X11", "SDL2" ]
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
