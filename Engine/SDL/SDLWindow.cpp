#include "SDLWindow.h"
#include <bx/math.h>
#include <bgfx/platform.h>

#include "Core/Engine.h"
#include "SDLApp.h"
#include "fs_ocornut_imgui.bin.h"
#include "vs_ocornut_imgui.bin.h"

#include <chrono>
#include <thread>
#include <cstdarg>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
//#include <easy/profiler.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

namespace atlas
{
bool          SDLWindow::_initialized = false;
SDL_GLContext SDLWindow::_glContext   = nullptr;
u32           SDLWindow::_debug       = BGFX_DEBUG_NONE;  // BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS | BGFX_DEBUG_PROFILER;
u32           SDLWindow::_reset       = BGFX_RESET_NONE;
u8            SDLWindow::_windowCount = 0;

///////////////// IMGUI Rendering

struct ImGuiBgfx
{
    void init(int width, int height)
    {
        ImGuiIO& io    = ImGui::GetIO();
        io.DisplaySize = ImVec2(width, height);
        io.DeltaTime   = 1.0f / 60.0f;
        io.IniFilename = NULL;

        const bgfx::Memory* vsmem;
        const bgfx::Memory* fsmem;

        switch (bgfx::getRendererType())
        {
            case bgfx::RendererType::Direct3D9:
                vsmem = bgfx::makeRef(vs_ocornut_imgui_dx9, sizeof(vs_ocornut_imgui_dx9));
                fsmem = bgfx::makeRef(fs_ocornut_imgui_dx9, sizeof(fs_ocornut_imgui_dx9));
                break;

            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                vsmem = bgfx::makeRef(vs_ocornut_imgui_dx11, sizeof(vs_ocornut_imgui_dx11));
                fsmem = bgfx::makeRef(fs_ocornut_imgui_dx11, sizeof(fs_ocornut_imgui_dx11));
                break;

            case bgfx::RendererType::Metal:
                vsmem = bgfx::makeRef(vs_ocornut_imgui_mtl, sizeof(vs_ocornut_imgui_mtl));
                fsmem = bgfx::makeRef(fs_ocornut_imgui_mtl, sizeof(fs_ocornut_imgui_mtl));
                break;

            default:
                vsmem = bgfx::makeRef(vs_ocornut_imgui_glsl, sizeof(vs_ocornut_imgui_glsl));
                fsmem = bgfx::makeRef(fs_ocornut_imgui_glsl, sizeof(fs_ocornut_imgui_glsl));
                break;
        }

        bgfx::ShaderHandle vsh = bgfx::createShader(vsmem);
        bgfx::ShaderHandle fsh = bgfx::createShader(fsmem);
        _program               = bgfx::createProgram(vsh, fsh, true);

        _vDecl.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        _tex = bgfx::createUniform("s_tex", bgfx::UniformType::Int1);

        u8* data;
        i32 texWidth;
        i32 texHeight;
        i32 texBits;
        io.Fonts->GetTexDataAsRGBA32(&data, &texWidth, &texHeight, &texBits);
        _texture = bgfx::createTexture2D((u16)texWidth,               //
                                         (u16)texHeight,              //
                                         false,                       //
                                         1,                           //
                                         bgfx::TextureFormat::BGRA8,  //
                                         0,                           //
                                         bgfx::copy(data, texWidth * texHeight * texBits));
    }

    void render(u8 viewId, bgfx::FrameBufferHandle framebuffer, ImDrawData* drawData)
    {
        //        EASY_FUNCTION(profiler::colors::Teal);
        const ImGuiIO& io     = ImGui::GetIO();
        const float    width  = io.DisplaySize.x;
        const float    height = io.DisplaySize.y;

        {
            bgfx::setViewFrameBuffer(viewId, framebuffer);
            bgfx::setViewRect(viewId, 0, 0, uint16_t(width), uint16_t(height));
            const bgfx::Caps* caps = bgfx::getCaps();
            float             ortho[16];
            bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, -1.0f, 1.0f, 0, caps->homogeneousDepth);
            bgfx::setViewTransform(viewId, NULL, ortho);
        }

