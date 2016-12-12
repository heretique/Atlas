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
    vector<std::unique_ptr<SDLWindow>> killWindows;

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
            for (auto it = _windows.begin(); it != _windows.end();)
            {
                if (shouldCloseWindow(e, *(*it).get()))
                {
                    (*it)->releaseFramebuffer();
                    killWindows.push_back(std::move(*it));
                    it = _windows.erase(it);
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
            // TODO add proper timing
            window->doUpdate(1.f/60.f);
        }
        bgfx::frame();

        if (killWindows.size())
        {
            for (auto it = killWindows.begin(); it != killWindows.end();)
            {
                it = killWindows.erase(it);
            }
            bgfx::frame();
        }
    }

    return 0;
}
