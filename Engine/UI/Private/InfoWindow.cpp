#include "UI/InfoWindow.h"
#include "Input/InputManager.h"
#include "Engine/Engine.h"
#include <bgfx/c99/bgfx.h>

namespace atlas
{
namespace ui = ImGui;
void InfoWindow::onGUI(float deltaTime)
{
    const bgfx_stats_t* stats    = Engine::bgfxStats();
    const double        toMsCpu  = 1000.0 / stats->cpuTimerFreq;
    const double        toMsGpu  = 1000.0 / stats->gpuTimerFreq;
    const double        frameMs  = double(stats->cpuTimeFrame) * toMsCpu;
    static bool         showDemo = false;

    ui::Text("Frame %0.3f [ms], %0.3f FPS", frameMs, 1000.0 / frameMs);

    ui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)", double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu,
             double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu, stats->maxGpuLatency);
    ui::Separator();
    ui::Text("Horizontal Axis: %0.1f", Engine::input().horizontalAxis());
    ui::Text("Vertical Axis: %0.1f", Engine::input().verticalAxis());
    ui::Text("Mouse Horizontal Axis: %0.1f", Engine::input().mouseHorizontalAxis());
    ui::Text("Mouse Vertical Axis: %0.1f", Engine::input().mouseVerticalAxis());
    ui::Checkbox("Show ImGui Demo", &showDemo);
    if (showDemo)
    {
        ui::ShowDemoWindow(&showDemo);
    }
}

}  // atlas namespace