        // Render command lists
        for (i32 ii = 0, num = drawData->CmdListsCount; ii < num; ++ii)
        {
            bgfx::TransientVertexBuffer tvb;
            bgfx::TransientIndexBuffer  tib;

            const ImDrawList* drawList    = drawData->CmdLists[ii];
            u32               numVertices = (u32)drawList->VtxBuffer.size();
            u32               numIndices  = (u32)drawList->IdxBuffer.size();

            if (numVertices != bgfx::getAvailTransientVertexBuffer(numVertices, _vDecl) ||
                numIndices != bgfx::getAvailTransientIndexBuffer(numIndices))
            {
                // not enough space in transient buffer just quit drawing the rest...
                break;
            }

            bgfx::allocTransientVertexBuffer(&tvb, numVertices, _vDecl);
            bgfx::allocTransientIndexBuffer(&tib, numIndices);

            ImDrawVert* verts = (ImDrawVert*)tvb.data;
            memcpy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert));

            ImDrawIdx* indices = (ImDrawIdx*)tib.data;
            memcpy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx));

            u32 offset = 0;
            for (const ImDrawCmd *cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd;
                 ++cmd)
            {
                if (cmd->UserCallback)
                {
                    cmd->UserCallback(drawList, cmd);
                }
                else if (0 != cmd->ElemCount)
                {
                    u64 state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;

                    state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

                    const u16 xx = u16(fmax(cmd->ClipRect.x, 0.0f));
                    const u16 yy = u16(fmax(cmd->ClipRect.y, 0.0f));
                    bgfx::setScissor(xx, yy, u16(fmin(cmd->ClipRect.z, 65535.0f) - xx),
                                     u16(fmin(cmd->ClipRect.w, 65535.0f) - yy));

                    bgfx::setState(state);
                    bgfx::setTexture(0, _tex, _texture);
                    bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
                    bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
                    bgfx::submit(viewId, _program);
                }

                offset += cmd->ElemCount;
            }
        }
    }

    void destroy()
    {
        bgfx::destroy(_tex);
        bgfx::destroy(_texture);
        bgfx::destroy(_program);
    }

    bgfx::VertexDecl    _vDecl;
    bgfx::ProgramHandle _program;
    bgfx::TextureHandle _texture;
    bgfx::UniformHandle _tex;
};

static ImGuiBgfx s_imguiBgfx;

////////// BGFX Callbacks ////////////

struct BGFXCallbacss : public bgfx::CallbackI
{
    BGFXCallbacss() {}

    // CallbackI interface
public:
    virtual void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override
    {
        Engine::log().error(_str);
    }

    virtual void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override
    {
        Engine::log().info(format(_format, _argList));
    }
    virtual void profilerBegin(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override {}
    virtual void profilerBeginLiteral(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override
    {
    }
    virtual void     profilerEnd() override {}
    virtual uint32_t cacheReadSize(uint64_t _id) override
    {
        return 0;
    }
    virtual bool cacheRead(uint64_t _id, void* _data, uint32_t _size) override
    {
        return false;
    }
    virtual void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) override {}
    virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch,
                            const void* _data, uint32_t _size, bool _yflip) override
    {
    }
    virtual void captureBegin(uint32_t _width, uint32_t _height, uint32_t _pitch, bgfx::TextureFormat::Enum _format,
                              bool _yflip) override
    {
    }
    virtual void captureEnd() override {}
    virtual void captureFrame(const void* _data, uint32_t _size) override {}

private:
    std::string format(const char* const format, ...)
    {
        auto         temp   = std::vector<char> {};
        auto         length = std::size_t {63};
        std::va_list args;
        while (temp.size() <= length)
        {
            temp.resize(length + 1);
            va_start(args, format);
            const auto status = std::vsnprintf(temp.data(), temp.size(), format, args);
            va_end(args);
            if (status < 0)
                throw std::runtime_error {"string formatting error"};
            length = static_cast<std::size_t>(status);
        }
        return std::string {temp.data(), length};
    }
};

static BGFXCallbacss s_bgfxCallbacks;

SDLWindow::SDLWindow(const char* title, int x, int y, int w, int h)
{
    _width  = w;
    _height = h;
    _title  = title;

    //  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    //  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    //  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    //  SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    _window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (_window == nullptr)
    {
        fmt::print("SDL Window creation failed, err: {}\n", SDL_GetError());
        return;
    }

    _windowId = SDL_GetWindowID(_window);

    if (!_initialized)
    {
        _glContext = SDL_GL_CreateContext(_window);

        if (!bgfxInit(w, h))
            fmt::print("Failed to initialize bgfx\n");
        _initialized = true;
        _isDefault   = true;
    }

    _viewId = _windowCount++;

    if (!_isDefault)
    {
        _framebuffer = bgfx::createFrameBuffer(nativeHandle(), _width, _height);
    }
    else
    {
        _framebuffer = BGFX_INVALID_HANDLE;
    }
    bgfx::setViewFrameBuffer(_viewId, _framebuffer);
    bgfx::setViewName(_viewId, title);
    bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height));
    bgfx::setViewClear(_viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    imguiInit();
}

