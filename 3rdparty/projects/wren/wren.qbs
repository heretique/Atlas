import qbs
import qbs.ModUtils

StaticLibrary {
    id: wren
    name: "wren"
    files: [
        "../../wren/src/vm/*.c",
        "../../wren/src/optional/*.c",
    ]
    Depends { name: "cpp" }

    cpp.includePaths: [ 
        "../../wren/src/include",
        "../../wren/src/vm",
        "../../wren/src/optional",
    ]
    cpp.cxxLanguageVersion: "c++11"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
