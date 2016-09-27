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
        "../Engine/Core",
        "../3rdparty",
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
        "../3rdparty/bgfx/3rdparty",
        "../3rdparty/bgfx/3rdparty/khronos",
        "../3rdparty/bgfx/3rdparty/dxsdk/include",
        "../3rdparty/sdl/include",
        "../3rdparty/wren/src/include",
        "../3rdparty/wrenpp",
     ]
    cpp.cxxLanguageVersion: "c++14"
    cpp.dynamicLibraries: [ "psapi", "gdi32" ]

    Depends { name: "bgfx" }
    Depends { name: "fmt" }
    Depends { name: "imgui" }
    Depends { name: "wren" }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
