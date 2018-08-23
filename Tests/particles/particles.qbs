import qbs

Product {
    type: "application"
    name: "particles"
    consoleApplication: false
    files: [
        "../../3rdparty/bgfx/examples/32-particles/particles.cpp"
    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
        "../../3rdparty/bx/include",
        "../../3rdparty/bgfx/include",
        "../../3rdparty/bgfx/3rdparty",
        "../../3rdparty/bgfx/3rdparty/khronos",
        "../../3rdparty/bgfx/3rdparty/dxsdk/include",
        "../../3rdparty/bgfx/examples/common",
        "../../3rdparty/bgfx/examples/common/entry"
    ]

    Depends { name: "common" }
    Depends { name: "examples-common" }

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.dynamicLibraries: [ "psapi", "gdi32" ]
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.dynamicLibraries: [ "rt", "dl", "GL", "pthread", "X11" ]
    }

    Properties {
        condition: qbs.targetOS.contains("darwin")

        cpp.frameworks: [ "Cocoa", "OpenGL", "Metal", "QuartzCore"]
        cpp.libraryPaths: outer.uniqueConcat(["/usr/local/opt/sdl2/lib"])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    Group {
        name: "textures"
        files: [
            "../../3rdparty/bgfx/examples/runtime/textures/*"
        ]
        qbs.install: true
        qbs.installDir: "textures"
    }
}
