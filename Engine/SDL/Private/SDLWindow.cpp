#include "SDL/SDLWindow.h"
#include <bx/math.h>
#include <bgfx/c99/bgfx.h>
#include "Core/Logger.h"
#include "Core/MemoryManager.h"

#include "SDL/SDLApp.h"
#include "fs_ocornut_imgui.bin.h"
#include "vs_ocornut_imgui.bin.h"

#include <chrono>
#include <thread>
#include <cstdarg>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
//#include <easy/profiler.h>
#include <imgui/imgui.h>

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

        const bgfx_memory_t* vsmem;
        const bgfx_memory_t* fsmem;

        switch (bgfx_get_renderer_type())
        {
            case BGFX_RENDERER_TYPE_DIRECT3D9:
                vsmem = bgfx_make_ref(vs_ocornut_imgui_dx9, sizeof(vs_ocornut_imgui_dx9));
                fsmem = bgfx_make_ref(fs_ocornut_imgui_dx9, sizeof(fs_ocornut_imgui_dx9));
                break;

            case BGFX_RENDERER_TYPE_DIRECT3D11:
            case BGFX_RENDERER_TYPE_DIRECT3D12:
                vsmem = bgfx_make_ref(vs_ocornut_imgui_dx11, sizeof(vs_ocornut_imgui_dx11));
                fsmem = bgfx_make_ref(fs_ocornut_imgui_dx11, sizeof(fs_ocornut_imgui_dx11));
                break;

            case BGFX_RENDERER_TYPE_METAL:
                vsmem = bgfx_make_ref(vs_ocornut_imgui_mtl, sizeof(vs_ocornut_imgui_mtl));
                fsmem = bgfx_make_ref(fs_ocornut_imgui_mtl, sizeof(fs_ocornut_imgui_mtl));
                break;

            default:
                vsmem = bgfx_make_ref(vs_ocornut_imgui_glsl, sizeof(vs_ocornut_imgui_glsl));
                fsmem = bgfx_make_ref(fs_ocornut_imgui_glsl, sizeof(fs_ocornut_imgui_glsl));
                break;
        }

        bgfx_shader_handle_t vsh = bgfx_create_shader(vsmem);
        bgfx_shader_handle_t fsh = bgfx_create_shader(fsmem);
        _program                 = bgfx_create_program(vsh, fsh, true);

        bgfx_vertex_layout_begin(&_layout, bgfx_get_renderer_type());
        bgfx_vertex_layout_add(&_layout, BGFX_ATTRIB_POSITION, 2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx_vertex_layout_add(&_layout, BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx_vertex_layout_add(&_layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8, true, false);
        bgfx_vertex_layout_end(&_layout);

        _tex = bgfx_create_uniform("s_tex", BGFX_UNIFORM_TYPE_SAMPLER, 1);

        u8* data;
        i32 texWidth;
        i32 texHeight;
        i32 texBits;
        io.Fonts->GetTexDataAsRGBA32(&data, &texWidth, &texHeight, &texBits);
        _texture = bgfx_create_texture_2d((u16)texWidth,              //
                                          (u16)texHeight,             //
                                          false,                      //
                                          1,                          //
                                          BGFX_TEXTURE_FORMAT_BGRA8,  //
                                          0,                          //
                                          bgfx_copy(data, texWidth * texHeight * texBits));
    }

    void render(u8 viewId, bgfx_frame_buffer_handle_t framebuffer, ImDrawData* drawData)
    {
        //        EASY_FUNCTION(profiler::colors::Teal);
        const ImGuiIO& io     = ImGui::GetIO();
        const float    width  = io.DisplaySize.x;
        const float    height = io.DisplaySize.y;

        {
            bgfx_set_view_frame_buffer(viewId, framebuffer);
            bgfx_set_view_rect(viewId, 0, 0, uint16_t(width), uint16_t(height));
            const bgfx_caps_t* caps = bgfx_get_caps();
            float              ortho[16];
            bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, -1.0f, 1.0f, 0, caps->homogeneousDepth);
            bgfx_set_view_transform(viewId, NULL, ortho);
        }

        // Render command lists
        for (i32 ii = 0, num = drawData->CmdListsCount; ii < num; ++ii)
        {
            bgfx_transient_vertex_buffer_t tvb;
            bgfx_transient_index_buffer_t  tib;

            const ImDrawList* drawList    = drawData->CmdLists[ii];
            u32               numVertices = (u32)drawList->VtxBuffer.size();
            u32               numIndices  = (u32)drawList->IdxBuffer.size();

            if (numVertices != bgfx_get_avail_transient_vertex_buffer(numVertices, &_layout) ||
                numIndices != bgfx_get_avail_transient_index_buffer(numIndices))
            {
                // not enough space in transient buffer just quit drawing the rest...
                break;
            }

            bgfx_alloc_transient_vertex_buffer(&tvb, numVertices, &_layout);
            bgfx_alloc_transient_index_buffer(&tib, numIndices, false);

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
                    bgfx_set_scissor(xx, yy, u16(fmin(cmd->ClipRect.z, 65535.0f) - xx),
                                     u16(fmin(cmd->ClipRect.w, 65535.0f) - yy));

                    bgfx_set_state(state, 0);
                    bgfx_set_texture(0, _tex, _texture, UINT32_MAX);
                    bgfx_set_transient_vertex_buffer(0, &tvb, 0, numVertices);
                    bgfx_set_transient_index_buffer(&tib, offset, cmd->ElemCount);
                    bgfx_submit(viewId, _program, 0, BGFX_DISCARD_ALL);
                }

                offset += cmd->ElemCount;
            }
        }
    }

    void destroy()
    {
        bgfx_destroy_uniform(_tex);
        bgfx_destroy_texture(_texture);
        bgfx_destroy_program(_program);
    }

    bgfx_vertex_layout_t  _layout;
    bgfx_program_handle_t _program;
    bgfx_texture_handle_t _texture;
    bgfx_uniform_handle_t _tex;
};

