import qbs

Project {

    qbsSearchPaths: "qbs"

    references: [
        "fcpp.qbs",
        "glsl-optimizer.qbs",
        "glslang.qbs",
        "shaderc.qbs"
    ]
    name: "bgfx_shaderc"
}

