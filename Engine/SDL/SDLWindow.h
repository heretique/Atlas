#pragma once

#include "Core/Debug.h"
#include "Hq/BasicTypes.h"

#include <string>

#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>

struct ImDrawData;
struct ImGuiContext;

namespace atlas {
class SDLWindow {
  friend class SDLApp;

public:
  struct Size {
    u32 width;
    u32 height;
  };

  virtual ~SDLWindow();
  u32 winId() const;
  bool isMain() const;
  virtual void onInit();
  virtual void onUpdate(float dt);
  virtual void onEvent(const SDL_Event &e);
  virtual void onWindowEvent(const SDL_WindowEvent &e);
  virtual void onInputEvent(const SDL_Event &e);
  Size windowSize() const;

protected:
  SDLWindow(const char *title, int x, int y, int w, int h);
  // imgui related
  bool imguiInit();
  void imguiShutdown();
  void imguiNewFrame();
  void imguiRender();
  void imguiPushCtx();
  void imguiPopCtx();
  void imguiMoveWindow();

  // bgfx related
  bool bgfxInit(uint32_t w, uint32_t h);

private:
  void *nativeHandle();
  void doUpdate(float dt);
  void handleEvent(const SDL_Event &e);
  void handleWindowEvent(const SDL_WindowEvent &e);
  void handleInputEvent(const SDL_Event &e);
  void releaseFramebuffer();

private:
  static bool _initialized;
  static SDL_GLContext _glContext;
  static u32 _debug;
  static u32 _reset;
  static u8 _windowCount;

  u8 _viewId{0};
  SDL_Window *_window{nullptr};
  u32 _windowId{0};
  bgfx::FrameBufferHandle _framebuffer;

  bool _isDefault{false};

  std::string _title;
  bool _open{true};
  u32 _width{0};
  u32 _height{0};
  ImGuiContext *_imguiCtx{nullptr};
  ImGuiContext *_prevImguiCtx{nullptr};

  int _moveOffsetX{0};
  int _moveOffsetY{0};
};
} // namespace atlas
