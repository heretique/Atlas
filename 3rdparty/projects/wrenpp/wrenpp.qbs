import qbs
import qbs.ModUtils

StaticLibrary {
    id: wrenpp
    name: "wrenpp"
    files: [
        "../../wrenpp/Wren++.cpp",
        "../../wrenpp/Wren++.h",
    ]
    Depends { name: "cpp" }
    Depends { name: "common" }
    Depends { name: "wren" }

    cpp.includePaths: [ 
        "../../wren/src/include",
        "../../wren/src/vm",
        "../../wren/src/optional",
    ]
    cpp.cxxLanguageVersion: "c++14"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
