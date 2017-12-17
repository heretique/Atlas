import qbs
import qbs.ModUtils

StaticLibrary {
    id: bimg
    name: "bimg"
    files: [
        "../../bimg/include/bimg/bimg.h",
        "../../bimg/src/bimg_p.h",
//        "../../bimg/src/image_decode.cpp",
//        "../../bimg/src/image_encode.cpp",
        "../../bimg/src/image_gnf.cpp",
        "../../bimg/src/image.cpp"
    ]

    Depends { name: "cpp" }
    Depends { name: "bx" }

    cpp.includePaths: [
        path + "/../../bimg/include"
    ]
    cpp.cxxLanguageVersion: "c++14"

    Export {
        Depends { name: "cpp" }

        cpp.includePaths: [
            path + "/../../bimg/include"
        ]
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
