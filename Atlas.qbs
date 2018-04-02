import qbs

Project {
    name: "Atlas"

    qbsSearchPaths: "qbs"

    references: [
//        "3rdparty/projects/angelscript/angelscript.qbs",
        "3rdparty/projects/bx/bx.qbs",
        "3rdparty/projects/bimg/bimg.qbs",
        "3rdparty/projects/bgfx/bgfx.qbs",
        "3rdparty/projects/bgfx_shaderc/bgfx_shaderc.qbs",
//        "3rdparty/projects/easy_profiler/easy_profiler.qbs",
        "3rdparty/projects/entityx/entityx.qbs",
        "3rdparty/projects/fmt/fmt.qbs",
        "3rdparty/projects/imgui/imgui.qbs",
//        "3rdparty/projects/nanovg/nanovg.qbs",
        "3rdparty/projects/wren/wren.qbs",
        "3rdparty/projects/wrenpp/wrenpp.qbs",
        "Engine/Engine.qbs",
        "Tests/Tests.qbs",
    ]
}