SDLWindow::~SDLWindow()
{
    imguiShutdown();

    releaseFramebuffer();

    if (_isDefault)
    {
        bgfx::shutdown();
        SDL_GL_DeleteContext(_glContext);
        SDLApp::get().quit();
    }

    SDL_DestroyWindow(_window);
}

u32 SDLWindow::winId() const
{
    return _windowId;
}

bool SDLWindow::isMain() const
{
    return _isDefault;
}

void SDLWindow::init() {}

void SDLWindow::handleEvent(SDL_Event& e)
{
    // If an event was detected for this window
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == _windowId)
    {
        handleWindowEvent(e.window);
        return;
    }

    u32 flags = SDL_GetWindowFlags(_window);
    if (flags & SDL_WINDOW_INPUT_FOCUS || flags & SDL_WINDOW_MOUSE_FOCUS)
    {
        handleInputEvent(e);
    }
}

void SDLWindow::handleWindowEvent(SDL_WindowEvent& e)
{
    switch (e.event)
    {
        // Get new dimensions and recreate framebuffer
        case SDL_WINDOWEVENT_RESIZED:
            if (_width != u32(e.data1) || _height != u32(e.data2))
            {
                _width  = e.data1;
                _height = e.data2;

                if (bgfx::isValid(_framebuffer))
                {
                    bgfx::resetView(_viewId);
                    bgfx::destroy(_framebuffer);
                    _framebuffer = bgfx::createFrameBuffer(nativeHandle(), _width, _height);
                    bgfx::setViewFrameBuffer(_viewId, _framebuffer);
                    bgfx::setViewClear(_viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
                }
                else
                {
                    bgfx::reset(_width, _height);
                }

                bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height));
                Engine::log().info("Resize event, window: {}, view: {}, size({},{})\n ", _windowId, _viewId, _width,
                                   _height);
            }
            break;
    }
}

void SDLWindow::handleInputEvent(SDL_Event& e)
{
    imguiPushCtx();
    ImGuiIO& io = ImGui::GetIO();
    imguiPopCtx();
    switch (e.type)
    {
        case SDL_MOUSEMOTION:
            io.MousePos = ImVec2(e.motion.x, e.motion.y);
            break;
        case SDL_MOUSEWHEEL:
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            io.MouseDown[0] = (e.button.button == SDL_BUTTON_LEFT && e.type == SDL_MOUSEBUTTONDOWN);
            io.MouseDown[1] = (e.button.button == SDL_BUTTON_RIGHT && e.type == SDL_MOUSEBUTTONDOWN);
            io.MouseDown[3] = (e.button.button == SDL_BUTTON_MIDDLE && e.type == SDL_MOUSEBUTTONDOWN);
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            int key          = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;
            io.KeysDown[key] = (e.type == SDL_KEYDOWN);
            io.KeyShift      = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            io.KeyCtrl       = ((SDL_GetModState() & KMOD_CTRL) != 0);
            io.KeyAlt        = ((SDL_GetModState() & KMOD_ALT) != 0);
            io.KeySuper      = ((SDL_GetModState() & KMOD_GUI) != 0);
            break;
    }
}

void SDLWindow::releaseFramebuffer()
{
    if (bgfx::isValid(_framebuffer))
    {
        bgfx::setViewFrameBuffer(_viewId, BGFX_INVALID_HANDLE);

        bgfx::destroy(_framebuffer);

        _framebuffer = BGFX_INVALID_HANDLE;
    }
}

void SDLWindow::update(float /*dt*/)
{
    bgfx::dbgTextPrintf(0, 5, 0x2f, "SDLWindow::update");
}

void SDLWindow::onGUI() {}

SDLWindow::Size SDLWindow::windowSize() const
{
    return Size {_width, _height};
}

