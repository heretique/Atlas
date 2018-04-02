import qbs

StaticLibrary {
    id: fcpp
	name: "fcpp"

    files: [
        "../../bgfx/3rdparty/fcpp/FPPBase.h",
        "../../bgfx/3rdparty/fcpp/FPP_protos.h",
        "../../bgfx/3rdparty/fcpp/cpp.h",
        "../../bgfx/3rdparty/fcpp/cpp1.c",
        "../../bgfx/3rdparty/fcpp/cpp2.c",
        "../../bgfx/3rdparty/fcpp/cpp3.c",
        "../../bgfx/3rdparty/fcpp/cpp4.c",
        "../../bgfx/3rdparty/fcpp/cpp5.c",
        "../../bgfx/3rdparty/fcpp/cpp6.c",
        "../../bgfx/3rdparty/fcpp/cppadd.h",
        "../../bgfx/3rdparty/fcpp/cppdef.h",
        "../../bgfx/3rdparty/fcpp/fpp.h",
        "../../bgfx/3rdparty/fcpp/fpp_pragmas.h",
        ]

	Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++14"
    cpp.defines: [
        "__STDC_LIMIT_MACROS",
        "__STDC_FORMAT_MACROS",
        "__STDC_CONSTANT_MACROS",
        "NINCLUDE=64",
        "NWORK=65536",
        "NBUFF=65536",
        "OLD_PREPROCESSOR=0",
        ]
    Depends { name: "common" }


    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
