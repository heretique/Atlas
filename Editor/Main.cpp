#define SDL_MAIN_HANDLED

#include "MainWindow.h"
#include "SDLApp.h"
#include "SDLWindow.h"
#include "fmt/format.h"

using namespace atlasEditor;

int main(int argc, char **argv)
{
    SDLApp::get().init(SDL_INIT_VIDEO);

    for (int i = 0; i < 3; ++i)
        new SDLWindow(fmt::format("BGFX {}", i).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN
                                        | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    return SDLApp::get().exec();
}

