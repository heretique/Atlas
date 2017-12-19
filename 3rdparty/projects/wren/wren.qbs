import qbs
import qbs.ModUtils

StaticLibrary {
    id: wren
    name: "wren"
    files: [
        "../../wren/src/vm/*.c",
        "../../wren/src/optional/*.c",
        "../../wren/src/include/*.h",
    ]
    Depends { name: "cpp" }

    cpp.includePaths: [ 
        path + "/../../wren/src/include",
        path + "/../../wren/src/vm",
        path + "/../../wren/src/optional",
    ]
    cpp.cLanguageVersion: "c11"
    cpp.cxxLanguageVersion: "c++14"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
