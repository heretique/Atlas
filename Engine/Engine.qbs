import qbs

Product {
    type: "application"
    name: "AtlasEngine"
    consoleApplication: false
    files: [
        "*.cpp",
        "*.h",
        "Allocators/*.cpp",
        "Allocators/*.h",
        "Assets/*.cpp",
        "Assets/*.h",
        "Components/*.cpp",
        "Components/*.h",
        "Core/*.cpp",
        "Core/*.h",
        "Math/*.cpp",
        "Math/*.h",
        "Math/*.inl",
        "Managers/*.cpp",
        "Managers/*.h",
        "Nodes/*.h",
        "Nodes/*.cpp",
        "Scripting/*.cpp",
        "Scripting/*.h",
        "SDL/*.cpp",
        "SDL/*.h",
        "Systems/*cpp",
        "Systems/*.h",
        "Utils/*.cpp",
        "Utils/*.h"
    ]

    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++14"
    cpp.includePaths: [
        ".",
        "../3rdparty/concurrentqueue",
        "../3rdparty/cereal/include",
        "../3rdparty/projects",
        "../3rdparty/entityx",
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
        "../3rdparty/bimg/include",
        "../3rdparty/stb",
        "../3rdparty/picojson",
        "../3rdparty/tinygltf",
        "../3rdparty/signals-cpp",
        "../3rdparty/spdlog/include",
    ]

    cpp.defines: ["BUILD_WITH_EASY_PROFILER", "SPDLOG_FMT_EXTERNAL"]

    Depends { name: "common" }
//    Depends { name: "easy_profiler" }
    Depends { name: "bx" }
    Depends { name: "bgfx" }
    Depends { name: "shaderc_lib" }
    Depends { name: "imgui" }
    Depends { name: "fmt" }
//    Depends { name: "angelscript" }
    Depends { name: "wren" }
    Depends { name: "wrenpp" }
    Depends { name: "entityx" }

    Properties {
        condition: qbs.targetOS.contains("windows")

        cpp.dynamicLibraries: [ "psapi", "gdi32", "user32", "ole32", "winmm", "imm32", "oleaut32", "version", "shell32" ,"ws2_32" ]
        cpp.systemIncludePaths: outer.uniqueConcat(["../../bx/include/compat/" + common.toolchain])
        cpp.libraryPaths: outer.uniqueConcat(["../3rdparty/sdl/win/" + common.toolchain + "/" + common.arch])
        cpp.staticLibraries: outer.uniqueConcat(["SDL2"])
        cpp.architecture: common.arch

        Properties {
            condition: qbs.toolchain.contains("msvc")
            cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../3rdparty/bx/include/compat/msvc" ])
        }

        Properties {
            condition: qbs.toolchain.contains("mingw")
            cpp.systemIncludePaths: outer.uniqueConcat([ path + "/../3rdparty/bx/include/compat/mingw" ])
        }
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.dynamicLibraries: [ "rt", "dl", "bcm_host", "EGL", "GLESv2", "pthread", "udev", "SDL2" ]
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
        qbs.installDir: "home/pi/atlas"
    }

    Group {
        name: "Wren scripts"
        files: [
        "Scripting/wren/*.wren"
        ]
        qbs.install: true
        qbs.installDir: "home/pi/atlas/scripts"
    }

    Group {
        name: "Test Assets"
        files: [
            "../Assets/Models/caruta.obj",
            "../Assets/Models/caruta.mtl",
            "../Assets/Shaders/bgfx_shader.sh",
            "../Assets/Shaders/common.sh",
            "../Assets/Shaders/cube/fs_cubes.sc",
            "../Assets/Shaders/cube/varying.def.sc",
            "../Assets/Shaders/cube/vs_cubes.sc",
            "../Assets/Shaders/shaderlib.sh",
            "../Assets/Textures/caruta.png",
            "../Assets/Materials/unlit_textured.material"
        ]
        qbs.install: true
        qbs.installDir: "home/pi/atlas/assets"
    }
}

