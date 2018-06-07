cmake_minimum_required(VERSION 3.5)

add_executable(atlas "")

target_sources(atlas PRIVATE Allocators/Allocator.h
    Allocators/FreelistAllocator.cpp
    Allocators/FreelistAllocator.h
    Allocators/LinearAllocator.cpp
    Allocators/LinearAllocator.h
    Allocators/PoolAllocator.cpp
    Allocators/PoolAllocator.h
    Allocators/ProxyAllocator.cpp
    Allocators/ProxyAllocator.h
    Allocators/StackAllocator.cpp
    Allocators/StackAllocator.h
    Assets/Asset.cpp
    Assets/Asset.h
    Assets/Geometry.cpp
    Assets/Geometry.h
    Assets/Material.cpp
    Assets/Material.h
    Assets/Script.cpp
    Assets/Script.h
    Assets/Shader.cpp
    Assets/Shader.h
    Assets/Texture.cpp
    Assets/Texture.h
    Assets/Types.cpp
    Core/CompileMurmur.h
    Core/Debug.h
    Core/Delegate.h
    Core/Engine.cpp
    Core/Engine.h
    Core/FreeList.h
    Core/Handle.h
    Core/Hash.h
    Core/NonCopyable.h
    Core/PackUtils.h
    Core/SerializationArchives.h
    Core/SimpleMeshVertex.h
    Core/Spinlock.h
    Core/Streams.h
    Core/Stringhash.cpp
    Core/StringHash.h
    Core/Types.h
    Managers/AssetManager.cpp
    Managers/AssetManager.h
    Managers/JobManager.cpp
    Managers/JobManager.h
    Managers/PluginManager.cpp
    Managers/PluginManager.h
    Managers/SceneManager.cpp
    Managers/SceneManager.h
    Math/BoundingBox.cpp
    Math/BoundingBox.h
    Math/BoundingBox.inl
    Math/BoundingSphere.cpp
    Math/BoundingSphere.h
    Math/BoundingSphere.inl
    Math/Frustum.cpp
    Math/Frustum.h
    Math/MathUtil.cpp
    Math/MathUtil.h
    Math/MathUtil.inl
    Math/MathUtilNeon.inl
    Math/Matrix.cpp
    Math/Matrix.h
    Math/Matrix.inl
    Math/Plane.cpp
    Math/Plane.h
    Math/Plane.inl
    Math/Quaternion.cpp
    Math/Quaternion.h
    Math/Quaternion.inl
    Math/Ray.cpp
    Math/Ray.h
    Math/Ray.inl
    Math/Rectangle.cpp
    Math/Rectangle.h
    Math/Transform.cpp
    Math/Transform.h
    Math/Utils.h
    Math/Vector2.cpp
    Math/Vector2.h
    Math/Vector2.inl
    Math/Vector3.cpp
    Math/Vector3.h
    Math/Vector3.inl
    Math/Vector4.cpp
    Math/Vector4.h
    Math/Vector4.inl
    Nodes/Node.cpp
    Nodes/Node.h
    Nodes/NodePath.cpp
    Nodes/NodePath.h
    Nodes/NodeScript.cpp
    Nodes/NodeScript.h
    Nodes/Spatial.cpp
    Nodes/Spatial.h
    Nodes/Types.cpp
    Nodes/Types.h
    Scripting/WrenBindings.cpp
    Scripting/WrenBindings.h
    Scripting/WrenBindingsImpl.cpp
    Scripting/WrenBindingsImpl.h
    SDL/fs_ocornut_imgui.bin.h
    SDL/Main.cpp
    SDL/MainWindow.cpp
    SDL/MainWindow.h
    SDL/SDLApp.cpp
    SDL/SDLApp.h
    SDL/SDLWindow.cpp
    SDL/SDLWindow.h
    SDL/vs_ocornut_imgui.bin.h
    Utils/BgfxUtils.cpp
    Utils/BgfxUtils.h
    Utils/PrintContainers.h)

find_package(SDL2 REQUIRED)

target_include_directories(atlas PRIVATE .
    ../3rdparty/cereal/include
    ../3rdparty/spdlog/include
    ../3rdparty/concurrentqueue
    ${SDL2_INCLUDE_DIRS})

target_link_libraries(atlas bx bimg bgfx fmt imgui wren wrenpp ${SDL2_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_LIBRARY Metal)
    find_library(QUARTZCORE_LIBRARY QuartzCore)

    message(${COCOA_LIBRARY})
    message(${METAL_LIBRARY})
    message(${QUARTZCORE_LIBRARY})

    target_link_libraries(atlas ${COCOA_LIBRARY} ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY})
endif()

target_compile_features(atlas PUBLIC cxx_std_14)
target_compile_definitions(atlas PRIVATE SPDLOG_FMT_EXTERNAL)


add_custom_command(TARGET atlas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Models/caruta.obj
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Models/caruta.mtl
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/cube/fs_cubes.bin
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/cube/vs_cubes.bin
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Textures/caruta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Materials/unlit_textured.material
    $<TARGET_FILE_DIR:atlas>/assets
    )

add_custom_command(TARGET atlas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/Assets.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/Engine.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/ImGui.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/main.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/Math.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/Scene.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/TestNodeScript.wren
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/Utils.wren
    $<TARGET_FILE_DIR:atlas>/scripts
    )


#install(TARGETS atlas RUNTIME DESTINATION ~/atlas)
#install(FILES ../Assets/Models/caruta.obj
#    ../Assets/Models/caruta.mtl
#    ../Assets/Shaders/cube/fs_cubes.bin
#    ../Assets/Shaders/cube/vs_cubes.bin
#    ../Assets/Textures/caruta.png
#    ../Assets/Materials/unlit_textured.material
#    DESTINATION ~/atlas/assets)
