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

bool SDLApp::shouldCloseWindow(SDL_Event &e, SDLWindow &window)
{
    if ((e.type == SDL_WINDOWEVENT) && (e.window.windowID == window.winId()) && (e.window.event == SDL_WINDOWEVENT_CLOSE))
        return true;

    return false;
}

void SDLApp::quit()
{
    _running = false;
}

int SDLApp::exec()
{
    SDL_Event e;
    int eventCount = 0;

    while (_running) {
        eventCount = 0;
        while(SDL_PollEvent( &e ) != 0 && eventCount++ < 10 ) {
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
            for (auto it = _windows.begin(); it != _windows.end();)
            {
                if (shouldCloseWindow(e, *(*it).get()))
                {
                    if ((*it)->isMain()){
                        quit();
                        break;
                    } else {
                        it = _windows.erase(it);
                    }
                }
                else
                {
                    (*it)->handleEvent(e);
                    ++it;
                }
            }
        }

        for (auto& window: _windows)
        {
            window->doUpdate(1.f/60.f);
        }
        bgfx::frame();
    }

    return 0;
}
