import qbs
import qbs.ModUtils

StaticLibrary {
    id: examples_common
    name: "examples-common"
    files: [
        "../../3rdparty/bgfx/examples/common/*.h",
        "../../3rdparty/bgfx/examples/common/*.cpp",
        "../../3rdparty/bgfx/examples/common/debugdraw/*.cpp",
        "../../3rdparty/bgfx/examples/common/debugdraw/*.h",
        "../../3rdparty/bgfx/examples/common/entry/*.cpp",
        "../../3rdparty/bgfx/examples/common/entry/*.h",
        "../../3rdparty/bgfx/examples/common/font/*.cpp",
        "../../3rdparty/bgfx/examples/common/font/*.h",
        "../../3rdparty/bgfx/examples/common/imgui/*.cpp",
        "../../3rdparty/bgfx/examples/common/imgui/*.h",
        "../../3rdparty/bgfx/examples/common/nanovg/*.cpp",
        "../../3rdparty/bgfx/examples/common/nanovg/*.h",
        "../../3rdparty/bgfx/examples/common/ps/*.cpp",
        "../../3rdparty/bgfx/examples/common/ps/*.h",
        "../../3rdparty/bgfx/3rdparty/dear-imgui/*.cpp",
        "../../3rdparty/bgfx/3rdparty/dear-imgui/*.h",
        "../../3rdparty/bgfx/3rdparty/ib-compress/*.cpp",
        "../../3rdparty/bgfx/3rdparty/ib-compress/*.h",
    ]
    Depends { name: "cpp" }
    Depends { name: "common" }
    Depends { name: "bx" }
    Depends { name: "bimg" }

    cpp.includePaths: [ "../../3rdparty/bx/include", "../../3rdparty/bgfx/include", "../../3rdparty/bgfx/3rdparty", "../../3rdparty/bgfx/3rdparty/khronos", "../../3rdparty/bgfx/3rdparty/dxsdk/include", "../../3rdparty/bgfx/examples/common", "../../3rdparty/bgfx/examples/common/entry" ]
    cpp.defines: ["ENTRY_CONFIG_IMPLEMENT_MAIN", "ENTRY_CONFIG_USE_SDL"]


    Properties {
        condition: qbs.targetOS.contains("darwin")
        files: outer.uniqueConcat(["../../3rdparty/bgfx/examples/common/entry/entry_osx.mm"])
    }

    Properties {
        condition: cpp.defines.contains("ENTRY_CONFIG_USE_SDL")
        cpp.systemIncludePaths: outer.uniqueConcat(["/usr/local/include"])
    }

    Export {
        Depends { name: "cpp" }
        Depends { name: "bx" }
        Depends { name: "bimg" }
        Depends { name: "bgfx" }
        cpp.includePaths: [
            path + "/../../3rdparty/bgfx/examples/common",
            path + "/../../3rdparty/bgfx/examples/common/entry"
        ]
        cpp.defines: ["ENTRY_CONFIG_IMPLEMENT_MAIN", "ENTRY_CONFIG_USE_SDL"]
    }


    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
