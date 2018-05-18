import qbs
import qbs.ModUtils

StaticLibrary {
    id: fmt
    name: "fmt"
    files: [
        "../../fmt/format.cc",
        "../../fmt/format.h",
        "../../fmt/ostream.cc",
        "../../fmt/ostream.h",
        "../../fmt/posix.cc",
        "../../fmt/posix.h",
        "../../fmt/printf.h",
        "../../fmt/string.h",
        "../../fmt/time.h",
    ]
    Depends { name: "cpp" }
    Depends { name: "common" }

    cpp.includePaths: [ "../..", ".." ]
    cpp.cxxLanguageVersion: "c++17"


    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
