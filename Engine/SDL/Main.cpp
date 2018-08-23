#define SDL_MAIN_HANDLED

#include "Core/DynFreeList.h"
#include "Core/Engine.h"
#include "Core/Handle.h"
#include "MainWindow.h"
#include "SDLApp.h"
#include "SDLWindow.h"

//#include <easy/profiler.h>
#include <fmt/printf.h>

using namespace atlas;

int main(int argc, char** argv)
{
    DynFreeList<uint64_t, Handle<20, 12> > test(1024);
    //    EASY_MAIN_THREAD;

    SDLApp& app = SDLApp::get();
    if (0 < app.init(SDL_INIT_VIDEO))
    {
        const char* err = SDL_GetError();
        fmt::print("SDL failed to init, err: {}\n", err);
        return -1;
    }

    //    SDL_DisplayMode dm;
    //    SDL_GetDesktopDisplayMode(0, &dm);

    MainWindow* win = new MainWindow(fmt::format("BGFX {}", 1).c_str(), SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, 1280, 720 /*dm.w, dm.h*/);
    win->init();
    app.addWindow(win);

    //    profiler::startListen();

    return SDLApp::get().exec();
}
