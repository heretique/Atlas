import qbs

Module {

    property string arch: {
        if (qbs.architecture == "x86")
            return "x86"
        if (qbs.architecture == "x86_64")
            return "x86_64"
    }

    property string toolchain: qbs.toolchain[0]


    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++14"
    cpp.includePaths: [
        path + "/../../../3rdparty",
        path + "/../../../3rdparty/easy_profiler/easy_profiler_core/include",
        path + "/../../../3rdparty/bx/include",
        path + "/../../../3rdparty/bgfx/include",
        path + "/../../../3rdparty/bgfx/3rdparty",
        path + "/../../../3rdparty/bgfx/3rdparty/dxsdk/include",
        path + "/../../../3rdparty/concurrentqueue",
        path + "/../../../3rdparty/wren/src/include",
        path + "/../../../3rdparty/wrenpp"
    ]

    cpp.architecture: arch

//    cpp.debugInformation: true

    Properties {
        condition: qbs.targetOS.contains("windows")
        toolchain:     {
            if (qbs.toolchain.contains("msvc"))
                return "msvc"
            if (qbs.toolchain.contains("mingw"))
                return "mingw"
        }
        cpp.includePaths: outer.uniqueConcat([
            path + "/../../../3rdparty/sdl/include",
            path + "/../../../3rdparty/bgfx/3rdparty/khronos"
        ])
        cpp.windowsApiCharacterSet: ""
    }


    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.systemIncludePaths: outer.uniqueConcat([
            "/usr/include",
            "c:/apps/SysGCC/raspberry/arm-linux-gnueabihf/sysroot/usr/local/include",
            "c:/apps/SysGCC/raspberry/arm-linux-gnueabihf/sysroot/opt/vc/include"
        ])
        cpp.defines : outer.uniqueConcat(["__VCCOREVER__", "BX_PLATFORM_RPI"]) // for RPI
        cpp.libraryPaths: outer.uniqueConcat(["c:/apps/SysGCC/raspberry/arm-linux-gnueabihf/sysroot/opt/vc/lib"])
    }

    Properties {
        condition: qbs.targetOS.contains("darwin")
        toolchain:     {
            if (qbs.toolchain.contains("clang"))
                return "clang"
        }
        cpp.minimumMacosVersion: "10.9"
        cpp.includePaths: outer.uniqueConcat([
            "/usr/local/opt/sdl2/include"
        ])   
    }
}
