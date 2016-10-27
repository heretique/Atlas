import qbs

Module {

    property string arch: {
        if (qbs.architecture == "x86")
            return "x86"
        if (qbs.architecture == "x86_64")
            return "x64"
    }

    property string toolchain: qbs.toolchain


    Depends { name: "cpp" }
    cpp.includePaths: [
        "../3rdparty",
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
        "../3rdparty/bgfx/3rdparty",
        "../3rdparty/bgfx/3rdparty/khronos",
        "../3rdparty/bgfx/3rdparty/dxsdk/include",
        "../3rdparty/concurrentqueue",
        "../3rdparty/wren/src/include",
        "../3rdparty/wrenpp"
    ]


    Properties {
        condition: qbs.targetOS.contains("windows")
        toolchain:     {
            if (qbs.toolchain.contains("msvc"))
                return "msvc"
            if (qbs.toolchain.contains("mingw"))
                return "mingw"
        }
        cpp.includePaths: outer.uniqueConcat([
            "../3rdparty/sdl/include"
        ])
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
    }
}
