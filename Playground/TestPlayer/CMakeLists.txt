add_executable(TestPlayer
        Main.cpp
        MainWindow.cpp
    )

find_package(rttr CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(TestPlayer Atlas::Core Atlas::SDL Atlas::Engine SDL2::SDL2 RTTR::Core imgui::imgui fmt::fmt)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if (MSVC)
        # added because rttr reflection causes "Fatal Error C1128" on some files
        add_definitions(/bigobj)
    endif()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_LIBRARY Metal)
    find_library(QUARTZCORE_LIBRARY QuartzCore)

    message(${COCOA_LIBRARY})
    message(${METAL_LIBRARY})
    message(${QUARTZCORE_LIBRARY})

    target_link_libraries(TestPlayer ${COCOA_LIBRARY} ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY})
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    find_library(DL_LIBRARY dl)
    find_library(RT_LIBRARY rt)

    message(${DL_LIBRARY})
    message(${RT_LIBRARY})

    target_link_libraries(TestPlayer Threads::Threads ${RT_LIBRARY} ${DL_LIBRARY} EGL GLESv2 ${X11_LIBRARIES} stdc++fs)
endif()

target_compile_features(TestPlayer PUBLIC cxx_std_17)
target_compile_definitions(TestPlayer PRIVATE _USE_MATH_DEFINES)


add_custom_command(TARGET TestPlayer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Assets/Models
    ${CMAKE_BINARY_DIR}/assets/models

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Assets/Shaders
    ${CMAKE_BINARY_DIR}/assets/shaders

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Assets/Textures
    ${CMAKE_BINARY_DIR}/assets/textures

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Assets/Materials
    ${CMAKE_BINARY_DIR}/assets/materials
    )

#add_custom_command(TARGET TestPlayer POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/wren/TestNodeScript.wren
#    $<TARGET_FILE_DIR:atlas>/scripts
#    )


#install(TARGETS TestPlayer RUNTIME DESTINATION ~/atlas)
#install(FILES ../Assets/Models/caruta.obj
#    ../Assets/Models/caruta.mtl
#    ../Assets/Shaders/cube/fs_cubes.bin
#    ../Assets/Shaders/cube/vs_cubes.bin
#    ../Assets/Textures/caruta.png
#    ../Assets/Materials/unlit_textured.material
#    DESTINATION ~/atlas/assets)
