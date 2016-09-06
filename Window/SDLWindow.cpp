#include "SDLWindow.h"
#include <SDL2/SDL.h>
#include "SDLApp.h"
#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <SDL2/SDL_syswm.h>
#include <imgui/imgui.h>
#include <fmt/printf.h>
#include <bx/fpumath.h>

#include "vs_ocornut_imgui.bin.h"
#include "fs_ocornut_imgui.bin.h"

bool SDLWindow::_initialized = false;
SDL_GLContext SDLWindow::_glContext = 0;
u32 SDLWindow::_debug  = BGFX_DEBUG_TEXT;
u32 SDLWindow::_reset  = BGFX_RESET_VSYNC;
u8 SDLWindow::_windowCount = 0;



struct ImGuiBgfx {

    void init(int width, int height) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(width, height);
        io.DeltaTime = 1.0f / 60.0f;
        io.IniFilename = NULL;


        const bgfx::Memory* vsmem;
        const bgfx::Memory* fsmem;

        switch (bgfx::getRendererType() )
        {
        case bgfx::RendererType::Direct3D9:
            vsmem = bgfx::makeRef(vs_ocornut_imgui_dx9, sizeof(vs_ocornut_imgui_dx9) );
            fsmem = bgfx::makeRef(fs_ocornut_imgui_dx9, sizeof(fs_ocornut_imgui_dx9) );
            break;

        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:
            vsmem = bgfx::makeRef(vs_ocornut_imgui_dx11, sizeof(vs_ocornut_imgui_dx11) );
            fsmem = bgfx::makeRef(fs_ocornut_imgui_dx11, sizeof(fs_ocornut_imgui_dx11) );
            break;

        case bgfx::RendererType::Metal:
            vsmem = bgfx::makeRef(vs_ocornut_imgui_mtl, sizeof(vs_ocornut_imgui_mtl) );
            fsmem = bgfx::makeRef(fs_ocornut_imgui_mtl, sizeof(fs_ocornut_imgui_mtl) );
            break;

        default:
            vsmem = bgfx::makeRef(vs_ocornut_imgui_glsl, sizeof(vs_ocornut_imgui_glsl) );
            fsmem = bgfx::makeRef(fs_ocornut_imgui_glsl, sizeof(fs_ocornut_imgui_glsl) );
            break;
        }

        bgfx::ShaderHandle vsh = bgfx::createShader(vsmem);
        bgfx::ShaderHandle fsh = bgfx::createShader(fsmem);
        _program = bgfx::createProgram(vsh, fsh, true);

        _vDecl
            .begin()
            .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
            .end();

        _tex = bgfx::createUniform("s_tex", bgfx::UniformType::Int1);

        u8* data;
        i32 texWidth;
        i32 texHeight;
        i32 texBits;
        io.Fonts->GetTexDataAsRGBA32(&data, &texWidth, &texHeight, &texBits);
        _texture = bgfx::createTexture2D(
              (u16)texWidth
            , (u16)texHeight
            , 1
            , bgfx::TextureFormat::BGRA8
            , 0
            , bgfx::copy(data, texWidth*texHeight*texBits)
            );
    }

    void render(u8 viewId, ImDrawData* drawData)
    {
        const ImGuiIO& io = ImGui::GetIO();
        const float width  = io.DisplaySize.x;
        const float height = io.DisplaySize.y;

        {
            float ortho[16];
            bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, -1.0f, 1.0f);
            bgfx::setViewTransform(viewId, NULL, ortho);
        }

        // Render command lists
        for (i32 ii = 0, num = drawData->CmdListsCount; ii < num; ++ii)
        {
            bgfx::TransientVertexBuffer tvb;
            bgfx::TransientIndexBuffer tib;

            const ImDrawList* drawList = drawData->CmdLists[ii];
            u32 numVertices = (u32)drawList->VtxBuffer.size();
            u32 numIndices  = (u32)drawList->IdxBuffer.size();

            if (!bgfx::checkAvailTransientVertexBuffer(numVertices, _vDecl)
            ||  !bgfx::checkAvailTransientIndexBuffer(numIndices) )
            {
                // not enough space in transient buffer just quit drawing the rest...
                break;
            }

            bgfx::allocTransientVertexBuffer(&tvb, numVertices, _vDecl);
            bgfx::allocTransientIndexBuffer(&tib, numIndices);

            ImDrawVert* verts = (ImDrawVert*)tvb.data;
            memcpy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert) );

            ImDrawIdx* indices = (ImDrawIdx*)tib.data;
            memcpy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx) );

            u32 offset = 0;
            for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
            {
                if (cmd->UserCallback)
                {
                    cmd->UserCallback(drawList, cmd);
                }
                else if (0 != cmd->ElemCount)
                {
                    u64 state = 0
                        | BGFX_STATE_RGB_WRITE
                        | BGFX_STATE_ALPHA_WRITE
                        | BGFX_STATE_MSAA
                        ;

                    state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

                    const u16 xx = u16(bx::fmax(cmd->ClipRect.x, 0.0f) );
                    const u16 yy = u16(bx::fmax(cmd->ClipRect.y, 0.0f) );
                    bgfx::setScissor(xx, yy
                            , u16(bx::fmin(cmd->ClipRect.z, 65535.0f)-xx)
                            , u16(bx::fmin(cmd->ClipRect.w, 65535.0f)-yy)
                            );

                    bgfx::setState(state);
                    bgfx::setTexture(0, _tex, _texture);
                    bgfx::setVertexBuffer(&tvb, 0, numVertices);
                    bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
                    bgfx::submit(viewId, _program);
                }

                offset += cmd->ElemCount;
            }
        }
    }

    void destroy() {
        ImGui::Shutdown();

        bgfx::destroyUniform(_tex);
        bgfx::destroyTexture(_texture);
        bgfx::destroyProgram(_program);
    }

    bgfx::VertexDecl    _vDecl;
    bgfx::ProgramHandle _program;
    bgfx::TextureHandle _texture;
    bgfx::UniformHandle _tex;
};

