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
    Assets/Light.cpp
    Assets/Light.h
    Assets/Material.cpp
    Assets/Material.h
    Assets/Script.cpp
    Assets/Script.h
    Assets/Shader.cpp
    Assets/Shader.h
    Assets/Texture.cpp
    Assets/Texture.h
    Assets/Types.cpp
    Components/Camera.cpp
    Components/Camera.h
    Components/LightComponent.h
    Components/MaterialComponent.cpp
    Components/MaterialComponent.h
    Components/MeshComponent.h
    Components/TransformComponent.cpp
    Components/TransformComponent.h
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
    Math/MathUtilNeon64.inl
    Math/MathUtilSSE.inl
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
    SDL/fs_ocornut_imgui.bin.h
    SDL/Main.cpp
    SDL/MainWindow.cpp
    SDL/MainWindow.h
    SDL/SDLApp.cpp
    SDL/SDLApp.h
    SDL/SDLWindow.cpp
    SDL/SDLWindow.h
    SDL/vs_ocornut_imgui.bin.h
    Systems/RenderSystem.cpp
    Systems/RenderSystem.h
    Utils/BgfxUtils.cpp
    Utils/BgfxUtils.h
    Utils/PrintContainers.h)

target_include_directories(atlas PRIVATE .
    ../3rdparty/entt/src
    ../3rdparty/cereal/include
    ../3rdparty/spdlog/include
    ../3rdparty/concurrentqueue)

target_link_libraries(atlas bx bimg bgfx atlas::fmt imgui wren wrenpp)

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

    set(RPI_LIB_PATHS "/opt/vc/lib")
    find_package(Threads REQUIRED)
    find_package(OpenGL REQUIRED)
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
    target_link_libraries(atlas Threads::Threads ${RT_LIBRARY} ${DL_LIBRARY} ${OPENGL_LIBRARIES} ${X11_LIBRARIES} ${SDL2_LIBRARY})
endif()

target_compile_features(atlas PUBLIC cxx_std_14)
#target_compile_definitions(atlas PRIVATE SPDLOG_FMT_EXTERNAL)
target_compile_definitions(atlas PRIVATE _USE_MATH_DEFINES)


add_custom_command(TARGET atlas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Models/caruta.obj
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Models/caruta.mtl
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/textured_unlit/fs_textured_unlit.bin
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Shaders/textured_unlit/vs_textured_unlit.bin
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Textures/caruta.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../Assets/Materials/unlit_textured.material
    $<TARGET_FILE_DIR:atlas>/assets
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
