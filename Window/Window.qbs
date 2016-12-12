import qbs

StaticLibrary {
    id: AtlasWindow
    name: "AtlasWindow"
    files: [
        "SDLApp.cpp",
        "SDLApp.h",
        "SDLWindow.cpp",
        "SDLWindow.h",
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        ".",
        "../Engine/Core"
     ]
    cpp.cxxLanguageVersion: "c++14"
    cpp.dynamicLibraries: [ "psapi", "gdi32" ]

    Depends { name: "common" }
    Depends { name: "bgfx" }
    Depends { name: "fmt" }
    Depends { name: "imgui" }

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.libraryPaths: outer.uniqueConcat(["../3rdparty/sdl/win/" + common.toolchain + "/" + common.arch])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
