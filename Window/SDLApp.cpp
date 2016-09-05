#include "SDLApp.h"
#include "SDL2/SDL.h"
#include "SDLWindow.h"



SDLApp::~SDLApp()
{
    while(!_windows.empty())
        _windows.pop_back();
    SDL_Quit();
}


int SDLApp::init(u32 flags)
{
    return SDL_Init(flags);
}

int SDLApp::addWindow(SDLWindow *window)
{
    _windows.push_back(std::unique_ptr<SDLWindow>(window));
    return _windows.size();
}

void SDLApp::closeWindow(SDLWindow *window)
{
    for(auto& w: _windows)
        if (w.get() == window)
        {
            _windows.remove(w);
            break;
        }
}

void SDLApp::quit()
{
    _running = false;
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
            if (e.type == SDL_KEYDOWN)
            {
                SDL_Keycode keyPressed = e.key.keysym.sym;

                switch (keyPressed)
                {
                case SDLK_ESCAPE:
                    _running = false;
                    break;
                }
            }
            for (auto& window: _windows)
                window->handleEvent(e);
        }

        for (auto& window: _windows)
        {
            window->doUpdate(0.0f);
        }
        bgfx::frame();
    }

    return 0;
}
