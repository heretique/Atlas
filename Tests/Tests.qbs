import qbs

Project {
    references: [
        "bgfx-examples-common/examples-common.qbs",
        "drawstress/drawstress.qbs",
        "particles/particles.qbs",
    ]
    name: "Tests"
}