void SDLWindow::doUpdate(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    bgfx::setViewFrameBuffer(_viewId, _framebuffer);
    bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height));
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
    assert(_imguiCtx == nullptr);
    _imguiCtx = ImGui::CreateContext();
    if (_isDefault)
    {
        s_imguiBgfx.init(_width, _height);
    }

    imguiPushCtx();
    ImGuiIO& io = ImGui::GetIO();
    imguiPopCtx();
    io.ImeWindowHandle   = nativeHandle();
    io.RenderDrawListsFn = nullptr;

    io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;  // Keyboard mapping. ImGui will use those
                                         // indices to peek into the io.KeyDown[]
                                         // array that we will update during the
                                         // application lifetime.
    io.KeyMap[ImGuiKey_LeftArrow]  = SDLK_LEFT;
    io.KeyMap[ImGuiKey_LeftArrow]  = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp]     = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown]   = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home]       = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End]        = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert]     = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete]     = SDLK_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = SDLK_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter]      = SDLK_RETURN;
    io.KeyMap[ImGuiKey_Escape]     = SDLK_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = SDLK_a;
    io.KeyMap[ImGuiKey_C]          = SDLK_c;
    io.KeyMap[ImGuiKey_V]          = SDLK_v;
    io.KeyMap[ImGuiKey_X]          = SDLK_x;
    io.KeyMap[ImGuiKey_Y]          = SDLK_y;
    io.KeyMap[ImGuiKey_Z]          = SDLK_z;

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
    //    EASY_FUNCTION(profiler::colors::Teal);
    ImGuiIO& io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(float(_width), float(_height));
    io.DeltaTime   = 1.0f / 60.0f;  // TODO
    ImGui::NewFrame();
}

void SDLWindow::imguiPushCtx()
{
    //    EASY_FUNCTION(profiler::colors::Teal);
    _prevImguiCtx = ImGui::GetCurrentContext();
    ImGui::SetCurrentContext(_imguiCtx);
}

void SDLWindow::imguiPopCtx()
{
    ImGui::SetCurrentContext(_prevImguiCtx);
}

void SDLWindow::imguiMoveWindow()
{
    //    EASY_FUNCTION(profiler::colors::Teal);

    if (ImGui::IsMouseClicked(0))
    {
        int wx, wy, mx, my;
        SDL_GetWindowPosition(_window, &wx, &wy);
        SDL_GetGlobalMouseState(&mx, &my);
        _moveOffsetX = mx - wx;
        _moveOffsetY = my - wy;
    }

    if ((_moveOffsetX >= 0 && _moveOffsetY <= 22) &&  // cursor must be *on* the title bar
        ImGui::IsMouseDragging(0))
    {
        int posX, posY;
        SDL_GetGlobalMouseState(&posX, &posY);
        SDL_SetWindowPosition(_window, posX - _moveOffsetX, posY - _moveOffsetY);
    }
}

void SDLWindow::imguiRender()
{
    //    EASY_FUNCTION(profiler::colors::Teal);

    //    ImGui::End();
    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();

    s_imguiBgfx.render(255, _framebuffer, drawData);
}

bool SDLWindow::bgfxInit(uint32_t w, uint32_t h)
{
    bgfx::PlatformData pd;
    SDL_SysWMinfo      wmi;
    SDL_VERSION(&wmi.version);
    //  if (!SDL_GetWindowWMInfo(_window, &wmi))
    //    return false;

    SDL_GetWindowWMInfo(_window, &wmi);
#if BX_PLATFORM_WINDOWS
    pd.ndt = NULL;
    pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_RPI

#elif BX_PLATFORM_LINUX
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    pd.ndt = NULL;
    pd.nwh = wmi.info.cocoa.window;
#endif
    pd.context      = nullptr;
    pd.backBuffer   = nullptr;
    pd.backBufferDS = nullptr;

    setPlatformData(pd);

    bgfx::Init initParams;
    initParams.type                   = bgfx::RendererType::OpenGL;
    initParams.debug                  = true;
    initParams.resolution.width       = w;
    initParams.resolution.height      = h;
    initParams.limits.transientIbSize = 1 << 20;
    initParams.limits.transientVbSize = 1 << 20;

    if (!bgfx::init(initParams))
        return false;

    bgfx::reset(_width, _height, _reset);

    // Enable debug text.
    bgfx::setDebug(_debug);

    return true;
}

void* SDLWindow::nativeHandle()
{
    // get native  window handle
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi))
        return nullptr;

#if BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#elif BX_PLATFORM_LINUX
    return (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    return (void*)wmi.info.cocoa.window;
#endif

    return nullptr;
}
}  // namespace atlas
