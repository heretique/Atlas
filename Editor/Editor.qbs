import qbs

Product {
    type: "application"
    name: "AtlasEditor"
    consoleApplication: false
    files: [

    ]

    Depends { name: "cpp" }
    cpp.includePaths: [
     ]
    cpp.cxxLanguageVersion: "c++11"


    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
