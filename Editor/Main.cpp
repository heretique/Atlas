#define SDL_MAIN_HANDLED

#include "MainWindow.h"
#include "SDLApp.h"
#include "SDLWindow.h"

using namespace atlasEditor;

int main(int argc, char **argv)
{
    SDLApp::get().init(SDL_INIT_VIDEO);

    SDLWindow window("BGFX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN
                     | SDL_WINDOW_RESIZABLE);

    return SDLApp::get().exec();
}

