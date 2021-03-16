add_library(UI STATIC
    PRIVATE
        Private/FileBrowser.cpp
        Private/InfoWindow.cpp
        Private/InspectorWindow.cpp
        Private/UIManager.cpp
        Private/UIWindow.cpp
        Private/ImGuiSerializer.cpp
    PUBLIC
        Public/UI/FileBrowser.h
        Public/UI/InfoWindow.h
        Public/UI/InspectorWindow.h
        Public/UI/Spinner.h
        Public/UI/UIManager.h
        Public/UI/UIWindow.h
        Public/UI/ImGuiSerializer.h
    )

add_library(Atlas::UI ALIAS UI)

target_include_directories(UI
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(UI Atlas::Core Atlas::Input Atlas::Ecs Atlas::Engine)
