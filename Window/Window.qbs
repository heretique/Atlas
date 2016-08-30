import qbs

StaticLibrary {
    id: AtlasWindow
    name: "AtlasWindow"
    files: [
        "QBGFXWindow.cpp",
        "QBGFXWindow.h",
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
        "../3rdparty/bgfx/3rdparty",
        "../3rdparty/bgfx/3rdparty/khronos",
        "../3rdparty/bgfx/3rdparty/dxsdk/include",
     ]
    cpp.cxxLanguageVersion: "c++11"
    cpp.dynamicLibraries: [ "psapi", "gdi32" ]

    Depends { name: "Qt"; submodules: ["core", "gui", "gui-private", "platformsupport-private"] }
    Depends { name: "bgfx" }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
