import qbs

Project {
    name: "bgfx_shaderc"

    qbsSearchPaths: "qbs"

    references: [
        "fcpp.qbs",
        "glsl-optimizer.qbs",
        "glslang.qbs",
        "shaderc_lib.qbs",
        "shaderc.qbs"
    ]
}

