add_library(Assets STATIC
    PRIVATE
        Private/Asset.cpp
        Private/AssetManager.cpp
        Private/Types.cpp
    PUBLIC
        Public/Assets/Asset.h
        Public/Assets/AssetManager.h
        Public/Assets/Types.h
    )
add_library(Atlas::Assets ALIAS Assets)

target_include_directories(Assets
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(Assets Atlas::Core)
