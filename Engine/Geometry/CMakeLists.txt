add_library(Geometry STATIC
    PRIVATE
        Private/Geometry.cpp
    PUBLIC
        Public/Geometry/Geometry.h
        Public/Geometry/MeshComponent.h
    )

add_library(Atlas::Geometry ALIAS Geometry)

target_include_directories(Geometry
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

find_package(tinyobjloader CONFIG REQUIRED)
target_link_libraries(Geometry Atlas::Core Atlas::Assets Atlas::SDL RTTR::Core tinyobjloader::tinyobjloader_double)