static ImGuiBgfx s_imguiBgfx;

////////// BGFX Callbacks ////////////

struct BGFXCallbacss
{
    BGFXCallbacss() { }

    // CallbackI interface
public:
    void fatal(const char* _filePath, uint16_t _line, bgfx_fatal_t _code, const char* _str)
    {
        Logger::log().error(_str);
    }

    void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList)
    {
        Logger::log().info(format(_format, _argList));
    }
    void     profilerBegin(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) { }
    void     profilerBeginLiteral(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) { }
    void     profilerEnd() { }
    uint32_t cacheReadSize(uint64_t _id)
    {
        return 0;
    }
    bool cacheRead(uint64_t _id, void* _data, uint32_t _size)
    {
        return false;
    }
    void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) { }
    void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data,
                    uint32_t _size, bool _yflip)
    {
    }
    void captureBegin(uint32_t _width, uint32_t _height, uint32_t _pitch, bgfx_texture_format_t _format, bool _yflip) {
    }
    void captureEnd() { }
    void captureFrame(const void* _data, uint32_t _size) { }

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
        _framebuffer = bgfx_create_frame_buffer_from_nwh(nativeHandle(), _width, _height, BGFX_TEXTURE_FORMAT_COUNT,
                                                         BGFX_TEXTURE_FORMAT_COUNT);
    }
    else
    {
        _framebuffer = BGFX_INVALID_HANDLE;
    }
    bgfx_set_view_frame_buffer(_viewId, _framebuffer);
    bgfx_set_view_name(_viewId, title);
    bgfx_set_view_rect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height));
    bgfx_set_view_clear(_viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    imguiInit();
}

