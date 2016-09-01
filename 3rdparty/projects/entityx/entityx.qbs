import qbs
import qbs.ModUtils

StaticLibrary {
    id: entityx
    name: "entityx"
    files: [
        "config.h",
        "../../entityx/entityx/Entity.cc",
        "../../entityx/entityx/Entity.h",
        "../../entityx/entityx/entityx.h",
        "../../entityx/entityx/Event.cc",
        "../../entityx/entityx/Event.h",
        "../../entityx/entityx/quick.h",
        "../../entityx/entityx/System.cc",
        "../../entityx/entityx/System.h",
        "../../entityx/entityx/3rdparty/catch.hpp",
        "../../entityx/entityx/3rdparty/simplesignal.h",
        "../../entityx/entityx/deps/Dependencies.h",
        "../../entityx/entityx/help/NonCopyable.h",
        "../../entityx/entityx/help/Pool.cc",
        "../../entityx/entityx/help/Pool.h",
        "../../entityx/entityx/help/Timer.cc",
        "../../entityx/entityx/help/Timer.h",
        "../../entityx/entityx/tags/TagsComponent.h"
    ]
    Depends { name: "cpp" }

    cpp.includePaths: [ "../../entityx", ".." ]
    cpp.cxxLanguageVersion: "c++11"


    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: false
    }
}
