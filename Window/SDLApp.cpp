#include "SDLApp.h"
#include "SDL2/SDL.h"
#include "SDLWindow.h"



SDLApp::~SDLApp()
{
    SDL_Quit();
}


int SDLApp::init(u32 flags)
{
    return SDL_Init(flags);
}

int SDLApp::addWindow(SDLWindow *window)
{
    _windows.push_back(window);
    return _windows.size();
}


int SDLApp::exec()
{
    SDL_Event e;

    while (_running) {
        while(SDL_PollEvent( &e ) != 0) {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                _running = false;
            }
            for (SDLWindow* window: _windows)
                window->handleEvent(e);
        }

        for (SDLWindow* window: _windows)
        {
            window->doUpdate(0.0f);
            bgfx::frame();
        }
    }

    return 0;
}
