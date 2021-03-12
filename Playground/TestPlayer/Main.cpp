#define SDL_MAIN_HANDLED

#include "Hq/DynFreeList.h"
#include "Engine/Engine.h"
#include "Hq/Handle.h"
#include "MainWindow.h"
#include "SDL/SDLApp.h"
#include "SDL/SDLWindow.h"

//#include <easy/profiler.h>
#include <fmt/printf.h>

using namespace atlas;

int main(int argc, char** argv)
{
    //    EASY_MAIN_THREAD;

    try
    {
        SDLApp app = SDLApp();
        if (0 < app.init(SDL_INIT_VIDEO))
        {
            const char* err = SDL_GetError();
            fmt::print("SDL failed to init, err: {}\n", err);
            return -1;
        }

        //    SDL_DisplayMode dm;
        //    SDL_GetDesktopDisplayMode(0, &dm);

        MainWindow* win = new MainWindow(fmt::format("Atlas Demo").c_str(), SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, 1280, 720 /*dm.w, dm.h*/);
        win->onInit();
        app.addWindow(win);

        //    profiler::startListen();

        return app.exec();
    }
    catch (std::logic_error& e)
    {
        fmt::print("Exception: {}\n", e.what());
    }
    return -1;
}
