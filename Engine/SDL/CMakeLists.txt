add_library(SDL STATIC
    PRIVATE
        Private/SDLApp.cpp
        Private/SDLWindow.cpp
    PUBLIC
        Public/SDL/SDLApp.h
        Public/SDL/SDLWindow.h
    )

add_library(Atlas::SDL ALIAS SDL)


target_include_directories(SDL
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)

target_link_libraries(SDL Atlas::Core)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_library(sdl2 STATIC IMPORTED)
    set_target_properties(sdl2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/sdl/include)
    set_target_properties(sdl2 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/sdl/win/msvc/x64/SDL2.lib)
    target_link_libraries(SDL sdl2)
endif()
