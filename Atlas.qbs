import qbs

Project {
    references: [
        "3rdparty/projects/bgfx/bgfx.qbs",
        "3rdparty/projects/entityx/entityx.qbs",
        "Engine/Engine.qbs",
        "Tests/Tests.qbs",
        "Editor/Editor.qbs",
        "Window/Window.qbs",
    ]
    name: "Atlas"
}

