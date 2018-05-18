import qbs
import qbs.ModUtils

StaticLibrary {
    id: easy_profiler
    name: "easy_profiler"
    files: [
        "../../easy_profiler/easy_profiler_core/*.h",
        "../../easy_profiler/easy_profiler_core/*.cpp"
    ]
    Depends { name: "cpp" }
    Depends { name: "common" }
    cpp.cxxLanguageVersion: "c++17"

    cpp.includePaths: [ "../../easy_profiler/easy_profiler_core/include" ]

    cpp.defines: [
        "EASY_PROFILER_STATIC",
        "BUILD_WITH_EASY_PROFILER",
        "EASY_PROFILER_VERSION_MAJOR=1",
        "EASY_PROFILER_VERSION_MINOR=3",
        "EASY_PROFILER_VERSION_PATCH=0",
        // a hack to make easy_profiler compile
        "EASY_CONSTEXPR=const",
        "EASY_STATIC_CONSTEXPR=static const",
        "EASY_CONSTEXPR_FCN="
    ]

    Properties {
        condition: qbs.toolchain.contains("mingw")
        cpp.defines: outer.uniqueConcat(["STRSAFE_NO_DEPRECATE"])
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
