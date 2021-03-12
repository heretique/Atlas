#pragma once

#include "Hq/BasicTypes.h"
#include <list>
#include <memory>

union SDL_Event;

namespace atlas
{
class SDLWindow;

class SDLApp
{

public:
    SDLApp() = default;
    virtual ~SDLApp();

    int  init(u32 flags);
    int  addWindow(SDLWindow* window);
    int  exec();
    bool shouldCloseWindow(SDL_Event& e, SDLWindow& window);
    void quit();

private:
    bool                                   _running {true};
    std::list<std::unique_ptr<SDLWindow> > _windows;
};
}
