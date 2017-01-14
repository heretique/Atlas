import qbs

Project {

    qbsSearchPaths: "qbs"

    references: [
        "3rdparty/projects/bx/bx.qbs",
        "3rdparty/projects/bgfx/bgfx.qbs",
        "3rdparty/projects/entityx/entityx.qbs",
        "3rdparty/projects/fmt/fmt.qbs",
        "3rdparty/projects/imgui/imgui.qbs",
        "3rdparty/projects/nanovg/nanovg.qbs",
        "3rdparty/projects/wren/wren.qbs",
        "3rdparty/projects/wrenpp/wrenpp.qbs",
        "Engine/Engine.qbs",
        "Editor/Editor.qbs",
        "Player/Player.qbs",
        "Tests/Tests.qbs",
        "Window/Window.qbs",
    ]
    name: "Atlas"
}

