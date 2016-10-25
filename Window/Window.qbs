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

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
