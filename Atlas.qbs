import qbs

Project {
    references: [
        "3rdparty/projects/bgfx/bgfx.qbs",
        "3rdparty/projects/entityx/entityx.qbs",
        "Engine/Engine.qbs",
        "Editor/Editor.qbs",
        "Player/Player.qbs",
        "Tests/Tests.qbs",
        "Window/Window.qbs",
    ]
    name: "Atlas"
}

