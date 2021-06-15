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

find_package (SDL2 CONFIG REQUIRED)
target_link_libraries(SDL Atlas::Core SDL2::SDL2)
