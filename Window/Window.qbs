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
    cpp.cxxLanguageVersion: "c++14"
    cpp.includePaths: [
        ".",
        "../Engine"
     ]
    cpp.defines: ["BUILD_WITH_EASY_PROFILER"]


    Depends { name: "common" }
    Depends { name: "easy_profiler" }
    Depends { name: "bgfx" }
    Depends { name: "fmt" }
    Depends { name: "imgui" }

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.libraryPaths: outer.uniqueConcat(["/../3rdparty/sdl/win/" + common.toolchain + "/" + common.arch])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
    }

    Properties {
        condition: qbs.toolchain.contains("msvc")
        cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../3rdparty/bx/include/compat/msvc" ])
    }

    Properties {
        condition: qbs.toolchain.contains("mingw")
        cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../3rdparty/bx/include/compat/mingw" ])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