static ImGuiBgfx s_imguiBgfx;


SDLWindow::SDLWindow(const char *title, int x, int y, int w, int h, u32 flags)
{
    _width = w;
    _height = h;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    _window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (_window == nullptr)
    {
        fmt::print("SLD Window creation failed, err: {}\n", SDL_GetError());
        return;
    }

    _windowId = SDL_GetWindowID(_window);

    if (!_initialized)
    {
        _glContext = SDL_GL_CreateContext(_window);

        if (!bgfxInit()) fmt::print("Failed to initialize bgfx\n");
        _initialized = true;
        _isDefault = true;
        const bgfx::Caps* caps = bgfx::getCaps();
        bool swapChainSupported = 0 != (caps->supported & BGFX_CAPS_SWAP_CHAIN);
        fmt::print("Swapchain supported: {}\n", swapChainSupported);
    }

    if (!_isDefault)
    {
        fmt::print("Native Window Handle: {}\n", nativeHandle());
        _framebuffer = bgfx::createFrameBuffer(nativeHandle(), _width, _height);
        bgfx::setViewFrameBuffer(_viewId, _framebuffer);
    }
    else
    {
        _framebuffer = BGFX_INVALID_HANDLE;
    }

    _viewId = _windowCount++;
    bgfx::setViewName(_viewId, title);
    bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height) );
    bgfx::setViewClear(_viewId
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , 0x303030ff
                       , 1.0f
                       , 0
                       );

    imguiInit();
    SDLApp::get().addWindow(this);
}

SDLWindow::~SDLWindow()
{
    imguiShutdown();

    if (bgfx::isValid(_framebuffer))
    {
        bgfx::destroyFrameBuffer(_framebuffer);
        bgfx::setViewFrameBuffer(_viewId, BGFX_INVALID_HANDLE);
    }

    if (_isDefault)
    {
        bgfx::shutdown();
        SDL_GL_DeleteContext(_glContext);
    }

    SDL_DestroyWindow(_window);
}

void SDLWindow::handleEvent(SDL_Event &e)
{
    //If an event was detected for this window
    if( e.type == SDL_WINDOWEVENT && e.window.windowID == _windowId )
    {
        switch(e.window.event)
        {
        //Get new dimensions and recreate framebuffer
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            if (_width != e.window.data1 ||
                    _height != e.window.data2)
            {
                _width = e.window.data1;
                _height = e.window.data2;

                if (bgfx::isValid(_framebuffer))
                {
                    bgfx::destroyFrameBuffer(_framebuffer);
                    _framebuffer = bgfx::createFrameBuffer(nativeHandle(), _width, _height);
                    bgfx::setViewFrameBuffer(_viewId, _framebuffer);
                }

                bgfx::reset(_width, _height);
                bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height) );
                bgfx::setViewClear(_viewId
                    , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                    , 0x303030ff
                    , 1.0f
                    , 0
                    );
            }
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (_isDefault)
            {
                SDLApp::get().quit();
                return;
            }
            SDLApp::get().closeWindow(this);
            break;
        }
    }
}

void SDLWindow::update(float dt)
{

}

void SDLWindow::onGUI()
{

}

void SDLWindow::doUpdate(float dt)
{
    bgfx::setViewFrameBuffer(_viewId, _framebuffer);
    bgfx::touch(_viewId);
    // render content
    update(dt);
    // GUI
    imguiPushCtx();
    imguiNewFrame();
    onGUI();
    imguiRender();
    imguiPopCtx();
}

bool SDLWindow::imguiInit()
{
    if (_isDefault)
        s_imguiBgfx.init(_width, _height);

    _imguiCtx = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
#if BX_PLATFORM_WINDOWS
    io.ImeWindowHandle =  nativeHandle();
#endif
    io.RenderDrawListsFn = NULL;

    return true;
}

void SDLWindow::imguiShutdown()
{
    ImGui::DestroyContext(_imguiCtx);
    if (_isDefault)
    {
        s_imguiBgfx.destroy();
    }
}


void SDLWindow::imguiNewFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)_width, (float)_height);
    io.DeltaTime = 1.0f/60.0f; // TODO
    ImGui::NewFrame();
}

void SDLWindow::imguiPushCtx()
{
    _prevImguiCtx = ImGui::GetCurrentContext();
    ImGui::SetCurrentContext(_imguiCtx);
}

void SDLWindow::imguiPopCtx()
{
    ImGui::SetCurrentContext(_prevImguiCtx);
}


void SDLWindow::imguiRender()
{

    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();

    s_imguiBgfx.render(_viewId, drawData);
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
#endif
    setPlatformData(pd);

    if (!bgfx::init(bgfx::RendererType::OpenGL))
        return false;

    bgfx::reset(_width, _height, _reset);

    // Enable debug text.
    bgfx::setDebug(_debug);

    return true;
}

void *SDLWindow::nativeHandle()
{
    // get native  window handle
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
        return nullptr;

#if BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#endif

    return nullptr;
}


