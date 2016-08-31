#include "SDLWindow.h"
#include <SDL2/SDL.h>
#include "SDLApp.h"
#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <SDL2/SDL_syswm.h>

SDLWindow::SDLWindow(const char *title, int x, int y, int w, int h, u32 flags)
{
    _width = w;
    _height = h;

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    _window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (_window == nullptr)
        throw std::system_error();

    _windowId = SDL_GetWindowID(_window);
    SDLApp::get().addWindow(this);

    _glContext = SDL_GL_CreateContext(_window);



    bgfx::PlatformData pd;
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
    {
        throw std::system_error();
    }

#if BX_PLATFORM_WINDOWS
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.win.window;
    pd.context      = _glContext;
#endif
    setPlatformData(pd);

    _debug  = BGFX_DEBUG_TEXT;
    _reset  = BGFX_RESET_VSYNC;
    bgfx::init(bgfx::RendererType::OpenGL);
    bgfx::reset(w, h, _reset);
    // Enable debug text.
    bgfx::setDebug(_debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , 0x303030ff
                       , 1.0f
                       , 0
                       );
}

SDLWindow::~SDLWindow()
{
    bgfx::shutdown();
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
}

void SDLWindow::handleEvent(SDL_Event &e)
{
    //If an event was detected for this window
    if( e.type == SDL_WINDOWEVENT && e.window.windowID == _windowId )
    {
        switch(e.window.event)
        {
        //Get new dimensions and repaint
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            _width = e.window.data1;
            _height = e.window.data2;
            break;
        }
    }
}

void SDLWindow::update(float dt)
{

}

void SDLWindow::doUpdate(float dt)
{
    bgfx::setViewRect(0, 0, 0, uint16_t(_width), uint16_t(_height) );

    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);
    update(dt);
    bgfx::frame();
}



