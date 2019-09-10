#include "SDLApp.h"

#include <SDL2/SDL.h>
#include "SDLWindow.h"

#include <chrono>
#include <vector>
//#include <easy/profiler.h>
namespace atlas
{
SDLApp::~SDLApp()
{
    while (!_windows.empty())
        _windows.pop_back();
    SDL_Quit();
}

int SDLApp::init(u32 flags)
{
    return SDL_Init(flags);
}

int SDLApp::addWindow(SDLWindow* window)
{
    _windows.push_back(std::unique_ptr<SDLWindow>(window));
    return _windows.size();
}

bool SDLApp::shouldCloseWindow(SDL_Event& e, SDLWindow& window)
{
    if ((e.type == SDL_WINDOWEVENT) && (e.window.windowID == window.winId()) &&
        (e.window.event == SDL_WINDOWEVENT_CLOSE))
        return true;

    return false;
}

void SDLApp::quit()
{
    _running = false;
}

int SDLApp::exec()
{
    SDL_Event                                e;
    std::vector<std::unique_ptr<SDLWindow> > killWindows;

    while (_running)
    {
        using namespace std::chrono;
        static high_resolution_clock::time_point lastTime = high_resolution_clock::now();
        //        EASY_BLOCK("Main loop", profiler::colors::Green);
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
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
                    if ((*it)->isMain())
                    {
                        _running = false;
                        break;
                    }
                    else
                    {
                        killWindows.push_back(std::move(*it));
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

        high_resolution_clock::time_point nowTime = high_resolution_clock::now();
        duration<double>                  delta   = duration_cast<duration<double> >(nowTime - lastTime);
        lastTime                                  = nowTime;
        for (auto& window : _windows)
        {
            // TODO add proper timing
            window->doUpdate(delta.count());
        }
        bgfx::frame();

        for (auto it = killWindows.begin(); it != killWindows.end();)
        {
            it = killWindows.erase(it);
        }
    }

    return 0;
}
}  // namespace atlas
