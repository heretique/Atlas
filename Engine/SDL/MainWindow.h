#ifndef WINDOW_H
#define WINDOW_H

#include "SDL/SDLWindow.h"

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
    void onGUI() override;
    void onInputEvent(const SDL_Event& e) override;

private:
    void renderAxes();
    void render(float dt);

    bgfx::VertexBufferHandle _axesVbh;
    bgfx::IndexBufferHandle  _axesIbh;
    bgfx::ProgramHandle      _axesProgram;
};

}  // namespace atlas

#endif  // WINDOW_H
