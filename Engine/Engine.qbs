import qbs

StaticLibrary {
    id: AtlasEngine
    name: "AtlasEngine"
    files: [
    	"*.cpp",
    	"*.h",
        "API/*.h",
        "Allocators/*.cpp",
        "Allocators/*.h",
        "Components/*.cpp",
        "Components/*.h",
        "Core/*.cpp",
        "Core/*.h",
    	"Math/*.cpp",
    	"Math/*.h",
    	"Math/*.inl",
        "Managers/*.cpp",
        "Managers/*.h",
        "Resources/*.cpp",
        "Resources/*.h",
        "Systems/*cpp",
        "Systems/*.h",
    ]

    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++14"
    cpp.includePaths: [
        ".",
        "..",
        "API",
        "Allocator",
        "Components",
        "Core",
        "Math",
        "Managers",
        "Resources",
        "Systems",
        "../3rdparty/concurrentqueue",
        "../3rdparty/cereal/include",
        "../3rdparty/projects",
        "../3rdparty/entityx",
        "../3rdparty/bx/include",
        "../3rdparty/bgfx/include",
    ]

    Depends { name: "common" }
    Depends { name: "bgfx" }
    Depends { name: "entityx" }
    Depends { name: "AtlasWindow" }
    Depends { name: "fmt" }

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.dynamicLibraries: [ "psapi", "gdi32" ]
    }

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}

