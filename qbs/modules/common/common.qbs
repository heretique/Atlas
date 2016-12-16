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
        path + "/../../../3rdparty",
        path + "/../../../3rdparty/bx/include",
        path + "/../../../3rdparty/bgfx/include",
        path + "/../../../3rdparty/bgfx/3rdparty",
        path + "/../../../3rdparty/bgfx/3rdparty/khronos",
        path + "/../../../3rdparty/bgfx/3rdparty/dxsdk/include",
        path + "/../../../3rdparty/concurrentqueue",
        path + "/../../../3rdparty/wren/src/include",
        path + "/../../../3rdparty/wrenpp"
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
            path + "/../../../3rdparty/sdl/include"
        ])
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.includePaths: outer.uniqueConcat([
            "/usr/local/include"
        ])
    }
}
