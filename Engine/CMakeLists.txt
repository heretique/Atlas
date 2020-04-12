cmake_minimum_required(VERSION 3.5)

add_executable(atlas "")

target_sources(atlas PRIVATE
    Assets/Asset.cpp
    Assets/Asset.h
    Assets/Geometry.cpp
    Assets/Geometry.h
    Assets/Light.cpp
    Assets/Light.h
    Assets/Material.cpp
    Assets/Material.h
    Assets/ParticleEffect.cpp
    Assets/ParticleEffect.h
    Assets/Script.cpp
    Assets/Script.h
    Assets/Shader.cpp
    Assets/Shader.h
    Assets/Texture.cpp
    Assets/Texture.h
    Assets/Types.cpp
    Components/Camera.cpp
    Components/Camera.h
    Components/Common.h
    Components/LightComponent.h
    Components/MaterialComponent.cpp
    Components/MaterialComponent.h
    Components/MeshComponent.h
    Components/ParticleEffectComponent.h
    Components/TransformComponent.cpp
    Components/TransformComponent.h
    Core/Debug.h
    Core/Delegate.h
    Core/Engine.cpp
    Core/Engine.h
    Core/SimpleMeshVertex.h
    Managers/AssetManager.cpp
    Managers/AssetManager.h
    Managers/ECSManager.cpp
    Managers/ECSManager.h
    Managers/InputManager.cpp
    Managers/InputManager.h
    Managers/PluginManager.cpp
    Managers/PluginManager.h
    SDL/fs_ocornut_imgui.bin.h
    SDL/Main.cpp
    SDL/MainWindow.cpp
    SDL/MainWindow.h
    SDL/SDLApp.cpp
    SDL/SDLApp.h
    SDL/SDLWindow.cpp
    SDL/SDLWindow.h
    SDL/vs_ocornut_imgui.bin.h
    Systems/ISystem.h
    Systems/AnimationSystem.cpp
    Systems/AnimationSystem.h
    Systems/ParticleSystem.cpp
    Systems/ParticleSystem.h
    Systems/PickingSystem.cpp
    Systems/PickingSystem.h
    Systems/RenderSystem.cpp
    Systems/RenderSystem.h
    Utils/BgfxUtils.cpp
    Utils/BgfxUtils.h
    Utils/DebugDraw.cpp
    Utils/DebugDraw.h
    Utils/ImGuiSerializer.cpp
    Utils/ImGuiSerializer.h
    )

target_include_directories(atlas PRIVATE .
    ../3rdparty/spdlog/include)

target_link_libraries(atlas hq bx bimg bgfx atlas::fmt imgui)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_library(sdl2 STATIC IMPORTED)
    set_target_properties(sdl2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/sdl/include)
    set_target_properties(sdl2 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/sdl/win/msvc/x64/SDL2.lib)
    target_link_libraries(atlas sdl2)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_LIBRARY Metal)
    find_library(QUARTZCORE_LIBRARY QuartzCore)

    message(${COCOA_LIBRARY})
    message(${METAL_LIBRARY})
    message(${QUARTZCORE_LIBRARY})

    target_link_libraries(atlas ${COCOA_LIBRARY} ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    find_library(DL_LIBRARY dl)
    find_library(RT_LIBRARY rt)
    find_library(SDL2_LIBRARY SDL2)
    find_path(SDL2_INCLUDE SDL2)

    message(${DL_LIBRARY})
    message(${RT_LIBRARY})
    message(${SDL2_LIBRARY})
    message(${SDL2_INCLUDE})

    target_include_directories(atlas PRIVATE ${SDL2_INCLUDE})
    target_link_libraries(atlas Threads::Threads ${RT_LIBRARY} ${DL_LIBRARY} EGL GLESv2 ${X11_LIBRARIES} ${SDL2_LIBRARY} stdc++fs)
endif()

target_compile_features(atlas PUBLIC cxx_std_17)
#target_compile_definitions(atlas PRIVATE SPDLOG_FMT_EXTERNAL)
target_compile_definitions(atlas PRIVATE _USE_MATH_DEFINES)


add_custom_command(TARGET atlas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Models
    $<TARGET_FILE_DIR:atlas>/assets/models

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders
    $<TARGET_FILE_DIR:atlas>/assets/shaders

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Textures
    $<TARGET_FILE_DIR:atlas>/assets/textures

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Materials
    $<TARGET_FILE_DIR:atlas>/assets/materials
    )

#add_custom_command(TARGET atlas POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/TestNodeScript.wren
#    $<TARGET_FILE_DIR:atlas>/scripts
#    )


#install(TARGETS atlas RUNTIME DESTINATION ~/atlas)
#install(FILES ../Assets/Models/caruta.obj
#    ../Assets/Models/caruta.mtl
#    ../Assets/Shaders/cube/fs_cubes.bin
#    ../Assets/Shaders/cube/vs_cubes.bin
#    ../Assets/Textures/caruta.png
#    ../Assets/Materials/unlit_textured.material
#    DESTINATION ~/atlas/assets)