SDLWindow::~SDLWindow()
{
    imguiShutdown();

    releaseFramebuffer();

    if (_isDefault)
    {
        bgfx_shutdown();
        SDL_GL_DeleteContext(_glContext);
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

void SDLWindow::onInit() { }

void SDLWindow::handleEvent(const SDL_Event& e)
{
    // If an event was detected for this window
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == _windowId)
    {
        handleWindowEvent(e.window);
        onEvent(e);
        return;
    }

    u32 flags = SDL_GetWindowFlags(_window);
    if (flags & SDL_WINDOW_INPUT_FOCUS || flags & SDL_WINDOW_MOUSE_FOCUS)
    {
        handleInputEvent(e);
    }
    onEvent(e);
}

void SDLWindow::handleWindowEvent(const SDL_WindowEvent& e)
{
    switch (e.event)
    {
        // Get new dimensions and recreate framebuffer
        case SDL_WINDOWEVENT_RESIZED:
            if (_width != u32(e.data1) || _height != u32(e.data2))
            {
                _width  = e.data1;
                _height = e.data2;

                if (BGFX_HANDLE_IS_VALID(_framebuffer))
                {
                    bgfx_reset_view(_viewId);
                    bgfx_destroy_frame_buffer(_framebuffer);
                    _framebuffer = bgfx_create_frame_buffer_from_nwh(
                        nativeHandle(), _width, _height, BGFX_TEXTURE_FORMAT_COUNT, BGFX_TEXTURE_FORMAT_COUNT);
                    bgfx_set_view_frame_buffer(_viewId, _framebuffer);
                    bgfx_set_view_clear(_viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
                }
                else
                {
                    bgfx_reset(_width, _height, BGFX_RESET_NONE, BGFX_TEXTURE_FORMAT_COUNT);
                }

                bgfx_set_view_rect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height));
                Logger::log().info("Resize event, window: {}, view: {}, size({},{})\n ", _windowId, _viewId, _width,
                                   _height);
            }
            break;
    }
    onWindowEvent(e);
}

void SDLWindow::handleInputEvent(const SDL_Event& e)
{
    imguiPushCtx();
    ImGuiIO& io = ImGui::GetIO();
    imguiPopCtx();
    switch (e.type)
    {
        case SDL_TEXTINPUT:
            io.AddInputCharactersUTF8(e.text.text);
            break;
        case SDL_MOUSEMOTION:
            io.MousePos = ImVec2(e.motion.x, e.motion.y);
            break;
        case SDL_MOUSEWHEEL:
            io.MouseWheel = e.wheel.y;
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
    onInputEvent(e);
}

void SDLWindow::releaseFramebuffer()
{
    if (BGFX_HANDLE_IS_VALID(_framebuffer))
    {
        bgfx_set_view_frame_buffer(_viewId, BGFX_INVALID_HANDLE);

        bgfx_destroy_frame_buffer(_framebuffer);

        _framebuffer = BGFX_INVALID_HANDLE;
    }
}

void SDLWindow::onUpdate(float /*dt*/)
{
    bgfx_dbg_text_printf(0, 5, 0x2f, "SDLWindow::update");
}

void SDLWindow::onEvent(const SDL_Event& e) { }

void SDLWindow::onWindowEvent(const SDL_WindowEvent& e) { }

void SDLWindow::onInputEvent(const SDL_Event& e) { }

SDLWindow::Size SDLWindow::windowSize() const
{
    return Size {_width, _height};
}

void SDLWindow::doUpdate(float dt)
{
    //    EASY_FUNCTION(profiler::colors::Amber);
    bgfx_set_view_frame_buffer(_viewId, _framebuffer);
    bgfx_set_view_rect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height));
    bgfx_touch(_viewId);

    // GUI
    imguiPushCtx();
    imguiNewFrame();

    // render content
    onUpdate(dt);

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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = "imgui.ini";

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
    io.KeyMap[ImGuiKey_Space]      = SDLK_SPACE;
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
    bgfx_init_t initParams;
    std::memset(&initParams, 0, sizeof(bgfx_init_t));

    bgfx_platform_data_t& pd = initParams.platformData;
    SDL_SysWMinfo         wmi;
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

    initParams.type                   = BGFX_RENDERER_TYPE_OPENGLES;
    initParams.debug                  = true;
    initParams.resolution.width       = w;
    initParams.resolution.height      = h;
    initParams.limits.transientIbSize = 1 << 20;
    initParams.limits.transientVbSize = 1 << 20;

    //initParams.allocator = reinterpret_cast<bgfx_allocator_interface_t*>(MemoryManager::bxAllocator());

    //    static bgfx::AllocatorC99 s_allocator;
    //    s_allocator.m_interface = init.allocator;
    //    init.allocator = reinterpret_cast<bgfx_allocator_interface_t*>(&s_allocator);

    if (!bgfx_init(&initParams))
        return false;

    bgfx_reset(_width, _height, _reset, BGFX_TEXTURE_FORMAT_COUNT);

    // Enable debug text.
    bgfx_set_debug(_debug);

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
