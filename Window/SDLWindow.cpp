#include "SDLWindow.h"
#include <SDL2/SDL.h>
#include "SDLApp.h"
#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <SDL2/SDL_syswm.h>
#include <ImGUI/imgui.h>
#include <fmt/printf.h>

SDLWindow::SDLWindow(const char *title, int x, int y, int w, int h, u32 flags)
{
    _width = w;
    _height = h;

    // Setup window
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    _window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (_window == nullptr)
    {
        fmt::print("SLD Window creation failed, err: {}", SDL_GetError());
        return;
    }

    _windowId = SDL_GetWindowID(_window);
    SDLApp::get().addWindow(this);

    _glContext = SDL_GL_CreateContext(_window);


    if (!bgfxInit()) fmt::print("Failed to initialize bgfx");
    if (!imguiInit()) fmt::print("Failed to initialize imgui");
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
            bgfx::reset(_width, _height);
            bgfx::setViewRect(0, 0, 0, uint16_t(_width), uint16_t(_height) );
            break;
        }
    }
}

void SDLWindow::update(float dt)
{

}

void SDLWindow::doUpdate(float dt)
{
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);
    update(dt);
    bgfx::frame();
}

bool SDLWindow::imguiInit()
{
    ImGuiIO& io = ImGui::GetIO();

    // get native  window handle
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
        return false;
#if BX_PLATFORM_WINDOWS
    io.ImeWindowHandle =  wmi.info.win.window;
#endif

    io.RenderDrawListsFn = &imguiRenderDrawLists;

    return true;
}

void SDLWindow::imguiShutdown()
{

}

void SDLWindow::imguiNewFrame()
{

}

bool SDLWindow::bgfxInit()
{
    bgfx::PlatformData pd;
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
        return false;

#if BX_PLATFORM_WINDOWS
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.win.window;
    pd.context      = _glContext;
#endif
    setPlatformData(pd);

    _debug  = BGFX_DEBUG_TEXT;
    _reset  = BGFX_RESET_VSYNC;

    if (!bgfx::init(bgfx::RendererType::OpenGL))
        return false;

    bgfx::reset(_width, _height, _reset);
    bgfx::setViewRect(0, 0, 0, uint16_t(_width), uint16_t(_height) );
    // Enable debug text.
    bgfx::setDebug(_debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , 0x303030ff
                       , 1.0f
                       , 0
                       );
    return true;
}

void imguiRenderDrawLists(ImDrawData *drawData)
{
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    drawData->ScaleClipRects(io.DisplayFramebufferScale);
}


