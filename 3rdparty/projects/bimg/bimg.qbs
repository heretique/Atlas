import qbs
import qbs.ModUtils

StaticLibrary {
    id: bimg
    name: "bimg"
    files: [
        "../../bimg/include/bimg/bimg.h",
        "../../bimg/include/bimg/decode.h",
        "../../bimg/include/bimg/encode.h",
        "../../bimg/src/bimg_p.h",
        "../../bimg/src/image_decode.cpp",
        "../../bimg/src/image_encode.cpp",
        "../../bimg/src/image_gnf.cpp",
        "../../bimg/src/image.cpp"
    ]

    Depends { name: "cpp" }
    Depends { name: "common" }
    Depends { name: "bx" }

    cpp.includePaths: [
        path + "/../../bimg/include",
        path + "/../../bimg/3rdparty",
        path + "/../../bimg/3rdparty/iqa/include",
        path + "/../../bimg/3rdparty/nvtt",
        path + "/../../bx/include",
    ]
    cpp.cxxLanguageVersion: "c++17"

    cpp.defines: [
        "__STDC_LIMIT_MACROS",
        "__STDC_FORMAT_MACROS",
        "__STDC_CONSTANT_MACROS"
        ]

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
