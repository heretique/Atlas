#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "Base.h"
#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>

struct ImDrawData;
struct ImGuiContext;

class SDLWindow {
  friend class SDLApp;

public:
  SDLWindow(const char *title, int x, int y, int w, int h);
  virtual ~SDLWindow();
  u32 winId() const;
  bool isMain() const;
  virtual void init();
  virtual void update(float dt);
  virtual void onGUI();

protected:
  // imgui related
  bool imguiInit();
  void imguiShutdown();
  void imguiNewFrame();
  void imguiRender();
  void imguiPushCtx();
  void imguiPopCtx();
  void imguiMoveWindow();

  // bgfx related
  bool bgfxInit();

private:
  void *nativeHandle();
  void doUpdate(float dt);
  void handleEvent(SDL_Event &e);
  void handleWindowEvent(SDL_WindowEvent &e);
  void handleInputEvent(SDL_Event &e);

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

  string _title;
  bool _open{true};
  u32 _width{0};
  u32 _height{0};
  ImGuiContext *_imguiCtx;
  ImGuiContext *_prevImguiCtx;

  int _moveOffsetX{0};
  int _moveOffsetY{0};
};

#endif // SDLWINDOW_H
