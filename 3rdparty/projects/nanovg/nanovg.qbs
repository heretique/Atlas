import qbs
import qbs.ModUtils

StaticLibrary {
    id: nanovg
    name: "nanovg"
    files: [
    	"../../bgfx/examples/common/nanovg/fontstash.h",
        "../../bgfx/examples/common/nanovg/nanovg_bgfx.h",
        "../../bgfx/examples/common/nanovg/nanovg_bgfx.cpp",
        "../../bgfx/examples/common/nanovg/nanovg.h",
        "../../bgfx/examples/common/nanovg/nanovg.cpp"
    ]
    Depends { name: "cpp" }
    cpp.includePaths: [
        "../../bgfx/examples/common/nanovg",
    ]
    cpp.cxxLanguageVersion: "c++14"
    Depends { name: "common" }
    Depends { name: "bgfx" }

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.systemIncludePaths: outer.uniqueConcat(["../../bx/include/compat/" + common.toolchain])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
