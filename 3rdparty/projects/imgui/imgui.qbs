import qbs
import qbs.ModUtils

StaticLibrary {
    id: imgui
    name: "imgui"
    files: [
        "../../imgui/imconfig.h",
        "../../imgui/imgui.h",
        "../../imgui/imgui_internal.h",
        "../../imgui/imgui.cpp",
        "../../imgui/imgui_draw.cpp",
        "../../imgui/stb_rect_pack.h",
        "../../imgui/stb_textedit.h",
        "../../imgui/stb_truetype.h",
    ]
    Depends { name: "cpp" }


    cpp.includePaths: [ 
        "../../imgui"
    ]
    cpp.cxxLanguageVersion: "c++14"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
