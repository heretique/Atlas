#ifndef WINDOW_H
#define WINDOW_H

#include "SDL/SDLWindow.h"
#include "entt/fwd.hpp"
#include <bgfx/c99/bgfx.h>

#include <chrono>
#include <memory>
#include <vector>

namespace atlas
{
struct JobData;

class MainWindow : public SDLWindow
{
    using Super = SDLWindow;

public:
    MainWindow(const char* title, int x, int y, int w, int h);
    ~MainWindow() override;

public:
    void onInit() override;
    void onUpdate(float dt) override;
    void onInputEvent(const SDL_Event& e) override;

private:
    void                     renderAxes();
    void                     render(float dt);

private:
    bgfx_vertex_buffer_handle_t _axesVbh;
    bgfx_index_buffer_handle_t  _axesIbh;
    bgfx_program_handle_t       _axesProgram;
};

}  // namespace atlas

#endif  // WINDOW_H
